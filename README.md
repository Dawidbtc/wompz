This is Wompz. A simple programming language built in C++ that is Fortran-Like.

I took inspiration for this from something similair I was building in one of my Computer Science classes in college.
I saw some improvements I can make to the language so this is an improved version. 
The parser in the language is a recursive descent parser which traces the parse tree recursively. 
--------------------------------------------------------------------------------------------------------------------
Here is the Backus-Nauf form for the language:
Prog = PROGRAM IDENT {Decl} {Stmt} END PROGRAM IDENT
Decl = (INTEGER|REAL|CHAR) : IdList
IdList = IDENT {,IDENT}
Stmt = AssignStmt | IfStmt | PrintStmt | ReadStmt
PrintStmt = PRINT, ExprList
IfStmt = IF (LogicExpr) THEN {Stmt} END IF
AssignStmt = Var = Expr
ReadStmt = READ, VarList
VarList = Var {,Var}
ExprList = Expr {,Expr}
Expr = Term{(+|-) Term}
Term = SFactor {(*|/) SFactor}
SFactor = Sign Factor | Factor
LogicExpr = Expr (== | < | > | <= | >= | !=) Expr
Var = IDENT
Sign + | -
Factor = IDENT | ICONST | RCONST | SCONST | (Expr)
----------------------------------------------------------------------------------------------------------------------
