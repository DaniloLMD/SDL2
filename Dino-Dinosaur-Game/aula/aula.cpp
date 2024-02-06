
#include<SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>

//#include<SDL.h>
//#include <SDL_image.h>
//#include<SDL_ttf.h>


#include<iostream>
#include <string>
#include <vector>


using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 400;

const char* SPRITES_FOLDER = "sprites/";
const double GRAVITY = 0.1;
const double JUMP_FORCE = 6.0;
//const char* FONT_PATH = "arial.ttf";
//const int FONT_SIZE = 30;

bool gameOver = false;


SDL_Texture* dinoTexture = NULL;
SDL_Texture* groundTexture1 = NULL;
SDL_Texture* groundTexture2 = NULL;
SDL_Texture* ptera1Texture = NULL;
SDL_Texture* ptera2Texture = NULL;
SDL_Texture* gameOverTexture = NULL;
SDL_Texture* replayTexture = NULL;

//TTF_Font *font = NULL;


int groundImageWidth = 0;
int groundSpeed = 1;
int ground1X = 0;
int ground2X = 0;

SDL_Window *window = NULL;
SDL_Renderer *renderer=NULL;

// Dino variables
int dinoX = 100;
int dinoY = SCREEN_HEIGHT - 48 - 40;
double dinoYVelocity = 0.0;
bool isJumping = false;


// Cactus class
class Cactus {
public:
    Cactus(int type, int x, int speed);

    void Update();
    void Render();

    SDL_Rect GetHitbox() const { return hitbox; }

private:
    SDL_Texture* texture;
    SDL_Rect hitbox;
    int speed;
};

Cactus::Cactus(int type, int x, int speed) : speed(speed) {
    std::string imagePath;
    int width;

    if (type == 0) { // Small cactus
        imagePath = SPRITES_FOLDER + std::string("cacti-small.png");
        width = 45;
    } else { // Big cactus
        imagePath = SPRITES_FOLDER + std::string("cacti-big.png");
        width = 65;
    }

    SDL_Surface* surface = IMG_Load(imagePath.c_str());
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    hitbox = { x, SCREEN_HEIGHT - 40 - 44, width, 44 };
}

void Cactus::Update() {
    hitbox.x -= speed;
}

void Cactus::Render() {
    SDL_RenderCopy(renderer, texture, NULL, &hitbox);
}

class Ptera {
	public:
	    Ptera(int type, int x,int altitude, int speed);
	
	    void Update();
	    void Render();
	
	    SDL_Rect GetHitbox() const { return hitbox; }
	
	private:
	    SDL_Texture* texture;
	    SDL_Rect hitbox;
	    int speed;
	    int altitude;
};

Ptera::Ptera(int type, int x,int altitude, int speed) : speed(speed),altitude(altitude) {
    std::string imagePath;
    int width;

    if (type == 0) { 
        imagePath = SPRITES_FOLDER + std::string("ptera1.png");
        
    } else { 
        imagePath = SPRITES_FOLDER + std::string("ptera2.png");
        
    }

    SDL_Surface* surface = IMG_Load(imagePath.c_str());
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    hitbox = { x, altitude, 46, 40 };
}


void Ptera::Update() {
    hitbox.x -= speed;
}

void Ptera::Render() {
    SDL_RenderCopy(renderer, texture, NULL, &hitbox);
}


vector<Cactus> cactuses;
vector<Ptera> pteras;

