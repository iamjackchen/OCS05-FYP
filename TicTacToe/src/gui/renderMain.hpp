#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../gameBoard.hpp"
#include "textureHandler.hpp"
#include "buttonHandler.hpp"
#include <iostream>
#include <stdio.h>
using namespace std;

//Window and renderer
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Rect windowRect;

//UI Objects
Gameboard tttBoard;
Texture title;
Texture displayStatus;
Button clearGame;

bool init(int w, int h) {

    bool success = true;

    //initialises rectangle to hold window dimensions
    windowRect.x = 0;
    windowRect.y = 0;
    windowRect.w = w;
    windowRect.h = h;

    if (SDL_Init( SDL_INIT_VIDEO ) < 0) { //initialises sdl video engine
        success = false;
        cout << "Error: SDL Video not initialised" << SDL_GetError() << endl;
    } else {
    
        if (!SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ))
			cout << "Warning: Linear texture filtering not enabled!" << endl;
        
        //initialises window
        window = SDL_CreateWindow( "Tic Tac Toe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN );
        
        if (window == NULL) success = false;
        else {

            //create renderer for window
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

			if (renderer == NULL) {
				cout << "Error: Renderer could not be created!" << SDL_GetError() << endl;
				success = false;
			} else {

				//initialize renderer colour
				SDL_SetRenderDrawColor(renderer, 0x1d, 0x1d, 0x1d, 0xFF);

				//initialize PNG Loader
				int imgFlags = IMG_INIT_PNG;
				if (!( IMG_Init(imgFlags) & imgFlags)) {
					cout << "SDL_image could not initialize! Error:" << SDL_GetError() << endl;
					success = false;

				}
			}
        }
    }

    return success;
}


//loads main user interface
bool loadUI() {

    bool success = true;

    SDL_RenderClear(renderer);
    
    //initialises reset button
    clearGame.initTextures("src/gui/assets/buttonInert.png", "src/gui/assets/buttonActive.png");
    if (!clearGame.loadTexture(renderer)) {
        cout << "Texture loader: " << SDL_GetError() << endl; 
        success = false;
    }
    clearGame.setPosition(466, 216);

    //initialises title
    if (!title.loadFromFile("src/gui/assets/title.png", renderer)) {
        cout << "Texture loader: " << SDL_GetError() << endl; 
        success = false;
    }

    //initialises status displayer
    if (!displayStatus.loadFromFile("src/gui/assets/turn1MSG.png", renderer)) {
        cout << "Texture loader: " << SDL_GetError() << endl; 
        success = false;
    }

    //initialises gameboard
    tttBoard.initialiseGameboard(renderer, 3, 3, "src/gui/assets/gameBoard.png", "src/gui/assets/crossToken.png", 
                                "src/gui/assets/roundToken.png", 3, 38, 42);


    SDL_RenderPresent(renderer);
    
    return success;

}

//destroys/closes everything; used as a shut down function
void close()
{
    SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = NULL;

    IMG_Quit();
	SDL_Quit();
}