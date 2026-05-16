#ifndef __gorg_h__
#define __gorg_h__

// C++ Standard Library
#include <string>
#include <memory>

// GTK/GLib Libraries
#include <giomm/simpleactiongroup.h>
#include <gtkmm/entry.h>
#include <gtkmm/hvbox.h>
#include <gtkmm/scrolledwindow.h>

// Local Headers
#include <TransparentWindow.h>
#include <finder.h>
#include <settings.h>

class Gorg
{
public:
    ~Gorg();
    void setupArguments(int argc, char *argv[]);
    void setupWindow();
    void setupKeybindings();
    void setupActions();
    void search(bool skipUpdate = false);
    int run();

private:
    void handleRunResult(Match *match);

    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create("one.lavallee.gorg");
    Glib::RefPtr<Gio::SimpleActionGroup> actionGroup = Gio::SimpleActionGroup::create();
    Gtk::VBox mainVerticalLayout;
    Gtk::Entry prompt;
    Gtk::ScrolledWindow optionsScroll;
    Gtk::VBox options;

    TransparentWindow window;

    std::unique_ptr<Finder> finder;
    Match *selectedMatch = nullptr;
    float imageSize = getImageSize();
};
#endif
