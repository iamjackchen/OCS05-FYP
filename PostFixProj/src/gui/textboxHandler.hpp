#include "textureHandler.hpp"
using namespace std;

//most of this code is just a watered down version of the input textbox handler, some features removed,
//some features added; I chose not to make one class for both input and display textboxes just to keep things clean
//and simple, seeing as there are already enough variables.


class Textbox {

    private:

	SDL_Keymod systemKey = (strcmp(SDL_GetPlatform(), "Mac OS X") == 0) ? KMOD_GUI : KMOD_CTRL;

        Texture boxTexture; //texture for the physical textbox
        Texture textTexture; //texture for the text
        Texture cursorTexture; //texture for the cursor
        SDL_Rect visibleTextHolder; //clipping rectangle for text (so the text doesn't get displayed beyond the bounds of the box)
        SDL_Rect selectionRect;
        TTF_Font* fontHolder; //font for text

        //holds position data for textbox
        SDL_Point position;

        //holds position data for text
        SDL_Point textPosition;
        SDL_Point textOriginPosition;

        //holds position data for cursor
        SDL_Point cursorPosition; //physical location on screen
        int cursorPos; //location in input string

        //Dimension data
        int width, height;
        int cursorWidth, cursorHeight;
        int fontWidth;
        int fontHeight;

        //Colour data
        SDL_Color textColour;
        SDL_Color selectColour;

        //Holds user-inputted text
        string text1, text2; //used to hold text on either side of the cursor 

        bool focus; //flag for whether the textbox is focused or not
        bool select; //flag for whether the text in the textbox is selected

        int boxBorderThickness;

        //function used for assigning values to a structure (since c++ doesn't support simulataneous assignment)
        void assignRect(SDL_Rect &a, int x1, int y1, int w1, int h1) {a.x = x1; a.y = y1; a.w = w1; a.h = h1;}

    public:

        //Constructor for loading empty object
        Textbox() {
            position.x = 0;
            position.y = 0;
            width = 0;
            height = 0;
            focus = false;
            boxBorderThickness = 0;

        }

        //Constructor that can be used to load stuff directly
        Textbox(SDL_Renderer* renderer, TTF_Font* font, SDL_Color a, SDL_Color b, string boxTexturePath, string cursorPath, 
                string defaultPromptT = "", int borderThickness = 0, int x = 0, int y = 0) {
                
                initTextbox(renderer, font, a, b, boxTexturePath, cursorPath, borderThickness, x, y);
        }

        //Initialise function
        void initTextbox(SDL_Renderer* renderer, TTF_Font* font, SDL_Color a, SDL_Color b, string boxTexturePath, string cursorPath, 
                         int borderThickness = 0, int x = 0, int y = 0);

        //Destructor
        ~Textbox() {boxTexture.free(); textTexture.free(); cursorTexture.free();}

        //Used for updating and rendering textures
        void updateAndRender(SDL_Renderer* renderer);

        //Used for handling mouse and keyboard events
        void handleEvents(SDL_Event* e, SDL_Renderer* renderer);

        //Sets output text
        void setOutputText(string x) {text1 = x;}

};


void Textbox::initTextbox(SDL_Renderer* renderer, TTF_Font* font, SDL_Color a, SDL_Color b, string boxTexturePath, string cursorTexturePath, 
                          int borderThickness, int x, int y) {

    //Loads box texture
    if (boxTexturePath != "") {
        boxTexture.loadFromFile(boxTexturePath, renderer);
        width = boxTexture.getWidth();
        height = boxTexture.getHeight();
    }

    //inits box position
    position.x = x;
    position.y = y;

    //Loads cursor texture
    if (cursorTexturePath != "") {
        cursorTexture.loadFromFile(cursorTexturePath, renderer);
        cursorWidth = cursorTexture.getWidth();
        cursorHeight = cursorTexture.getHeight();

        //Sets cursor to a position that is outside of the clipping box -> causes it to be invisible
        cursorPosition.x = 0;
        cursorPosition.y = 0;
    }

    //Sets cursor position to the beginning of all text
    cursorPos = 0;

    //loads font
    fontHolder = font;

    //inits clip rectangle
    boxBorderThickness = borderThickness;
    assignRect(visibleTextHolder, position.x+boxBorderThickness, position.y+boxBorderThickness, width-2*boxBorderThickness, height-2*boxBorderThickness);

    //Sets colour and resets/inits text fields
    textColour = a;
    selectColour = b;
    text1.clear();
    text2.clear();

    //gets the height and width of the font using a "wide" character
    int *advance = new int;
    TTF_GlyphMetrics(font, 'g', NULL, NULL, NULL, NULL, advance);
    fontHeight = TTF_FontHeight(font);
    fontWidth = *advance;

    //inits text positions
    textOriginPosition.y = textPosition.y = visibleTextHolder.y + (visibleTextHolder.h - TTF_FontHeight(font))/2;
    textOriginPosition.x = textPosition.x = visibleTextHolder.x + (*(advance));

    //clears dynamically allocated memory
    delete advance;
    advance = nullptr;

    //initialises flag variables
    focus = false;
    select = false;

    //inits selection highlighter box
    assignRect(selectionRect, textOriginPosition.x, textOriginPosition.y, 0, 0);
    
}

