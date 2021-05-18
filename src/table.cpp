#include "table.h"

/*
 `cells_per_row` will be stored as `n` in Table in order to
 simplify the syntax of indexing
 */
Table::Table(int cells_per_row) : n(cells_per_row), cell_size(WIN_SCALE / cells_per_row), button_pressed(ButtonPressed::NotPressed), cells(cells_per_row * cells_per_row, false)
{
	/*
	 This class uses a single dimensional array to store 2d data.
	 The reason why this is done is to make allocation and memory setting
	 simpler here and copying simpler in the iteration method.
	 */
	add_events(Gdk::BUTTON_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
}

void Table::draw_cell(int x, int y)
{
	/*
	 Does a paranoia check to ensure that the button is pressed
	 and that the x/y coordinates are within the bounds of the array.
	 Assigns the cell either alive or dead depending on
	 whether the button is the left button or the right button.
	 */
	if (button_pressed == ButtonPressed::NotPressed
			|| x >= n || x < 0 || y >= n || y < 0)
		return;
	if (button_pressed == ButtonPressed::LeftMouseButton)
		cells[x + y * n] = true;
	else if (button_pressed == ButtonPressed::RightMouseButton) // redundant expression (could be replaced with just else), just to clarify the code
		cells[x + y * n] = false;
	queue_draw();
}

bool Table::on_button_press_event(GdkEventButton *event)
{
	/*
	 When a mouse button is pressed, checks whether the
	 button is the left or the right and save that to a variable
	 that is then checked by the motion notify method,
	 which draws the cell alive or dead according to the
	 button pressed. Also draws the cell as the button is
	 pressed to make sure that the user can
	 draw individual cells with single button presses
	 */
	if (button_pressed != ButtonPressed::NotPressed)
		return (true);
	if (event->button == static_cast<guint>(ButtonPressed::LeftMouseButton)
			|| event->button == static_cast<guint>(ButtonPressed::RightMouseButton))
		button_pressed = static_cast<ButtonPressed>(event->button);
	int x = (int)event->x / cell_size;
	int y = (int)event->y / cell_size;
	draw_cell(x, y);
	return (true);
}

bool Table::on_button_release_event(GdkEventButton *event)
{
	/*
	 Check whether the button released is the button that was set as pressed earlier.
	 If so, update the button_pressed variable to show that no button is pressed
	 and drawing can cease.
	 */
	if (event->button == static_cast<guint>(ButtonPressed::LeftMouseButton)
			&& button_pressed == ButtonPressed::LeftMouseButton)
		button_pressed = ButtonPressed::NotPressed;
	else if (event->button == static_cast<guint>(ButtonPressed::RightMouseButton)
			&& button_pressed == ButtonPressed::RightMouseButton)
		button_pressed = ButtonPressed::NotPressed;
	return (true);
}

bool Table::on_motion_notify_event(GdkEventMotion *event)
{
	/*
	 Check whether the button is pressed at all.
	 If so, I first rescale the coordinates to
	 work with my increased pixel size and then send
	 them to the draw_cell function which draws the
	 cell either alive or dead.
	 */
	if (button_pressed == ButtonPressed::NotPressed)
		return (true);
	int x = (int)event->x / cell_size;
	int y = (int)event->y / cell_size;
	draw_cell(x, y);
	return (true);
}

Table::~Table() {}

bool Table::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	/*
	 Start by making the whole drawing area black. Then set the color
	 to be white for the cells that are alive. We then check
	 which cells are alive and use cairo to draw `cell_size` sized
	 rectangles accordingly. If the cell is dead, we simply skip it
	 with `continue`.
	 */
	cr->set_source_rgb(0.0, 0.0, 0.0);
	cr->rectangle(0, 0, n * cell_size, n * cell_size);
	cr->fill();
	cr->set_source_rgb(1.0, 1.0, 1.0);
	for (int x = 0; x < n; x++)
	{
		for (int y = 0; y < n; y++)
		{
			if (!cells[x + y * n])
				continue;
			cr->rectangle(x * cell_size, y * cell_size, cell_size, cell_size);
			cr->fill();
		}
	}
	return true;
}

int	Table::count_surrounding_alive(int x, int y)
{
	int	ret = 0;

	/*
	 Adds up true (==1) values to get a total
	 value of cells that are true, aka alive.
	 Ensures that x or y are not out of bounds
	 to avoid segfault.
	 */
	ret += (x - 1 >= 0) && cells[(x - 1) + y * n];
	ret += (x + 1 < n) && cells[(x + 1) + y * n];
	if (y > 0)
	{
		ret += (x - 1 >= 0) && cells[(x - 1) + (y - 1) * n];
		ret += cells[x + (y - 1) * n];
		ret += (x + 1 < n) && cells[(x + 1) + (y - 1) * n];
	}
	if (y + 1 < n)
	{
		ret += (x - 1 >= 0) && cells[(x - 1) + (y + 1) * n];
		ret += cells[x + (y + 1) * n];
		ret += (x + 1 < n) && cells[(x + 1) + (y + 1) * n];
	}
	return (ret);
}

void Table::iterate()
{
	std::vector<bool> next_generation(cells);
	int	num_surrounding_cells;

	/*
	 Uses a temporary bool vector to make sure that
	 all of the new values for the cells are taken
	 from the previous generation.
	 */
	for (int x = 0; x < n; x++)
	{
		for (int y = 0; y < n; y++)
		{
			num_surrounding_cells = count_surrounding_alive(x, y);
			/*
			 If cell is alive and the number of
			 surrounding cells that are alive is neither 2 or 3,
			 then this cell dies by over/underpopulation.
			 */
			if (cells[x + y * n] && num_surrounding_cells != 2 && num_surrounding_cells != 3)
				next_generation[x + y * n] = false;
			/*
			 If this cell is dead and
			 there are exactly 3 surrounding cells,
			 the cell is born.
			 */
			if (!cells[x + y * n] && num_surrounding_cells == 3)
				next_generation[x + y * n] = true;
		}
	}
	/*
	 Moves the values from the temporary vector to
	 the primary array, moving the cells to the next generation.
	 */
	cells = next_generation;
	queue_draw();
}
