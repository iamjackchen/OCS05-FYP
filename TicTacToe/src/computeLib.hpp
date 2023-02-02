#include "gameBoard.hpp"
#include <SDL2/SDL.h>

//Checks if there are moves left on the board. I originally tried to pass in a movesLeft integer, where each call of the minimax would use movesLeft-1
//But I couldn't get it to work, so I had to resort to a less efficient way of checking if there are moves left in the board
bool movesLeft(char board[3][3]) 
{ 
    for (int i = 0; i<3; i++) 
        for (int j = 0; j<3; j++) 
            if (board[i][j]== 0) 
                return true; 
    return false; 
} 

//function for evaluating a complete gameboard, from a computer's perspective
int evaluateBoard(char board[][3], int r, int c, int depth = 0) {

    //returns 10 - # of steps taken if the computer wins
    //returns -10 + # of steps taken if the player wins

    //the depth here is useful, since it allows the computer to
    //know the steps taken, and attempt to win with the least steps as possible

    //Checks rows
    for (int i = 0; i < r; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            if (board[i][0] == 'C') {
                return 10 - depth;
            } else if (board[i][0] == 'P') {
                return -10 + depth;
            }
        }
    }

    //Checks columns
    for (int i = 0; i < c; i++) {
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            if (board[0][i] == 'C') {
                return 10 - depth; 
            } else if (board[0][i] == 'P') {
                return -10 + depth;
            }
        }
    }


    //Checks diagonals, works only when rows = columns (square board)
    if (r == c) {

        bool diagStreak = true;

        //Checks for diagonals going down
        for (int i = 1; i < c; i++) { 
            if (board[i][i] != board[i-1][i-1])
                diagStreak = false;
        }

        if (diagStreak) {
            if (board[1][1] == 'C') {
                return 10 - depth; 
            } else if (board[1][1] == 'P') {
                return -10 + depth;
            }
        }

        diagStreak = true;

        //Checks for diagonals going up
        for (int i = 1; i < c; i++) {
            if (board[c-i-1][i] != board[c-i][i-1])
                diagStreak = false;
        } 
        
        if (diagStreak) {
            if (board[1][1] == 'C') {
                return 10 - depth; 
            } else if (board[1][1] == 'P') {
                return -10 + depth;
            }
        }

    }

    return 0; //occurs if there is a tie
}

//the main minimax algorithm, basically a recursive function that bruteforces all possible outcomes of the game
//this can be applied to literally any board game (like chess), but is too computationally intensive for more complex games
//where the # of move combinations are too high

//generally minimax is called to a certain depth
//but tictac toe is very simple so I can calculate all outcomes without using too many resources
//much better than having every move combination hard coded into the game at least (I've heard it takes ~4000 lines of code in C++):P

int minimaxAlgo(char a[][3], int r, int c, int depth, bool isComputerTurn, int alpha, int beta) {

    //evaluates the board
    int score = evaluateBoard(a, r, c, depth);

    //if the game has a winner
    if (score != 0) 
        return score; 

    //if the game has ended (no more moves left) but there is no winner
    if (!movesLeft(a)) 
        return 0;


    //The stuff above is the base case for recursion ^^
        
    //flag for alpha-beta pruning, allows the recursive function to skip some computations if it knows already that
    //there is going to be an always better choice in the "recursion tree" at the given depth level
    bool prune = false;

    // If this is the computer's move 
    if (isComputerTurn) 
    { 
        int best = INT_MIN; 

        // Traverse all cells 
        for (int i = 0; i < r; i++){ 
            for (int j = 0; j < c; j++){ 
                
                // Check if cell is empty 
                if (a[i][j] == 0){ 

                    //make hypothetical move
                    a[i][j] = 'C';

                    //Calls minimax (recursively) for the move made and takes maximum value
                    best = max(best, minimaxAlgo(a, r, c, depth+1, !isComputerTurn, alpha, beta));

                    //Undo hypothetical move
                    a[i][j] = 0;

                    //Alpha-beta pruning implementation
                    alpha = max(alpha, best);
                    if (beta <= alpha) {
                        prune = true; break;
                    } 
                        

                }
            
            if (prune)
                break;

            } 
        } 
        return best; 
    } 
  
    // If this minimizer's move 
    else
    { 
        int best = INT_MAX; 
  
        // Traverse all cells 
        for (int i = 0; i < r; i++) { 
            for (int j = 0; j < c; j++) { 

                // Check if cell is empty 
                if (a[i][j] == 0 ) { 

                    //Make hypothetical move
                    a[i][j] = 'P'; 
  
                    //Calls minimax (recursively) for the move made and takes minimum value
                    best = min(best, minimaxAlgo(a, r, c, depth+1, !isComputerTurn, alpha, beta)); 

                    //Undo hypothetical move
                    a[i][j] = 0;

                    //Alpha-beta pruning implementation
                    beta = min(beta, best); 
                    if (beta <= alpha) {
                        prune = true; break;
                    } 
                } 

                if (prune)
                    break;

            } 
        } 
        return best; 
    }

    //^^ I had to use a prune flag instead of just having one break statement, so I could break out of the nested for loop

}

//function for finding best move
SDL_Point findBestMove(char a[][3], int r, int c) {

    SDL_Point returnVal = {-1, -1}; //-1 is the flag for if something goes wrong xD
    int bestMove = INT_MIN;

    for (int i = 0; i < r; i++) { 
        for (int j = 0; j < c; j++) { 
            if (a[i][j] == 0) { 

                //make a hypothetical move
                a[i][j] = 'C'; 
                
                //compute evaluation for this move
                int moveVal = minimaxAlgo(a, r, c, 0, false, INT_MIN, INT_MAX); 
  
                //undo hypothetical move
                a[i][j] = '0';
  
                //allows the computer to take the move with the best evaluation score
                if (moveVal > bestMove) 
                { 
                    returnVal.y = i; //note that y = which row 
                    returnVal.x = j; //note that x = which column
                    bestMove = moveVal; 
                } 
            } 
        } 
    } 

    return returnVal; 

}