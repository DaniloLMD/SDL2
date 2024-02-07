
#include<SDL2/SDL.h>
#include<iostream>
#include <SDL2/SDL_image.h>
#include <vector>
#include <string>


using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 400;

const char* SPRITES_FOLDER = "sprites/";
const double GRAVITY = 0.1;
const double JUMP_FORCE = 6.0;

SDL_Texture* dinoTexture = NULL;
SDL_Texture* groundTexture1 = NULL;
SDL_Texture* groundTexture2 = NULL;

int groundImageWidth = 0;
int groundSpeed = 2;
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
        imagePath = SPRITES_FOLDER + std::string("cactus_small.png");
        width = 45;
    } else { // Big cactus
        imagePath = SPRITES_FOLDER + std::string("cactus_big.png");
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

vector<Cactus> cactuses;

bool InitializeSDL(){
	
	SDL_Init(SDL_INIT_VIDEO);
	
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
    string dinoImagePath = SPRITES_FOLDER + string("dino_0.png");
    string groundImagePath = SPRITES_FOLDER + string("ground.png");
    
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

    return true;
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
                if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_SPACE) {
                    HandleJump();
                }
            }
		}
		UpdateDino();
		UpdateGround();
		
		for (size_t i = 0; i < cactuses.size(); ++i) {
                cactuses[i].Update();

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
        
        SDL_RenderPresent(renderer);
        
        SDL_Delay(3);

	}
	SDL_Quit();
	
	
	
	
	
	
	
	
}