bool InitializeSDL(){
	
	SDL_Init(SDL_INIT_VIDEO);
	
	if(TTF_Init()==-1){
		cout<<"TTF_Init Error"<<TTF_GetError()<<endl;
		return false;
	}
	
	window = SDL_CreateWindow("Dino Game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
	
	if(!window){
		cout<<"SDL_CreateWindow Error "<<SDL_GetError<<endl;
		SDL_Quit();
		return false;
	}
	
	renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
	
	if(!renderer){
		cout<<"SDL_CreateRenderer Error "<<SDL_GetError<<endl;
		SDL_Quit();
		return false;
	}
	
	//SDL_Delay(5000);
	
	return true;
	
	
	
}

bool LoadMedia() {
    // Construct the full paths to the images
    string dinoImagePath = SPRITES_FOLDER + string("dino.png");
    string groundImagePath = SPRITES_FOLDER + string("ground.png");
    string ptera1ImagePath = SPRITES_FOLDER + string("ptera1.png");
    //string ptera2ImagePath = SPRITES_FOLDER + string("ptera2.png");
    
    
    SDL_Surface* dinoSurface = IMG_Load(dinoImagePath.c_str());
    if (!dinoSurface) {
        std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
        return false;
    }
    dinoTexture = SDL_CreateTextureFromSurface(renderer, dinoSurface);
    SDL_FreeSurface(dinoSurface);
    
    SDL_Surface* groundSurface = IMG_Load(groundImagePath.c_str());
    if (!groundSurface) {
        std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
        return false;
    }
    groundTexture1 = SDL_CreateTextureFromSurface(renderer, groundSurface);
    groundTexture2 = SDL_CreateTextureFromSurface(renderer, groundSurface);
    groundImageWidth = groundSurface->w;
    SDL_FreeSurface(groundSurface);
    
    SDL_Surface* ptera1Surface = IMG_Load(ptera1ImagePath.c_str());
    if (!ptera1Surface) {
        std:ptera2:cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
        return false;
    }
    ptera1Texture = SDL_CreateTextureFromSurface(renderer, ptera1Surface);
    SDL_FreeSurface(ptera1Surface);
    /*SDL_Surface* ptera2Surface = IMG_Load(ptera2ImagePath.c_str());
    if (!ptera2Surface) {
        std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
        return false;
    }
    
    ptera2Texture = SDL_CreateTextureFromSurface(renderer, ptera2Surface);
    SDL_FreeSurface(ptera2Surface);
    */


    /*font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    
    if(!font){
    	cout<<"TTF Open Font Error"<<TTF_GetError()<<endl;
    	return false;
	}*/
    return true;
}

void RenderText(const char* text,SDL_Texture*& texture, SDL_Rect& destRect){
	SDL_Color textColor = {255,0,0};
	//SDL_Surface *textSurface = TTF_RenderText_Solid(font,text,textColor);
	//texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	
	//destRect.w = textSurface->w;
	//destRect.h= textSurface->h;
	
	//SDL_FreeSurface(textSurface);
	
	destRect.x = (SCREEN_WIDTH - destRect.w)/2;
	
}

// Update ground positions for continuous scrolling
void UpdateGround() {
    ground1X -= groundSpeed;
    ground2X -= groundSpeed;

    if (ground1X + groundImageWidth < 0) {
        ground1X = ground2X + groundImageWidth;
    }
    else if (ground2X + groundImageWidth < 0) {
        ground2X = ground1X + groundImageWidth;
    }
}

// Handle Dino jump
void HandleJump() {
    if (!isJumping) {
        dinoYVelocity = -JUMP_FORCE;
        isJumping = true;
    }
}
bool checkCollision(const SDL_Rect &rect1, const SDL_Rect &rect2){
	if(rect1.y + rect1.h<rect2.y || rect1.y > rect2.y+ rect2.h || rect1.x + rect1.w<rect2.x || rect1.x > rect2.x+ rect2.w ){
		return false;
	}
	return true;
}
void RenderGameOverText(){
	SDL_Rect gameOverRect;
	SDL_Rect replayRect;
	
	gameOverRect.y = SCREEN_HEIGHT/2 - 30;
	replayRect.y = SCREEN_HEIGHT/2 + 50;
	
	//RenderText("Game Over",gameOverTexture,gameOverRect);
	//RenderText("Press Spacebar to Replay",replayTexture,replayRect);
	
	SDL_RenderCopy(renderer,gameOverTexture,NULL,&gameOverRect);
	SDL_RenderCopy(renderer,replayTexture,NULL,&replayRect);
	

}

// Update Dino position and simulate gravity
void UpdateDino() {
    dinoY += static_cast<int>(dinoYVelocity);
    dinoYVelocity += GRAVITY;

    if (dinoY >= SCREEN_HEIGHT - 48 - 40) {
        dinoY = SCREEN_HEIGHT - 48 - 40;
        dinoYVelocity = 0.0;
        isJumping = false;
    }
}


bool quit = false;
SDL_Event event;

void ResetGame(){
	cactuses.clear();
	pteras.clear();
	cactuses.push_back(Cactus(0, 800, groundSpeed)); // Small cactus, initially off-screen
    cactuses.push_back(Cactus(1, 1200, groundSpeed)); // Big cactus, initially off-screen
    
    dinoY = SCREEN_HEIGHT-48-40;
    dinoYVelocity = 0;
    
    isJumping = false;
    groundSpeed = 2;
    ground1X = 0;
    ground2X = SCREEN_WIDTH;
    gameOver = false;

	
}

int main(int argc, char *args[]){
	
	if(!InitializeSDL()){
		return 1;
	}
	
	if (!LoadMedia()) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
		SDL_Quit();
        return 1;
    }
    cactuses.push_back(Cactus(0, 800, groundSpeed)); // Small cactus, initially off-screen
    cactuses.push_back(Cactus(1, 1200, groundSpeed)); // Big cactus, initially off-screen

	
	while(!quit){
		
		while(SDL_PollEvent(&event)!=0){
			if(event.type == SDL_QUIT){
				quit = true;
			}
			else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_UP /*|| event.key.keysym.sym == SDLK_SPACE*/) {
                    HandleJump();
                }
            }
            else if(event.type == SDL_KEYUP){
            	if(gameOver){
            		if (event.key.keysym.sym == SDLK_SPACE) {
                    	ResetGame();
                	
					}
				}
            	
			}
		}
		if(!gameOver){
			
			UpdateDino();
			UpdateGround();
	
			for (size_t i = 0; i < cactuses.size(); ++i) {
	            cactuses[i].Update();
	            if(checkCollision({dinoX,dinoY,44,48},cactuses[i].GetHitbox())){
	            	gameOver=true;
				}
	
	    	}
	    //Spawn pteras periodically
	    	if(SDL_GetTicks()%3000==0){ //Adjust the timing as needed
	        	int type = rand()%2;  //randomly chosing ptera type
	        	int altitude = rand()%100 + 100; //random altitudes
	        	pteras.push_back(Ptera(type,SCREEN_WIDTH,altitude,groundSpeed));
			}
		
			for (size_t i = 0; i < pteras.size(); ++i) {
	                pteras[i].Update();
	                if(checkCollision({dinoX,dinoY,44,48},pteras[i].GetHitbox())){
	                	gameOver=true;
					}
	        }   
			
		}
		
		
        
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        
        SDL_Rect groundRect1 = { ground1X, SCREEN_HEIGHT - 40, groundImageWidth, 40 };
        SDL_RenderCopy(renderer, groundTexture1, NULL, &groundRect1);

        SDL_Rect groundRect2 = { ground2X, SCREEN_HEIGHT - 40, groundImageWidth, 40 };
        SDL_RenderCopy(renderer, groundTexture2, NULL, &groundRect2);


		SDL_Rect dinoRect = { dinoX, dinoY, 44, 48 };
        SDL_RenderCopy(renderer, dinoTexture, NULL, &dinoRect);
        
        for (size_t i = 0; i < cactuses.size(); ++i) {
            cactuses[i].Render();
            if (cactuses[i].GetHitbox().x + cactuses[i].GetHitbox().w < 0) {
                cactuses[i] = Cactus(rand() % 2, SCREEN_WIDTH, groundSpeed);
            }
        }
        
        for (size_t i = 0; i < pteras.size(); ++i) {
            pteras[i].Render();
            if (pteras[i].GetHitbox().x + pteras[i].GetHitbox().w < 0) {
                pteras.erase(pteras.begin()+i);
            }
        }
        if(gameOver){
        	RenderGameOverText();
		}
        
        SDL_RenderPresent(renderer);
        
        SDL_Delay(1);

	}
	SDL_DestroyTexture(dinoTexture);
	SDL_DestroyTexture(groundTexture1);
	SDL_DestroyTexture(groundTexture2);
	SDL_DestroyTexture(gameOverTexture);
	SDL_DestroyTexture(ptera1Texture);
	SDL_DestroyTexture(ptera2Texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	//TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
		
	
}