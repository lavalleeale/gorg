#include <gtkmm.h>
#include <gtkmm/application.h>
#include <iostream>
#include "plugin.h"
#include "drun.h"

int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
    std::vector<Plugin *> plugins = {new Drun()};

    Gtk::Window window;
    auto action_group = Gio::SimpleActionGroup::create();
    window.set_default_size(200, 200);
    window.set_title("GTKmm Example");
    app->set_action_group(action_group);

    Gtk::VBox mainVerticalLayout;
    Gtk::Entry prompt;
    Gtk::Label label("Hello, world!");
    Gtk::Button button("Click me");

    window.add(mainVerticalLayout);
    mainVerticalLayout.show();

    mainVerticalLayout.add(prompt);
    prompt.show();
    prompt.signal_changed().connect([&prompt, &label, &plugins]()
                                    { 
                                        std::string text = prompt.get_text();
                                        std::vector<Match *> matches;
                                        for (auto plugin : plugins)
                                        {
                                            if (plugin->matchChance(text) > 0)
                                            {
                                                std::vector<Match *> pluginMatches = plugin->getMatches(text);
                                                matches.insert(matches.end(), pluginMatches.begin(), pluginMatches.end());
                                            }
                                        }
                                        std::sort(matches.begin(), matches.end(), [&text](Match *a, Match *b) { return a->getRelevance(text) > b->getRelevance(text); });
                                        std::string labelText = "";
                                        unsigned int i = 0;
                                        for (auto match : matches)
                                        {
                                            labelText += match->getDisplay() + "\n";
                                            if (i++ > 5)
                                            {
                                                break;
                                            }
                                        }
                                        label.set_text(labelText); });

    mainVerticalLayout.add(label);
    label.show();

    button.signal_clicked().connect([&prompt]()
                                    { std::cout << prompt.get_text() << std::endl; });

    mainVerticalLayout.add(button);
    button.show();

    auto quitAction = Gio::SimpleAction::create("quit");
    action_group->add_action(quitAction);
    quitAction->signal_activate().connect([&app](const Glib::VariantBase t)
                                          { app->quit(); });
    app->set_accel_for_action("app.quit", "<Primary>q");

    return app->run(window);
}