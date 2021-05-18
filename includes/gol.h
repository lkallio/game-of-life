#ifndef GOL_H
#define GOL_H

#include "table.h"
#include "global.h"

/*
 Default values in case user does not pass in
 the corresponding custom values.
 */
static constexpr int DEFAULT_ITERATIONS_PER_SEC = 10;
static constexpr int DEFAULT_CELLS_PER_ROW = 100;
static constexpr int MAX_ITERATIONS_PER_SEC = 60;

class GoL : public Gtk::Window
{
	public:
		GoL(int, int);
		virtual ~GoL();
	
	private:
		bool on_timeout();
		Table table;
		Gtk::Box buttons;
		Gtk::Box master;
		Gtk::Button click;
		Gtk::ToggleButton toggle;
};

#endif