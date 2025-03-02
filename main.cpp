#include <TransparentWindow.h>
#include <gtkmm.h>
#include <gtkmm/application.h>
#include <iostream>
#include <plugin.h>
#include <finder.h>
#include <settings.h>

void handleRunResult(RunResult result, Glib::RefPtr<Gtk::Application> app, Gtk::Box &mainVerticalLayout, Gtk::Box &options, Gtk::Widget *takeoverWidget = nullptr)
{
    switch (result)
    {
    case RunResult::CLOSE:
        app->quit();
        break;
    case RunResult::TAKEOVER:
        if (takeoverWidget)
        {
            options.hide();
            mainVerticalLayout.add(*takeoverWidget);
            takeoverWidget->show();
        }
        break;
    default:
        break;
    }
}

int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create(argc, argv, "one.lavallee.gorg");
    Settings::getInstance().load();
    Finder finder;

    TransparentWindow window;
    auto action_group = Gio::SimpleActionGroup::create();
    window.set_default_size(Settings::getInstance().getWindowWidth(),
                            Settings::getInstance().getWindowHeight());
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

    // Load custom CSS if it exists
    auto custom_css_provider = Gtk::CssProvider::create();
    try
    {
        custom_css_provider->load_from_data(Settings::getInstance().getCustomCss());
        Gtk::StyleContext::add_provider_for_screen(
            Gdk::Screen::get_default(),
            custom_css_provider,
            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }
    catch (const Glib::Error &ex)
    {
        std::cerr << "Failed to load custom CSS: " << ex.what() << std::endl;
    }

    Gtk::StyleContext::add_provider_for_screen(
        Gdk::Screen::get_default(),
        css_provider,
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    Gtk::VBox mainVerticalLayout;
    Gtk::Entry prompt;
    Gtk::ScrolledWindow optionsScroll;
    Gtk::VBox options;

    window.add(mainVerticalLayout);
    mainVerticalLayout.show();

    mainVerticalLayout.add(prompt);
    prompt.show();
    prompt.grab_focus(); // Focus the entry field on startup
    prompt.signal_activate().connect([&app, &finder, &mainVerticalLayout, &options]()
                                     {
                                        if (!finder.getMatches().empty() && options.is_visible()) {
                                         handleRunResult(finder.RunMatch(), app, mainVerticalLayout, options, finder.getMatches()[0]->getWidget());
                                        } });
    prompt.signal_changed().connect([&prompt, &options, &finder, &app, &mainVerticalLayout]()
                                    { 
                                        if (!options.is_visible())
                                        {
                                            mainVerticalLayout.remove(*finder.getMatches()[0]->getWidget());
                                            options.show();
                                        }
                                        for (auto child : options.get_children())
                                        {
                                            options.remove(*child);
                                        }
                                        std::string text = prompt.get_text();
                                        finder.find(text);
                                        unsigned int i = 0;
                                        for (auto match : finder.getMatches())
                                        {
                                            if (++i > Settings::getInstance().getMaxResults())
                                            {
                                                break;
                                            }
                                            Gtk::Button *button = Gtk::manage(new Gtk::Button(match->getDisplay()));
                                            button->signal_clicked().connect([match, &app, &options, &mainVerticalLayout]()
                                                                             {
                                                                                    handleRunResult(match->run(), app, mainVerticalLayout, options, match->getWidget());
                                                                             });
                                            options.add(*button);
                                            button->show();
                                        } });

    mainVerticalLayout.add(optionsScroll);
    optionsScroll.show();
    optionsScroll.add(options);
    options.show();

    // Set scrolled window properties
    optionsScroll.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
    optionsScroll.set_min_content_height(150);

    auto quitAction = Gio::SimpleAction::create("quit");
    action_group->add_action(quitAction);
    quitAction->signal_activate().connect([&app](const Glib::VariantBase t)
                                          { app->quit(); });
    app->set_accel_for_action("app.quit", "Escape");

    return app->run(window);
}