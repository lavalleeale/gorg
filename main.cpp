#include <gtkmm.h>
#include <gtkmm/application.h>
#include <iostream>
#include "plugin.h"
#include "finder.h"
#include "widgets.h"

int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
    Finder finder;

    Gtk::Window window;
    auto action_group = Gio::SimpleActionGroup::create();
    int width = 600, height = 200;
    window.set_default_size(width, height);
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

    Gtk::VBox mainVerticalLayout;
    Gtk::Entry prompt;
    Gtk::ScrolledWindow optionsScroll;
    Gtk::VBox options;

    window.add(mainVerticalLayout);
    mainVerticalLayout.show();

    mainVerticalLayout.add(prompt);
    prompt.show();
    prompt.signal_activate().connect([&app, &finder]()
                                     {
                                         if (finder.RunMatch())
                                         {
                                             app->quit();
                                         } });
    prompt.signal_changed().connect([&prompt, &options, &finder, &app]()
                                    { 
                                        for (auto child : options.get_children())
                                        {
                                            options.remove(*child);
                                        }
                                        std::string text = prompt.get_text();
                                        finder.find(text);
                                        unsigned int i = 0;
                                        for (auto match : finder.getMatches())
                                        {
                                            if (++i > 25)
                                            {
                                                break;
                                            }
                                            Gtk::Button *button = Gtk::manage(new Gtk::Button(match->getDisplay()));
                                            button->signal_clicked().connect([match, &app]()
                                                                             {
                                                                                 if (match->run())
                                                                                 {
                                                                                     app->quit();
                                                                                 }
                                                                             });
                                            options.add(*button);
                                            button->show();
                                        } });

    mainVerticalLayout.add(optionsScroll);
    optionsScroll.show();
    optionsScroll.add(options);
    options.show();

    auto quitAction = Gio::SimpleAction::create("quit");
    action_group->add_action(quitAction);
    quitAction->signal_activate().connect([&app](const Glib::VariantBase t)
                                          { app->quit(); });
    app->set_accel_for_action("app.quit", "Escape");

    return app->run(window);
}