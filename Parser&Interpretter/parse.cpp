#include <cctype>
#include <map>
#include "parse.h"
#include <vector>
#include <queue>
//checks if string is a int or rconst
bool isNumber(string str){
    int count=0;
    for(int i=0;i<str.size();i++){
        if(isdigit(str[i])||str[i]=='.'){
            count++;
        }
    }
    if(count!=str.size()){
        return false;
    }else{
        return true;
    }
}
map<string, Value> TempsResults;
queue <Value> * ValQue;
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
    std::string name=token.GetLexeme();
    //checks for successful parse of Declaration
    bool declarationCheck=Decl(in,line);
    if(!declarationCheck){
        ParseError(line,"Incorrect Declaration in Program");
        return false;
    }
    //checks for successful parse of Statement
    bool statementCheck=Stmt(in,line);
    if(!statementCheck){
        ParseError(line,"Incorrect Statement in Program");
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
    //checks that ending and beginning program name is same.
    if(name!=token.GetLexeme()){
        ParseError(line,"Incorrect Program Name");
        return false;
    }
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
            return false;
        }
    }
    Parser::PushBackToken(t);
    return true;
}

//IdList Function
bool IdList(istream& in, int& line, LexItem &type){
	bool status;
	LexItem tok = Parser::GetNextToken(in, line);
	if(tok.GetToken()==IDENT){
		if(defineVar.count(tok.GetLexeme())==0){
			defineVar.insert(std::pair<std::string, bool>(tok.GetLexeme(),true));
			SymbolTable.insert(std::pair<std::string, Token>(tok.GetLexeme(),type.GetToken()));
		}else{
			ParseError(line, "Variable Redefinition");
			return false;
		}
        tok = Parser::GetNextToken(in, line);
	    if (tok==COMA) {
		    status = IdList(in,line,type);
	    }
	    else if(tok.GetToken()==ERR){
		    ParseError(line, "Unrecognized Input Pattern");
		    cout <<"("<<tok.GetLexeme()<<")"<< endl;
		    return false;
	    }
        else{
            Parser::PushBackToken(tok);
            return true;
        } 
	}else{
		ParseError(line, "Missing Variable");
		return false;
	}
	return status;
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
    std::string varName = t.GetLexeme();
    Token type = SymbolTable[varName];
    std::string userInput;
    int input; 
    float finput;
    bool sInput=true;
    if(t.GetToken()!=IDENT){
        ParseError(line,"unkown variable");
        return false;
    }
    if(t.GetToken()==ERR){
        ParseError(line, "Unrecognized Input Pattern");
        cout<<"("<<t.GetLexeme()<<")"<<endl;
        return false;
    }   
    if(readFlag&&TempsResults.count(t.GetLexeme())==0){
        cin>>userInput;
        if(isNumber(userInput)){
            input=stoi(userInput);
            finput=stof(userInput);
            sInput=false;
        }
        if(type==INTEGER){  
            if(sInput){
                ParseError(line,"Cant read in alphanumeric characters into a integer");
                return false;
            }else{
                Value val1(input);
                TempsResults[varName]=val1;
            }
        }
        else if(type==REAL){
            if(sInput){
                ParseError(line,"Cant read in alphanumeric characters into a real");
                return false;
            }else{
                Value val1(finput);
                TempsResults[varName]=val1;
            }
        }
        else if(type==CHAR){
            Value val1(userInput);
            TempsResults[varName]=val1;
        }else{
            ParseError(line,"Cannot read in to types other than INT,REAL,CHAR");
        }
    }
    else if(readFlag&&TempsResults.count(t.GetLexeme())>0){
        cin>>userInput;
        if(isNumber(userInput)){
            input=stoi(userInput);
            finput=stof(userInput);
            sInput=false;
        }
        if(TempsResults[varName].GetType()==VINT){
            if(sInput){
                ParseError(line,"Cant read in alphanumeric characters into a real");
                return false;
            }else{
                TempsResults[varName].SetInt(input);
            }
        }
        else if(TempsResults[varName].GetType()==VREAL){
            if(sInput){
                ParseError(line,"Cant read in alphanumeric characters into a real");
                return false;
            }else{
            TempsResults[varName].SetReal(finput);
            }
        }
        else if(TempsResults[varName].GetType()==VCHAR){
            if(sInput){
                ParseError(line,"Cant read in alphanumeric characters into a real");
                return false;
            }else{
                TempsResults[varName].SetChar(userInput);
            }
        }else{
            ParseError(line,"Cannot read in to types other than INT,REAL,CHAR");
            return false;
        }
    }
    return true;
}

