#ifndef __gorg_h__
#define __gorg_h__

#include <vector>
#include <string>
#include <gtkmm.h>
#include <settings.h>
#include <finder.h>
#include <TransparentWindow.h>

class Gorg
{
public:
    ~Gorg();
    void setupArguments(int argc, char *argv[]);
    void setupWindow();
    void setupKeybindings();
    void setupActions();
    void search(const std::string &query);
    int run();

private:
    void handleRunResult(Match *match);

    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create("one.lavallee.gorg");
    Glib::RefPtr<Gio::SimpleActionGroup> action_group = Gio::SimpleActionGroup::create();
    Gtk::VBox mainVerticalLayout;
    Gtk::Entry prompt;
    Gtk::ScrolledWindow optionsScroll;
    Gtk::VBox options;

    TransparentWindow window;

    Finder *finder;
    Match *selectedMatch = nullptr;
};
#endif