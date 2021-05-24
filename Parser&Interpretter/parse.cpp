#include <cctype>
#include <map>
#include "parse.h"
#include <vector>
bool Prog(istream &in, int &line){
    LexItem token;
    bool status=true;
    token=Parser::GetNextToken(in,line);
    //checks for program start with PROGRAM
    if(token.GetToken()!=PROGRAM){
        ParseError(line,"Program does not start with PROGAM");
        status=false;
    }
    token=Parser::GetNextToken(in,line);
    //checks for program name
    if(token.GetToken()!=IDENT){
        ParseError(line,"PROGRAM does not have a name");
        status=false;
    }
    //returns false if either check is false.
    if(!status){
        return status;
    }
    //checks for successful parse of Declaration
    bool declarationCheck=Decl(in,line);
    if(!declarationCheck){
        return false;
    }
    //checks for successful parse of Statement
    bool statementCheck=Stmt(in,line);
    if(!statementCheck){
        return false;
    }
    token=Parser::GetNextToken(in,line);
    //checks for END following declaration
    if(token.GetToken()!=END){
        ParseError(line,"Missing END of Program");
        return false;
    }
    token=Parser::GetNextToken(in,line);
    //checks for PROGRAM following END
    if(token.GetToken()!=PROGRAM){
        ParseError(line,"Program doesn't end with PROGRAM");
        status=false;
    }
    token=Parser::GetNextToken(in,line);
    //checks for program name following PROGRAM
    if(token.GetToken()!=IDENT){
        ParseError(line,"Program doesn't end with name");
        status=false;
    }
    return status;
}   
//Declaration Function
bool Decl(istream &in, int& line){
    bool status= false;
    LexItem tok;
    LexItem t= Parser::GetNextToken(in,line);
    if(t==INTEGER||t==REAL||t==CHAR){
        tok=t;
        tok=Parser::GetNextToken(in,line);
        if(tok.GetToken()==COLON){
            status=IdList(in,line,t);
            if(status){
                status=Decl(in,line);
                return status;
            }
        }else{
            ParseError(line,"Missing Colon");
            ParseError(line,"Incorrect Declaration in Program");
            return false;
        }
    }
    Parser::PushBackToken(t);
    return true;
}

//IdList Function
bool IdList(istream& in, int& line, LexItem tok){
    bool status;
    LexItem next;
    LexItem t = Parser::GetNextToken(in,line);
    if(t.GetToken()==IDENT){
        if(defineVar.count(t.GetLexeme())==0){
            defineVar.insert(std::pair<std::string, bool>(t.GetLexeme(),true));
            SymbolTable.insert(std::pair<std::string, Token>(t.GetLexeme(),t.GetToken()));
        }
        else{
            ParseError(line,"Variable Redefinition");
            return false;
        }
        next=t;
        next=Parser::GetNextToken(in,line);
        if(next.GetToken()==COMA){
            status=IdList(in,line,tok);
            if(status){
                return status;
            }else{
                return false;
            }
        }else{
            Parser::PushBackToken(next);
            return true;
        }
    }else{
        ParseError(line,"Missing Variable");
        return false;
    }
}
//Statement Function
bool Stmt(istream& in, int& line){
    bool status;
    LexItem t=Parser::GetNextToken(in,line);
    switch(t.GetToken()){
        case PRINT:
            status=PrintStmt(in,line);
            if(status){
                status=Stmt(in,line);
            }
            else{
                return status;
            }
            break;
        case IF:
            status=IfStmt(in,line);
            if(status){
                status=Stmt(in,line);
            }else{
                return status;
            }
            break;
        case IDENT:
            Parser::PushBackToken(t);
            status=AssignStmt(in,line);
            if(status){
                status=Stmt(in,line);
            }else{
                return status;
            }
            break;
        case READ:
            status=ReadStmt(in,line);
            if(status){
                status=Stmt(in,line);
            }
            else{
                return status;
            }
            break;
        default:
            Parser::PushBackToken(t);
            return true;
    }
        return status;
}
//Read Statement Function
bool readFlag=false;
bool ReadStmt(istream& in, int& line){
    bool status;
    LexItem t=Parser::GetNextToken(in,line);
    if(t.GetToken()==COMA){
        readFlag=true;
        status=VarList(in,line);
        readFlag=false;
        if(!status){
            ParseError(line,"Missing variable after READ");
            ParseError(line,"Incorrect Statement in program");
            return status;
        }
    }else{
        ParseError(line,"No coma following READ statement");
        ParseError(line,"Incorrect Statement in program");
        return false;
    }
    return status;
}
//Variable List Function
bool VarList(istream& in, int& line){
    bool status;
    LexItem next;
    status=Var(in,line);
    if(!status){
        ParseError(line,"Missing Variable");
        return status;
    }
    next=Parser::GetNextToken(in,line);
    if(next.GetToken()==COMA){
        status=VarList(in,line);
        return status;
    }else{
        Parser::PushBackToken(next);
        return true;
    }
}
//Variable Function
bool Var(istream& in, int& line){
    LexItem t = Parser::GetNextToken(in,line);
    if(defineVar.count(t.GetLexeme())==0){
        ParseError(line,"Undeclared Variable");
        return false;
    }
    if(t.GetToken()!=IDENT){
        ParseError(line,"unkown variable");
        return false;
    }
    if(readFlag){
        std::string check;
        std::cin>>check;
    }
    return true;
}
bool AssignStmt(istream& in, int& line){
    bool status;
    status=Var(in,line);
    if(!status){
        ParseError(line,"Missing Left-Hand Side Variable in Assignment statement");
        ParseError(line,"Incorrect Statement in program");
        return false;
    }
    LexItem t = Parser::GetNextToken(in,line);
    if(t.GetToken()!=ASSOP){
        ParseError(line,"Missing equal operator after LHS");
        ParseError(line,"Incorrect Statement in program");
        return false;
    }
    status=Expr(in,line);
    if(!status){
        ParseError(line,"Incorrect Statement in program");
        return false;
    }
    return status;
}

