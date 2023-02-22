/*
 * @brief Brackets test execution
 *
 * Assumption:
 * The program takes as input a "test.txt" file in which the inputs to be processed are inserted.
 * Each row represents a new input.
 * The length of input that can be processed by the program has a default size of 40 characters, 
 * longer inputs will be truncated and processed based on the first 40 characters.
 * The maximum input size of 40 was chosen arbitrarily as reasonable.
 * Processing of the inputs is output in the "output.txt" file, maintaining the correspondence beetween the input file 
 * lines and output file lines.
 * Processing is done managing cases with a state machine defined in the source code.
 * The algorithm analyzes the parenthesis found in input distinguishing them between exterior (to be removed in output) 
 * and internal (not to be removed).
 * For both internal and external brackets, the algorithm aims to match those open with those closed 
 * for a correct identification of the sequences.
 * The program is robust in the case of pathological inputs, spaces, empty lines, buffer overflows.
 * @date 21 February 2023
 * 
 * @author M.Fiamingo 
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LINE_LENGTH 40 //40 is assumed as the maximum legible length of the line  

enum state_machine{
    NO_STATE,                                   //initial state
    OPEN_FOUND,                                 //found the open parenthesis
    CLOSE_FOUND,                                //found the firts close parenthesis
    NO_EXTERNAL_BRACKETS                        //the line is not enclosed in parentheses
};

typedef struct prog_var{
    char skipBrackets;
    char lastCloseIndex ;
    char line[MAX_LINE_LENGTH+1];               //40 characters and string terminator
    char bracketsIndex[MAX_LINE_LENGTH+1];      //buffer for matching the indexes of the brackets to remove   
    char inputChar;                             //character read from file 
    char openBracketsCounter;                   //counter of the open parentheses for the match with the closed ones in the case of multiple external parentheses
    bool notExternalBracket;
    char charCounter;
    enum state_machine stateMachine;
    char internalCounter;                       //counter of internal parentheses
}progVar;
progVar var;

void updateMatchingBrackets(void);
void resetMatchingBrackets(void);
void printOutput(FILE *output); //function declaration

void updateMatchingBrackets(void){
    var.bracketsIndex[var.charCounter] = var.openBracketsCounter--;
    var.lastCloseIndex = var.charCounter;
    var.notExternalBracket = true; //there will be no more open parenthesis to be considered as external
}

void resetMatchingBrackets(void){
    var.bracketsIndex[var.lastCloseIndex] = 0; //reset last closing brackets matched
    var.lastCloseIndex = 0;
    var.stateMachine = OPEN_FOUND; 
}

void printOutput(FILE *output){ //function definition
    if(var.stateMachine != CLOSE_FOUND){
        fputs(var.line, output);   //same as input
    }
    else{   //output line buffer, skipping matched brackets
        for(int i=0; i<strlen(var.line); i++){
            if(var.bracketsIndex[i] > 0){
                if(var.skipBrackets == 0 || var.bracketsIndex[i] <= var.skipBrackets)
                    continue;
            }
            fputc(var.line[i], output);
        }
    }
    fputc('\n', output);   
}


int main ()
{
    /* INIT VARIABLES */
    FILE *fptr = NULL;                             
    FILE *fOut = NULL;                             
    memset(&var, 0, sizeof(progVar));
    /* INITIAL CHECKS */
    fptr = fopen("test.txt", "r");
    fOut = fopen("output.txt", "w");
    if(fptr == NULL || fOut == NULL){
        exit(EXIT_FAILURE);
    }
    /* FILE READING */ 
    while(!feof(fptr)){ 
        var.inputChar = fgetc(fptr);  //read char by char untill the file ends    
        if(var.inputChar == EOF || var.inputChar == '\n'){ /* PRINT OUTPUT */
            printOutput(fOut);
            memset(&var, 0, sizeof(progVar));  
            continue;            
        }      
        if(strlen(var.line) >= MAX_LINE_LENGTH){ /* CHECK BUFFER OVERFLOW */  
            var.charCounter++; //go to end of line or end of file if line buffer is full
            continue;
        }
        var.line[var.charCounter] = var.inputChar; //copy each input character into the line buffer;  
        /* STATE MACHINE */   
        if(var.stateMachine == NO_STATE){ 
            if(var.inputChar == '('){          
               var.bracketsIndex[var.charCounter] = ++var.openBracketsCounter;  //counts external brackets number that have to be matched   
               var.stateMachine = OPEN_FOUND;
            }
            else if(var.inputChar != ' ' && var.inputChar != '\n') //spaces before parentheses are tolerated, otherwise no external brackets enclosing the line
                var.stateMachine = NO_EXTERNAL_BRACKETS; 
        }
        else if(var.stateMachine == OPEN_FOUND){
            if(var.inputChar == ')'){
                if(var.internalCounter > 0) //there are internal open brackets to be matched
                    var.internalCounter--;
                else{
                    updateMatchingBrackets();
                    if(var.openBracketsCounter <= 0) 
                        var.stateMachine = CLOSE_FOUND; //no other brackets to match
                }
            }
            else{
                if(var.inputChar == '('){
                    if(!var.notExternalBracket){ //multiple external brackets
                        var. bracketsIndex[var.charCounter] = ++var.openBracketsCounter; 
                        var.charCounter++;
                        continue;
                    }
                    else{
                        var.internalCounter++;
                    }
                }
                var.notExternalBracket = true;
                if(var.lastCloseIndex != 0){
                    var.skipBrackets = var.openBracketsCounter;
                    resetMatchingBrackets();
                }
            }
        }
        else if(var.stateMachine == CLOSE_FOUND){ //be sure there are only spaces (tolerated) till the end of line or end of file 
            if((var.inputChar != ' ') && (var.inputChar != '\n') && (var.inputChar != EOF)){
                    var.stateMachine = NO_EXTERNAL_BRACKETS; 
            }
        }
        var.charCounter++;
    }
    fclose(fptr);
    fclose(fOut);    
    exit(EXIT_SUCCESS);
}