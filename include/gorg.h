#ifndef __gorg_h__
#define __gorg_h__

// C++ Standard Library
#include <string>

// GTK/GLib Libraries
#include <giomm/simpleactiongroup.h>
#include <gtkmm/entry.h>
#include <gtkmm/hvbox.h>
#include <gtkmm/scrolledwindow.h>

// Local Headers
#include <TransparentWindow.h>
#include <finder.h>

class Gorg
{
public:
    ~Gorg();
    void setupArguments(int argc, char *argv[]);
    void setupWindow();
    void setupKeybindings();
    void setupActions();
    void search();
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