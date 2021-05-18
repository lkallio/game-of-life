#include "gol.h"

bool GoL::on_timeout()
{
	/*
	 Check whether the "Run" button is toggled,
	 if so, run an iteration to get the next generation.
	 */
	if (toggle.get_active())
		table.iterate();
	return (true);
}

GoL::~GoL(){}

/*
 master is initialized as vertical to pack the buttons below the cells.
 */
GoL::GoL(int cells_per_row, int it_per_sec) : table(cells_per_row), buttons(), master(Gtk::ORIENTATION_VERTICAL)
{
	int scale = (WIN_SCALE / cells_per_row) * cells_per_row;

	/*
	 The window will be sized according to WIN_SCALE variable.
	 To ensure that it functions with the table drawing area, it runs
	 through the same formula the drawing area uses to calculate its area.
	 It leaves an extra 50 pixels on the bottom to account for the buttons.
	 */
	set_default_size(scale, scale + 50);
	set_title("Game of Life");
	set_resizable(false);
	/*
	 This connects the timeout function, which is used
	 to check in intervals if the program needs to perform the game of
	 life iteration and redraw the cells. The interval is given in
	 milliseconds, so 1000 milliseconds divided by a `it_per_sec` will give
	 `it_per_sec` function calls per second.
	 */
	Glib::signal_timeout().connect(sigc::mem_fun(*this, &GoL::on_timeout), 1000 / it_per_sec);
	/*
	 The window consists of two components, the cells and the buttons.
	 Master is the Gtk::Box which contains both of them.
	 */
	add(master);
	master.set_homogeneous(false);
	master.pack_start(table);
	master.pack_start(buttons, Gtk::PACK_SHRINK);
	click.set_label("Next");
	/*
	 Connect the "next" button to the iteration method
	 to run a single iteration whenever it is clicked.
	 */
	click.signal_clicked().connect(sigc::mem_fun(table, &Table::iterate));
	toggle.set_label("Run");
	buttons.pack_start(click);
	buttons.pack_start(toggle);
	/*
	 Show all of the widgets.
	 */
	click.show();
	toggle.show();
	table.show();
	buttons.show();
	master.show();
}