bool Expr(istream& in, int& line){
    bool status;
    LexItem next;
    status=Term(in,line);
    if(!status){
        return false;
    }
    next=Parser::GetNextToken(in,line);
    if(next.GetToken()==PLUS||next.GetToken()==MINUS){
        status=Expr(in,line);
    }else{
        Parser::PushBackToken(next);
        return status;
    }
    return status;
}

bool Term(istream& in, int& line){
    bool status;
    LexItem next;
    status=SFactor(in,line);
    if(!status){
        return false;
    }
    next=Parser::GetNextToken(in,line);
    if(next.GetToken()==MULT||next.GetToken()==DIV){
        status=Term(in,line);
    }else{
        Parser::PushBackToken(next);
        return status;
    }
    return status;
}
bool SFactor(istream& in, int& line){
    bool status;
    LexItem t;
    t=Parser::GetNextToken(in,line);
    if(t.GetToken()==PLUS||t.GetToken()==MINUS){
        status=Factor(in,line,1);
    }else{
        Parser::PushBackToken(t);
        status=Factor(in,line,0);
    }
    return status;
}
bool Factor(istream& in, int& line, int sign){
    bool status=false;
    LexItem t; 
    t=Parser::GetNextToken(in,line);
    if(t.GetToken()==IDENT||t.GetToken()==ICONST||t.GetToken()==RCONST||t.GetToken()==SCONST){
        if(t.GetToken()==IDENT){
            if(defineVar.count(t.GetLexeme())==0){
                ParseError(line,"Undefined Variable");
                return false;
            }
        }
        return true;
    }
    else if(t.GetToken()==LPAREN){
        status=Expr(in,line);
        t=Parser::GetNextToken(in,line);
        if(t.GetToken()==RPAREN){
            return status;
        }else{
            ParseError(line,"Missing RPAREN in factor");
            status=false;
        }
    }
    //checks for errors from lexer
    if(t.GetToken()==ERR){
        ParseError(line,"Unrecognized Input Pattern\n("+t.GetLexeme()+")");
    }
    return status;
}

bool PrintStmt(istream& in, int& line){
    bool status=false;
    LexItem t;
    t=Parser::GetNextToken(in,line);
    if(t.GetToken()!=COMA){
        ParseError(line,"Missing COMA after PRINT");
        ParseError(line,"Incorrect Statement in program");
        return false;
    }
    status=ExprList(in,line);
    if(!status){
        ParseError(line,"Missing Expression after PRINT");
        ParseError(line,"Incorrect Statement in program");
        return status;
    }
    return status;
}

bool ExprList(istream& in, int& line){
    bool status=false;
    LexItem next;
    status=Expr(in,line);
    if(!status){
        ParseError(line,"Incorrect Expression");
        return status;
    }
    next=Parser::GetNextToken(in,line);
    if(next.GetToken()==COMA){
        status=ExprList(in,line);
        return status;
    }else{
        Parser::PushBackToken(next);
        return true;
    }    
}
bool IfStmt(istream &in, int& line){
    bool status;
    LexItem next;
    next=Parser::GetNextToken(in,line);
    if(next.GetToken()!=LPAREN){
        ParseError(line,"Missing LPAREN in IF statement");
        return false;
    }
    status=LogicExpr(in,line);
    if(!status){
        ParseError(line,"Incorrect Logic Expression");
        return status;
    }
    next=Parser::GetNextToken(in,line);
    if(next.GetToken()!=RPAREN){
        ParseError(line,"Missing RPAREN in IF statement");
        return false;
    }
    next=Parser::GetNextToken(in,line);
    if(next.GetToken()!=THEN){
        ParseError(line,"Missing THEN in IF statement");
        return false;
    }
    status=Stmt(in,line);
    if(!status){
        ParseError(line,"Incorrect Statement following THEN");
        return false;
    }
    next=Parser::GetNextToken(in,line);
    if(next.GetToken()!=END){
        ParseError(line,"Missing END in IF statement");
        return false;
    }
    next=Parser::GetNextToken(in,line);
    if(next.GetToken()!=IF){
        ParseError(line,"Missing closing IF in IF statement");
        return false;
    }
    return status;
}
bool LogicExpr(istream &in, int &line){
    bool status;
    LexItem next;
    status=Expr(in,line);
    if(!status){
        ParseError(line,"Incorrect expression in logical expression");
        return false;
    }
    next=Parser::GetNextToken(in,line);
    if(next.GetToken()!=LTHAN&&next.GetToken()!=EQUAL){
        ParseError(line,"Invalid operator in Logical Expr");
        return false;
    }
    status=Expr(in,line);
    if(!status){
        ParseError(line,"Incorrect expression in logical expression");        
        return false;
    }
    return status;
}