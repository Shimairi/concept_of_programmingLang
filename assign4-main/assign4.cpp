// Airi Shimamura R11652198 Assignment#4 10/30/2020
// This program develops a recursive descent syntax parser in C++
// by using my lexical analyzer developed in the previous assignment

#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <fstream>
#include <ios>
#include <cstring>
#include <stdlib.h>

using namespace std;

//global variables
int nextToken;
int charClass;
int lexLen;
int lex();
int errorCount = 0;
 
char lexeme [100];
char token;
char nextChar;
char previousChar = '\0';

void addChar();
void getChar();
void getNonBlank();
void statement();
void compOp();
void expr();
void term();
void factor();
void error();


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
 
cout << "DanC Parser::R11652198" << endl << endl;
//open the input data file 
if(argc > 1){
 if ((in_fp = fopen(argv[1],"r")) == NULL){   
    cout << "The file does not exist" << endl;
    exit(3);
}
else{
    getChar();

    do {
        lex();
        statement();
    } while (nextToken != EOF);

    //when there is no syntax error, print "Syntax Validated" as the last line of output 
    if(errorCount == 0) {
        cout << "Syntax Validated" << endl;
    }
    }
    }else{ // print error statement when input file is not provided 
        cout<<"no input file is provided"<< endl;
        exit(2);
        }
