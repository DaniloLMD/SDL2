#include "../include/includes.hpp"
#include "../include/ernanni.hpp"
#include "../include/coordenadas.hpp"

Ernanni::Ernanni(SDL_Renderer* renderer, int x, int y, int Xspeed) : renderer(renderer), Xspeed(Xspeed)
{
    state = PARADO_1;
    rect = {x, y, ERNANNI_WIDTH, ERNANNI_HEIGTH};
    animationCounter = 1;
    movement = PARADO;
    caindo = false;

    for(int i = 0; i < END_ANIMATION_STATE; i++){
        textures[i] = LoadTexture(i);
    }

}

void Ernanni::Teleport(int x, int y){
    rect.x = x;
    rect.y = y;
}

void Ernanni::Update(){

    if(movement == PARADO){ 
        if(animationCounter <= 50)
            state = PARADO_1;
        else if(animationCounter <= 100)
            state = PARADO_2;

        animationCounter++;
        if(animationCounter > 100)
            animationCounter = 1;
    }


    else if(movement == CORRENDO_RIGHT){
        rect.x += Xspeed;

            if(animationCounter <= 25)
                state = CORRENDO_1_RIGHT;
            else if(animationCounter <= 50)
                state = CORRENDO_2_RIGHT;
            else if(animationCounter <= 75)
                state = CORRENDO_3_RIGHT;
            else if(animationCounter <= 100)
                state = CORRENDO_4_RIGHT;
            else if(animationCounter <= 125)
                state = CORRENDO_5_RIGHT;

        animationCounter++;
        if(animationCounter > 125)
            animationCounter = 1;
    }

    else if(movement == CORRENDO_LEFT){
        rect.x += Xspeed;

        if(animationCounter <= 25)
            state = CORRENDO_1_LEFT;
        else if(animationCounter <= 50)
            state = CORRENDO_2_LEFT;
        else if(animationCounter <= 75)
            state = CORRENDO_3_LEFT;
        else if(animationCounter <= 100)
            state = CORRENDO_4_LEFT;
        else if(animationCounter <= 125)
            state = CORRENDO_5_LEFT;

        animationCounter++;
        if(animationCounter > 125)
            animationCounter = 1;
    }


    if(isJumping){
        
        if(this->caindo){
            GRAVITY = GRAVITY2;
        }
        else GRAVITY = GRAVITY1;

        if(animationCounter <= 30)
            state = PULANDO_1;
        else if(animationCounter <= 60)
            state = PULANDO_2;
        else if(animationCounter <= 90)
            state = PULANDO_3;
        
        animationCounter++;
        if(animationCounter > 90)
            animationCounter = 1;

        bool caindo  = false;
        Yspeed += GRAVITY;
        if(Yspeed >= 0){
            caindo = true;
        }

        if(!caindo) rect.y += (int) Yspeed;
        else{
            state = PULANDO_4;
            if(this->caindo) rect.y += (int) Yspeed * 10;
            else rect.y += (int) Yspeed/4;
        }

        if(rect.y + rect.h > SCREEN_HEIGHT){
            rect.y = SCREEN_HEIGHT - ERNANNI_HEIGTH;
            Yspeed = 0.0;
            isJumping = false;
            animationCounter = 1;
        }

    }
    else this->caindo = false;
}

SDL_Texture* Ernanni::LoadTexture(int state)
{
    std::string spritePath;
    if(state == PARADO_1)
        spritePath = ERNANNI_PARADO_1_PATH;
    
    else if(state == PARADO_2)
        spritePath = ERNANNI_PARADO_2_PATH;



    else if(state == CORRENDO_1_RIGHT){
        spritePath = ERNANNI_CORRENDO_1_PATH;
    }
    else if(state == CORRENDO_1_LEFT){
        spritePath = ERNANNI_CORRENDO_ESQUERDA_1_PATH;
    }

    else if(state == CORRENDO_2_RIGHT){
        spritePath = ERNANNI_CORRENDO_2_PATH;
    }
    else if(state == CORRENDO_2_LEFT){
        spritePath = ERNANNI_CORRENDO_ESQUERDA_2_PATH;
    }

    else if(state == CORRENDO_3_RIGHT){
        spritePath = ERNANNI_CORRENDO_3_PATH;
    }
    else if(state == CORRENDO_3_LEFT){
        spritePath = ERNANNI_CORRENDO_ESQUERDA_3_PATH;
    }

    else if(state == CORRENDO_4_RIGHT){
        spritePath = ERNANNI_CORRENDO_4_PATH;
    }
    else if(state == CORRENDO_4_LEFT){
        spritePath = ERNANNI_CORRENDO_ESQUERDA_4_PATH;
    }

    else if(state == CORRENDO_5_RIGHT){
        spritePath = ERNANNI_CORRENDO_5_PATH;
    }
    else if(state == CORRENDO_5_LEFT){
        spritePath = ERNANNI_CORRENDO_ESQUERDA_5_PATH;
    }

    
    else if(state == PULANDO_1)
        spritePath = ERNANNI_PULANDO_1_PATH;

    else if(state == PULANDO_2)
        spritePath = ERNANNI_PULANDO_2_PATH;
    
    else if(state == PULANDO_3)
        spritePath = ERNANNI_PULANDO_3_PATH;

    else if(state == PULANDO_4)
        spritePath = ERNANNI_PULANDO_4_PATH;
    
    else{
        std::cout << "Ernanni::Update Texture Error : Invalid State\n";
        exit(EXIT_FAILURE);
    }

    SDL_Surface* surface = IMG_Load(spritePath.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void Ernanni::Render(){
    SDL_RenderCopy(renderer, textures[state], NULL, &rect);
}

void Ernanni::UpdateMovement(int movement)
{   
    animationCounter = 1;
    this->movement = movement;
}

void Ernanni::Jump(){
    if(!isJumping){
        animationCounter = 1;
        Yspeed = -JUMP_FORCE;
        isJumping = true;
    }
}

void Ernanni::changeMovementDirection(int direction){
    if(direction != LEFT && direction != RIGHT){
        std::cout << "ERROR in Ernanni::changeMovementDirection : invalid direction";
        exit(EXIT_FAILURE);
    }
    if(movementDirection != direction) Xspeed = -Xspeed;
    movementDirection = direction;

}