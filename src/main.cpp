#include "gol.h"

void print_usage()
{
	std::cout << "usage: ./gol [number of cells per row] [number of generations per second]" << std::endl
	<< "NOTE: use left mouse button to draw pixels, right mouse button to erase" << std::endl;
	exit(0);
}

int main(int ac, char **av)
{
	if (av[1] && !strcmp(av[1], "-h"))
		print_usage();
	/*
	 Create an application. Take the value passed in as the N value
	 determining the amount of cells per row in the NxN board and store it
	 as the variable `cells_per_row`. That is then passed to the GoL window constructor,
	 which then passes it onto the Table constructor.
	 */
	auto app = Gtk::Application::create("org.gtkmm.examples.base");

	int cells_per_row = DEFAULT_CELLS_PER_ROW;
	int iterations_per_sec = DEFAULT_ITERATIONS_PER_SEC;

	/*
	 Try to read the arguments passed as integers with std::stoi,
	 notify if an error occurs in the input.
	 */
	if (av[1])
	{
		try
		{
			cells_per_row = std::stoi(av[1]);
			if (cells_per_row <= 0)
				throw "Cannot have zero or negative cells per row";
			if (cells_per_row > WIN_SCALE)
				throw "Cells per row cannot exceed WIN_SCALE";
		}
		catch (const std::invalid_argument &ia)
		{
			cells_per_row = DEFAULT_CELLS_PER_ROW;
			std::cerr << "Input invalid: " << ia.what() << ", defaulting to DEFAULT_CELLS_PER_ROW." << std::endl;
		}
		catch (const std::out_of_range &oor)
		{
			cells_per_row = DEFAULT_CELLS_PER_ROW;
			std::cerr << "Input invalid: " << oor.what() << ", defaulting to DEFAULT_CELLS_PER_ROW." << std::endl;
		}
		catch (const char *msg)
		{
			cells_per_row = DEFAULT_CELLS_PER_ROW;
			std::cerr << msg << ", defaulting to DEFAULT_CELLS_PER_ROW." << std::endl;
		}
		/*
		 Read the second argument, if it is not null.
		 */
		if (av[2])
		{
			try
			{
				iterations_per_sec = std::stoi(av[2]);
				if (iterations_per_sec <= 0)
					throw "Cannot have zero or negative iterations per second";
				if (iterations_per_sec > 60)
					throw "Iterations per second cannot exceed MAX_ITERATIONS_PER_SEC";
			}
			catch (const std::invalid_argument &ia)
			{
				iterations_per_sec = DEFAULT_ITERATIONS_PER_SEC;
				std::cerr <<  "Input invalid: " <<ia.what() << ", defaulting to DEFAULT_ITERATIONS_PER_SEC." << std::endl;
			}
			catch (const std::out_of_range &oor)
			{
				iterations_per_sec = DEFAULT_ITERATIONS_PER_SEC;
				std::cerr << "Input invalid: " << oor.what() << ", defaulting to DEFAULT_ITERATIONS_PER_SEC." << std::endl;
			}
			catch (const char *msg)
			{
				iterations_per_sec = DEFAULT_ITERATIONS_PER_SEC;
				std::cerr << msg << ", defaulting to DEFAULT_ITERATIONS_PER_SEC." << std::endl;
			}
		}
	}

	GoL win (cells_per_row, iterations_per_sec);

	return (app->run(win)); // run the gtkmm loop.
}
