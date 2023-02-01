#include <iostream>
#include <stdio.h>
#include "computeLib.hpp"
#include "gui/renderMain.hpp"
using namespace std;

const string opStatusDecode[5] = { "Operation normal", "Invalid Char Detected", "Not Enough Operands (Stack Underflow)", "Too Many Operands", "Too Many Decimal Points" }; 
//used as a translator library for stack error messages

//main
int main(int argc, char* args[]) {

    if (!init(640, 180)) {
        cout << "Renderer Initialisation Failed" << endl; exit(EXIT_FAILURE);
    } else {
        
        //flags used in program runtime to keep track of app state
        bool quit = false;
        bool introPass = false;

        SDL_Event event; //holds events

        if(!loadIntro()) { //tries to load intro page
              cout << "Intro page failed to load" << endl; exit(EXIT_FAILURE);
        } else {

            while (!quit && !introPass) {

                while(SDL_PollEvent(&event)) {    

                    if (event.type == SDL_QUIT)
                        quit = true;

                introButton.handleEvent(&event, renderer);

                }

                //Clear screen
				SDL_RenderClear(renderer);
				
                //Update Text
                introText.render(renderer, 30, 36);

                //Update Button
                introButton.render(renderer);

				//Update screen
				SDL_RenderPresent(renderer);

                if (introButton.isKeyDown()) introPass = true;

            }

            //Resets screen
            destroyIntro();    

            if (!loadUI()) { //tries to load ui
                cout << "UI failed to load" << endl; exit(EXIT_FAILURE);
            } else {

                bool enterFlag; //flag for whether the enter key has been pressed or not
                    
                while (!quit) {

                    while(SDL_PollEvent(&event)) {    
                    
                    //checks for whether the user clicked the close window button
                    if (event.type == SDL_QUIT)
                        quit = true;

                    //checks for enter key events
                    if (event.type == SDL_KEYDOWN)
                        if (event.key.keysym.sym == SDLK_RETURN)
                            enterFlag = true;

                    if (event.type == SDL_KEYUP)
                        if (event.key.keysym.sym == SDLK_RETURN)
                            enterFlag = false;

                    //updates objects that interface with events
                    enterButton.handleEvent(&event, renderer);
                    userInput.handleEvents(&event, renderer);
                    outputText.handleEvents(&event, renderer);

                    }

                    SDL_RenderClear(renderer);

                    //renders output title and enter button
                    outputTitle.render(renderer, 26, 116);
                    enterButton.render(renderer);

                    //compute input string if needed
                    if (enterButton.isKeyDown() || enterFlag) {
                        
                        char computeString[(userInput.getInputText()).length()];
                        double* output = new double;
                        strcpy(computeString, (userInput.getInputText()).c_str());
                        //above code converts input string to char array (accepted by compute function)

                        
                        if ((userInput.getInputText().front()) == 'V' || (userInput.getInputText().front()) == 'v') { //triggers verbose mode
                            
                            switch (compute(computeString, (userInput.getInputText()).length(), output, true)) {
                                
                                //below a decoding string array is used to process/display compute status
                                case 0: {outputText.setOutputText(to_string(*output)); break;} //compute successful
                                case 1: {outputText.setOutputText("Error: " + opStatusDecode[1]); break;}
                                case 2: {outputText.setOutputText("Error: " + opStatusDecode[2]); break;}
                                case 3: {outputText.setOutputText("Error: " + opStatusDecode[3]); break;}
                                case 4: {outputText.setOutputText("Error: " + opStatusDecode[4]); break;}

                            }

                            //I used this switch case statement instead of just using opStatusDecode[compute()]
                            //because that would run the entire compute twice; can get resource consuming

                        } else {
                            
                            //if compute successful
                            if (compute(computeString, (userInput.getInputText()).length(), output) == 0) 
                                outputText.setOutputText(to_string(*output));
                            else
                                outputText.setOutputText("Error!");
                        
                        }
                    }

                    //updates textboxes
                    userInput.updateAndRender(renderer);
                    SDL_RenderSetClipRect(renderer, &windowRect); //resets clipping rectangle for renderer after text is rendered
                    outputText.updateAndRender(renderer);
                    SDL_RenderSetClipRect(renderer, &windowRect); //resets clipping rectangle for renderer after text is rendered
                    
                    SDL_RenderPresent(renderer);

                }
            }
        }
    }

    close();
    return 0;

}