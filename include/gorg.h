#ifndef __gorg_h__
#define __gorg_h__

// C++ Standard Library
#include <cstdint>
#include <deque>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

// GTK/GLib Libraries
#include <giomm/simpleactiongroup.h>
#include <gdkmm/pixbuf.h>
#include <glibmm/dispatcher.h>
#include <gtkmm/entry.h>
#include <gtkmm/hvbox.h>
#include <gtkmm/image.h>
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
    struct ThumbnailUpdate
    {
        std::uint64_t generation;
        std::string cacheKey;
        Gtk::Image *image;
        Glib::RefPtr<Gdk::Pixbuf> pixbuf;
    };
    struct ThumbnailRequest
    {
        std::uint64_t generation;
        std::string path;
        std::string cacheKey;
        Gtk::Image *image;
        int size;
    };

    void handleRunResult(Match *match);
    void queueThumbnailLoad(const std::string &path, Gtk::Image *image, std::uint64_t generation);
    void startQueuedThumbnailLoads();
    void loadThumbnail(ThumbnailRequest request);
    void handleThumbnailUpdates();
    std::string thumbnailCacheKey(const std::string &path) const;

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
    std::uint64_t thumbnailGeneration = 0;
    Glib::Dispatcher thumbnailDispatcher;
    std::mutex thumbnailMutex;
    std::map<std::string, Glib::RefPtr<Gdk::Pixbuf>> thumbnailCache;
    std::deque<ThumbnailRequest> thumbnailQueue;
    std::vector<ThumbnailUpdate> thumbnailUpdates;
    unsigned int activeThumbnailLoads = 0;
};
#endif
