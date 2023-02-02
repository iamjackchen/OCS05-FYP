
#pragma once
#include <SDL2/SDL.h>
#include "gui/textureHandler.hpp"

//Some Helper Functions

//Returns which tile a point is in //Negative -1 = flag for if the pointer is not in a tile (either outside of board or in a tile boundary)
SDL_Point pointToTile(int x, int y, int tW, int tH, int bWidth, SDL_Point tOPos) {

    SDL_Point returnVal;
    
    if (x-tOPos.x < ((((x-tOPos.x)/(tW+bWidth)) + 1) * (tW+bWidth) - bWidth))
        returnVal.x = ((x-tOPos.x)/(tW+bWidth));
    else {
        returnVal.x = -1;
    }

    if (y-tOPos.y < ((((y-tOPos.y)/(tH+bWidth)) + 1) * (tH+bWidth) - bWidth))
        returnVal.y = ((y-tOPos.y)/(tH+bWidth));
    else {
        returnVal.y = -1;
    }

    return returnVal;

}

//Returns the appropriate render location for a token in a given tile
SDL_Point tileLocToTokenPos(int tileLocX, int tileLocY, int tileWidth, int tileHeight, 
                            int tokenWidth, int tokenHeight, int borderWidth, SDL_Point tOPos) {

    SDL_Point returnVal;

    returnVal.x = (tOPos.x + (tileLocX)*(tileWidth+borderWidth)) + ((tileWidth-tokenWidth)/2);
    returnVal.y = (tOPos.y + (tileLocY)*(tileHeight+borderWidth)) + ((tileHeight-tokenHeight)/2);


    return returnVal;
}

//Function for writing to an SDL_Rect function, since you can't write to structures in one line of code otherwise (outside of declaration)
void setRect(SDL_Rect a, int x, int y, int w, int h) {

    a.x = x;
    a.y = y;
    a.w = w;
    a.h = h;

}


//Board Class

class Gameboard {

    private:

        //Holds data for the board itself
        char** board;
        int rows, columns;
        int moves; //moves made on the board so far

        //Holds information for the texture of the board
        Texture boardTexture;
        int borderWidth;
        SDL_Rect boardBounds;
        int tileWidth, tileHeight;
        SDL_Point tileOriginPos;

        //Holds information for the texture of the "tokens", which are the markers (ie. round and cross for tictactoe)
        int tokenWidth, tokenHeight;
        Texture playerToken, computerToken;


    public:

        //blank constructor
        Gameboard() {}
        
        //constructor that can initialise gameboard
        Gameboard(SDL_Renderer *renderer, int r, int c, string bPath, string token1Path, string token2Path, int bWidth, int x, int y) {
            
            initialiseGameboard(renderer, r, c, bPath, token1Path, token2Path, bWidth, x, y);

        }

        //init function
        void initialiseGameboard(SDL_Renderer *renderer, int r, int c, string bPath, string token1Path, string token2Path, int bWidth, int x, int y);

        //returns 2D char array of the board
        char** getBoard() {return board;}

        //handles events
        bool handleEvents(SDL_Event* e, SDL_Renderer* r);

        //writes to the board
        void makeMove(int r, int c, char x) {*(*(board+r)+c) = x; moves++;}

        //handles render operations
        void render(SDL_Renderer* r, bool isUserTurn);

        //returns the # of columsn and rows, ie. the range of the (x,y) positions for the board
        SDL_Point getDimensions() {SDL_Point returnVal = {columns, rows}; return returnVal;}

        //returns moves left (i dont think this ended up being used lol)
        int movesLeft() {return (rows*columns)-moves;}

};

