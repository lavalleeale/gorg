#include <gorg.h>
#include <TransparentWindow.h>
#include <gtkmm.h>
#include <gtkmm/application.h>
#include <iostream>
#include <plugin.h>
#include <finder.h>
#include <settings.h>
#include <glibmm/optioncontext.h>

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
                                    {
                                         std::string text = prompt.get_text();
                                             search(text); });
}

void Gorg::search(const std::string &query)
{
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
    finder->find(query);
    unsigned int i = 0;
    for (auto match : finder->getMatches())
    {
        if (++i > getMaxResults())
        {
            break;
        }
        Gtk::Button *button = Gtk::manage(new Gtk::Button(match->getDisplay()));
        button->signal_clicked().connect([match, this]()
                                         { handleRunResult(match); });
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
    app->set_action_group(action_group);

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

    mainVerticalLayout.add(prompt);
    prompt.show();
    prompt.grab_focus(); // Focus the entry field on startup
    mainVerticalLayout.add(optionsScroll);
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
    action_group->add_action(quitAction);
    quitAction->signal_activate().connect([this](const Glib::VariantBase)
                                          { app->quit(); });
    app->set_accel_for_action("app.quit", "Escape");
}
