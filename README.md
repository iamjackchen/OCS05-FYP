# OCS05 (C++) FYP

This is my final year project for Programming in C++ (OCS05), taken in 2019-2020. The project consists of two parts: a post-fix expression evaluator, and an unbeatable tic-tac-toe game. As an additional side challenge I built a custom GUI for both parts using SDL2 and my amateurish photoshop skills. 


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



  
