#include "../include/includes.hpp"
#include "../include/ernanni.hpp"
#include "../include/coordenadas.hpp"

Ernanni::Ernanni(SDL_Renderer* renderer, int x, int y, int Xspeed) : renderer(renderer), Xspeed(Xspeed)
{
    UpdateTexture(PARADO_1);
    
    rect = {x, y, ERNANNI_WIDTH, ERNANNI_HEIGTH};
    animationCounter = 1;
    movement = PARADO;
}

void Ernanni::Teleport(int x, int y){
    rect.x = x;
    rect.y = y;
}

void Ernanni::Update(){

    if(movement == PARADO){ 
        if(animationCounter <= 50)
            UpdateTexture(PARADO_1);
        else if(animationCounter <= 100)
            UpdateTexture(PARADO_2);

        animationCounter++;
        if(animationCounter > 100)
            animationCounter = 1;
    }


    else if(movement == CORRENDO){
        rect.x += Xspeed;

        if(movementDirection == RIGHT){
            if(animationCounter <= 25)
                UpdateTexture(CORRENDO_1);
            else if(animationCounter <= 50)
                UpdateTexture(CORRENDO_2);
            else if(animationCounter <= 75)
                UpdateTexture(CORRENDO_3);
            else if(animationCounter <= 100)
                UpdateTexture(CORRENDO_4);
            else if(animationCounter <= 125)
                UpdateTexture(CORRENDO_5);
        }

        else if(movementDirection == LEFT){
            if(animationCounter <= 25)
                UpdateTexture(CORRENDO_1);
            else if(animationCounter <= 50)
                UpdateTexture(CORRENDO_2);
            else if(animationCounter <= 75)
                UpdateTexture(CORRENDO_3);
            else if(animationCounter <= 100)
                UpdateTexture(CORRENDO_4);
            else if(animationCounter <= 125)
                UpdateTexture(CORRENDO_5);
        }

        animationCounter++;
        if(animationCounter > 125)
            animationCounter = 1;
    }

    if(isJumping){

        if(animationCounter <= 30)
            UpdateTexture(PULANDO_1);
        else if(animationCounter <= 60)
            UpdateTexture(PULANDO_2);
        else if(animationCounter <= 90)
            UpdateTexture(PULANDO_3);
        
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
            UpdateTexture(PULANDO_4);
            rect.y += (int) Yspeed/4;
        }

        if(rect.y + rect.h > SCREEN_HEIGHT){
            rect.y = SCREEN_HEIGHT - ERNANNI_HEIGTH;
            Yspeed = 0.0;
            isJumping = false;
            animationCounter = 1;
        }
        if(movement == CORRENDO){
            rect.x += Xspeed;
        }

    }
}

void Ernanni::UpdateTexture(int state)
{
    this->state = state;

    std::string spritePath;
    if(state == PARADO_1)
        spritePath = ERNANNI_PARADO_1_PATH;
    
    else if(state == PARADO_2)
        spritePath = ERNANNI_PARADO_2_PATH;

    else if(state == CORRENDO_1){
        if(movementDirection == RIGHT)
            spritePath = ERNANNI_CORRENDO_1_PATH;
        else
            spritePath = ERNANNI_CORRENDO_ESQUERDA_1_PATH;
    }

    else if(state == CORRENDO_2){
        if(movementDirection == RIGHT)
            spritePath = ERNANNI_CORRENDO_2_PATH;
        else
            spritePath = ERNANNI_CORRENDO_ESQUERDA_2_PATH;
    }

    else if(state == CORRENDO_3){
        if(movementDirection == RIGHT)
            spritePath = ERNANNI_CORRENDO_3_PATH;
        else
            spritePath = ERNANNI_CORRENDO_ESQUERDA_3_PATH;
    }
    
    else if(state == CORRENDO_4){
        if(movementDirection == RIGHT)
            spritePath = ERNANNI_CORRENDO_4_PATH;
        else
            spritePath = ERNANNI_CORRENDO_ESQUERDA_4_PATH;
    }
    
    else if(state == CORRENDO_5){
        if(movementDirection == RIGHT)
            spritePath = ERNANNI_CORRENDO_5_PATH;
        else
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
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void Ernanni::Render(){
    SDL_RenderCopy(renderer, texture, NULL, &rect);
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