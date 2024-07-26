#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>

using namespace std;

const int FRAME_WIDTH = 40;
const int FRAME_HEIGHT = 50;
const int NUM_FRAMES = 6;
const Uint32 ANIMATION_SPEED = 100;

SDL_Texture* LoadTexture(const string& path, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        cerr << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        cerr << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << endl;
    }

    return texture;
}

void UpdateAnimation(Uint32& lastFrameChangeTime, int& currentFrame) {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime > lastFrameChangeTime + ANIMATION_SPEED) {
        currentFrame = (currentFrame + 1) % NUM_FRAMES;
        lastFrameChangeTime = currentTime;
    }
}

int main(int argc, char* argv[]) {
    // Inicialização do SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL2 Animation Example",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          800, 600, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    string spriteFilePath = "luffy_sprites.png";
    SDL_Texture* spriteSheet = LoadTexture(spriteFilePath, renderer);
    if (!spriteSheet) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    bool quit = false;
    SDL_Event e;
    Uint32 lastFrameChangeTime = 0;
    int currentFrame = 0;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        UpdateAnimation(lastFrameChangeTime, currentFrame);

        SDL_Rect srcRect = { currentFrame * FRAME_WIDTH, 50, FRAME_WIDTH, FRAME_HEIGHT };
        SDL_Rect dstRect = { 200, 100, 200, 200};

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, spriteSheet, &srcRect, &dstRect);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(spriteSheet);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
