APPS = ./apps
BIN = ./bin
INCLUDE = ./include
OBJ = ./obj
SRC = ./src

SDL_FLAGS = `sdl2-config --cflags --libs` -lSDL2 -lSDL2_image
MULTIPLE_FLAGS = "-Wl,--allow-multiple-definition"

APP = main

all: _src clean_apps _apps

_src: \
		$(OBJ)/sdl_manager.o \
		$(OBJ)/convex_hull.o \
		$(OBJ)/cor.o \

clean_apps:
	rm -rf $(BIN)/*

_apps: $(BIN)/$(APP)
	$(BIN)/$(APP)

clean:
	rm $(OBJ)/*

$(OBJ)/%.o: $(SRC)/%.cpp $(INCLUDE)/%.hpp
	g++ -c $< -I $(INCLUDE) -o $@ $(SDL_FLAGS) $(MULTIPLE_FLAGS)

$(BIN)/%: $(APPS)/%.cpp
	g++ $< $(OBJ)/*.o -I $(INCLUDE) -o $@ $(SDL_FLAGS) $(MULTIPLE_FLAGS)

$(OBJ)/%.o: 
	g++ -c $< -I $(INCLUDE) -o $@ $(SDL_FLAGS) $(MULTIPLE_FLAGS)
