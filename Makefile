NAME = out
CPPFLAGS = -Wall -Wextra -Wshadow -std=c++20 -MD -Ofast -flto
LIBS = -lSDL2 -lpng
CC = g++
SRC = main.cpp Window.cpp LineerFunction.cpp TestArea.cpp Map.cpp MapData.cpp Image.cpp
INCDIR = include /usr/include/SDL2
SRCDIR = src

OBJ = $(SRC:.cpp=.o)
SRCFILES = $(addprefix $(SRCDIR), $(SRC))
INCDIRS = $(addprefix -I, $(INCDIR))
DFILES = $(SRC:.cpp=.d)

all: $(NAME)

debug: CPPFLAGS += -g
debug: all

address: CPPFLAGS += -fsanitize=address -g
address: all

run: all
	@./$(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CPPFLAGS) $^ -o $@ $(INCDIRS) $(LIBS) $(DEBUG)
	@echo finish

%.o: $(SRCDIR)/%.cpp
	@echo building $@
	@$(CC) -c $(CPPFLAGS) $< -o $@ $(INCDIRS) $(DEBUG)

clean:
	@rm -rf $(OBJ) $(DFILES)
	@echo cleaned

fclean: clean
	@rm -rf $(NAME)

watch:
	while :; do ABC=$$(make 2>&1 && clear && ./$(NAME)); clear; echo "$$ABC"; sleep 0.5; done

re: fclean all

install_sdl:
	sudo apt install libsdl2-dev -y

-include $(DFILES)

.PHONY: all clean fclean re debug run watch install_sdl
