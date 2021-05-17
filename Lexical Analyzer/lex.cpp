#include "lex.h"
#include <cctype>
//function which checks for invalid sign.
bool invSign(char c){
    char invSigns[15] ={'`','~','@','#','$','^','_','|','\\','}',']','{','[',';','?'};
        for(int i=0;i<15;i++){
            if(c==invSigns[i]){
                return true;
            }
        }
        return false;
}
//checks if operator
bool checkOperator(char c){
    char check[10] = {'+','-','*','/','=','<',':',',','(',')'};
    for(int i=0;i<10;i++){
        if(c==check[i]){
            return true;
        }
    }
    return false;
}
LexItem getNextToken(std::istream& in, int& linenumber){
    enum State{START,INID,INSTRING,ININT,INREAL,INCOMMENT};
    //initial state is START
    State lexState=START;
    std::string lexeme;
    char curr;
    //get characters
    while(in.get(curr)){
        switch(lexState){
            case START:
                //checks for newline
                if(curr=='\n'){
                    linenumber++;
                }
                //checks if invalid sign
                if(invSign(curr)){
                    lexeme+=curr;
                    return LexItem(ERR,lexeme,linenumber);
                }
                //checks for comment/or notequal operator
                else if(curr=='!'){
                    char c = in.peek();
                    if(c=='='){
                        lexeme="NOTEQUAL";
                        in.get(c);
                        return LexItem(NOTEQUAL,lexeme,linenumber);
                    }else{
                        lexState=INCOMMENT;
                    }
                 }
                 //checks for alphabetical types and determines if IDENTIFIER
                else if(isalpha(curr)){
                    lexeme+=curr;
                    char c = in.peek();
                    if(checkOperator(c)||c=='.'||c==' '||c=='\n'||c==EOF||!isalnum(c)){
                        for(int i=0;i<lexeme.size();i++){
                            lexeme[i]=toupper(lexeme[i]);
                        }
                        return LexItem(IDENT,lexeme,linenumber);
                    }
                    else{
                     lexState=INID;
                    }
                }
                //checks for integer types. ex.INREAL or ININT
                else if(isdigit(curr)){
                    lexeme+=curr;
                    char c = in.peek();
                    if(c=='.'){
                        lexState=INREAL;
                    }              
                    else if(c==' '||c=='\n'||c==EOF||!isdigit(c)){
                        return LexItem(ICONST,lexeme,linenumber);
                    }else{
                        lexState=ININT;
                    }
                    if(c=='\n'){
                        linenumber++;
                    }
                }
                //checks for SCONST types
                else if(curr=='"'||curr=='\''){
                    lexeme+=curr;
                    char c=in.peek();
                    if(c==EOF){
                        return LexItem(ERR,lexeme,linenumber);
                    }
                    lexState=INSTRING;
                }
                //checks for RCONSTS that dont have leading 0
                else if(curr=='.'){   
                    lexeme+=curr;
                    char c = in.peek();
                    if(c==' '||c==EOF||c=='\n'||!isdigit(c)){
                        lexeme.push_back(c);
                        return LexItem(ERR,lexeme,linenumber);
                    }else{
                        lexState=INREAL;
                    }    
                }
                //checks for + operator
                else if(curr=='+'){
                    lexeme="PLUS";
                    return LexItem(PLUS,lexeme,linenumber);
                }
                //checks for - operator
                else if(curr=='-'){
                    lexeme="MINUS";
                    return LexItem(MINUS,lexeme,linenumber);
                }
                //checks for * operator
                else if(curr=='*'){
                    lexeme="MULT";
                    return LexItem(MULT,lexeme,linenumber);
                }
                //checks for = or == operator
                else if(curr=='='){
                    char c=in.peek();
                    if(c=='='){
                        lexeme="EQUAL";
                        in.get(c);
                        return LexItem(EQUAL,lexeme,linenumber);
                    }else{
                        lexeme="ASSOP";
                        return LexItem(ASSOP,lexeme,linenumber);
                    }
                }
                //checks for <= or < operator
                else if(curr=='<'){
                    char c=in.peek();
                    if(c=='='){
                        in.get(c);  
                        lexeme="LTHANEQUAL";
                        return LexItem(LTHANEQUAL,lexeme,linenumber);
                    }else{
                        lexeme="LTHAN";
                        return LexItem(LTHAN,lexeme,linenumber);
                    }
                }
                //checks for > or >= operator
                else if(curr=='>'){
                    char c=in.peek();
                    if(c=='='){
                        in.get(c);
                        lexeme="RTHANEQUAL";
                        return LexItem(RTHANEQUAL,lexeme,linenumber);
                    }else{
                        lexeme="RTHAN";
                        return LexItem(RTHAN,lexeme,linenumber);
                    }
                }
                //checks for colon
                else if(curr==':'){
                    lexeme="COLON";
                    return LexItem(COLON,lexeme,linenumber);
                }
                //checks for coma
                else if(curr==','){
                    lexeme="COMA";
                    return LexItem(COMA,lexeme,linenumber);
                }
                //check for left parenthesis
                else if(curr=='('){
                    lexeme="LPAREN";
                    return LexItem(LPAREN,lexeme,linenumber);
                }
                //check for right parenthesis
                else if(curr==')'){
                    lexeme="RPAREN";
                    return LexItem(RPAREN,lexeme,linenumber);
                }
                //checks for div or concat
                else if(curr=='/'){
                    char c=in.peek();
                    if(c=='/'){
                        lexeme="CONCAT";
                        in.get(c);
                        return LexItem(CONCAT,lexeme,linenumber);
                    }else{
                        lexeme="DIV";
                        return LexItem(DIV,lexeme,linenumber);
                    }
                }
                break;
                //IDENT case
            case INID:
            {
                if(isdigit(curr)||isalpha(curr)){
                    lexeme+=curr;
                }
                if(!isalnum(curr)){
                    lexeme+=curr;
                    return LexItem(ERR,lexeme,linenumber);
                }
                char next = in.peek();
                if(checkOperator(next)||next=='.'||next==' '||next=='\n'||!isalnum(next)||next==EOF){
                        for(int i=0;i<lexeme.size();i++){
                            lexeme[i]=toupper(lexeme[i]);
                        }
                    return LexItem(IDENT,lexeme,linenumber);
                }
                if(next==EOF){
                        for(int i=0;i<lexeme.size();i++){
                            lexeme[i]=toupper(lexeme[i]);
                        }
                    return LexItem(IDENT,lexeme,linenumber);
                }
                if(next=='\n'){
                    linenumber++;
                }
                break;
            }
            case INSTRING:
                break;
            case ININT:
                break;
            case INREAL:
                break;
            //comment state
            case INCOMMENT:
                if(curr=='\n'){
                    linenumber++;
                    lexState=START;
                }
                break;
        }
    }
    return LexItem(DONE,"DONE",linenumber);
}
