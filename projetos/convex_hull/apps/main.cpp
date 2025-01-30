#include "../include/sdl_manager.hpp"
#include "../include/cor.hpp"

struct point {
    double x, y;
};

int orientation(point a, point b, point c) {
    double v = a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y);
    if (v < 0) return -1; // clockwise
    if (v > 0) return +1; // counter-clockwise
    return 0;
}

bool cw(point a, point b, point c, bool include_collinear) {
    int o = orientation(a, b, c);
    return o < 0 || (include_collinear && o == 0);
}
bool ccw(point a, point b, point c, bool include_collinear) {
    int o = orientation(a, b, c);
    return o > 0 || (include_collinear && o == 0);
}

void calc(std::vector<point>& a, bool include_collinear) {
    if (a.size() <= 1)
        return;

    sort(a.begin(), a.end(), [](point a, point b) {
        return std::make_pair(a.x, a.y) < std::make_pair(b.x, b.y);
    });
    point p1 = a[0], p2 = a.back();
    std::vector<point> up, down;
    up.push_back(p1);
    down.push_back(p1);
    for (int i = 1; i < (int)a.size(); i++) {
        if (i == a.size() - 1 || cw(p1, a[i], p2, include_collinear)) {
            while (up.size() >= 2 && !cw(up[up.size()-2], up[up.size()-1], a[i], include_collinear))
                up.pop_back();
            up.push_back(a[i]);
        }
        if (i == a.size() - 1 || ccw(p1, a[i], p2, include_collinear)) {
            while (down.size() >= 2 && !ccw(down[down.size()-2], down[down.size()-1], a[i], include_collinear))
                down.pop_back();
            down.push_back(a[i]);
        }
    }

    if (include_collinear && up.size() == a.size()) {
        reverse(a.begin(), a.end());
        return;
    }
    a.clear();
    for (int i = 0; i < (int)up.size(); i++)
        a.push_back(up[i]);
    for (int i = down.size() - 2; i > 0; i--)
        a.push_back(down[i]);
}

int main(int argc, char* argv[]){

    if(!InitializeSDL()){
        printf("Erro na inicializacao do SDL\n");
        return 1;
    }

    bool quit = false;

    int mouseX, mouseY; //coordenadas da posicao do mouse

    std::vector<point> points;
    std::vector<point> convex;
    const int POINT_SIZE = 10;

    
    while(!quit){ //loop principal
        while(SDL_PollEvent(&event) != 0){ //loop que consome todos os eventos
            if(event.type == SDL_QUIT){
                quit = true; 
            }

            if(event.type == SDL_MOUSEBUTTONDOWN){
                SDL_GetMouseState(&mouseX, &mouseY);
                point p = {(double) mouseX, (double) mouseY};
                points.push_back(p);
            }

            if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_z){
                    if(points.size()){
                        points.pop_back();
                    }
                }
            }
        }        

        colorirFundo(verde);

        convex.clear();
        for(auto p: points){
            SDL_Rect r = {(int) p.x-POINT_SIZE/2 , (int) p.y-POINT_SIZE/2, POINT_SIZE, POINT_SIZE};
            colorirRect(&r, vermelho);
            convex.push_back(p);
        }
        
        calc(convex,false);
        
        for(int i=0;i<(int)convex.size(); i++){
            int j=(i+1)%convex.size();
            SDL_RenderDrawLine(renderer, convex[i].x, convex[i].y, convex[j].x, convex[j].y);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_Quit();

    return 0;
}