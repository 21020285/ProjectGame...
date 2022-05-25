#pragma once
#ifndef FUNCTION_H
#define FUNCTION_H
#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include "ChooseWolf.h"
const int MAX_PLAYER = 16;
const int WOLF_PLAYER = 5;
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;
const int BUTTON_WIDTH = 180;
const int BUTTON_HEIGHT = 180;
const int Row = 4;
const int Column = 4;

using namespace std;
const string WINDOW_TITLE = "Wolf Game";
void logSDLError(ostream& os, const string &msg, bool fatal = false);
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
bool loadMusic();
void waitUntilKeyPressed();
void ChoosePlayerBeKilledByWolf();
void ChoosePlayerBeKilledByLynch();
void backgroundOn();
void gamePlay();
void infoGame();
void convention();
void placeNumber();
void NightAction();
void CheckAlive();
void GetOut();
void DayAction();
bool Result();
void closeMusic();
void Winner();
void Music();
SDL_Texture* loadTexture( string path, SDL_Renderer* renderer );

#endif //FUNCTION_H

