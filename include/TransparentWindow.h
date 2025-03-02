#ifndef __transparent_window_h__
#define __transparent_window_h__
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <cairomm/context.h>
#include <gdkmm/screen.h>
#include <gdkmm/visual.h>

class TransparentWindow : public Gtk::Window
{
public:
    TransparentWindow();
    virtual ~TransparentWindow();

protected:
    // Override default signal handlers
    bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr) override;
    bool on_button_press_event(GdkEventButton *event) override;
    bool on_button_release_event(GdkEventButton *event) override;
    bool on_motion_notify_event(GdkEventMotion *event) override;

private:
    void setup_transparency();

    // Variables for window dragging
    bool m_isDragging;
    int m_dragX;
    int m_dragY;
};
#endif