void Textbox::handleEvents(SDL_Event *event, SDL_Renderer* renderer) {

    int textWidth; //to hold string render width 
    int textHeight; //to hold char or string render height
    int charWidth; //to hold char render width

    if(event->type == SDL_MOUSEBUTTONDOWN) {

        //gets mouse data
        int x, y;
		SDL_GetMouseState(&x, &y);

        //if mouse clicks inside the box
        if((x > position.x) && (x < position.x + width) && (y > position.y) && (y < position.y + height)) {
             
            //singular click focuses on textbox if not focused already
             if (!focus) {
                focus = true;

                //following block of text WILL APPEAR MANY TIMES - getting render width of a text uses a function THAT DOES NOT ACCEPT STRINGS (rip)
                char textToChar[text1.length()]; //variable to hold char array created from string, since some functions used to obtain textWidth info requires char* instead of string
                strcpy(textToChar, text1.c_str());                               
                TTF_SizeUTF8(fontHolder, textToChar, &textWidth, NULL);

                //Scroll support, checks if text is too large and exceeds vision bounds
                if ((textWidth) > (visibleTextHolder.w - fontWidth))
                    //sets cursor and text position accordingly
                    textPosition.x = visibleTextHolder.x + visibleTextHolder.w - fontWidth - textWidth;

                if (!(text1+text2).empty()) {
                    //cursor set to appropriate visible position
                    cursorPosition.x = textPosition.x + textWidth - cursorWidth;
                    cursorPosition.y = (visibleTextHolder.y+((visibleTextHolder.h - cursorHeight)/2));
                }

             } else {

                 //double click selects all text when focused
                 if (event->button.clicks == 2) {
                    if (!select) {
                        select = true;

                        char textToChar[text1.length() + text2.length()];
                        strcpy(textToChar, (text1+text2).c_str());                               
                        TTF_SizeUTF8(fontHolder, textToChar, &textWidth, NULL); //gets width of text in the textbox
                        
                        //generates selection rectangle from metrics obtained
                        assignRect(selectionRect, textPosition.x + fontWidth/2, textPosition.y + fontHeight/2, textWidth, fontHeight);

                    } 

                 } else {
                
                    //single click deselects all text when focused
                    if (select) {
                        select = false;

                    }

                 }
             }

        } else { //if mouse clicks outside of the box

            //de-focuses
            if (focus) { 
                focus = false;
                textPosition.x = textOriginPosition.x;

                //cursor set to nonvisible (outside of clip rectangle) location
                cursorPosition.x = 0;
                cursorPosition.y = 0;

                //text before and after cursor combined; basically "saves" any edits
                text1.append(text2);
                text2 = "";

            }
        }
    }

    //key events are only handled when the textbox is focused
    if (focus) {
        
        if(event->type == SDL_KEYDOWN ) {

			if (event->key.keysym.sym == SDLK_a && SDL_GetModState() & systemKey) { //Handle select-all command (ctrl A or cmd A)

                //if the text has already been selected, then do nothing
                if (!select) {
                    select = true;

                    char textToChar[text1.length() + text2.length()];
                    strcpy(textToChar, (text1+text2).c_str());                               
                    TTF_SizeUTF8(fontHolder, textToChar, &textWidth, NULL);

                    //generates selection highlighter rectangle based on text dimensions
                    assignRect(selectionRect, textPosition.x + fontWidth/2, textPosition.y + fontHeight/2, textWidth, fontHeight);

                }

			} else if (event->key.keysym.sym == SDLK_c && SDL_GetModState() & systemKey){ //Handle copy

                //will only copy selected text, but this program only allows for select all
				if (select) SDL_SetClipboardText((text1+text2).c_str());
                    
			} else if (event->key.keysym.sym == SDLK_RIGHT) {

                if (select) {

                        //handles text field strings (all text moved to string holding pre-cursor - no pun intended - text) and cursor position
                        text1.append(text2);
                        cursorPos = text1.length();
                        text2.clear();
                        
                        //used for getting render width of text field
                        char textToChar[text1.length()];
                        strcpy(textToChar, text1.c_str());                               
                        TTF_SizeUTF8(fontHolder, textToChar, &textWidth, NULL);

                        cursorPosition.x = (textPosition.x + textWidth - cursorWidth); //handles physical cursor position

                        select = false; //moving cursor to end of selection also causes deselection

                }

                if (cursorPos < (text1+text2).length()) { //right arrow key only does something if the cursor is not yet at the end of the input field

                    TTF_GlyphMetrics(fontHolder, text2.front(), NULL, NULL, NULL, NULL, &charWidth); //gets width of character in front of cursor

                    //handles text field strings (single char moved from beginning of post-cursor string to end of pre-cursor string) and cursor position
                    text1.push_back(text2.front());
                    text2.erase(0,1);
                    cursorPos++;

                    cursorPosition.x += charWidth; //handles physical cursor position

                }

                //used for getting render width of text field. I originally wanted to avoid this repeat by just having this section of code
                //occur once out of the conditional statement, however, this block of code must be used in the above part to handle cursor position so rip 
                char textToChar[text1.length()];
                strcpy(textToChar, text1.c_str());  
                TTF_SizeUTF8(fontHolder, textToChar, &textWidth, NULL);    

                //Scrolling support, check if cursor off bounds
                if (cursorPosition.x+cursorWidth > (visibleTextHolder.x + visibleTextHolder.w - fontWidth)) {
                    
                    //sets cursor and text position accordingly
                    cursorPosition.x =(visibleTextHolder.x + visibleTextHolder.w - fontWidth - cursorWidth);
                    textPosition.x = ((cursorPosition.x+cursorWidth)-textWidth);

                }
                
            } else if (event->key.keysym.sym == SDLK_LEFT) {
            
                if (select) { //if all text is selected

                    //handles text fields (all text moved to post-cursor text string)
                    text2.insert(0,text1);
                    text1.clear();

                    //sets cursor and text positions 
                    textPosition.x = textOriginPosition.x;
                    cursorPos = 0;
                    cursorPosition.x = textOriginPosition.x-cursorWidth;

                    select = false; //moving cursor in completely selected text causes deselection

                }
                
                if (cursorPos > 0) { //events that wont be handled if cursor is already at the leftmost edge of the text field

                    TTF_GlyphMetrics(fontHolder, text1.back(), NULL, NULL, NULL, NULL, &charWidth); //gets width of character in front of cursor

                    //handles text fields (last char of pre-cursor string added to the beginning of post-cursor string)
                    text2.insert(text2.begin(), text1.back());
                    text1.pop_back();
                    
                    //handles cursor position
                    cursorPosition.x -= charWidth;
                    cursorPos--;
                    
    
                }

                //Scroll support, checks if cursor is too far left out of view
                if ((cursorPosition.x+cursorWidth) < (visibleTextHolder.x+fontWidth)) {

                    cursorPosition.x += 10*fontWidth;
                    textPosition.x += 10*fontWidth;

                    //checks if the text has been overscrolled backwards
                    if (textPosition.x > textOriginPosition.x) {
                        
                        //sets cursor and text position accordingly
                        cursorPosition.x -= (textPosition.x - textOriginPosition.x);
                        textPosition.x = textOriginPosition.x;
                        
                    }

                }

            } 

        } 
    }
}


