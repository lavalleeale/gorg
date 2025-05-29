// C++ Standard Library
#include <iostream>
#include <thread>

// GTK/GLib Libraries
#include <glibmm/optioncontext.h>
#include <gtkmm/application.h>
#include <gtkmm/button.h>
#include <gtkmm/cssprovider.h>
#include <glibmm/main.h>

// Local Headers
#include <TransparentWindow.h>
#include <finder.h>
#include <gorg.h>
#include <plugin.h>
#include <settings.h>

void Gorg::handleRunResult(Match *match)
{
    selectedMatch = match;
    RunResult result = match->run();
    auto takeoverWidget = match->getWidget();
    switch (result)
    {
    case RunResult::CLOSE:
        app->quit();
        break;
    case RunResult::TAKEOVER:
        if (takeoverWidget)
        {
            optionsScroll.hide();
            mainVerticalLayout.add(*takeoverWidget);
            takeoverWidget->show();
        }
        break;
    default:
        break;
    }
}

Gorg::~Gorg()
{
    if (finder)
    {
        delete finder;
    }
    // Save the query to the configuration file
    saveLastQuery(prompt.get_text());
}

int Gorg::run()
{
    return app->run(window);
}

void Gorg::setupActions()
{
    prompt.signal_activate().connect([this]()
                                     {
                                        if (!finder->getMatches().empty() && optionsScroll.is_visible()) {
                                        handleRunResult(finder->getMatches().front());
                                        } });
    prompt.signal_changed().connect([this]()
                                    { search(); });
}

void Gorg::search(bool skipUpdate)
{
    // Cancel any in-flight image loads, then reset
    if (imageCancellable)
        imageCancellable->cancel();
    imageCancellable = Gio::Cancellable::create();

    const std::string query = prompt.get_text();

    if (!options.is_visible())
    {
        options.show();
    }
    if (selectedMatch)
    {
        mainVerticalLayout.remove(*selectedMatch->getWidget());
        selectedMatch = nullptr;
    }
    for (auto child : options.get_children())
    {
        options.remove(*child);
    }
    if (!skipUpdate)
    {
        finder->find(query);
    }
    unsigned int i = 0;
    for (auto match : finder->getMatches())
    {
        if (++i > getMaxResults())
        {
            break;
        }
        Gtk::Label *label = Gtk::manage(new Gtk::Label(match->getDisplay()));
        label->set_line_wrap(true);
        label->set_line_wrap_mode(Pango::WrapMode::WRAP_WORD_CHAR);
        label->set_ellipsize(Pango::EllipsizeMode::ELLIPSIZE_END);
        label->set_max_width_chars(40);
        label->set_size_request(50, -1);
        label->show();
        Gtk::Image *icon = nullptr;
        if (!match->getIcon().empty())
        {
            icon = Gtk::manage(new Gtk::Image);
            icon->set_pixel_size(32);

            if (match->getIcon().find('/') != std::string::npos)
            {
                // Async load from file using a background thread
                auto file_path = match->getIcon();
                std::thread([file_path, icon, this]()
                            {
                    try {
                        auto pix = Gdk::Pixbuf::create_from_file(file_path);
                        auto scaled = pix->scale_simple(static_cast<int>(std::round(imageSize / pix->get_height() * pix->get_width())), static_cast<int>(imageSize), Gdk::INTERP_HYPER);
                        Glib::signal_idle().connect_once([icon, scaled]() {
                            icon->set(scaled);
                        });
                    } catch (const Glib::Error &ex) {
                        std::cerr << "Icon load failed: " << ex.what() << std::endl;
                    } })
                    .detach();
            }
            else
            {
                icon->set_from_icon_name(match->getIcon(),
                                         Gtk::ICON_SIZE_LARGE_TOOLBAR);
            }
            icon->show();
        }
        Gtk::Button *button = Gtk::manage(new Gtk::Button());
        button->signal_clicked().connect([match, this]()
                                         { handleRunResult(match); });
        Gtk::HBox *optionBox = Gtk::manage(new Gtk::HBox());
        if (icon)
        {
            optionBox->pack_start(*icon, Gtk::PACK_SHRINK);
        }
        else
        {
            auto emptyIcon = Gtk::manage(new Gtk::Image());
            emptyIcon->set_pixel_size(imageSize);
            emptyIcon->show();
            optionBox->pack_start(*emptyIcon, Gtk::PACK_SHRINK);
        }
        optionBox->pack_start(*label, Gtk::PACK_EXPAND_WIDGET);
        button->add(*optionBox);
        optionBox->show();
        options.add(*button);
        button->show();
    }
}

