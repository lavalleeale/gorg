#ifndef __gorg_h__
#define __gorg_h__

// C++ Standard Library
#include <string>

// GTK/GLib Libraries
#include <giomm/cancellable.h>
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
    struct ImageInfo
    {
        Gtk::Image *image;
        std::string iconPath;
        bool isFile;
    };
    std::vector<ImageInfo> imagesToLoad;
    void handleRunResult(Match *match);

    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create("one.lavallee.gorg");
    Glib::RefPtr<Gio::SimpleActionGroup> actionGroup = Gio::SimpleActionGroup::create();
    Gtk::VBox mainVerticalLayout;
    Gtk::Entry prompt;
    Gtk::ScrolledWindow optionsScroll;
    Gtk::VBox options;

    TransparentWindow window;

    Finder *finder;
    Match *selectedMatch = nullptr;
    Glib::RefPtr<Gio::Cancellable> imageCancellable;
    float imageSize = getImageSize();
};
#endif