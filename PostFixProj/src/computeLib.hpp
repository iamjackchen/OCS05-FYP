#include <cmath>
#include "stackLib.hpp"

bool isOperator(char x) {  //tests for if a character is an operator
    if (x == '+' || x == '-' || x == '*' || x == '/' || x == '^')
        return true;
    return false;
}

bool isNumber(char x) { //tests for if a character is number
    if (x >= '0' && x <= '9')
        return true;
    return false;
}

double charToDouble(char x) { //converts char value to double value; e.g. '6' != 6

    return x - '0';

}

void parseNewInput(int* position, int size, char* x, Stack* y) { 
    
    //used for converting a char array section representing a number
    //to a double which is pushed onto the stack
    //supports decimals!! :D 

    double pushVal = 0;
    int decimalCounter = 1;

    //parses whole number (non-decimal) section
    while (isNumber(*(x+(*position))) && ((*position) < size)) {
        pushVal *= 10;
        pushVal += charToDouble(*(x+((*position)++)));
    }

    if (((*position) < size)) {
        if (*(x+(*position)) == ' ') { y->push(pushVal);/*if a space is detected (end of number), push to stack*/}

        //if a period (decimal point) is detected
        else if (*(x+(*position)) == '.') {

            (*position)++;
            while (isNumber(*(x+(*position))) && ((*position) < size)) {

                //parses decimal section (if exists, detected by enclosing conditional)
                pushVal += (charToDouble(*(x+((*position)++))) * (1.0/(pow(10,(decimalCounter++)))));
                
            }

            if (((*position) < size)) {
                if (*(x+(*position)) == ' ') {y->push(pushVal); /*if a space is detected (end of number), push to stack */}
                else if (*(x+(*position)) == '.') y->setStatus(4); //second period detected leads to error status
                else {y->setStatus(1);} //no space (or period) detected leads to error status
            } else y->push(pushVal);

        } else y->setStatus(1); //no period or space detected after the whole number section leads to error status

    } else {y->push(pushVal);} //if there are no spaces after character; ie. character is the last in the array

}

int compute(char* x, int size, double* output, bool verbose = false) {   
    
    //this function actually returns the debug status instead of the output of the evaluation,
    //which is written to another variable passed by pointer into the function
    //I figured this might be better since only under the situation that the compute function runs
    //without error, would the output be useful for return to its caller

    int returnVal; 
    double *temp1, *temp2; //temp vars used later
    int* i = new int; *i = (verbose)?1:0; //index counter for traversing through the array. starts at 1 for verbose mode (first char = 'v' or 'V')

    Stack* computeStack = new Stack; //dynamically allocated stack

    while (*i < size && computeStack->getStatus() == 0) { //scans through the array along with (*i)++, see last statement of loop

        if (isOperator(*(x+(*i)))) { //if current char is an operator

            if (computeStack->getStatus() == 0 && computeStack->getCurrentSize() < 2)
                computeStack->setStatus(2);
            else {

                //2 temporary variables created to hold the last 2 elements of the stack
                temp2 = new double; 
                *temp2 = computeStack->peek();
                computeStack->pop();

                temp1 = new double;
                *temp1 = computeStack->peek();
                computeStack->pop();

                switch (*(x+(*i))) {
                    
                    //various cases for operations depending on which operator is detected; result is pushed to stack
                    case '+' : { computeStack->push(((*temp1)+(*temp2))); break; }
                    case '-' : { computeStack->push(((*temp1)-(*temp2))); break; }
                    case '*' : { computeStack->push(((*temp1)*(*temp2))); break; }
                    case '/' : { computeStack->push(((*temp1)/(*temp2))); break; }
                    case '^' : { computeStack->push(pow((*temp1),(*temp2))); break; }

                }

                //temp vars deleted
                delete temp1;
                temp1 = nullptr;

                delete temp2;
                temp2 = nullptr;

            }

        } else if (isNumber(*(x+(*i)))) {parseNewInput(i,size,x,computeStack);} //if current char is a number, then parse the following chars for a number

        else if (*(x+(*i)) == ' ') { /*does nothing if a space is detected in the char array */} 

        else if (computeStack->getStatus() == 0) {computeStack->setStatus(1);} //no character, space or operand detected leads to error

        (*i)++;

    }

    //deletes index counter (has no use after main compute operations)
    delete i;
    i = nullptr;

    //checks if there is more than 1 value remaining on the stack, if so, then set error code for too many operands
    if (computeStack->getStatus() == 0 && computeStack->getCurrentSize() > 1)
        computeStack->setStatus(3);
    else {
        *output = computeStack->peek(); //if only 1 value on stack, then write that value to output (all other errors checked during compute)
    }

    //sets return value to operating status
    returnVal = computeStack->getStatus();

    //deletes compute stack  
    delete computeStack;
    computeStack = nullptr;

    return returnVal;

}