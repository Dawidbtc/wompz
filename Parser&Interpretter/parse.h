#include <iostream>
#include "../Lexical Analyzer/lex.cpp"

map<string, bool> defineVar;
map<string, Token> SymbolTable;

namespace Parser {
	bool pushBack = false;
	LexItem	Pushed;
	static LexItem GetNextToken(istream& in, int& line) {
		if(pushBack) {
			pushBack = false;
			return Pushed;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & tok) {
		if(pushBack) {
			abort();
		}
		pushBack = true;
		Pushed = tok;	
	}
}
static int errCount = 0;
int ErrCount()
{
    return errCount;
}

void ParseError(int line, string err)
{
	++errCount;
	cout << line << ": " << err << endl;
}
extern bool Prog(istream& in, int& line);
extern bool Stmt(istream& in, int& line);
extern bool Decl(istream& in, int& line);
extern bool PrintStmt(istream& in, int& line);
extern bool IfStmt(istream& in, int& line);
extern bool ReadStmt(istream& in, int& line);
extern bool IdList(istream& in, int& line, LexItem tok);
extern bool VarList(istream& in, int& line);
extern bool Var(istream& in, int& line);
extern bool AssignStmt(istream& in, int& line);
extern bool ExprList(istream& in, int& line);
extern bool LogicExpr(istream& in, int& line);
extern bool Expr(istream& in, int& line);
extern bool Term(istream& in, int& line);
extern bool SFactor(istream& in, int& line);
extern bool Factor(istream& in, int& line, int sign);
extern int ErrCount();