//initialises variables and loads textures
void Gameboard::initialiseGameboard(SDL_Renderer *renderer, int r, int c, string bPath, string token1Path, string token2Path, int bWidth, int x, int y) {

    //inits some variables
    rows = r;
    columns = c;

    tileOriginPos.x = x+bWidth;
    tileOriginPos.y = y+bWidth;

    borderWidth = bWidth;
    
    //dynamically allocates char array 
    board = new char*[rows];

    for (int i = 0; i < rows; i++){
        *(board+i) = new char[columns];
        for (int j = 0; j < columns; j++) {
            *(*(board+i)+j) = 0;
        }
    }

    //loads textyre
    boardTexture.loadFromFile(bPath, renderer);
    playerToken.loadFromFile(token1Path, renderer);
    computerToken.loadFromFile(token2Path, renderer); 

    //inits other variables
    boardBounds.x = x;
    boardBounds.y = y;
    boardBounds.w = boardTexture.getWidth();
    boardBounds.h = boardTexture.getHeight();

    tileWidth = (boardBounds.w-((c+1)*bWidth))/(c);
    tileHeight = (boardBounds.h-((r+1)*bWidth))/(r);

    tokenWidth = playerToken.getWidth();
    tokenHeight = playerToken.getHeight();

    moves = 0;

}


bool Gameboard::handleEvents(SDL_Event* e, SDL_Renderer* r) { //returns if a move has been made

    bool returnVal = false;

    //if the user has clicked
    if (e->type == SDL_MOUSEBUTTONDOWN) {

        //gets mouse position data
        int x, y;
		SDL_GetMouseState(&x, &y);
        SDL_Point mouseLoc = {x, y};

        //if mouse position is within the board boundary
        if (SDL_PointInRect(&mouseLoc,  &boardBounds)) {

            SDL_Point tileCoord = pointToTile(x, y, tileWidth, tileHeight, borderWidth, tileOriginPos);

            //if mouse position is in a tile
            if (tileCoord.x != -1 && tileCoord.y != -1 && *(*(board+tileCoord.y)+tileCoord.x) == 0) {
                
                makeMove(tileCoord.y, tileCoord.x, 'P');
                returnVal = true;
            }

        }

    }

    return returnVal;

}

void Gameboard::render(SDL_Renderer* r, bool isUserTurn) {

    //Renders base board
    boardTexture.render(r, boardBounds.x, boardBounds.y);

    //Scans through board 2D array and renders tokens at appropriate locations if needed
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++) {
            
            SDL_Point tokenPos = tileLocToTokenPos(j, i, tileWidth, tileHeight, tokenWidth, tokenHeight, borderWidth, tileOriginPos);
            
            if (*(*(board+i)+j) == 'P') {
                playerToken.render(r, tokenPos.x, tokenPos.y);
            } else if (*(*(board+i)+j) == 'C') {
                computerToken.render(r, tokenPos.x, tokenPos.y);
            }
        }
    }

    //Renders half-opacity tokens at the mouse pointer location when it is the user's turn
    if (isUserTurn) {

        //gets mouse position
        int x, y;
		SDL_GetMouseState(&x, &y);
        SDL_Point mouseLoc = {x, y};

        //if mouse pointer is within the bounds of the board
        if (SDL_PointInRect(&mouseLoc,  &boardBounds)) {

            //obtains which tile the mouse pointer is in
            SDL_Point tileCoord = pointToTile(x, y, tileWidth, tileHeight, borderWidth, tileOriginPos);
            
            //if the mouse pointer is in a tile (and not a boundary)
            if (tileCoord.x != -1 && tileCoord.y != -1 && *(*(board+tileCoord.y)+tileCoord.x) == 0) {
                
                //Obtains previous alpha in a temporarily allocated variable
                Uint8* oldAlpha = new Uint8;
                SDL_GetTextureAlphaMod(playerToken.getTexture(), oldAlpha);

                //Temporarily sets alpha
                SDL_SetTextureAlphaMod(playerToken.getTexture(), 0x50);

                //Renders token at the appropriate position in the tile
                playerToken.render(r, tileLocToTokenPos(tileCoord.x, tileCoord.y, tileWidth, tileHeight, tokenWidth, tokenHeight, borderWidth, tileOriginPos).x,  
                                    tileLocToTokenPos(tileCoord.x, tileCoord.y, tileWidth, tileHeight, tokenWidth, tokenHeight, borderWidth, tileOriginPos).y);
                

                //Reverts alpha
                SDL_SetTextureAlphaMod(playerToken.getTexture(), *oldAlpha);

                //Clears memory allocated to oldalpha
                delete oldAlpha;
                oldAlpha = nullptr;
            }
        }

    }

}
