#include "gol.h"

void print_usage(void)
{
	puts("usage: ./gol [number of cells per row] [number of generations per second]");
	puts("NOTE: use left mouse button to draw pixels, right mouse button to erase");
	exit(0);
}

bool is_numeric(char *s)
{
	if (!s)
		return (false);
	if (*s == '-' || *s == '+')
		s++;
	while (*s)
		if (!isdigit(*s++))
			return (false);
	return (true);
}

int main(int ac, char **av)
{
	if (av[1] && !strcmp(av[1], "-h"))
		print_usage();
	/*
	 Create an application. Take the value passed in as the N value
	 determining the amount of cells in the NxN board and store it
	 as the variable `n`. That is then passed to the GoL window constructor,
	 which then passes it onto the Table constructor.
	 */
	auto app = Gtk::Application::create("org.gtkmm.examples.base");
	int	n = av[1] && is_numeric(av[1]) ? atoi(av[1]) : DEFAULT_TABLE_SIZE;
	int iterations_per_sec = av[1] && av[2] && is_numeric(av[2]) ? atoi(av[2]) : ITERATE_PER_SEC;

	if (n <= 0)
	{
		puts("N can't be 0 or less, defaulting to DEFAULT_TABLE_SIZE.");
		n = DEFAULT_TABLE_SIZE;
	}
	if (iterations_per_sec <= 0)
	{
		puts("Can't have negative or 0 iterations per sec, defaulting to ITERATE_PER_SEC.");
		iterations_per_sec = ITERATE_PER_SEC;
	}
	GoL win (n, iterations_per_sec);

	return (app->run(win)); // run the gtkmm loop.
}
