#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace std;



//----------------------------------------------------//
//----------------  VARIAVEIS GLOBAIS ----------------//
//----------------------------------------------------//



//----------------  Variaveis Normais ----------------//
const char* SPRITES_FOLDER = "sprites/";

bool quit = false; //booleano que controla o evento do botao de fechar o programa

const double GRAVITY = 0.1; //força que puxa o dinossauro pra baixo

const int SCREEN_WIDTH = 800; //largura da janela principal em pixels
const int SCREEN_HEIGHT = 400; //altura da janela principal em pixels
const int GROUND_HEIGHT = SCREEN_HEIGHT-48-40;


int groundImageWidth;
int groundSpeed = 2;
int ground1X = 0;
int ground2X = 0;

//----------------  Variaveis do SDL -----------------//
SDL_Window* window = NULL; //janela principal do programa

SDL_Renderer* renderer = NULL;

SDL_Event event;

//textures
SDL_Texture *dinoTexture = NULL;
SDL_Texture* dinoTexture1 = NULL;
SDL_Texture* dinoTexture2 = NULL;

SDL_Texture* groundTexture1 = NULL;
SDL_Texture* groundTexture2 = NULL;
SDL_Texture* groundTexture3 = NULL;


//----------------  Variaveis do Dino ----------------//
int dinoX = 100;
int dinoY = GROUND_HEIGHT;

double dinoYVelocity = 0.0;
bool isJumping = false;

const double JUMP_FORCE = 6.0; //força de subida do pulo


//----------------------------------------------------//
//----------------  FUNCOES DO SDL -------------------//
//----------------------------------------------------//

/**
 * @brief inicializa o SDL, criando uma janela e um renderer
 * @return true, se conseguiu criar tanto a janela quanto o renderer ou false caso contrario
*/
bool InitializeSDL(){
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "Dino Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
    );

    if(!window){
        cout << "SDL_CreateWindow Error: " << SDL_GetError << endl;
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if(!renderer){
        cout << "SDL_CreateRenderer Error: " << SDL_GetError << endl;
        SDL_Quit();
        return false;
    }

    //SDL_Delay(5000);

    return true;
}   

/**
 * @brief lida com o pulo do dinossauro
*/
void HandleJump(){
    if(!isJumping){
        isJumping = true;
        dinoYVelocity = -JUMP_FORCE;
    }
}

/**
 * @brief atualiza a coordenada Y do dinossauro no pulo
*/
void UpdateDino(){
    //dinoY += static_cast<int>(dinoYVelocity);
    dinoY += dinoYVelocity;
    dinoYVelocity += GRAVITY;

    if(dinoY >= GROUND_HEIGHT){
        dinoY = GROUND_HEIGHT;
        dinoYVelocity = 0.0;
        isJumping = false;
    }
}

/**
 * @brief atualiza a posicao do chao para efeito de movimento
*/
void UpdateGround(){
    ground1X -= groundSpeed;
    ground2X -= groundSpeed;

    //se o chao desaparecer da tela(coordenada X da ponta esquerda
    //menor que 0), voltar para a ponta do outro chao
    if(ground1X + groundImageWidth < 0){
        //ground1X = SCREEN_WIDTH - groundImageWidth;
        ground1X = ground2X + groundImageWidth;    
    }
    if(ground2X + groundImageWidth < 0){
        ground2X = ground1X + groundImageWidth;
    }

}

/**
 * @brief carrega as imagens
*/
bool LoadMedia(){

    //carregando o dinossauro
    string dinoImagePath = SPRITES_FOLDER + string("dino_0.png");
    SDL_Surface* dinoSurface = IMG_Load(dinoImagePath.c_str());

    if(!dinoSurface){
        cout << "Problem loading dino_0.png "<< IMG_GetError() << endl;
        return false;
    }

    dinoTexture = SDL_CreateTextureFromSurface(renderer, dinoSurface);
    SDL_FreeSurface(dinoSurface);

    //carregando o dinossauro 1
    string dinoImagePath1 = SPRITES_FOLDER + string("dino_1.png");
    SDL_Surface* dinoSurface1 = IMG_Load(dinoImagePath1.c_str());

    if(!dinoSurface1){
        cout << "Problem loading dino_1.png "<< IMG_GetError() << endl;
        return false;
    }

    dinoTexture1 = SDL_CreateTextureFromSurface(renderer, dinoSurface1);
    SDL_FreeSurface(dinoSurface1);

//carregando o dinossauro 2
    string dinoImagePath2 = SPRITES_FOLDER + string("dino_2.png");
    SDL_Surface* dinoSurface2 = IMG_Load(dinoImagePath2.c_str());

    if(!dinoSurface2){
        cout << "Problem loading dino_2 .png "<< IMG_GetError() << endl;
        return false;
    }

    dinoTexture2 = SDL_CreateTextureFromSurface(renderer, dinoSurface2);
    SDL_FreeSurface(dinoSurface2);


    //carregando o chao
    string groundImagePath = SPRITES_FOLDER + string("ground2.png"); 
    SDL_Surface* groundSurface = IMG_Load(groundImagePath.c_str());
    if(!groundSurface){
        cout << "Problem loading ground.png "<< IMG_GetError() << endl;
        return false;
    }

    groundTexture1 = SDL_CreateTextureFromSurface(renderer, groundSurface);
    groundTexture2 = SDL_CreateTextureFromSurface(renderer, groundSurface);

    groundImageWidth = groundSurface->w;

    SDL_FreeSurface(groundSurface);

    return true;
} 


int main(int argc, char* argv[]){
    int c = 0;
    if(!InitializeSDL()){
        return 1;
    }

    if(!LoadMedia()){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

        return 1;
    }

    ground2X = ground1X + groundImageWidth;
    while(!quit){ //loop for game loop
        while(SDL_PollEvent(&event) != 0){ //loop que aguarda algum evento
            if(event.type == SDL_QUIT){
                quit = true; 
            }
            else if(event.type == SDL_KEYDOWN){ //evento de qualquer tecla ser pressionada
                //checa se a tela pressionada é a seta pra cima ou o espaco
                {
                if(event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_SPACE)
                    HandleJump();
                }
            }
        }

        SDL_RenderClear(renderer); //limpa o frame atual para carregar o proximo frame

        UpdateGround();

        UpdateDino();

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_Rect dinoRect = {dinoX, dinoY, 44, 48};
        
        if(c <= 50)
            SDL_RenderCopy(renderer, dinoTexture, NULL, &dinoRect);
        else if(c <= 100)
            SDL_RenderCopy(renderer, dinoTexture1, NULL, &dinoRect);
        else 
            SDL_RenderCopy(renderer, dinoTexture2, NULL, &dinoRect);
        c++;
        if(c > 150) c = 1;

        SDL_Rect groundRect1 = {ground1X, GROUND_HEIGHT + 25, groundImageWidth, 40};
        SDL_RenderCopy(renderer, groundTexture1, NULL, &groundRect1);

        SDL_Rect groundRect2 = {ground2X, GROUND_HEIGHT + 25, groundImageWidth, 40};
        SDL_RenderCopy(renderer, groundTexture2, NULL, &groundRect2);

        SDL_RenderPresent(renderer);

        SDL_Delay(3);
    }

    SDL_Quit();

    return 0;
}