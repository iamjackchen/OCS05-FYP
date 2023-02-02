#include "textureHandler.hpp"
using namespace std;

class Button
{
	public:
		//Constructor
		Button() {
            position.x = 0; 
            position.y = 0; 
            width = 0;
            height = 0;
            currentTexture = 0; 
            state = false;
        }

        //Destructor
        ~Button() {textureHolder.free();}

        //Initialises texture paths array 
        void initTextures(string path1, string path2) {
            texturePath = new string[2];
            *(texturePath) = path1;
            *(texturePath+1) = path2;

        } 

		//Sets position
		void setPosition(int x, int y) {position.x = x; position.y = y;}

		//Handles mouse events
		void handleEvent(SDL_Event* e, SDL_Renderer* renderer);
	
		//Renders button texture
		void render(SDL_Renderer* renderer) {textureHolder.render(renderer, position.x, position.y);}

        bool loadTexture(SDL_Renderer* renderer); //function for loading texture

        bool isKeyDown() {return state;} //returns if button is pressed

        void freeTexture() {textureHolder.free();} //frees texture

	private:
		
        Texture textureHolder; //Holds texture

		SDL_Point position; //Holds position data

        bool state; //Holds button if-pressed data

        int width, height;

        string* texturePath; //array used for storing texture paths
        //Index 0 for inactive texture
        //Index 1 for active texture

		int currentTexture; 
        //Holds current texture state, I don't *need* this, but keeping track of this allows for memory optimisation for texture updating
        //I can check for whether the texture needs updating (1 compute step) instead of blindly updating each time (multiple compute steps)
        //Although there wouldn't be any visible difference to the user

};



void Button::handleEvent(SDL_Event* e, SDL_Renderer* renderer) {

    //checks for mouse event
	if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP ) {
		
        //Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		//Checks if mouse is inside button bounds
		if((x > position.x) && (x < position.x + width) && (y > position.y) && (y < position.y + height)) {
            
            //changes texture and state accordingly to mouse event type
			switch(e->type)
			{
				case SDL_MOUSEMOTION:
                    if (currentTexture != 1) // I can check here for whether a texture update is needed or not, can save resource comsumption from redundant updates*/ 
                    { currentTexture = 1; loadTexture(renderer); }
				break;
			
				case SDL_MOUSEBUTTONDOWN:
				    state = true;
				break;
				
				case SDL_MOUSEBUTTONUP:
				    state = false;
				break;
			}

        } else {
            if (currentTexture != 0) { currentTexture = 0; loadTexture(renderer); } //refresh texture if necessary, if cursor is not within button bounds
	    }
    }

}

bool Button::loadTexture(SDL_Renderer* renderer) {

    bool returnVal = textureHolder.loadFromFile(*(texturePath+currentTexture), renderer); 
    width = textureHolder.getWidth(); 
    height = textureHolder.getHeight();
    
    return returnVal;

}