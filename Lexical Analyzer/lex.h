#include <iostream>
#include <map>
#include <string>

enum Token{
    //key words
    PROGRAM,PRINT,READ,INTEGER,END,IF,THEN,REAL,CHAR,
    //identifier
    IDENT,
    //integer types and string constants
    ICONST,RCONST,SCONST,
    //operators
    PLUS,MINUS,MULT,DIV,ASSOP,LPAREN,RPAREN,COMA,EQUAL,LTHAN,RTHAN,LTHANEQUAL,RTHANEQUAL,NOTEQUAL,CONCAT,COLON,
    //error token
    ERR,
    //EOF-DONE token
    DONE
};
//token print map
std::map<Token,std::string> tPrint = {
    {PROGRAM, "PROGRAM"},
    {INTEGER, "INTEGER"},
    {REAL, "REAL"},
    {CHAR, "CHAR"},
    {PRINT, "PRINT"},
    {IF, "IF"},
    {END, "END"},
    {THEN, "THEN"},
    {IDENT, "IDENT"},
    {ICONST, "ICONST"},
    {RCONST, "RCONST"},
    {SCONST, "SCONST"},
    {PLUS, "PLUS"},
    {MINUS, "MINUS"},
    {MULT, "MULT"},
    {DIV, "DIV"},
    {ASSOP, "ASSOP"},
    {LPAREN, "LPAREN"},
    {RPAREN, "RPAREN"},
    {COLON, "COLON"},
    {COMA, "COMA"},
    {EQUAL, "EQUAL"},
    {LTHAN, "LTHAN"},
    {RTHAN, "RTHAN"},
    {LTHANEQUAL,"LTHANEQUAL"},
    {RTHANEQUAL,"RTHANEQUAL"},
    {NOTEQUAL,"NOTEQUAL"},
    {CONCAT, "CONCAT"},
    {ERR, "ERR"},
    {DONE, "DONE"},
};
std::map<std::string,Token> keywordMap = {
		{"PROGRAM",PROGRAM},
		{"READ",READ},
		{"INTEGER",INTEGER},
		{"REAL",REAL},
		{"CHAR",CHAR},
		{"PRINT",PRINT },
		{"IF",IF},
		{"THEN",THEN},
		{"END",END},   
};
//LexItem Defintion
class LexItem{
    Token token;
    int line;
    std::string lexeme;

public: 
    LexItem() {
        token = ERR;
        line = -1;
    }
    LexItem(Token token, std::string lexeme, int line){
        this->token = token;
        this->line = line;
        this->lexeme = lexeme;
    }
    bool operator==(const Token token) const{
        return this->token == token;
    }
    bool operator!=(const Token token) const{
        return this->token != token;
    }
    Token GetToken() const{
        return token;
    }
    std::string GetLexeme() const{
        return lexeme;
    }
    int GetLinenum() const{
        return line;
    }
};