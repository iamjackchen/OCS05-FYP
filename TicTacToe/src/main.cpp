#include <iostream>
#include "gui/renderMain.hpp"
#include "computeLib.hpp"
#include "gameBoard.hpp"
using namespace std;

//main
int main(int argc, char* args[]) {

    if (!init(720, 480)) {
        cout << "Renderer Initialisation Failed" << endl; exit(EXIT_FAILURE);
    } else {
        
        //flags used in program runtime to keep track of app state
        bool quit = false;
        bool introPass = false;

        SDL_Event event; //holds events
                    
        while (!quit) { //if the game has been quit (window has been exited)
            
            if(!loadUI()) { //tries to load UI
                cout << "Failed to load new game" << endl; exit(EXIT_FAILURE);
            } else {
                
                
                bool sessionEnd = false; //flag for whether the game has been reset
                bool isGameOver = false; //flag for whether the game has ended or not (either if someone has won or if there are no moves left)
                bool isUserTurn = true; //flag for whose turn it is (computer or player)

                //while the game has not been reset
                while (!sessionEnd) {
                    
                    //set moveMade to false
                    bool moveMadeFlag = false;

                    int r = (tttBoard.getDimensions().y);
                    int c = (tttBoard.getDimensions().x);

                    //obtains 2D char array from the gameboard, to pass into compute functions. I originally tried
                    //to just write compute functions where you can pass the gameboard object in by value, making things way easier, 
                    //but SDL doesn't seem to like me passing in objects that contain other SDL objects. It messes up the render
                    //and causes the gameboard to be invisible even though the object is still "there" and i can click on it, etc.

                    char board[3][3]; 
                    //i would do board[r][c] but C++ does not like that, since i need to pass in a variable of char(*)[3] to
                    //the compute function, and c isn't always guaranteed to be 3.

                    for (int i = 0; i < tttBoard.getDimensions().y; i++) 
                        for (int j = 0; j < tttBoard.getDimensions().x; j++) {
                                board[i][j] = *(*(tttBoard.getBoard()+i)+j);
                            }


                    //checks if the game is over (if anyone has won or if there is tie when there are no moves left) 
                    //updates status texture if needed
                    switch(evaluateBoard(board, r, c)) {
                        case(0): {
                            if (tttBoard.movesLeft() == 0) {
                                if (!displayStatus.loadFromFile("src/gui/assets/tieMSG.png", renderer)) {
                                    cout << "Failed to load status texture" << endl; exit(EXIT_FAILURE);
                                }
                                isGameOver = true;  
                            }
                            break;
                        }

                        case(10): {
                            if (!displayStatus.loadFromFile("src/gui/assets/win2MSG.png", renderer)) {
                                cout << "Failed to load status texture" << endl; exit(EXIT_FAILURE);
                            }
                            isGameOver = true;
                            break;
                        }

                        case(-10): {
                            if (!displayStatus.loadFromFile("src/gui/assets/win1MSG.png", renderer)) {
                                cout << "Failed to load status texture" << endl; exit(EXIT_FAILURE);
                            }
                            isGameOver = true;
                            break;
                        }
                    }
            
                    //handles event cue
                    while(SDL_PollEvent(&event)) {    

                        //checks for whether the user clicked the close window button
                        if (event.type == SDL_QUIT) {
                            quit = true; sessionEnd = true; break;
                        }

                        //updates clear game button
                        clearGame.handleEvent(&event, renderer);

                        //if it is the user's turn to move, and if the game hasn't ended, then handle user input
                        if (isUserTurn && !isGameOver) {
                            if (tttBoard.handleEvents(&event, renderer)) { //calling the bool handles events

                                //updates status texture
                                if (!displayStatus.loadFromFile("src/gui/assets/turn2MSG.png", renderer)) {
                                    cout << "Failed to load status texture" << endl;
                                    exit(EXIT_FAILURE);
                                }
                                
                                moveMadeFlag = true;
                            } 
                        }
                        

                    }

                    //break out of loop if the clear game button is down, leads to a reloading of the UI
                    if (clearGame.isKeyDown()) {sessionEnd = true; break;}
                    
                    //if it is the computer's turn, make a move and update status texture after
                    if (!isUserTurn && !isGameOver) {
                        
                        SDL_Point moveToMake = findBestMove(board, r, c);

                        tttBoard.makeMove(moveToMake.y, moveToMake.x, 'C');

                        if (!displayStatus.loadFromFile("src/gui/assets/turn1MSG.png", renderer)) {
                            cout << "Failed to load status texture" << endl;
                            exit(EXIT_FAILURE);
                        }
                        

                        moveMadeFlag = true;
                            
                    }              

                    SDL_RenderClear(renderer);

                    //renders output title and enter button
                    title.render(renderer, 464, 59);
                    clearGame.render(renderer);
                    
                    tttBoard.render(renderer, isUserTurn);

                    displayStatus.renderCentred(renderer, 578, 362);

                    //changes turn variable if a move has been made
                    //i would've coded this into the parts handling user input etc, but I needed to 
                    //preserve the isUserTurn variable for rendering, and so that only 1 move is made
                    //per render cycle

                    if (moveMadeFlag)
                        isUserTurn = (!isUserTurn);

                    SDL_RenderPresent(renderer);  
  
                }
            }
        }

    }

    close();
    return 0;

}
