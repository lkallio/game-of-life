NAME = gol
SRC = gol.cpp main.cpp table.cpp
SRCDIR = src/
INC = includes/
FLAGS = -std=c++11
PKG_CONFIG = `pkg-config gtkmm-3.0 --cflags --libs`

.PHONY: all clean re

all: $(NAME)

$(NAME): $(addprefix $(SRCDIR), $(SRC))
	g++ $(FLAGS) -I $(INC) $^ -o $@ $(PKG_CONFIG)

clean:
	@rm $(NAME)

re: clean all
