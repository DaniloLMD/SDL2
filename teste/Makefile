APPS = ./apps
BIN = ./bin
INCLUDE = ./include
OBJ = ./obj
SRC = ./src

SDL_FLAGS = `sdl2-config --cflags --libs` -lSDL2 -lSDL2_image
MULTIPLE_FLAGS = "-Wl,--allow-multiple-definition" -lm

APP = main

all: _src clean_apps _apps

_src: \
		$(OBJ)/sdl_manager.o \
		$(OBJ)/cor.o \

clean_apps:
	rm -rf $(BIN)/*

_apps: $(BIN)/$(APP)
	$(BIN)/$(APP)

clean:
	rm $(OBJ)/*

$(OBJ)/%.o: $(SRC)/%.c $(INCLUDE)/%.h
	gcc -c $< -I $(INCLUDE) -o $@ $(SDL_FLAGS) $(MULTIPLE_FLAGS)

$(BIN)/%: $(APPS)/%.c
	gcc $< $(OBJ)/*.o -I $(INCLUDE) -o $@ $(SDL_FLAGS) $(MULTIPLE_FLAGS)

$(OBJ)/%.o: 
	gcc -c $< -I $(INCLUDE) -o $@ $(SDL_FLAGS) $(MULTIPLE_FLAGS)
