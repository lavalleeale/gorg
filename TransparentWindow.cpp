#include <TransparentWindow.h>
#include <iostream>
#include <gtk/gtk.h>

TransparentWindow::TransparentWindow() : m_isDragging(false),
                                         m_dragX(0),
                                         m_dragY(0)
{
    // Set up window properties
    set_title("Transparent Window");
    set_default_size(400, 300);
    set_app_paintable(true);
    set_decorated(false); // No window decorations

    // Enable events
    add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::POINTER_MOTION_MASK);

    setup_transparency();
}

TransparentWindow::~TransparentWindow()
{
}

void TransparentWindow::setup_transparency()
{
    // Check if the screen supports alpha channels (transparency)
    auto screen = get_screen();
    auto visual = screen->get_rgba_visual();
    if (visual && screen->is_composited())
    {
        gtk_widget_set_visual(GTK_WIDGET(gobj()), visual->gobj());
    }
    else
    {
        std::cerr << "Warning: Your screen doesn't support alpha channels. Transparency disabled." << std::endl;
    }
}

bool TransparentWindow::on_draw(const Cairo::RefPtr<Cairo::Context> &cr)
{
    // Clear the window with transparency
    cr->set_source_rgba(0.0, 0.0, 0.0, 0.85); // More opaque background for better visibility
    cr->set_operator(Cairo::OPERATOR_SOURCE);
    cr->paint();

    // Draw a rounded rectangle border instead of filling the entire window
    double radius = 20;
    double degrees = M_PI / 180.0;
    int width = get_allocated_width();
    int height = get_allocated_height();

    // Set a border color
    cr->set_source_rgba(0.2, 0.2, 0.4, 0.2);
    cr->set_line_width(2.0);

    cr->begin_new_sub_path();
    cr->arc(width - radius, radius, radius, -90 * degrees, 0);
    cr->arc(width - radius, height - radius, radius, 0, 90 * degrees);
    cr->arc(radius, height - radius, radius, 90 * degrees, 180 * degrees);
    cr->arc(radius, radius, radius, 180 * degrees, 270 * degrees);
    cr->close_path();

    // Stroke the path instead of filling it
    cr->fill();

    return Gtk::Window::on_draw(cr); // Let parent class draw child widgets
}

bool TransparentWindow::on_button_press_event(GdkEventButton *event)
{
    // Handle left button clicks for dragging
    if (event->button == 1)
    {
        m_isDragging = true;
        m_dragX = event->x;
        m_dragY = event->y;
    }

    return true;
}

bool TransparentWindow::on_motion_notify_event(GdkEventMotion *event)
{
    // Handle window dragging
    if (m_isDragging)
    {
        int x, y;
        get_position(x, y);
        move(x + (int)event->x - m_dragX, y + (int)event->y - m_dragY);
    }

    return true;
}

// Add the missing button release handler
bool TransparentWindow::on_button_release_event(GdkEventButton *event)
{
    if (event->button == 1)
    {
        m_isDragging = false;
    }
    return true;
}
