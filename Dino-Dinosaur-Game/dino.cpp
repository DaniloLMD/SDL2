#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <unistd.h>

using namespace std;

#define SMALL_CACTUS 0 
#define BIG_CACTUS 1

//----------------------------------------------------//
//----------------  VARIAVEIS GLOBAIS ----------------//
//----------------------------------------------------//



//----------------  Variaveis Normais ----------------//
const char* SPRITES_FOLDER = "sprites/";

bool quit = false; //booleano que controla o evento do botao de fechar o programa
bool gameOver = false; //booleano que controla se houve colisao

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
SDL_Texture* dinoDuckingTexture = NULL;
SDL_Texture* dinoDucking2Texture = NULL;

SDL_Texture* groundTexture1 = NULL;
SDL_Texture* groundTexture2 = NULL;
SDL_Texture* ptera1Texture = NULL;
SDL_Texture* ptera2Texture = NULL;
SDL_Texture* gameOverTexture = NULL;
SDL_Texture* replayTexture = NULL;

//------------ fontes ---------------------
const char* FONT_PATH = "fonts/aakar-medium.ttf";
const int FONT_SIZE = 28; //tamanho da fonte em pixels

TTF_Font* font = NULL;


//----------------  Variaveis do Dino ----------------//
int dinoX = 100;
int dinoY = GROUND_HEIGHT;

double dinoYVelocity = 0.0;
bool isJumping = false;
bool isDucking = false;

const double NORMAL_JUMP_FORCE = 6.0; //força de subida do pulo
const double DUCKING_JUMP_FORCE = 5.0; //força de subida do pulo enquanto ta agachado
double JUMP_FORCE = 6.0;

int DINO_HEIGTH = 44;
int DINO_WIDTH = 48;

int DINO_DUCKING_HEIGTH = 22;
int DINO_DUCKING_WIDTH = 60;


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

    //inicializando a biblioteca de fonte(Texto)
    if( TTF_Init() == -1 ){
        cout << "TTF_Init Error " << TTF_GetError() << endl;
        return false;
    }

    //inicializando a janela
    window = SDL_CreateWindow(
        "Dino Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
    );

    if(!window){
        cout << "SDL_CreateWindow Error: " << SDL_GetError << endl;
        SDL_Quit();
        return false;
    }

    //inicializando o renderer (para carregar coisas na janela)
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        cout << "SDL_CreateRenderer Error: " << SDL_GetError << endl;
        SDL_Quit();
        return false;
    }



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
 * @brief recebe dois rect e retorna se um rect encosta no outro
 * @param rect1 primeiro rect, ex: rect do dinossauro
 * @param rect2 segundo rect, ex: rect do cacto ou do ptera
 * @return true, se colidiram, false caso contrario
*/
bool checkCollision(SDL_Rect rect1, SDL_Rect rect2){
    //1 caso: o rect1 esta abaixo do rect2
    if(rect1.y + rect1.h < rect2.y){
        return false;
    }
    //2 caso: o rect1 esta acima do rect2
    else if(rect1.y > rect2.h + rect2.y){
        return false;
    }
    //3 caso: o rect1 esta a esquerda do rect2
    else if(rect1.x + rect1.w < rect2.x){
        return false;
    }
    //4 caso: o rect1 esta a direita do rect2
    else if(rect1.x > rect2.x + rect2.w){
        return false;
    }

    //5 caso: houve colisao
    return true;
    
}

/**
 * @brief atualiza a coordenada Y do dinossauro no pulo
*/
void UpdateDino(){

    (isDucking) ? JUMP_FORCE = DUCKING_JUMP_FORCE : JUMP_FORCE = NORMAL_JUMP_FORCE;
    
    //dinoY += static_cast<int>(dinoYVelocity);
    dinoY += (int) dinoYVelocity;
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
        cout << "Problem loading dino_2.png "<< IMG_GetError() << endl;
        return false;
    }

    dinoTexture2 = SDL_CreateTextureFromSurface(renderer, dinoSurface2);
    SDL_FreeSurface(dinoSurface2);

    //carregando o dinossauro abaixado 1
    string dinoDuckingImagePath = SPRITES_FOLDER + string("dino_ducking_1.png");
    SDL_Surface* dinoDuckingSurface = IMG_Load(dinoDuckingImagePath.c_str());

    if(!dinoDuckingSurface){
        cout << "Problem loading dino_ducking_1.png "<< IMG_GetError() << endl;
        return false;
    }

    dinoDuckingTexture = SDL_CreateTextureFromSurface(renderer, dinoDuckingSurface);
    SDL_FreeSurface(dinoDuckingSurface);

     //carregando o dinossauro abaixado 2
    string dinoDucking2ImagePath = SPRITES_FOLDER + string("dino_ducking_2.png");
    SDL_Surface* dinoDucking2Surface = IMG_Load(dinoDucking2ImagePath.c_str());

    if(!dinoDucking2Surface){
        cout << "Problem loading dino_ducking_2.png "<< IMG_GetError() << endl;
        return false;
    }

    dinoDucking2Texture = SDL_CreateTextureFromSurface(renderer, dinoDucking2Surface);
    SDL_FreeSurface(dinoDucking2Surface);



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

    //carregando a fonte
    font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    if(!font){
        cout << "TTF_OpenFont Error " << TTF_GetError() << endl;
        return false;
    }


    return true;
} 