void Textbox::updateAndRender(SDL_Renderer* renderer) { //updates text and renders everything

    //loads text box box before rendering cursor + text, which are both clipped
    boxTexture.render(renderer, position.x, position.y);
    SDL_RenderSetClipRect(renderer, &visibleTextHolder);

    if ((text1+text2).empty()) //if text field is empty
        textTexture.loadFromRenderedText(" ", textColour, renderer, fontHolder);
    else
        textTexture.loadFromRenderedText((text1 + text2), textColour, renderer, fontHolder); 
    
    if (select) { //if select, render selection rectangle
        
        //saves previous render colour and temporarily modifies it
        Uint8* oldColor = new Uint8[4];
        SDL_GetRenderDrawColor(renderer, (oldColor), (oldColor+1), (oldColor+2), (oldColor+3));
        SDL_SetRenderDrawColor(renderer, selectColour.r, selectColour.g, selectColour.b, selectColour.a);

        //set rect position
        selectionRect.x = textPosition.x;
        selectionRect.y = textPosition.y;

        //draw rect and revert renderer colour
        SDL_RenderFillRect(renderer, &selectionRect);
        SDL_SetRenderDrawColor(renderer, *(oldColor), *(oldColor+1), *(oldColor+2), *(oldColor+3));

    }

    if (focus) { //text shown at different positions depending on whether box is focused or not
        textTexture.render(renderer, textPosition.x, textPosition.y);
        cursorTexture.render(renderer, cursorPosition.x, cursorPosition.y);

    } else
        textTexture.render(renderer, textOriginPosition.x, textOriginPosition.y);

}
