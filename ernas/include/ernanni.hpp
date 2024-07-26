#ifndef __ERNANNI_HPP__
#define __ERNANNI_HPP__

#include "includes.hpp"
#include "coordenadas.hpp"

const int ERNANNI_WIDTH = 150;
const int ERNANNI_HEIGTH = 200;


const int NUM_STATES = 11; 
enum ernanniAnimationState{
  PARADO_1, 
  PARADO_2,
  CORRENDO_1,
  CORRENDO_2,
  CORRENDO_3,
  CORRENDO_4,
  CORRENDO_5,
  PULANDO_1,
  PULANDO_2,
  PULANDO_3,
  PULANDO_4
};

enum ernanniAction{
    PARADO,
    CORRENDO,
    PULANDO
};

enum ernannniDirection{
    RIGHT,
    LEFT
};

class Ernanni{
    public:
        Ernanni(SDL_Renderer* renderer, int x, int y, int speed);

        void Update();
        void Render();
        void UpdateTexture(int state);
        void UpdateMovement(int movement);

        void Teleport(int x, int y);

        SDL_Rect GetRect() {return rect;}
        int GetMovement() {return movement;}

        void changeMovementDirection(int direction);
        int GetMovementDirection() {return movementDirection;}

        void Jump();

        int caindo;

    private:
        SDL_Texture* texture;
        SDL_Renderer* renderer;
        SDL_Rect rect;
        int movement;
        bool isJumping;
        int state;
        int animationCounter;

        double Xspeed = 0;
        double Yspeed = 0;
        double JUMP_FORCE = 10.0;
        int movementDirection;
};

//sprites do ernanni parado
std::string ERNANNI_PARADO_1_PATH = SPRITES_FOLDER + "ernanni_parado_1.png";
std::string ERNANNI_PARADO_2_PATH = SPRITES_FOLDER + "ernanni_parado_2.png";

//sprites do ernanni correndo para direita
std::string ERNANNI_CORRENDO_1_PATH = SPRITES_FOLDER + "ernanni_correndo_1.png";
std::string ERNANNI_CORRENDO_2_PATH = SPRITES_FOLDER + "ernanni_correndo_2.png";
std::string ERNANNI_CORRENDO_3_PATH = SPRITES_FOLDER + "ernanni_correndo_3.png";
std::string ERNANNI_CORRENDO_4_PATH = SPRITES_FOLDER + "ernanni_correndo_4.png";
std::string ERNANNI_CORRENDO_5_PATH = SPRITES_FOLDER + "ernanni_correndo_5.png";

//sprites do ernanni correndo para esquerda
std::string ERNANNI_CORRENDO_ESQUERDA_1_PATH = SPRITES_FOLDER + "ernanni_correndo_esquerda_1.png";
std::string ERNANNI_CORRENDO_ESQUERDA_2_PATH = SPRITES_FOLDER + "ernanni_correndo_esquerda_2.png";
std::string ERNANNI_CORRENDO_ESQUERDA_3_PATH = SPRITES_FOLDER + "ernanni_correndo_esquerda_3.png";
std::string ERNANNI_CORRENDO_ESQUERDA_4_PATH = SPRITES_FOLDER + "ernanni_correndo_esquerda_4.png";
std::string ERNANNI_CORRENDO_ESQUERDA_5_PATH = SPRITES_FOLDER + "ernanni_correndo_esquerda_5.png";

//sprites do ernanni pulando
std::string ERNANNI_PULANDO_1_PATH = SPRITES_FOLDER + "ernanni_pulando_1.png";
std::string ERNANNI_PULANDO_2_PATH = SPRITES_FOLDER + "ernanni_pulando_2.png";
std::string ERNANNI_PULANDO_3_PATH = SPRITES_FOLDER + "ernanni_pulando_3.png";
std::string ERNANNI_PULANDO_4_PATH = SPRITES_FOLDER + "ernanni_pulando_4.png";

#endif

enum numeros{
    ZERO = 7,
    UM, //8
    DOIS  //2
};

void print_num(int num){
    if(num == ZERO) printf("0");
    if(num == UM) printf("1");
    if(num == DOIS) printf("2");

}