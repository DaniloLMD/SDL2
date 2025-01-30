#include "../include/sdl_manager.hpp"
#include "../include/cor.hpp"

const int N = 500;
std::vector<SDL_Rect> points;
std::vector<std::set<int>>adj(N);
std::set<int>centroids;
std::vector<int>vis(N,0);
int removido;

int dfs(int v, int p){
    int tam=1;
    vis[v]=1;
    for(int u:adj[v]){
        if(u == p || vis[u] || u==removido) continue;
        tam+=dfs(u,v);
    }

    return tam;
}

void calc_centroid(){
    centroids.clear();

    int target=points.size()/2;
    for(int i=0;i<(int)points.size();i++){
        removido=i;
        int ok=1;

        fill(vis.begin(), vis.begin() + points.size(), 0);

        for(int j=0;j<(int)points.size();j++){
            if(j==i)continue;
            if(vis[j]) continue;


            int tam=dfs(j,-1);

            if(tam>target){
                ok=0;
                break;
            }
        }   

        if(ok){
            centroids.insert(i);
        }
    }
}

int main(int argc, char* argv[]){

    if(!InitializeSDL()){
        printf("Erro na inicializacao do SDL\n");
        return 1;
    }

    bool quit = false;

    int mouseX, mouseY; //coordenadas da posicao do mouse
    int mouse=-1;

    const int POINT_SIZE = 10;
    
    while(!quit){ //loop principal
        while(SDL_PollEvent(&event) != 0){ //loop que consome todos os eventos
            if(event.type == SDL_QUIT){
                quit = true; 
            }

            if(event.type == SDL_MOUSEBUTTONDOWN){
                SDL_GetMouseState(&mouseX, &mouseY);

                SDL_Rect p = {mouseX - POINT_SIZE/2, mouseY - POINT_SIZE/2, POINT_SIZE, POINT_SIZE};
                int clicado = -1;
                for(int i=0;i<(int)points.size();i++){
                        if(check_colision(&p, &points[i])){
                            clicado=i;
                        }
                }
                if(!points.size()){
                    points.push_back(p);
                }
                else if (mouse==-1){
                    for(int i=0;i<(int)points.size();i++){
                        if(check_colision(&p, &points[i])){
                            mouse=i;
                        }
                    }
                }
                else{
                    adj[mouse].insert(points.size());
                    points.push_back(p);
                    mouse=-1;
                }
            }

            if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_z){
                    if(points.size()){
                        points.pop_back();
                        for(int i=0;i<(int)points.size(); i++){
                            adj[i].erase(points.size());
                        }
                    }
                }
            }
        }        

        colorirFundo(verde);

        for(auto p: points){
            colorirRect(&p, preto);
        }
        
        for(int i=0;i<(int)adj.size(); i++){
            for(int j: adj[i]){
                SDL_RenderDrawLine(renderer, points[i].x, points[i].y, points[j].x, points[j].y);
            }
        }
        
        
        calc_centroid();
        for(int i: centroids){
            colorirRect(&points[i], azul);
        }
        
        if(mouse!=-1){
            if(centroids.find(mouse)==centroids.end())
                colorirRect(&points[mouse], vermelho);
            else
                colorirRect(&points[mouse], branco);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_Quit();

    return 0;
}