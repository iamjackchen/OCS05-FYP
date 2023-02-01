#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "buttonHandler.hpp"
#include "inputTextboxHandler.hpp"
#include "textboxHandler.hpp"

//Window and renderer
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Rect windowRect;

//Colors
SDL_Color white = {0xFF, 0xFF, 0xFF, 0xFF};
SDL_Color lightGrey = {0x63, 0x63, 0x63, 0xFF};

//Intro Objects
Texture introText;
Button introButton;

//UI Objects
InputTextbox userInput;
Textbox outputText;
Texture outputTitle;
Button enterButton;
TTF_Font* font = NULL;

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
        window = SDL_CreateWindow( "Postfix Expression Evaluator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN );
        
        if (window == NULL) success = false;
        else {

            //create renderer for window
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

			if (renderer == NULL) {
				cout << "Error: Renderer could not be created!" << SDL_GetError() << endl;
				success = false;
			} else {

				//initialize renderer colour
				SDL_SetRenderDrawColor(renderer, 0x2a, 0x2a, 0x2a, 0x2a);

				//initialize PNG Loader
				int imgFlags = IMG_INIT_PNG;
				if (!( IMG_Init(imgFlags) & imgFlags)) {
					cout << "SDL_image could not initialize! Error:" << SDL_GetError() << endl;
					success = false;

				}

                //initialize Font Loader
                if (TTF_Init() == -1) {
                    cout << "SDL_ttf could not initialize! Error:" << SDL_GetError() << endl;
					success = false;
                }
                
			}
        }
    }

    return success;
}

//loads intro page
bool loadIntro() {

    bool success = true;

    SDL_RenderClear(renderer);
    
    //loads up image with prompt text
    if (!introText.loadFromFile("src/gui/assets/introText.png", renderer)) {
        cout << "Texture loader: " << SDL_GetError() << endl; 
        success = false;
    }

    introText.render(renderer, 30, 36);

    //loads button
    introButton.initTextures("src/gui/assets/okInert.png", "src/gui/assets/okActive.png");
    if (!introButton.loadTexture(renderer)) {
        cout << "Texture loader: " << SDL_GetError() << endl; 
        success = false;
    }
    introButton.setPosition(536, 36);
    introButton.render(renderer);

    SDL_RenderPresent(renderer);

    return success;

}

//destroys window
void destroyIntro() {

    SDL_RenderClear(renderer);
    introText.free();
    introButton.freeTexture();
    SDL_RenderPresent(renderer);

}

//loads main user interface
bool loadUI() {

    bool success = true;

    SDL_RenderClear(renderer);

    //loads render font
    font = TTF_OpenFont("src/gui/assets/calibriLight.ttf", 30);
    if (font == NULL) {
        cout << "Failed to load font: " << TTF_GetError() << endl;
        success = false;
    }

    //initialises input textbox
    userInput.initInputTextbox(renderer, font, lightGrey, white, lightGrey, "src/gui/assets/rectUP.png",
                               "src/gui/assets/cursor.png", "Enter Postfix Expression Here", 3, 25, 26);
    
    //initialises button
    enterButton.initTextures("src/gui/assets/enterInert.png", "src/gui/assets/enterActive.png");
    if (!enterButton.loadTexture(renderer)) {
        cout << "Texture loader: " << SDL_GetError() << endl; 
        success = false;
    }
    enterButton.setPosition(525, 26);

    //initialises title for output textbox
    if (!outputTitle.loadFromFile("src/gui/assets/outTITLE.png", renderer)) {
        cout << "Texture loader: " << SDL_GetError() << endl; 
        success = false;
    }

    //initialises output textbox
    outputText.initTextbox(renderer, font, white, lightGrey, "src/gui/assets/rectDOWN.png",
                               "src/gui/assets/cursor.png", 3, 137, 100);
    
    outputText.updateAndRender(renderer);

    SDL_RenderPresent(renderer);

    return success;

}

//destroys/closes everything; used as a shut down function
void close()
{
    SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
    TTF_CloseFont(font);
	window = NULL;

    IMG_Quit();
    TTF_Quit();
	SDL_Quit();
}