return 0;
}
//check character by character to find lexme
int lookup(char ch) {
    
    switch (ch) {
        case ':':
          addChar();
          getChar();

          if(nextChar == '='){ //when := is found 
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

            if(nextChar == '>'){ // when lexem is <>
              addChar();
              nextToken = D_NEQUAL_OP;
              break;
            }
            else if (nextChar == '='){ // when lexlem is <=
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

            if(nextChar == '='){ // when lexleme is >= 
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

//find "read"
   string c = lexeme;
   string::size_type i = c.find("read");
   if(i != string::npos){
     nextToken = D_KEY_READ;
     c.erase(i,4);
    // if(i == 0){
    //  cout << "read  " << token_name[nextToken-10] << endl; //print lexeme and token(KEY_READ)
    //  if(c.length())
     // cout << c << "  " << token_name[16] << endl; // print lexeme and token(IDENT)
    // }
    // else{
     // if(c.length())
     // cout << c << "  " << token_name[16] << endl;
     // cout << "read  " << token_name[nextToken-10] << endl;
    //}
   }
//find "write" 
   else if((i = c.find("write")) != string::npos){
     nextToken = D_KEY_WRITE;
     c.erase(i,5);
    // if(i == 0){ 
       //cout << "write  " << token_name[nextToken-10] << endl; // print lexeme and token(KEY_WRITE)
     //  if(c.length())
    // cout << c << "  " << token_name[16] << endl; // print lexeme and token(IDENT)
   //  }
    // else{
    //  if(c.length())
     // cout << c << "  " << token_name[16] << endl;
     // cout << "write  " << token_name[nextToken-10] << endl;
   //  }
   }
// find "while"   
   else if((i = c.find("while")) != string::npos){
     nextToken = D_KEY_WHILE;
     c.erase(i,5);
    // if(i == 0){
      //cout << "while  " << token_name[nextToken-10] << endl; // print lexeme and token(KEY_WHILE)
    //  if(c.length())
     // cout << c << "  " << token_name[16] << endl; // print lexeme and token(IDENT)
    //  }
    //  else{
    //   if(c.length())
     // cout << c << "  " << token_name[16] << endl;
     // cout << "while  " << token_name[nextToken-10] << endl; 
    // }
   }
// find "od"
   else if((i = c.find("od")) != string::npos){
     nextToken = D_KEY_OD;
     c.erase(i,2);
    // if(i == 0){
    // cout << "od  " << token_name[nextToken-10] << endl; // print lexeme and token(KEY_OD)
     //  if(c.length())
     //  cout << c << "  " << token_name[16] << endl; // print lexeme and token(IDENT)
    // }
    // else{
      //  if(c.length())
      //  cout << c << "  " << token_name[16] << endl;
      //  cout << "od  " << token_name[nextToken-10] << endl;
   //  }
   }
// find "do"
   else if((i = c.find("do")) != string::npos){
     nextToken = D_KEY_DO;
     c.erase(i,2);
    // if(i == 0){
     //  cout << "do  " << token_name[nextToken-10] << endl; // print lexeme and token(KEY_DO)
       //if(c.length() > 0)
     //  cout << c << "  " << token_name[16] << endl; // print lexeme and token(IDENT)
    // }
   //  else{
     // if(c.length() > 0)
     // cout << c << "  " << token_name[16] << endl;
     // cout << "do  " << token_name[nextToken-10] << endl;
    // }
   }

//  else  //display output for other lexemes and tokens
//   cout << lexeme << "  " << token_name[nextToken-10] << endl;

   if(previousChar != '\0'){ //for operators
//     nextToken = lookup(previousChar);
//     cout << previousChar << "  " << token_name[nextToken-10] << endl;
     previousChar = '\0';  /* reset previousChar */
   }

    return nextToken;
} /* End of function lex */

//print out syntax error 
void error(){
    errorCount++;
    
    if(nextToken == EOF){
     cout << "Error encounter: The next lexeme was EOF and the next token was EOF" << endl;
    }
    else{
      cout << "Error encounter: The next lexeme was " << lexeme << " and the next token was " << token_name[nextToken-10] << endl;
    }
    exit(1);
}


/* statement
 * Parses strings in the language generated by the rule:
 * <statement> -> <V> := <expr> | read(V) | write(V) | while C do S od | S;S
 */  
void statement(){
    //<V> := <expr>
    if(nextToken == D_IDENT){  
        lex();
        if(nextToken == D_ASSIGN_OP){  //check := 
            lex();
            expr();
        //check semicoloum
       if (nextToken==D_SEMICOLON){
                lex();
                statement();  
                      }
        else if(nextToken != EOF && nextToken != D_KEY_OD){  
              if(nextToken != D_SEMICOLON){
                   error();
                    }
               }      
        }
        else{
          
            error();
        }
        
    } /* End of the if tatement when nextToken is IDENT */
    
    //read(V) or write(V) 
    else if(nextToken == D_KEY_READ||nextToken == D_KEY_WRITE){
        lex();
        if(nextToken == D_LEFT_PAREN){
            lex();
            if(nextToken == D_IDENT){
                lex();
                 if(nextToken == D_RIGHT_PAREN) { 
                    lex();  

                    //check semicoloum
                if (nextToken==D_SEMICOLON){
                      lex();
                      statement();  
                      }
                else if(nextToken != EOF && nextToken != D_KEY_OD){  
                    if(nextToken != D_SEMICOLON){
                          error();
                          }
                     }  
                }
                else{ // error statement when nextToken is not RIGHT_PAREN
                  error();
                }
            }
            else {  // error statement when nextToken is not IDENT
                 error();
            }
        }
        else{  //error statement when nextToken is not LEFT_PAREN
            error();
        }
        
    }/* End of else if statement when nextToken is KEY_READ */
        
    //while C do S od
    else if(nextToken == D_KEY_WHILE){
        lex();
        compOp();
        if(nextToken == D_KEY_DO){
           do{
               lex();
               statement();
               
           } while(nextToken != D_KEY_OD);
        
            lex();
            //check semicoloum
             if (nextToken == D_SEMICOLON){
                      lex();
                      statement();  
                      }
                    else if(nextToken != EOF && nextToken != D_KEY_OD){  
                       if(nextToken != D_SEMICOLON){
                              error();
                              }
                      } 
        }
        else{
            lex();
            error();
        }
       
    } /* End of else if statement when nextToken is KEY_WHILE */

    //print error sentence when none of tokens are matched
    else{
       lex();
        error();
    }  
          
}/* End of function statement */

/* compOP
 * Parses strings in the language generated by the rule:
 * C ::= E < E | E > E | E = E | E <> E | E <= E | E >= E
 */  
void compOp(){
    expr();
   
    if(nextToken == D_LESSER_OP|| nextToken == D_GREATER_OP || nextToken == D_EQUAL_OP || nextToken == D_NEQUAL_OP || nextToken == D_LEQUAL_OP || nextToken == D_GEQUAL_OP) {

    lex();
    expr();
   
    }
    else{
        error();      
    }
} /* End of function compOP */

    
/* expr
 * Parses strings in the language generated by the rule:
 * <expr> -> <term> {(+ | -) <term>}
 * E:= T | E+T | E-T
 */  
void expr(){   
    term();
    
    while(nextToken == D_ADD_OP || nextToken == D_SUB_OP){
        lex();
        term();
    }
}
      
 /* term
 * Parses strings in the language generated by the rule:
 * <term> -> <factor> {(* | /) <factor>)
 * T := F | T*F | T/F
 */   
void term() {
     factor();

     while(nextToken == D_MULT_OP || nextToken == D_DIV_OP){
         lex();
         factor();
     }
 }
 
 /* factor
 * Parses strings in the language generated by the rule:
 * <factor> -> id | int_constant | ( <expr )
 * F := (E) | N | V
 * */    
void factor(){
     
    if (nextToken == D_IDENT || nextToken == D_INT_LIT){
       lex(); 
    }
    else if(nextToken == D_LEFT_PAREN){
            lex();
            expr();

            if(nextToken == D_RIGHT_PAREN) {
                lex();
            }
            else{
                lex();
                error();
            }
        }
   else{
    
      error();
    }
 }
