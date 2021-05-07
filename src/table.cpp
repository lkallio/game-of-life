#include "table.h"

Table::Table(int number) : n(number), pixel_size(WIN_SCALE / number), button_pressed(0)
{
	/*
	 This class uses a single dimensional array to store 2d data.
	 The reason why this is done is to make allocation and memory setting
	 simpler here and copying simpler in the iteration method.
	 A 2d array can easily be replaced by a 1d array by this simple index formula:
	 2D_TO_1D_INDEX(x, y, row_len) = x + y * row_len.
	 */
	cells = (bool *)malloc(n * n * sizeof(bool));
	if (!cells)
	{
		puts(strerror(errno));
		exit(0);
	}
	memset(cells, 0, n * n * sizeof(bool));
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
	if (!button_pressed || x >= n || x < 0 || y >= n || y < 0)
		return;
	if (button_pressed == LEFT_MOUSE_BUTTON)
		cells[x + y * n] = true;
	else if (button_pressed == RIGHT_MOUSE_BUTTON) // redundant expression (could be replaced with just else), just to clarify the code
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
	if (event->button == LEFT_MOUSE_BUTTON || event->button == RIGHT_MOUSE_BUTTON)
		button_pressed = event->button;
	int x = (int)event->x / pixel_size;
	int y = (int)event->y / pixel_size;
	draw_cell(x, y);
	return (true);
}

bool Table::on_button_release_event(GdkEventButton *event)
{
	if (event->button == LEFT_MOUSE_BUTTON || event->button == RIGHT_MOUSE_BUTTON)
		button_pressed = 0;
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
	if (!button_pressed)
		return (true);
	int x = (int)event->x / pixel_size;
	int y = (int)event->y / pixel_size;
	draw_cell(x, y);
	return (true);
}

Table::~Table() {}

bool Table::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	/*
	 Start by making the whole drawing area black. Then set the color
	 to be white for the cells that are alive. We then check
	 which cells are alive and use cairo to draw `pixel_size` sized
	 rectangles accordingly. If the cell is dead, we simply skip it
	 with `continue`.
	 */
	cr->set_source_rgb(0.0, 0.0, 0.0);
	cr->rectangle(0, 0, n * pixel_size, n * pixel_size);
	cr->fill();
	cr->set_source_rgb(1.0, 1.0, 1.0);
	for (int x = 0; x < n; x++)
	{
		for (int y = 0; y < n; y++)
		{
			if (!cells[x + y * n])
				continue;
			cr->rectangle(x * pixel_size, y * pixel_size, pixel_size, pixel_size);
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
	bool next_generation[n * n];
	int	num_surrounding_cells;

	/*
	 Uses a temporary bool VLA to make sure that
	 all of the new values for the cells are taken
	 from the previous generation.
	 */
	memmove(next_generation, cells, n * n * sizeof(bool));
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
	 Moves the values from the temporary VLA to
	 the primary array, moving the cells to the next generation.
	 */
	memmove(cells, next_generation, n * n * sizeof(bool));
	queue_draw();
}
