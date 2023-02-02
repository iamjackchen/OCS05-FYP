# OCS05 (C++) FYP

This is my final year project for Programming in C++ (OCS05), taken in 2019-2020. The project consists of two parts: a [post-fix expression evaluator](https://github.com/iamjackchen/OCS05-FYP/blob/main/README.md#postfix-expression-evaluator), and an [unbeatable tic-tac-toe game](https://github.com/iamjackchen/OCS05-FYP/blob/main/README.md#unbeatable-tic-tac-toe). As an additional side challenge I built a custom GUI for both parts using SDL2 and my amateurish photoshop skills. 

<br/>

## Postfix Expression Evaluator

This app evaluates post-fix expressions by means of a stack. The program iterates through each character in the expression, delimited by a space. Operands are pushed onto the stack. Operators are applied to the top-most two items on the stack (and the result is pushed onto the stack). 

The stack is implemented as a dynamically allocated array whose length adjusts throughout runtime for memory efficiency.

All GUI components are custom-built. SDL2 provides low-level access to I/O and graphics hardware; it is not a GUI library per se, so it doesn't have built-in UI components. Building buttons with a hover effect was fairly simple. More notably, I built my own [textbox component](https://github.com/iamjackchen/OCS05-FYP/blob/main/PostFixProj/src/gui/inputTextboxHandler.hpp) that calculates substring render widths (using elementary arithmetic) in order to handle cursor positioning and support text wrapping and selection.


### Features:
* Verbose mode (see bottom right screenshot). Provides error printouts when expression evaluation fails
* Works with decimal values (but not negative numbers... yet)
* Clipboard integration. Textbox input supports select-all, cut, copy, and paste operations. Output display supports select-all and copy


### Screenshots:
<p align="center">
  <img align="center" src="https://raw.githubusercontent.com/iamjackchen/OCS05-FYP/main/PostFixProj/Screenshots/StartPage.png" width="49%" />
  <img align="center"  src="https://raw.githubusercontent.com/iamjackchen/OCS05-FYP/main/PostFixProj/Screenshots/MainPage.png" width="49%" /> 
</p>

<p align="center">
  <img align="center" src="https://raw.githubusercontent.com/iamjackchen/OCS05-FYP/main/PostFixProj/Screenshots/NormalOperation.png" width="49%" />
  <img  align="center"  src="https://raw.githubusercontent.com/iamjackchen/OCS05-FYP/main/PostFixProj/Screenshots/Verbose.png" width="49%" /> 
</p>

![](https://raw.githubusercontent.com/iamjackchen/OCS05-FYP/main/PostFixProj/Screenshots/InputTextbox.gif)

### Build/Installation Instructions:
You must have g++ installed. This can be done through a package manager on \*NIX systems. Windows users can use [MinGW](https://www.mingw-w64.org).

##### \*NIX Users:
* You must have [SDL2](https://wiki.libsdl.org/SDL2/FrontPage), [SDL2_image](https://wiki.libsdl.org/SDL_image/FrontPage), and [SDL2_ttf](https://wiki.libsdl.org/SDL2/FrontPage) installed. This can be done through a package manager on \*NIX systems.
* From the PostFixProj directory, run `g++ main.cpp -SDL2 -lSDL2_image -lSDL2_ttf`

##### Windows Users:
* I have included all the necessary SDL2 files within the directory, so there is no further action needed
* From the PostFixProj directory, run `g++ src\main.cpp -I.\SDL2\include -L.\SDL2\lib -w  -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -o main.exe`
* There is a pre-compiled main.exe executable 

<br/>

## Unbeatable Tic-Tac-Toe

This app allows the user to play tic-tac-toe against a computer. The computer uses a minimax algorithm to calculate the best move against the player, and never loses. 

The minimax algorithm works by recursively calculating all possible event paths (to a certain number of moves ahead). In this case, 3x3 Tic-Tac-Toe is simple enough such that the minimax algorithm calculates all possible event paths to the end of the game. A score is assigned to each event path depending on whether the computer wins/loses/draws, as well as how long it takes for this outcome to be achieved. The the first event on the highest-scoring event path is the move that will be played by the computer

Minimax tends to be a very computationally intensive algorithm since it brute-forces every possible outcome. To optimise things a little, I implemented alpha-beta pruning on the recursive tree of minimax calls. We do not need to compute every singular possible leaf on this tree-- in fact, given a choice of moves, if we know that a move will be definitely worse than a previously considered move, then we need not consider this move (and the possible moves after) at all. Alpha-beta pruning allows us to "prune" away some parts of the recursive tree, so we avoid computing event paths that cannot influence the final decision of what move to play.

### Demo:
![](https://raw.githubusercontent.com/iamjackchen/OCS05-FYP/main/TicTacToe/Screenshots/TicTacToeDemo.gif)

### Build/Installation Instructions:
You must have g++ installed. This can be done through a package manager on \*NIX systems. Windows users can use [MinGW](https://www.mingw-w64.org).

##### \*NIX Users:
* You must have [SDL2](https://wiki.libsdl.org/SDL2/FrontPage), [SDL2_image](https://wiki.libsdl.org/SDL_image/FrontPage), and [SDL2_ttf](https://wiki.libsdl.org/SDL2/FrontPage) installed. This can be done through a package manager on \*NIX systems.
* From the PostFixProj directory, run `g++ main.cpp -SDL2 -lSDL2_image -lSDL2_ttf`

##### Windows Users:
* I have included all the necessary SDL2 files within the directory, so there is no further action needed
* From the TicTacToe directory, run `g++ src\main.cpp -I.\SDL2\include -L.\SDL2\lib -w  -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -o main.exe`
* There is a pre-compiled main.exe executable 
  
