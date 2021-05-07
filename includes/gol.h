#ifndef GOL_H
#define GOL_H

#include "table.h"
#include "global.h"

/*
 Default values in case user does not pass in
 the corresponding custom values.
 */
#define ITERATE_PER_SEC	10
#define DEFAULT_TABLE_SIZE 100

class GoL : public Gtk::Window
{
	public:
		GoL(int, int);
		virtual ~GoL();
	
	protected:
		bool on_timeout();
		Table table;
		Gtk::Box buttons;
		Gtk::Box master;
		Gtk::Button click;
		Gtk::ToggleButton toggle;
};

#endif