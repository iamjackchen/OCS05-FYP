#include <string>
#include <limits>
using namespace std;

class Stack {

    private:
        int size; //keeps track of array size
        int indexCounter; //index counter = keeps track of the index of the highest full slot

        int opStatus; 
        // used to hold status, used for detection of runtime errors. For this specific prompt
        // I don't think the concept of user input validation was as applicable, since you would have to
        // compute the input before any errors could be dected. In which case, I just decided to create
        // a opStatus variable that can be used to output any runtime errors (that result from invalid input)
        // instead of user input validation 


        double *main;
        double *temp; //used for elongating stack size as a temporary variable

        void increaseSize(int sizeDifference = 25); //should eliminate stack overflow
        void optimiseMemory(); //for handing cases where a previous size increase is no longer necessary and is just wasting memory

    public:

        //returns operating status
        int getStatus() { return opStatus; }

        //sets operating status
        void setStatus(int x) { opStatus = x; }

        //returns char array length
        int getMaxSize() { return size; }

        int getCurrentSize() { return indexCounter; } //# of indexes that are filled

        //if array is empty
        bool isEmpty() { return (indexCounter == 0); }

        //resets array
        void reset(int x = 25);

        //pushes value to top of stack
        void push(double x) { *(main+(indexCounter++)) = x; }

        //pops value off top of stack (deletes last value on stack)
        void pop() { 
            if (isEmpty()) exit(EXIT_FAILURE); //if stack underflow is not caught by caller function 
            else { *(main+(indexCounter--)) = numeric_limits<double>::min(); optimiseMemory(); } } 

        //returns last value on stack but does not modify stack
        double peek() { return *(main + (indexCounter-1)); }

        //constructor
        Stack(int x = 100) { main = nullptr; reset(x); } 
        //I originally thought that I would have a stack that would reset itself after each calculation..
        //... and creating a new stack is basically the same as resetting it, so i recycled the code!
        //I ended up using pointers and dynamically allocated stack objects though, so the reset code remained unused :/



};

void Stack::reset(int x) {

    //deletes main if it is not a nullptr
    if (main != nullptr)
        delete[] main;     

    //creates new array of desired size
    main = new double[x];
    size = x;

    //inits index counter
    indexCounter = 0;

    for (int i = 0; i < size; i++) //fills it with null flags
        *(main + i) = numeric_limits<double>::min();      //minimum for is the null flag

    opStatus = 0; //inits operating status

}

void Stack::increaseSize(int sizeDifference) {

    //below creates a temporary array, copies current array to temporary array,
    //current array deleted and new one of desired size created. old array copied to new array, old array terminated
    temp = new double[size];
            
    for (int i = 0; i < size; i++) 
        *(temp + i) = *(main + i);
            
    delete[] main;

    main = new double[size + sizeDifference];

    for (int i = 0; i < size; i++) 
        *(main + i) = *(temp + i);

    delete[] temp;
    temp = nullptr;

    size += sizeDifference;

}


void Stack::optimiseMemory() { //clears excess memory (ie. if the stack is elongated, but then multple values are removed)
                               // in such a case there would be lots of vacant array indices on the stack, wastes memory

    if ((indexCounter + 35) < size) { //if more than 35 indexes unused 
        
        /*
          I set the threshold for memory optimisation at 35 indexes, 10 more than
          would originally be added by a default size increase. This should make
          the memory optimiser less sensitive. After all, each time it runs, it also
          consumes resources. I'm trying to strike the optimal balance between having
          the optimiser run all the time (too resource consuming) and leaving the memory
          unoptimised (also can get too resource consuming).
        */


        //below creates a temporary array, copies current array to temporary array,
        //current array deleted and new one of desired size created. old array copied to new array, old array terminated
        temp = new double[indexCounter];
            
        for (int i = 0; i < indexCounter; i++) 
            *(temp + i) = *(main + i);
            
        delete[] main;

        main = new double[indexCounter+35];
        size = indexCounter+35;

        for (int i = 0; i < indexCounter; i++) 
            *(main + i) = *(temp + i);

        delete[] temp;
        temp = nullptr;

    }

}