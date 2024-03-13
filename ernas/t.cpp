#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Renderer* renderer = NULL;

//ERNAS.HPP
const int ERNANNI_WIDTH = 50;
const int ERNANNI_HEIGTH = 50;

const int NUM_STATES = 7; 
enum ernanniAnimationState{
  PARADO_1, 
  PARADO_2,
  CORRENDO_1,
  CORRENDO_2,
  CORRENDO_3,
  CORRENDO_4,
  CORRENDO_5  
};

class Ernanni{
    public:
        Ernanni(int x, int y, int speed);

        void Update();
        void Render();
        void UpdateTexture(int state);
        void UpdateMoving();

        SDL_Rect GetRect() {return rect;}

    private:
        //SDL_Renderer* renderer;
        SDL_Rect rect;
        bool isMoving;
        int speed;
        int state;

        int animationCounter;

        //textures de sprites
        SDL_Texture* texture;
};

//sprites do ernanni parado
std::string SPRITES_FOLDER = "sprites/";
std::string ERNANNI_PARADO_1_PATH = SPRITES_FOLDER + "ernanni_parado_1.png";
std::string ERNANNI_PARADO_2_PATH = SPRITES_FOLDER + "ernanni_parado_2.png";

//sprites do ernanni correndo   
std::string ERNANNI_CORRENDO_1_PATH = SPRITES_FOLDER + "ernanni_correndo_1.png";
std::string ERNANNI_CORRENDO_2_PATH = SPRITES_FOLDER + "ernanni_correndo_2.png";
std::string ERNANNI_CORRENDO_3_PATH = SPRITES_FOLDER + "ernanni_correndo_3.png";
std::string ERNANNI_CORRENDO_4_PATH = SPRITES_FOLDER + "ernanni_correndo_4.png";
std::string ERNANNI_CORRENDO_5_PATH = SPRITES_FOLDER + "ernanni_correndo_5.png";








//ERNAS.CPP

Ernanni::Ernanni(int x, int y, int speed) : /*renderer(renderer),*/ speed(speed)
{
    UpdateTexture(PARADO_1);
    
    rect = {x, y, ERNANNI_WIDTH, ERNANNI_HEIGTH};
    
    animationCounter = 1;
}

void Ernanni::Update(){
    /*if(isMoving){
        rect.x += speed;

        if(animationCounter <= 50)
            UpdateTexture(CORRENDO_1);
        else if(animationCounter <= 100)
            UpdateTexture(CORRENDO_2);
        else if(animationCounter <= 150)
            UpdateTexture(CORRENDO_3);
        else if(animationCounter <= 200)
            UpdateTexture(CORRENDO_4);
        else if(animationCounter <= 250)
            UpdateTexture(CORRENDO_5);

        animationCounter++;
        if(animationCounter > 100)
            animationCounter = 1;
    }

    else{ //not moving
        if(animationCounter <= 50)
            UpdateTexture(PARADO_1);
        else if(animationCounter <= 100)
            UpdateTexture(PARADO_2);

        animationCounter++;
        if(animationCounter > 100)
            animationCounter = 1;
    }
    */
    UpdateTexture(PARADO_1);
}

void Ernanni::UpdateTexture(int state)
{
    //this->state = state;

    /*if(state == PARADO_1)
        spritePath = ERNANNI_PARADO_1_PATH;
    
    else if(state == PARADO_2)
        spritePath = ERNANNI_PARADO_2_PATH;

    else if(state == CORRENDO_1)
        spritePath = ERNANNI_CORRENDO_1_PATH;

    else if(state == CORRENDO_2)
        spritePath = ERNANNI_CORRENDO_2_PATH;

    else if(state == CORRENDO_3)
        spritePath = ERNANNI_CORRENDO_3_PATH;
    
    else if(state == CORRENDO_4)
        spritePath = ERNANNI_CORRENDO_4_PATH;
    
    else if(state == CORRENDO_5)
        spritePath = ERNANNI_CORRENDO_5_PATH;
    
    else{
        std::cout << "Ernanni::Update Texture Error : Invalid State\n";
        exit(EXIT_FAILURE);
    }*/

    std::string spritePath = "sprites/ernanni_parado_1.png";
    SDL_Surface* surface = IMG_Load(spritePath.c_str());
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void Ernanni::Render(){
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

void Ernanni::UpdateMoving(){
    /*isMoving = !isMoving;
    animationCounter = 1;*/
}














// MAIN //
const int SCREEN_HEIGHT = 400;
const int SCREEN_WIDTH = 800;

SDL_Window* window = NULL;

bool InitializeSDL(){
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "Fisica 2.0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
    );

    if(!window){
        std::cout << "SDL_CreateWindow Error " << SDL_GetError << std::endl;
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        std::cout << "SDL_CreateRenderer Error " << SDL_GetError << std::endl;
        SDL_Quit();
        return false;
    }

    return true;
}

int main(){

    if(!InitializeSDL()){
        return 1;
    }

    bool quit = false;
    SDL_Event event;

    const int GROUND_HEIGHT = SCREEN_HEIGHT - 50;
    const int ernas_speed = 2;

    Ernanni ernas = Ernanni(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, ernas_speed);

    while(!quit){
        while(SDL_PollEvent(&event) != 0){
            if(event.type == SDL_QUIT){
                quit = true;
            }
            else if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_RIGHT){
                    ernas.UpdateMoving();
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        
        ernas.Update();
        ernas.Render();

        SDL_RenderPresent(renderer);
        SDL_Delay(3);
    }

    SDL_Quit();

    return 0;
}