void RenderText(const char* text, SDL_Texture*& texture, SDL_Rect& rect){
    SDL_Color textColor = {255, 0, 0}; //cor em rgb

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);
    texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    
    rect.w = textSurface->w;
    rect.h = textSurface->h;
    rect.x = (SCREEN_WIDTH - rect.w) / 2; 

    SDL_FreeSurface(textSurface);
}

void RenderGameoverText(){
    SDL_Rect gameOverRect;
    SDL_Rect replayRect;

    gameOverRect.y = SCREEN_HEIGHT/2 - 30;
    replayRect.y = SCREEN_HEIGHT/2 + 50;

    RenderText("Game Over", gameOverTexture, gameOverRect);
    RenderText("Press Space to Replay", replayTexture, replayRect);

    SDL_RenderCopy(renderer, gameOverTexture, NULL, &gameOverRect);
    SDL_RenderCopy(renderer, replayTexture, NULL, &replayRect); 
}

void ResetGame(){
    gameOver = false;

    cactuses.clear();
    pteras.clear();

    cactuses.push_back(Cactus(SMALL_CACTUS, 800, groundSpeed)); //small cactus
    cactuses.push_back(Cactus(BIG_CACTUS, 1200, groundSpeed)); //big cactus

    ground1X = 0;
    ground2X = ground1X + groundImageWidth; //inicializando o segundo chao no final do primeiro

    dinoYVelocity = 0;
    dinoY = GROUND_HEIGHT;
    isJumping = false;
    isDucking = false;
}


int main(int argc, char* argv[]){

    int c = 0 , d = 0; //contadores para animacao de andar

    if(!InitializeSDL()){
        return 1;
    }

    if(!LoadMedia()){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

        return 1;
    }

    ResetGame();

    while(!quit){ //loop for game loop

        while(SDL_PollEvent(&event) != 0){ //loop que consome todos os eventos
            if(event.type == SDL_QUIT){
                quit = true; 
            }
            else if(event.type == SDL_KEYDOWN){ //evento de qualquer tecla ser pressionada
                //checa se a tela pressionada é a seta pra cima ou o espaco
                if(event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_SPACE)
                {
                    //checa se houve gameOver
                    if(gameOver){
                        ResetGame();
                    }
                    else{ //caso contrario, lida com o pulo dodinossauro
                        HandleJump();
                    } 
                }
                else if(event.key.keysym.sym == SDLK_DOWN){
                    isDucking = !isDucking;
                }
            }
        }

        if(!gameOver){ //atualiza a janela caso nao tenha dado gameOver(colisao)

            UpdateGround();
            UpdateDino();

            //spawna pteras periodicamente
            if(SDL_GetTicks() % 3000 == 0){
                pteras.push_back(Ptera(rand() %2, SCREEN_WIDTH, rand()%100 + 100, groundSpeed));
            }


            //atualiza os ptera
            for(int i = 0; i < pteras.size(); i++){
                pteras[i].Update();
            }

            //atualiza os cacto
            for(int i = 0; i < cactuses.size(); i++){
                cactuses[i].Update();
            }

            int altura_dino;
            (isDucking) ? altura_dino = DINO_DUCKING_HEIGTH : altura_dino = DINO_HEIGTH; 

             int largura_dino;
            (isDucking) ? largura_dino = DINO_DUCKING_WIDTH : largura_dino = DINO_WIDTH; 

            SDL_Rect dinoColisionRect = {dinoX, dinoY, largura_dino, altura_dino};
            //checando colisao de cacto
            for(int i = 0; i < cactuses.size(); i++){
                if(checkCollision(dinoColisionRect, cactuses[i].GetHitbox())){
                    gameOver = true;
                }
            }

            //checando colisao de ptera
            for(int i = 0; i < pteras.size(); i++){
                if(checkCollision(dinoColisionRect, pteras[i].GetHitbox())){
                    gameOver = true;
                }
            }


            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            SDL_RenderClear(renderer); //limpa o frame atual para carregar o proximo frame
            

            SDL_Rect dinoRect = {dinoX, dinoY, DINO_WIDTH, DINO_HEIGTH};
            if(isDucking){
                if(d <= 50)
                    SDL_RenderCopy(renderer, dinoDuckingTexture, NULL, &dinoRect);
                else if(d <= 100)
                    SDL_RenderCopy(renderer, dinoDucking2Texture, NULL, &dinoRect);
                d++;
                if(d > 100) d = 1;
            }

            else{
                if(c <= 50)
                    SDL_RenderCopy(renderer, dinoTexture, NULL, &dinoRect);
                else if(c <= 100)
                    SDL_RenderCopy(renderer, dinoTexture1, NULL, &dinoRect);
                else 
                    SDL_RenderCopy(renderer, dinoTexture2, NULL, &dinoRect);
                c++;
                if(c > 150) c = 1;
            }

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
        } //fim do if(!gameOver);

        if(gameOver){ //se houve gameover, mostra o texto na tela
            RenderGameoverText();
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(3);
    }

    printf("Tempo = %.2lfs\n", (double) SDL_GetTicks()/1000);

    SDL_Quit();

    return 0;
}