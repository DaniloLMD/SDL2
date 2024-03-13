APPS = ./apps
BIN = ./bin
INCLUDE = ./include
OBJ = ./obj
SRC = ./src

SDL_FLAGS = `sdl2-config --cflags --libs` -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf
MULTIPLE_FLAGS = "-Wl,--allow-multiple-definition"

all: _src clean_apps _apps

_src: \
		$(OBJ)/ernanni.o\
		$(OBJ)/ground.o\
		$(OBJ)/mangueira.o
		
clean_apps:
	rm -rf $(BIN)/*

_apps: $(BIN)/main
	$(BIN)/main

clean:
	rm $(OBJ)/*

$(OBJ)/%.o: $(SRC)/%.cpp $(INCLUDE)/%.hpp
	g++ -c $< -I $(INCLUDE) -o $@ $(SDL_FLAGS) $(MULTIPLE_FLAGS)

$(BIN)/%: $(APPS)/%.cpp
	g++ $< $(OBJ)/*.o -I $(INCLUDE) -o $@ $(SDL_FLAGS) $(MULTIPLE_FLAGS)

teste:
	g++ t.cpp -o t $(SDL_FLAGS)
	./t