void Gorg::setupArguments(int argc, char *argv[])
{
    std::locale::global(std::locale(""));

    Glib::OptionContext option_context;
    Glib::OptionGroup group("Gorg", "Gorg options", "Gorg options");

    std::vector<Glib::ustring> modes;
    Glib::OptionEntry entry;
    entry.set_long_name("modes");
    entry.set_short_name('m');
    entry.set_description("List of modes to load");
    group.add_entry(entry, modes);

    Glib::OptionEntry autoRestoreEntry;
    bool autoRestoreFlag = false;
    autoRestoreEntry.set_long_name("auto-restore");
    autoRestoreEntry.set_short_name('a');
    autoRestoreEntry.set_description("Enable or disable automatic query restoration (default: disabled)");
    group.add_entry(autoRestoreEntry, autoRestoreFlag);

    Glib::OptionEntry startingQueryEntry;
    Glib::ustring startingQuery;
    startingQueryEntry.set_long_name("starting-query");
    startingQueryEntry.set_short_name('q');
    startingQueryEntry.set_description("Set the starting query text");
    group.add_entry(startingQueryEntry, startingQuery);

    Glib::OptionEntry bigEntry;
    bool bigFlag = false;
    bigEntry.set_long_name("big");
    bigEntry.set_short_name('b');
    bigEntry.set_description("Enable or disable big mode (default: disabled)");
    group.add_entry(bigEntry, bigFlag);

    option_context.set_main_group(group);

    try
    {
        option_context.parse(argc, argv);
    }
    catch (const Glib::Error &ex)
    {
        std::cerr << "Option parsing failed: " << ex.what() << std::endl;
        throw ex;
    }

    std::vector<std::string> allModes;
    for (const auto &arg : modes)
    {
        std::stringstream ss(arg.raw());
        std::string item;
        while (std::getline(ss, item, ','))
        {
            allModes.push_back(item);
        }
    }
    finder = allModes.empty() ? new Finder() : new Finder(allModes);

    if (autoRestoreFlag && !startingQuery.empty())
    {
        std::cerr << "Warning: Both --auto-restore and --starting-query are set. Last query will be ignored." << std::endl;
    }

    if (autoRestoreFlag)
    {
        // Restore the last query on startup
        std::string lastQuery = getLastQuery();
        prompt.set_text(lastQuery);
    }

    if (!startingQuery.empty())
    {
        prompt.set_text(startingQuery);
    }

    if (bigFlag)
    {
        imageSize *= 6;
        window.set_size_request(getWindowWidth() * 2, getWindowHeight() * 4);
    }
}

void Gorg::setupWindow()
{
    window.set_default_size(getWindowWidth(),
                            getWindowHeight());
    window.set_title("GTKmm Example");
    window.set_decorated(false);
    window.set_position(Gtk::WIN_POS_CENTER);
    window.set_resizable(false);
    window.set_keep_above(true);
    window.set_keep_below(false);
    window.set_skip_taskbar_hint(true);
    window.set_skip_pager_hint(true);
    window.set_type_hint(Gdk::WINDOW_TYPE_HINT_DIALOG);
    window.set_accept_focus(true);
    window.set_focus_on_map(true);
    window.set_modal(true);
    app->set_action_group(actionGroup);

    // Set CSS provider for styling
    auto css_provider = Gtk::CssProvider::create();
    const std::string defaultCss = R"(
        button {
            color: white;
            background-color: rgba(40, 40, 80, 0.2);
            background-image: none;
            border-radius: 10px;
            padding: 5px;
            margin: 2px 10px;
        }
        button:hover, button:focus {
            background-color: rgba(60, 60, 120, 0.8);
        }
        entry {
            color: white;
            background-color: rgba(40, 40, 80, 0.2);
            background-image: none;
            border-radius: 10px;
            padding: 5px;
            margin: 10px;
        }
        label {
            color: white;
            background-color: rgba(40, 40, 80, 0.2);
            background-image: none;
            border-radius: 10px;
            padding: 5px;
            margin: 10px;
        }
    )";
    css_provider->load_from_data(defaultCss);

    Gtk::StyleContext::add_provider_for_screen(
        Gdk::Screen::get_default(),
        css_provider,
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // Load custom CSS if it exists
    auto custom_css_provider = Gtk::CssProvider::create();
    try
    {
        custom_css_provider->load_from_data(getCustomCss());
        Gtk::StyleContext::add_provider_for_screen(
            Gdk::Screen::get_default(),
            custom_css_provider,
            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }
    catch (const Glib::Error &ex)
    {
        std::cerr << "Failed to load custom CSS: " << ex.what() << std::endl;
    }

    window.add(mainVerticalLayout);
    mainVerticalLayout.show();

    mainVerticalLayout.pack_start(prompt, Gtk::PACK_SHRINK);
    prompt.show();
    prompt.grab_focus(); // Focus the entry field on startup
    mainVerticalLayout.pack_start(optionsScroll, Gtk::PACK_EXPAND_WIDGET);
    optionsScroll.show();
    optionsScroll.add(options);
    options.show();

    // Set scrolled window properties
    optionsScroll.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
    optionsScroll.set_min_content_height(150);
}

void Gorg::setupKeybindings()
{
    auto quitAction = Gio::SimpleAction::create("quit");
    actionGroup->add_action(quitAction);
    quitAction->signal_activate().connect([this](const Glib::VariantBase)
                                          { app->quit(); });
    app->set_accel_for_action("app.quit", "Escape");
    auto imageAction = Gio::SimpleAction::create("image-size");
    actionGroup->add_action(imageAction);
    imageAction->signal_activate().connect([this](const Glib::VariantBase)
                                           {
                                               if (std::fabs(imageSize - getImageSize()) < 1e-6)
                                               {
                                                   imageSize *= 4.0f;
                                                   window.set_size_request(getWindowWidth() * 2, getWindowHeight() * 4);
                                               }
                                               else
                                               {
                                                   imageSize = getImageSize();
                                                   window.set_size_request(getWindowWidth(), getWindowHeight());
                                               }
                                               search(true); // Refresh search results with new image size
                                           });
    // Ctrl + Enter
    app->set_accel_for_action("app.image-size", "<Control>Return");
}