bool AssignStmt(istream& in, int& line){
	bool status;
	LexItem t;
	t=Parser::GetNextToken(in,line);
	std::string varName=t.GetLexeme();
	Parser::PushBackToken(t);
	status=Var(in,line); 	
	Token type = SymbolTable[varName];
    if(!status){
		ParseError(line,"Missing Left-Hand Side Variable in Assignment statement");
		return false;
    }
	t = Parser::GetNextToken(in, line);
    if(t.GetToken()!=ASSOP){
        ParseError(line,"Missing equal operator after LHS");
        return false;
    }
    if(t.GetToken()==ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout <<"("<<t.GetLexeme()<<")"<< endl;
		return false;
    }
	Value val1;
	status = Expr(in, line,val1);
	if(!status){
	    ParseError(line, "Missing Expression in Assignment Statment");
		return false;
    }
    //type checking
	if(val1.GetType()==VINT){
		if(type==CHAR){
		    ParseError(line,"Run Time Error-Incompatitible Types");
		    return false;
	    }
		else if(type==INTEGER){
			TempsResults[varName]=val1;
		}
		else if(type==REAL){
			int value=val1.GetInt();
			val1.SetInt(0);
			val1.SetType(VREAL);
			float newVal=float(value);
			val1.SetReal(newVal);
		    TempsResults[varName]=val1;
		}
    }
    else if(val1.GetType()==VREAL){
	    if(type==CHAR){
			ParseError(line,"Run Time Error-Incompatible Types");
			return false;
		}
		else if(type==REAL){
			TempsResults[varName]=val1;
		}
		else if(type==INTEGER){
			float value=val1.GetReal();
			val1.SetReal(0.0);
			val1.SetType(VINT);
			int newVal=int(value);
		    val1.SetInt(newVal);
			TempsResults[varName]=val1;
		}
	}
	else if(val1.GetType()==VCHAR){
		if(type!=CHAR){
			ParseError(line,"Run Time Error- Incompatible Types");
			return false;
		}else{
			TempsResults[varName]=val1;
		}
	}
	return status;	
}
//Expr Function
bool Expr(istream& in, int& line, Value& retVal){
	Value val1;
    Value val2;
    bool status;
    LexItem next;
	status=Term(in,line,val1);
	if(!status){
		return false;
	}
	retVal=val1;
	next=Parser::GetNextToken(in, line);
	if(next.GetToken()==ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout <<"("<< next.GetLexeme()<<")"<<endl;
		return false;
	}
	while(next.GetToken()==PLUS||next.GetToken()==MINUS){
		status=Term(in,line,val2);
		if(!status){
			ParseError(line, "Missing operand after operator");
			return false;
		}
		if(retVal.GetType()==VCHAR||val2.GetType()==VCHAR){
			ParseError(line, "Run-Time Error-Illegal Mixed Type Operands");
			return false;
		}else{
			if(next.GetToken()==PLUS){
				retVal=retVal+val2;
			}
			else if(next.GetToken()==MINUS){
				retVal = retVal-val2;
			}
		}
		next=Parser::GetNextToken(in, line);
		if(next.GetToken()==ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout<<"("<<next.GetLexeme()<<")"<<endl;
			return false;
		}		
	}
	Parser::PushBackToken(next);
	return true;
}
//Term function
bool Term(istream& in, int& line, Value & retVal){
	Value val1;
    Value val2;
    bool status;
    LexItem next;
	status=SFactor(in,line,val1);
	if(!status){
		return false;
	}
	retVal=val1;
	next=Parser::GetNextToken(in,line);
	if(next.GetToken()==ERR){
			ParseError(line,"Unrecognized Input Pattern");
			cout<<"("<<next.GetLexeme()<<")"<<endl;
			return false;
	}
	while(next.GetToken()==MULT||next.GetToken()==DIV){
		status=SFactor(in,line,val2);
		if(!status){
			ParseError(line,"Missing operand after operator");
			return false;
		}
		if(retVal.GetType()==VCHAR||val2.GetType()==VCHAR){
			ParseError(line,"Run-Time Error-Illegal Mixed Type Operands");
			return false;
		}else{
			if(next.GetToken()==MULT){
				retVal=retVal*val2;
			}
			else if(next.GetToken()==DIV){
				if(val2.GetInt()==0||val2.GetReal()==0.0){
					ParseError(line,"Run-Time Error-Illegal Division by Zero");
					return false;
				}
				retVal = retVal/val2;
			}			
		}
		next=Parser::GetNextToken(in, line);
		if(next.GetToken()==ERR){
			ParseError(line,"Unrecognized Input Pattern");
			cout<<"("<<next.GetLexeme()<<")"<<endl;
			return false;
		}
	}
	Parser::PushBackToken(next);
	return true;
}
//SFactor Function
bool SFactor(istream& in, int& line, Value & retVal){
    bool status;
    LexItem t;
    Value val1;
    int sign=0;
    t=Parser::GetNextToken(in,line);
    if(t.GetToken()==PLUS){
        sign=1;
    }
    if(t.GetToken()==MINUS){
        sign=-1;
    }else{
        Parser::PushBackToken(t);
    }
    status=Factor(in,line,sign,val1);
	if(val1.GetType()==VINT&&sign!=0){
		val1.SetInt(sign*val1.GetInt());
		retVal=val1;
	}
	else if(val1.GetType()==VREAL&&sign!=0){
		val1.SetReal(sign*val1.GetReal());
		retVal=val1;
	}
	else if(val1.GetType()==VCHAR&&sign!=0){
		ParseError(line,"Runtime Error:cant have sign char");
		return false;
	}
	else{
		retVal=val1;
	}
    return status;
}
//Factor function. 
bool Factor(istream& in, int& line, int sign, Value& retVal){
    bool status=false;
    LexItem t;
    Value val1;
	t=Parser::GetNextToken(in,line);
    if(t.GetToken()==IDENT||t.GetToken()==ICONST||t.GetToken()==RCONST||t.GetToken()==SCONST){
	    if(t.GetToken()==IDENT){
		    if (defineVar.count(t.GetLexeme())==0){
			    ParseError(line, "Undefined Variable");
			    return false;	
		    }
		    if(TempsResults.find(t.GetLexeme())==TempsResults.end()){
                map<string, Value>::iterator it;
                for(it=TempsResults.begin();it!=TempsResults.end();it++){
                    cout<<it->first<<endl;
                    cout<<it->second<<endl;
                }
			    ParseError(line,"Run-Time Error:Var value not defined.");
			    return false;
		    }else{
			    retVal=TempsResults.find(t.GetLexeme())->second;
		    }
		    return true;
	    }
	    else if(t.GetToken()==ICONST){
		    Value val1(std::stoi(t.GetLexeme()));
		    retVal=val1;
		    return true;
	    }
	    else if(t.GetToken()==RCONST){
		    Value val1(std::stof(t.GetLexeme()));
		    retVal=val1;
		    return true;
	    }
	    else if(t.GetToken()==SCONST){
		    Value val1(t.GetLexeme());
		    retVal=val1;
		    return true;
	    }
    }
	else if(t.GetToken()==LPAREN){
		status=Expr(in,line,val1);
		retVal=val1;
		if(!status){
			ParseError(line,"No expression in ()");
			return false;
		}
        t=Parser::GetNextToken(in,line);
		if(t.GetToken()==RPAREN){
			return status;
        }
        else{
		    ParseError(line, "Missing ) after (expression");
		    return false;
        }
	}
	else if(t.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << t.GetLexeme() << ")" << endl;
		return false;
	}
	ParseError(line,"Non recognized Factor");
	return false;
}
//Print Statement
bool PrintStmt(istream& in, int& line){
    bool status=false;
    ValQue = new queue<Value>;   
    LexItem t;
    t=Parser::GetNextToken(in,line);
    if(t.GetToken()!=COMA){
        ParseError(line,"Missing COMA after PRINT");
        return false;
    }
    status=ExprList(in,line);
    if(!status){
        ParseError(line,"Missing Expression after PRINT");
        while(!(*ValQue).empty()){
            ValQue->pop();
        }
        delete ValQue;
        return status;
    }
    while(!(*ValQue).empty()){
	    Value nextVal = (*ValQue).front();
		cout<<nextVal;
		ValQue->pop();
	}
    cout<<endl;
    return status;
}
//Expression List
bool ExprList(istream& in, int& line){
    bool status=false;
    Value val1;
    LexItem next;
    status=Expr(in,line,val1);
    ValQue->push(val1);
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
    return status;   
}
//If Statement. Calls Logic Expr to determine if it should run body.
bool IfStmt(istream &in, int& line){
    bool status;
    LexItem next;
    Value val1;
    next=Parser::GetNextToken(in,line);
    if(next.GetToken()!=LPAREN){
        ParseError(line,"Missing LPAREN in IF statement");
        return false;
    }
    status=LogicExpr(in,line,val1);
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
    if(!(val1.GetBool())){
        while((next=Parser::GetNextToken(in,line))!=END){}
        Parser::PushBackToken(next);
    }else if(val1.GetBool()){
        status=Stmt(in,line);
        if(!status){
            ParseError(line,"Incorrect Statement following THEN");
            return false;
        }
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

//determines if the if statement continues or not.
bool LogicExpr(istream &in, int &line, Value& retVal){
    bool status;
    LexItem next;
	Value val1;
    Value val2;
	status=Expr(in,line,val1);
	if(!status){
		return false;
	}
	next=Parser::GetNextToken(in,line);
	if(next.GetToken()==ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout<<"("<<next.GetLexeme()<<")"<< endl;
		return false;
	}
    //if <
    if(next.GetToken()==LTHAN){
        status=Expr(in,line,val2);
        if(!status){
            ParseError(line, "Missing expression after operator in logic expression");
            return false;
        }
        if(val1.GetType()==VCHAR||val2.GetType()==VCHAR||val1.GetType()==VBOOL||val2.GetType()==VBOOL){
            ParseError(line,"Run-Time Error-Illegal Mixed Type operation");
            return false;
        }
        else if(val1.GetType()==VREAL&&val2.GetType()==VINT){
            if(val1.GetReal()<val2.GetInt()){
                Value val3(true);
                retVal=val3;
            }else{
                Value val3(false);
                retVal=val3;
            }
        }
        else if(val1.GetType()==VINT&&val2.GetType()==VREAL){
            if(val1.GetInt()<val2.GetReal()){
                Value val3(true);
                retVal=val3;
            }else{
                Value val3(false);
                retVal=val3;
            }
        }
        else if(val1.GetType()==VREAL&&val2.GetType()==VREAL){
            if(val1.GetReal()<val2.GetReal()){
                Value val3(true);
                retVal=val3;
            }else{
                Value val3(false);
                retVal=val3;
            }
        }
        else if(val1.GetType()==VINT&&val2.GetType()==VINT){
            if(val1.GetInt()<val2.GetInt()){
                Value val3(true);
                retVal=val3;
            }else{
                Value val3(false);
                retVal=val3;
            }
        }
    }
    //if <=
    else if(next.GetToken()==LTHANEQUAL){
        status=Expr(in,line,val2);
        if(!status){
            ParseError(line, "Missing expression after operator in logic expression");
            return false;
        }
        if(val1.GetType()==VCHAR||val2.GetType()==VCHAR||val1.GetType()==VBOOL||val2.GetType()==VBOOL){
            ParseError(line,"Run-Time Error-Illegal Mixed Type operation");
            return false;
        }
        else if(val1.GetType()==VREAL&&val2.GetType()==VINT){
            if(val1.GetReal()<=val2.GetInt()){
                Value val3(true);
                retVal=val3;
            }else{
                Value val3(false);
                retVal=val3;
            }
        }
        else if(val1.GetType()==VINT&&val2.GetType()==VREAL){
            if(val1.GetInt()<=val2.GetReal()){
                Value val3(true);
                retVal=val3;
            }else{
                Value val3(false);
                retVal=val3;
            }
        }
        else if(val1.GetType()==VREAL&&val2.GetType()==VREAL){
            if(val1.GetReal()<=val2.GetReal()){
                Value val3(true);
                retVal=val3;
            }else{
                Value val3(false);
                retVal=val3;
            }
        }
        else if(val1.GetType()==VINT&&val2.GetType()==VINT){
            if(val1.GetInt()<=val2.GetInt()){
                Value val3(true);
                retVal=val3;
            }else{
                Value val3(false);
                retVal=val3;
            }
        } 
    }
    //if >
    else if(next.GetToken()==RTHAN){
        status=Expr(in,line,val2);
        if(!status){
            ParseError(line, "Missing expression after operator in logic expression");
            return false;
        }
        if(val1.GetType()==VCHAR||val2.GetType()==VCHAR||val1.GetType()==VBOOL||val2.GetType()==VBOOL){
            ParseError(line,"Run-Time Error-Illegal Mixed Type operation");
            return false;
        }
        else if(val1.GetType()==VREAL&&val2.GetType()==VINT){
            if(val1.GetReal()>val2.GetInt()){
                Value val3(true);
                retVal=val3;
            }else{
                Value val3(false);
                retVal=val3;
            }
        }
        else if(val1.GetType()==VINT&&val2.GetType()==VREAL){
            if(val1.GetInt()>val2.GetReal()){
                Value val3(true);
                retVal=val3;
            }else{
                Value val3(false);
                retVal=val3;
            }
        }
        else if(val1.GetType()==VREAL&&val2.GetType()==VREAL){
            if(val1.GetReal()>val2.GetReal()){
                Value val3(true);
                retVal=val3;
            }else{
                Value val3(false);
                retVal=val3;
            }
        }
        else if(val1.GetType()==VINT&&val2.GetType()==VINT){
            if(val1.GetInt()>val2.GetInt()){
                Value val3(true);
                retVal=val3;
            }else{
                Value val3(false);
                retVal=val3;
            }
        } 
    }
    //if >=
    else if(next.GetToken()==RTHANEQUAL){
        status=Expr(in,line,val2);
        if(!status){
            ParseError(line, "Missing expression after operator in logic expression");
            return false;
        }
        if(val1.GetType()==VCHAR||val2.GetType()==VCHAR||val1.GetType()==VBOOL||val2.GetType()==VBOOL){
            ParseError(line,"Run-Time Error-Illegal Mixed Type operation");
            return false;
        }
        else if(val1.GetType()==VREAL&&val2.GetType()==VINT){
            if(val1.GetReal()>=val2.GetInt()){
                Value val3(true);
                retVal=val3;
            }else{
                Value val3(false);
                retVal=val3;
            }
        }
        else if(val1.GetType()==VINT&&val2.GetType()==VREAL){
            if(val1.GetInt()>=val2.GetReal()){
                Value val3(true);
                retVal=val3;
            }else{
                Value val3(false);
                retVal=val3;
            }
        }
        else if(val1.GetType()==VREAL&&val2.GetType()==VREAL){
            if(val1.GetReal()>=val2.GetReal()){
                Value val3(true);
                retVal=val3;
            }else{
                Value val3(false);
                retVal=val3;
            }
        }
        else if(val1.GetType()==VINT&&val2.GetType()==VINT){
            if(val1.GetInt()>=val2.GetInt()){
                Value val3(true);
                retVal=val3;
            }else{
                Value val3(false);
                retVal=val3;
            }
        } 
    }
    //if ==
    else if(next.GetToken()==EQUAL){
        status=Expr(in,line,val2);
        if(!status){
            ParseError(line,"Missing expression after operator in logic expression");
            return false;
        }
        if(val1.GetType()==VCHAR||val2.GetType()==VCHAR||val1.GetType()==VBOOL||val2.GetType()==VBOOL){
            ParseError(line,"Run-Time Error-Illegal Mixed Type operation");
            return false;
        }
        else if(val1.GetType()==VREAL&&val2.GetType()==VINT){
            if(val1.GetReal()==val2.GetInt()){
                Value val3(true);
                retVal=val3;
            }else{
                Value val3(false);
                retVal=val3;
            }
        }
        else if(val1.GetType()==VINT&&val2.GetType()==VREAL){
            if(val1.GetInt()==val2.GetReal()){
                Value val3(true);
                retVal=val3;
            }else{
                Value val3(false);
                retVal=val3;
            }
        }
        else if(val1.GetType()==VREAL&&val2.GetType()==VREAL){
            if(val1.GetReal()==val2.GetReal()){
                Value val3(true);
                retVal=val3;
            }else{
                Value val3(false);
                retVal=val3;
            }
        }
        else if(val1.GetType()==VINT&&val2.GetType()==VINT){
            if(val1.GetInt()==val2.GetInt()){
                Value val3(true);
                retVal=val3;
            }else{
                Value val3(false);
                retVal=val3;
            }
        } 
    }
    //if !=
    else if(next.GetToken()==NOTEQUAL){
        status=Expr(in,line,val2);
        if(!status){
            ParseError(line,"Missing expression after operator in logic expression");
            return false;
        }
        if(val1.GetType()==VCHAR||val2.GetType()==VCHAR||val1.GetType()==VBOOL||val2.GetType()==VBOOL){
            ParseError(line,"Run-Time Error-Illegal Mixed Type operation");
            return false;
        }
        else if(val1.GetType()==VREAL&&val2.GetType()==VINT){
            if(val1.GetReal()!=val2.GetInt()){
                Value val3(true);
                retVal=val3;
            }else{
                Value val3(false);
                retVal=val3;
            }
        }
        else if(val1.GetType()==VINT&&val2.GetType()==VREAL){
            if(val1.GetInt()!=val2.GetReal()){
                Value val3(true);
                retVal=val3;
            }else{
                Value val3(false);
                retVal=val3;
            }
        }
        else if(val1.GetType()==VREAL&&val2.GetType()==VREAL){
            if(val1.GetReal()!=val2.GetReal()){
                Value val3(true);
                retVal=val3;
            }else{
                Value val3(false);
                retVal=val3;
            }
        }
        else if(val1.GetType()==VINT&&val2.GetType()==VINT){
            if(val1.GetInt()!=val2.GetInt()){
                Value val3(true);
                retVal=val3;
            }else{
                Value val3(false);
                retVal=val3;
            }
        } 
    }else{
        ParseError(line,"Unrecognized Logical Operator");
        return false;
    }
    return true;
}