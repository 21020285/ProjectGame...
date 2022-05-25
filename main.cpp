#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Function.h"
#include "ChooseWolf.h"
#include <conio.h>
#include <stdio.h>
using namespace std;
int Number[MAX_PLAYER];
int WolfPeople = WOLF_PLAYER;
int VillagePeople = MAX_PLAYER - WOLF_PLAYER;
Mix_Music *gameMusic = NULL;
Mix_Music *WolfKilled = NULL;
Mix_Music *BeLynched = NULL;
Mix_Music *WinnerMusic = NULL;
//const int DEFAULT_COLOR = 255;
SDL_Window* window;
SDL_Renderer* renderer;
bool PlayerAlive[MAX_PLAYER] = {0};
bool PlayerRole[MAX_PLAYER] = {false};
void ChooseWolf(){
    //random mark wolf as 1 and village as 0
    srand(time(0));
    int Wolf = WOLF_PLAYER;
    while(Wolf){
        int Number = rand() % MAX_PLAYER + 1;
        if(!(PlayerRole[Number])){
            PlayerRole[Number] = true;
            Wolf--;
        }
    }
}
int main(int argc, char *argv[]){
    int Day = 1;
    initSDL(window, renderer);
    backgroundOn();
    loadMusic();
    gamePlay();
    infoGame();
    convention();
    placeNumber();
    ChooseWolf();
    for(int i = 1; i <= MAX_PLAYER; i++){
        system("cls");
        cout << "Player " << i << " is ";
        if(PlayerRole[i]) cout << "Wolf!";
        else cout << "Village!";
        cout << "\n" << "Press any key to continue for next player!";
        waitUntilKeyPressed();
        system("cls");
        cout << "Waiting for next player...." << "\n";
        waitUntilKeyPressed();
    }
    do{
    NightAction();
    CheckAlive();
    GetOut();
    if(Result()) break;
    cout << "Day " << Day << " started!" << "\n";
    DayAction();
    Day++;
    GetOut();
    }while(!Result());
    Winner();
    waitUntilKeyPressed();
    Mix_PlayMusic(WinnerMusic,0);
    closeMusic();
    quitSDL(window, renderer);
    return 0;
}
bool loadMusic(){
	bool success = true;
		if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 ){
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ){
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
        }
	gameMusic = Mix_LoadMUS("music/GameMusic.mp3");
	if(gameMusic == NULL){
		printf( "Failed to load game music! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	WolfKilled = Mix_LoadMUS("music/WolfKilled.mp3");
	if(WolfKilled == NULL){
		printf( "Failed to load wolf effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	BeLynched = Mix_LoadMUS("music/BeLynch.mp3");
	if(BeLynched == NULL){
		printf( "Failed to load lynch effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	WinnerMusic = Mix_LoadMUS("music/WinnerMusic.mp3");
		if(WinnerMusic == NULL){
		printf( "Failed to load winner effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	return success;
}
void closeMusic(){
	Mix_FreeMusic(gameMusic);
	Mix_FreeMusic(WolfKilled);
	Mix_FreeMusic(BeLynched);
	Mix_FreeMusic(WinnerMusic);
	gameMusic = NULL;
    WolfKilled = NULL;
    BeLynched = NULL;
    WinnerMusic = NULL;
	Mix_Quit();
}
void backgroundOn(){
    SDL_Texture* background = loadTexture("photo/background.png", renderer);
    SDL_RenderCopy( renderer, background, NULL, NULL);
    SDL_RenderPresent( renderer);
    Music();
    //waitUntilKeyPressed();
    SDL_DestroyTexture(background);
    background = NULL;
}

void gamePlay(){
    SDL_Texture* GamePlay = loadTexture("photo/gameplay.png", renderer);
    SDL_RenderCopy( renderer, GamePlay, NULL, NULL);
    SDL_RenderPresent( renderer );
    waitUntilKeyPressed();
    SDL_DestroyTexture( GamePlay );
    GamePlay = NULL;
}

void infoGame(){
    SDL_Texture* infogame = loadTexture("photo/infogame.png", renderer);
    SDL_RenderCopy( renderer, infogame, NULL, NULL);
    SDL_RenderPresent(renderer);
    waitUntilKeyPressed();
    SDL_DestroyTexture(infogame);
    infogame = NULL;
}

void convention(){
    SDL_Texture* convention = loadTexture("photo/convention.png", renderer);
    SDL_RenderCopy( renderer, convention, NULL, NULL);
    SDL_RenderPresent( renderer );
    waitUntilKeyPressed();
    SDL_DestroyTexture(convention);
    convention = NULL;
}

void placeNumber(){
    SDL_Texture* placePeople = loadTexture("photo/placenumber.png", renderer);
    SDL_RenderCopy( renderer, placePeople, NULL, NULL);
    SDL_RenderPresent( renderer );
    waitUntilKeyPressed();
    SDL_DestroyTexture(placePeople);
    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear(renderer);
}
void NightAction(){
    CheckAlive();
   SDL_Texture* wolfplace = loadTexture("photo/wolfplace.png", renderer);
   for(int i = 1; i <= Column; i++){
     for(int j = 1; j <= Row; j++){
     int Num = j + (i - 1) * 4;
     if(PlayerRole[Num] && !PlayerAlive[Num]){
    SDL_Rect Number[Num];
    Number[Num].w = 720;
    Number[Num].h = 720;
    SDL_QueryTexture(wolfplace, NULL, NULL, &Number[Num].w, &Number[Num].h);
    Number[Num].x = (j - 1) * BUTTON_WIDTH;
    Number[Num].y = (i - 1) * BUTTON_HEIGHT;
    SDL_RenderCopy(renderer, wolfplace, NULL, &Number[Num]);
    }
   }
   }
    SDL_RenderPresent(renderer);

    SDL_Texture* night = loadTexture("photo/night.png", renderer);
    SDL_Rect nightRect;
    nightRect.w = 360;
    nightRect.h = 720;
    SDL_QueryTexture(night, NULL, NULL, &nightRect.w, &nightRect.h);
    nightRect.x = 1080 - BUTTON_WIDTH * 2;
    nightRect.y = 0;
    SDL_RenderCopy( renderer, night, NULL, &nightRect );
    SDL_RenderPresent(renderer);
    ChoosePlayerBeKilledByWolf();

    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear(renderer);

    SDL_DestroyTexture(night);
    night = NULL;
}
void CheckAlive(){
     SDL_Texture* place = loadTexture("photo/place.png", renderer);
   for(int i = 1; i <= Column; i++){
     for(int j = 1; j <= Row; j++){
     int Num = j + (i - 1) * 4;
     if(!PlayerAlive[Num]){
    SDL_Rect Number[Num];
    Number[Num].w = 720;
    Number[Num].h = 720;
    SDL_QueryTexture(place, NULL, NULL, &Number[Num].w, &Number[Num].h);
    Number[Num].x = (j - 1) * BUTTON_WIDTH;
    Number[Num].y = (i - 1) * BUTTON_HEIGHT;
    SDL_RenderCopy(renderer, place, NULL, &Number[Num]);
    }
   }
   }
    SDL_RenderPresent(renderer);
}
void DayAction(){
    SDL_Texture* day = loadTexture("photo/day.png", renderer);
    SDL_Rect dayRect;
    dayRect.w = 360;
    dayRect.h = 720;
    SDL_QueryTexture(day, NULL, NULL, &dayRect.w, &dayRect.h);
    dayRect.x = 1080 - BUTTON_WIDTH * 2;
    dayRect.y = 0;
    SDL_RenderCopy( renderer, day, NULL, &dayRect );
    SDL_RenderPresent(renderer);
    ChoosePlayerBeKilledByLynch();
    SDL_Texture* place = loadTexture("photo/place.png", renderer);
   for(int i = 1; i <= Column; i++){
     for(int j = 1; j <= Row; j++){
     int Num = j + (i - 1) * 4;
     if(!PlayerAlive[Num]){
    SDL_Rect Number[Num];
    Number[Num].w = 720;
    Number[Num].h = 720;
    SDL_QueryTexture(place, NULL, NULL, &Number[Num].w, &Number[Num].h);
    Number[Num].x = (j - 1) * BUTTON_WIDTH;
    Number[Num].y = (i - 1) * BUTTON_HEIGHT;
    SDL_RenderCopy(renderer, place, NULL, &Number[Num]);
    }
   }
}
}
void GetOut(){
   SDL_Texture* placeGetOut = loadTexture("photo/deadplace.png", renderer);
   for(int i = 1; i <= Column; i++){
     for(int j = 1; j <= Row; j++){
     int Num = j + (i - 1) * 4;
     if(PlayerAlive[Num]){
    SDL_Rect Number[Num];
    Number[Num].w = 720;
    Number[Num].h = 720;
    SDL_QueryTexture(placeGetOut, NULL, NULL, &Number[Num].w, &Number[Num].h);
    Number[Num].x = (j - 1) * BUTTON_WIDTH;
    Number[Num].y = (i - 1) * BUTTON_HEIGHT;
    SDL_RenderCopy(renderer, placeGetOut, NULL, &Number[Num]);
    }
   }
   }
    SDL_RenderPresent(renderer);

}
bool Result(){
    if(WolfPeople == 0 || WolfPeople == VillagePeople) return true;
    return false;
}
void Winner(){
    SDL_Texture* villagewin = loadTexture("photo/villagewin.png", renderer);
    SDL_Texture* wolfwin = loadTexture("photo/wolfwin.png", renderer);
    if(WolfPeople == 0){
    SDL_RenderCopy( renderer, villagewin, NULL, NULL);
    SDL_RenderPresent(renderer);
    waitUntilKeyPressed();
    SDL_DestroyTexture(villagewin);
    villagewin = NULL;
    }
    else if(WolfPeople == VillagePeople){
    SDL_RenderCopy( renderer, wolfwin, NULL, NULL);
    SDL_RenderPresent(renderer);
    waitUntilKeyPressed();
    SDL_DestroyTexture(wolfwin);
    wolfwin = NULL;
    }
}

SDL_Texture* loadTexture( string path, SDL_Renderer* renderer ){
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if ( loadedSurface == nullptr )
        cout << "Unable to load image " << path << " SDL_image Error: "
             << IMG_GetError() << endl;
    else {
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
        if( newTexture == nullptr ){
            cout << "Unable to create texture from " << path << " SDL Error: "
                 << SDL_GetError() << endl;
        }
        SDL_FreeSurface( loadedSurface );
    }
    return newTexture;
}

void logSDLError(std::ostream& os, const std::string &msg, bool fatal){
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);
    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);
    //renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));
    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}
void quitSDL(SDL_Window* window, SDL_Renderer* renderer){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
void waitUntilKeyPressed(){
    SDL_Event e;
    bool running = true;
    while(running){
        while(SDL_PollEvent(&e)){
            switch(e.type){
                case SDL_QUIT:
                    exit(1);
                case SDL_KEYDOWN:
                    return;
                    SDL_Delay(50);
            }
        }

}
}
void ChoosePlayerBeKilledByWolf(){
      SDL_Event e;
    bool running = true;
    while(running){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT) exit(1);
            else if(e.type == SDL_KEYDOWN){
            if(e.key.keysym.sym == SDLK_1 && !PlayerRole[1] && !PlayerAlive[1]) {
             PlayerAlive[1] = 1;
                VillagePeople--;
                Mix_PlayMusic(WolfKilled,0);
                return;
                SDL_Delay(1000);
            }
            else if(e.key.keysym.sym == SDLK_2 && !PlayerRole[2] && !PlayerAlive[2]) {
             PlayerAlive[2] = 1;
                VillagePeople--;
                Mix_PlayMusic(WolfKilled,0);
                return;
                SDL_Delay(1000);
            }
            else if(e.key.keysym.sym == SDLK_3 && !PlayerRole[3] && !PlayerAlive[3]){
             PlayerAlive[3] = 1;
                VillagePeople--;
                Mix_PlayMusic(WolfKilled,0);
                return;
                SDL_Delay(1000);
            }
            else if(e.key.keysym.sym == SDLK_4 && !PlayerRole[4] && !PlayerAlive[4]){
             PlayerAlive[4] = 1;
                VillagePeople--;
                Mix_PlayMusic(WolfKilled,0);
                return;
                SDL_Delay(1000);
            }
            else if(e.key.keysym.sym == SDLK_5 && !PlayerRole[5] && !PlayerAlive[5]){
             PlayerAlive[5] = 1;
                VillagePeople--;
                Mix_PlayMusic(WolfKilled,0);
                return;
                SDL_Delay(1000);
            }
            else if(e.key.keysym.sym == SDLK_6 && !PlayerRole[6] && !PlayerAlive[6]){
             PlayerAlive[6] = 1;
                VillagePeople--;
                Mix_PlayMusic(WolfKilled,0);
                return;
                SDL_Delay(1000);
            }
            else if(e.key.keysym.sym == SDLK_7 && !PlayerRole[7] && !PlayerAlive[7]){
             PlayerAlive[7] = 1;
                VillagePeople--;
                Mix_PlayMusic(WolfKilled,0);
                return;
                SDL_Delay(1000);
            }
            else if(e.key.keysym.sym == SDLK_8 && !PlayerRole[8] && !PlayerAlive[8]){
             PlayerAlive[8] = 1;
                VillagePeople--;
                Mix_PlayMusic(WolfKilled,0);
                return;
                SDL_Delay(1000);
            }
            else if(e.key.keysym.sym == SDLK_9 && !PlayerRole[9] && !PlayerAlive[9]){
             PlayerAlive[9] = 1;
                VillagePeople--;
                Mix_PlayMusic(WolfKilled,0);
                return;
                SDL_Delay(1000);
            }
            else if(e.key.keysym.sym == SDLK_0 && !PlayerRole[10] && !PlayerAlive[10]){
             PlayerAlive[10] = 1;
                VillagePeople--;
                Mix_PlayMusic(WolfKilled,0);
                return;
                SDL_Delay(1000);
            }
            else if(e.key.keysym.sym == SDLK_a && !PlayerRole[11] && !PlayerAlive[11]){
             PlayerAlive[11] = 1;
                VillagePeople--;
                Mix_PlayMusic(WolfKilled,0);
                return;
                SDL_Delay(1000);
            }
            else if(e.key.keysym.sym == SDLK_b && !PlayerRole[12] && !PlayerAlive[12]){
             PlayerAlive[12] = 1;
                VillagePeople--;
                Mix_PlayMusic(WolfKilled,0);
                return;
                SDL_Delay(1000);
            }
            else if(e.key.keysym.sym == SDLK_c && !PlayerRole[13] && !PlayerAlive[13]){
             PlayerAlive[13] = 1;
                VillagePeople--;
                Mix_PlayMusic(WolfKilled,0);
                return;
                SDL_Delay(1000);
            }
            else if(e.key.keysym.sym == SDLK_d && !PlayerRole[14] && !PlayerAlive[14]){
             PlayerAlive[14] = 1;
                VillagePeople--;
                Mix_PlayMusic(WolfKilled,0);
                return;
                SDL_Delay(1000);
            }
            else if(e.key.keysym.sym == SDLK_e && !PlayerRole[15] && !PlayerAlive[15]){
             PlayerAlive[15] = 1;
                VillagePeople--;
                Mix_PlayMusic(WolfKilled,0);
                return;
                SDL_Delay(1000);
            }
            else if(e.key.keysym.sym == SDLK_f && !PlayerRole[16] && !PlayerAlive[16]){
             PlayerAlive[16] = 1;
                VillagePeople--;
                Mix_PlayMusic(WolfKilled,0);
                return;
                SDL_Delay(1000);
            }
            else break;
            }
        }

}
}

void ChoosePlayerBeKilledByLynch(){
      SDL_Event e;
    bool running = true;
    while(running){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT) exit(1);
            else if(e.type == SDL_KEYDOWN){
            if(e.key.keysym.sym == SDLK_1 && !PlayerAlive[1]){
             PlayerAlive[1] = 1;
             Mix_PlayMusic(BeLynched,0);
             if(PlayerRole[1]) WolfPeople--;
             else{
                VillagePeople--;
             }
                return;
                SDL_Delay(1000);
            }
            else if(e.key.keysym.sym == SDLK_2 && !PlayerAlive[2]){
             PlayerAlive[2] = 1;
             Mix_PlayMusic(BeLynched,0);
             if(PlayerRole[2]) WolfPeople--;
             else{
                VillagePeople--;
             }
                return;
                SDL_Delay(1000);
            }
            else if(e.key.keysym.sym == SDLK_3 && !PlayerAlive[3]){
             PlayerAlive[3] = 1;
             Mix_PlayMusic(BeLynched,0);
             if(PlayerRole[3]) WolfPeople--;
             else{
                VillagePeople--;
             }
                return;
                SDL_Delay(1000);
            }
            else if(e.key.keysym.sym == SDLK_4 && !PlayerAlive[4]){
             PlayerAlive[4] = 1;
             Mix_PlayMusic(BeLynched,0);
             if(PlayerRole[4]) WolfPeople--;
             else{
                VillagePeople--;
             }
                return;
                SDL_Delay(1000);
            }
            else if(e.key.keysym.sym == SDLK_5 && !PlayerAlive[5]){
             PlayerAlive[5] = 1;
             Mix_PlayMusic(BeLynched,0);
             if(PlayerRole[5]) WolfPeople--;
             else{
                VillagePeople--;
             }
                return;
                SDL_Delay(1000);
            }
            else if(e.key.keysym.sym == SDLK_6 && !PlayerAlive[6]){
             PlayerAlive[6] = 1;
             Mix_PlayMusic(BeLynched,0);
             if(PlayerRole[6]) WolfPeople--;
             else{
                VillagePeople--;
             }
                return;
                SDL_Delay(1000);
            }
            else if(e.key.keysym.sym == SDLK_7 && !PlayerAlive[7]){
             PlayerAlive[7] = 1;
             Mix_PlayMusic(BeLynched,0);
             if(PlayerRole[7]) WolfPeople--;
             else{
                VillagePeople--;
             }
                return;
                SDL_Delay(1000);
            }
            else if(e.key.keysym.sym == SDLK_8 && !PlayerAlive[8]){
             PlayerAlive[8] = 1;
             Mix_PlayMusic(BeLynched,0);
             if(PlayerRole[8]) WolfPeople--;
             else{
                VillagePeople--;
             }
                return;
                SDL_Delay(1000);
            }
            else if(e.key.keysym.sym == SDLK_9 && !PlayerAlive[9]){
             PlayerAlive[9] = 1;
             Mix_PlayMusic(BeLynched,0);
              if(PlayerRole[9]) WolfPeople--;
             else{
                VillagePeople--;
             }
                return;
                SDL_Delay(1000);
            }
            else if(e.key.keysym.sym == SDLK_0 && !PlayerAlive[10]){
             PlayerAlive[10] = 1;
             Mix_PlayMusic(BeLynched,0);
             if(PlayerRole[10]) WolfPeople--;
             else{
                VillagePeople--;
             }
                return;
                SDL_Delay(1000);
            }
            else if(e.key.keysym.sym == SDLK_a && !PlayerAlive[11]){
             PlayerAlive[11] = 1;
             Mix_PlayMusic(BeLynched,0);
             if(PlayerRole[11]) WolfPeople--;
             else{
                VillagePeople--;
             }
                return;
                SDL_Delay(1000);
            }
            else if(e.key.keysym.sym == SDLK_b && !PlayerAlive[12]){
             PlayerAlive[12] = 1;
             Mix_PlayMusic(BeLynched,0);
             if(PlayerRole[12]) WolfPeople--;
             else{
                VillagePeople--;
             }
                return;
                SDL_Delay(1000);
            }
            else if(e.key.keysym.sym == SDLK_c && !PlayerAlive[13]){
             PlayerAlive[13] = 1;
             Mix_PlayMusic(BeLynched,0);
             if(PlayerRole[13]) WolfPeople--;
             else{
                VillagePeople--;
             }
                return;
                SDL_Delay(1000);
            }
            else if(e.key.keysym.sym == SDLK_d && !PlayerAlive[14]){
             PlayerAlive[14] = 1;
             Mix_PlayMusic(BeLynched,0);
             if(PlayerRole[14]) WolfPeople--;
             else{
                VillagePeople--;
             }
                return;
                SDL_Delay(1000);
            }
            else if(e.key.keysym.sym == SDLK_e && !PlayerAlive[15]){
             PlayerAlive[15] = 1;
             Mix_PlayMusic(BeLynched,0);
             if(PlayerRole[15]) WolfPeople--;
             else{
                VillagePeople--;
             }
                return;
                SDL_Delay(1000);
            }
            else if(e.key.keysym.sym == SDLK_f && !PlayerAlive[16]){
             PlayerAlive[16] = 1;
             Mix_PlayMusic(BeLynched,0);
             if(PlayerRole[16]) WolfPeople--;
             else{
                VillagePeople--;
             }
                return;
                SDL_Delay(1000);
            }
            else break;
            }
        }

}
}
void Music(){
       SDL_Event e;
    bool running = true;
    while(running){
        while(SDL_PollEvent(&e)){
            switch(e.type){
                case SDL_QUIT:
                    exit(1);
                case SDL_KEYDOWN:
                  Mix_PlayMusic(gameMusic,0);
                    return;
                    SDL_Delay(1000);
            }
        }

}
}
