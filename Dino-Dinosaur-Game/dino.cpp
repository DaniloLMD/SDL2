#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace std;

#define SMALL_CACTUS 0 
#define BIG_CACTUS 1

//----------------------------------------------------//
//----------------  VARIAVEIS GLOBAIS ----------------//
//----------------------------------------------------//



//----------------  Variaveis Normais ----------------//
const char* SPRITES_FOLDER = "sprites/";

bool quit = false; //booleano que controla o evento do botao de fechar o programa

const double GRAVITY = 0.05; //força que puxa o dinossauro pra baixo

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
SDL_Texture* ptera1Texture = NULL;
SDL_Texture* ptera2Texture = NULL;



//----------------  Variaveis do Dino ----------------//
int dinoX = 100;
int dinoY = GROUND_HEIGHT;

double dinoYVelocity = 0.0;
bool isJumping = false;

const double JUMP_FORCE = 4.0; //força de subida do pulo



//----------------------------------------------------//
//----------------  Classes --------------------------//
//----------------------------------------------------//

class Cactus{
    public:
        Cactus(int type, int x, int speed);

        void Update();
        void Render();

        SDL_Rect GetHitbox() const {return hitbox;}

    private:
        SDL_Texture *texture;
        SDL_Rect hitbox;
        int speed;
};

Cactus::Cactus(int type, int x, int speed) : speed(speed)
{
    string imagePath = SPRITES_FOLDER;
    int width;

    if(type == SMALL_CACTUS){
        imagePath += string("cactus_small.png");
        width = 45;
    }   
    else{
        imagePath += string("cactus_big.png");
        width = 65;
    }
    

    SDL_Surface* surface = IMG_Load(imagePath.c_str());
    texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    hitbox = {x, GROUND_HEIGHT, width, 44};
}

void Cactus::Update(){
    hitbox.x -= speed;
}

void Cactus::Render(){
    SDL_RenderCopy(renderer, texture, NULL, &hitbox);
}

vector<Cactus> cactuses;



class Ptera{
    public:
        Ptera(int type, int x, int y, int speed);

        void Update();
        void Render();

        SDL_Rect GetHitbox() const {return hitbox;}

    private:
        SDL_Texture *texture1;
        SDL_Texture *texture2;
        SDL_Rect hitbox;
        int speed;
        int cont = 0;
};

Ptera::Ptera(int type, int x, int y, int speed) : speed(speed)
{
    string imagePath1 = SPRITES_FOLDER + string("ptera_1.png");
    string imagePath2 = SPRITES_FOLDER + string("ptera_2.png");
    

    SDL_Surface* surface1 = IMG_Load(imagePath1.c_str());
    SDL_Surface* surface2 = IMG_Load(imagePath2.c_str());

    texture1 = SDL_CreateTextureFromSurface(renderer, surface1);
    texture2 = SDL_CreateTextureFromSurface(renderer, surface2);

    SDL_FreeSurface(surface1);
    SDL_FreeSurface(surface2);

    hitbox = {x, y, 46, 40};
}

void Ptera::Update(){
    hitbox.x -= speed;
}

void Ptera::Render(){
    if(cont <= 50){
        SDL_RenderCopy(renderer, texture1, NULL, &hitbox);
    }
    else if(cont <= 100)  {
        SDL_RenderCopy(renderer, texture2, NULL, &hitbox);
    }     
    else cont = 0;   
    cont++;
}

vector<Ptera> pteras;


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


    //carregando ptera1
    string ptera1ImagePath = SPRITES_FOLDER + string("ptera_1.png"); 
    SDL_Surface* ptera1Surface = IMG_Load(groundImagePath.c_str());
    if(!ptera1Surface){
        cout << "Problem loading ptera_1.png "<< IMG_GetError() << endl;
        return false;
    }
    ptera1Texture = SDL_CreateTextureFromSurface(renderer, ptera1Surface);
    SDL_FreeSurface(ptera1Surface);

    //carregando ptera2
    string ptera2ImagePath = SPRITES_FOLDER + string("ptera_2.png"); 
    SDL_Surface* ptera2Surface = IMG_Load(groundImagePath.c_str());
    if(!ptera2Surface){
        cout << "Problem loading ptera_2.png "<< IMG_GetError() << endl;
        return false;
    }
    ptera2Texture = SDL_CreateTextureFromSurface(renderer, ptera2Surface);
    SDL_FreeSurface(ptera2Surface);


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

    cactuses.push_back(Cactus(SMALL_CACTUS, 800, groundSpeed)); //small cactus
    cactuses.push_back(Cactus(BIG_CACTUS, 1200, groundSpeed)); //big cactus

    ground2X = ground1X + groundImageWidth;
    while(!quit){ //loop for game loop
        while(SDL_PollEvent(&event) != 0){ //loop que aguarda algum evento
            if(event.type == SDL_QUIT){
                quit = true; 
            }
            else if(event.type == SDL_KEYDOWN){ //evento de qualquer tecla ser pressionada
                //checa se a tela pressionada é a seta pra cima ou o espaco
                if(event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_SPACE)
                {
                    HandleJump();
                }
            }
        }

        //spawna pteras periodicamente
        if(SDL_GetTicks() % 3000 == 0){
            pteras.push_back(Ptera(rand() %2, SCREEN_WIDTH, rand()%100 + 100, groundSpeed));
        }

        for(int i = 0; i < pteras.size(); i++){
            pteras[i].Update();
        }

        for(int i = 0; i < cactuses.size(); i++){
            cactuses[i].Update();
        }



        UpdateGround();

        UpdateDino();

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_RenderClear(renderer); //limpa o frame atual para carregar o proximo frame
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

        for(int i = 0; i < cactuses.size(); i++){
            cactuses[i].Render();
            if(cactuses[i].GetHitbox().x + cactuses[i].GetHitbox().w < 0){
                cactuses[i] = Cactus(rand()%2, SCREEN_WIDTH, groundSpeed);
            }
        }

        for(int i = 0; i < pteras.size(); i++){
            pteras[i].Render();
            if(pteras[i].GetHitbox().x + pteras[i].GetHitbox().w < 0){
                pteras.erase(pteras.begin() + i);
            }
        }
        

        SDL_RenderPresent(renderer);

        SDL_Delay(3);
    }

    printf("Tempo = %.2lfs\n", (double) SDL_GetTicks()/1000);

    SDL_Quit();

    return 0;
}