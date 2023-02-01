Compilation and runtime requires SDL2 library (as well as SDL2_image library extension).
 - this is pre-included in the file directory for windows users
 - can be installed via most package managers (ie. apt-get for Ubuntu and related distributions) on Linux
 - can be installed via brew on macOS

Source code can be directly compiled for *nix systems with
 - g++ main.cpp -SDL2 -lSDL2_image -lSDL2_ttf

Source code can be directly compiled for Windows after installation of MinGW

g++ main.cpp -I.\SDL2\include -L.\SDL2\lib -w  -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -o main.exe


--- ABOUT THE APP ---

 - Postfix expression evaluator
 - Operands and operators should be separated with spaces when inputting
 - Works with decimals
 - Built in verbose mode, can output compute debug messages
 - scrolling text support with moving cursor
 - Copy/paste/cut support
 - Select support (only works for select all at the moment)


--- NOTE ---

 - Although the compute code is nicely optimised, the render code isn't
 - Thus, resource consumption may be higher than expected
