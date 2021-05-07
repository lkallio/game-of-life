#ifndef TABLE_H
#define TABLE_H

#include "global.h"

/*
 Values for the `event->button` variable.
 */
#define LEFT_MOUSE_BUTTON		1
#define RIGHT_MOUSE_BUTTON	3

class Table : public Gtk::DrawingArea
{
	public:
		Table(int);
		virtual ~Table();
		void iterate(); // this is called by GoL, so it has to be public.

	protected:
		int count_surrounding_alive(int x, int y);
		bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
		bool on_motion_notify_event(GdkEventMotion *event) override;
		bool on_button_press_event(GdkEventButton *event) override;
		bool on_button_release_event(GdkEventButton *event) override;
		void draw_cell(int x, int y);
		const int n;
		const int pixel_size;
		int button_pressed;
		bool *cells;
};

#endif
