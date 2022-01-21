//Airi Shimamura R11652198 Assignment#3 10/12/2020
//This program identify lexemes and tokens found in a source code file provided by the user.

#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <fstream>
#include <ios>
#include <cstring>

using namespace std;

//global variables
int nextToken;
int charClass;
int lexLen;
int lex();
 
char lexeme [100];
char token;
char nextChar;
char previousChar = '\0';

void addChar();
void getChar();
void getNonBlank();

#define D_LETTER 0
#define D_DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define D_ASSIGN_OP 10
#define D_LESSER_OP 11
#define D_GREATER_OP 12
#define D_EQUAL_OP 13
#define D_NEQUAL_OP 14
#define D_LEQUAL_OP 15
#define D_GEQUAL_OP 16
#define D_ADD_OP 17
#define D_SUB_OP 18
#define D_MULT_OP 19
#define D_DIV_OP 20
#define D_KEY_READ 21
#define D_KEY_WRITE 22
#define D_KEY_WHILE 23
#define D_KEY_DO 24
#define D_KEY_OD 25
#define D_IDENT 26
#define D_INT_LIT 27
#define D_LEFT_PAREN 28
#define D_RIGHT_PAREN 29
#define D_SEMICOLON 30
#define D_UNKNOWN 31

bool isDouble = false;
FILE *in_fp;

const char *token_name [] = {
    "ASSIGN_OP",
    "LESSER_OP",
    "GREATER_OP",
    "EQUAL_OP",
    "NEQUAL_OP",
    "LEQUAL_OP",
    "GEQUAL_OP",
    "ADD_OP",
    "SUB_OP",
    "MULT_OP",
    "DIV_OP",
    "KEY_READ",
    "KEY_WRITE",
    "KEY_WHILE",
    "KEY_DO",
    "KEY_OD",
    "IDENT",
    "INT_LIT",
    "LEFT_PAREN",
    "RIGHT_PAREN",
    "SEMICOLON",
    "UNKNOWN"
    };

int main (int argc, char *argv[]){
    
cout << "DanC Analyzer::R11652198" << endl << endl;
//open the input data file 
 if ((in_fp = fopen(argv[1],"r")) == NULL){   
    cout << "Cannot open file" << endl;
    return 0;
}
else{
    getChar();
    do {
        lex();
    } while (nextToken != EOF);
    }
    
return 0;
}
//check character by character to find lexme
int lookup(char ch) {
    
    switch (ch) {
        case ':':
          addChar();
          getChar();

          if(nextChar == '='){
            addChar();
            nextToken = D_ASSIGN_OP;
            break;
          }
          else if(nextChar != ' ' && nextChar != '\n'){
            previousChar = nextChar;
          }
          
            nextToken = D_UNKNOWN;
            break;
        case '<':
            addChar();
            getChar();

            if(nextChar == '>'){
              addChar();
              nextToken = D_NEQUAL_OP;
              break;
            }
            else if (nextChar == '='){
              addChar();
              nextToken = D_LEQUAL_OP;
              break;
            }
            else if (nextChar != ' ' && nextChar != '\n'){
              previousChar = nextChar;
            }

            nextToken = D_LESSER_OP;
            break;
        case '>':
            addChar();
            getChar();

            if(nextChar == '='){
              addChar();
              nextToken = D_GEQUAL_OP;
              break;
            }
            else if(nextChar != ' ' && nextChar != '\n'){
              previousChar = nextChar;
            }

            nextToken = D_GREATER_OP;
            break;
        case '=':
            addChar();
            nextToken = D_EQUAL_OP;
            break;
        case '+':
            addChar();
            nextToken = D_ADD_OP;
            break;
        case '-':
            addChar();
            nextToken = D_SUB_OP;
            break;
        case '*':
            addChar();
            nextToken = D_MULT_OP;
            break;
        case '/':
            addChar();
            nextToken = D_DIV_OP;  
            break; 
        case '(':
            addChar();
            nextToken = D_LEFT_PAREN;     
            break;
        case ')':
            addChar();
            nextToken = D_RIGHT_PAREN;
            break;
       case ';':
            addChar();
            nextToken = D_SEMICOLON;
            break;
        default:
            addChar();
            nextToken = D_UNKNOWN;
            break;
    }

    if(isDouble){
      nextToken = lookup(nextChar);
    }
    return nextToken;
}

