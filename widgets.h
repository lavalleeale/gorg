#ifndef __widgets_h__
#define __widgets_h__
#include <gtkmm.h>
class MyVBox : public Gtk::Box
{
public:
    MyVBox(int height) : Gtk::Box(Gtk::ORIENTATION_VERTICAL), m_max_height(height) {} // max height in pixels

protected:
    void on_size_allocate(Gtk::Allocation &allocation) override
    {
        if (allocation.get_height() > m_max_height)
            allocation.set_height(m_max_height);
        Gtk::Box::on_size_allocate(allocation);
    }
    void get_preferred_height_vfunc(int &minimum_height, int &natural_height) const override
    {
        Gtk::Box::get_preferred_height_vfunc(minimum_height, natural_height);
        if (natural_height > m_max_height)
            natural_height = m_max_height;
        if (minimum_height > m_max_height)
            minimum_height = m_max_height;
    }

private:
    int m_max_height;
};
#endif