/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    } else {
        cout << "Error - lexeme is too long" << endl;
    }
}

/*****************************************************/
/* getChar - a function to get the next character of input and determine its 
 * character class */
void getChar() {

    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar))
            charClass = D_LETTER;
        else if (isdigit(nextChar))
            charClass = D_DIGIT;
        else charClass = UNKNOWN;
    } else {
        charClass = EOF;
    }
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it returns a non-whitespace 
 * character */
void getNonBlank() {
    while (isspace(nextChar)) getChar();
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic expressions */
int lex() {
    lexLen = 0;
    getNonBlank();

    switch (charClass) {
        /* Parse identifiers */
            
        case D_LETTER:
            addChar();
            getChar();
            while (charClass == D_LETTER || charClass == D_DIGIT) {
                addChar();
                getChar();
            }
            nextToken = D_IDENT;
            break;

        /* Parse integer literals */
        case D_DIGIT:
            addChar();
            getChar();
            while (charClass == D_DIGIT) {
                addChar();
                getChar();
            }
            nextToken = D_INT_LIT;
            break;

        /* Parentheses and operators */
        case UNKNOWN:
            nextToken = lookup(nextChar);
            getChar();
            break;

        /* EOF */
        case EOF:
            nextToken = EOF;
    } /* End of switch */
    
    if(nextToken == EOF){
      return nextToken;
    }

//find "read", "write", "while", "do", "od"
   string c = lexeme;
   string::size_type i = c.find("read");
   if(i != string::npos){
     nextToken = D_KEY_READ;
     c.erase(i,4);
     if(i == 0){
     cout << "read  " << token_name[nextToken-10] << endl;
     if(c.length())
     cout << c << "  " << token_name[16] << endl;
     }
     else{
     if(c.length())
     cout << c << "  " << token_name[16] << endl;
     cout << "read  " << token_name[nextToken-10] << endl;
    }
   }

   else if((i = c.find("write")) != string::npos){
     nextToken = D_KEY_WRITE;
     c.erase(i,5);
     if(i == 0){
       cout << "write  " << token_name[nextToken-10] << endl;
       if(c.length())
     cout << c << "  " << token_name[16] << endl;
     }
     else{
      if(c.length())
      cout << c << "  " << token_name[16] << endl;
      cout << "write  " << token_name[nextToken-10] << endl;
     }
   }
   else if((i = c.find("while")) != string::npos){
     nextToken = D_KEY_WHILE;
     c.erase(i,5);
     if(i == 0){
      cout << "while  " << token_name[nextToken-10] << endl;
      if(c.length())
      cout << c << "  " << token_name[16] << endl;
     }
     else{
      if(c.length())
      cout << c << "  " << token_name[16] << endl;
      cout << "while  " << token_name[nextToken-10] << endl; 
     }
   }
   else if((i = c.find("od")) != string::npos){
     nextToken = D_KEY_OD;
     c.erase(i,2);
     if(i == 0){
       cout << "od  " << token_name[nextToken-10] << endl;
       if(c.length())
       cout << c << "  " << token_name[16] << endl;
     }
     else{
       if(c.length())
       cout << c << "  " << token_name[16] << endl;
       cout << "od  " << token_name[nextToken-10] << endl;
     }
   }

   else if((i = c.find("do")) != string::npos){
     nextToken = D_KEY_DO;
     c.erase(i,2);
     if(i == 0){
       cout << "do  " << token_name[nextToken-10] << endl;
       if(c.length() > 0)
       cout << c << "  " << token_name[16] << endl;
     }
     else{
      if(c.length() > 0)
      cout << c << "  " << token_name[16] << endl;
      cout << "do  " << token_name[nextToken-10] << endl;
     }
     
   }

  else
   cout << lexeme << "  " << token_name[nextToken-10] << endl;

   if(previousChar != '\0'){

     nextToken = lookup(previousChar);
     cout << previousChar << "  " << token_name[nextToken-10] << endl;
     previousChar = '\0';  /* reset previousChar */
   }

    return nextToken;
} /* End of function lex */

