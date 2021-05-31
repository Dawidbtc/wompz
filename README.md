<h1>Wompz
<h1>This is Wompz. A simple programming language built in C++ that is Fortran-Like.

<h3>I took inspiration for this from something similar I was building in one of my Computer Science classes in college.
<h3>I saw some improvements I can make to the language so this is an improved version. 
<h3>The parser in the language is a recursive descent parser which traces the parse tree recursively. 

Here is the Backus-Nauf form for the language:
--------------------------------------------
* Prog = PROGRAM IDENT {Decl} {Stmt} END PROGRAM IDENT
* Decl = (INTEGER|REAL|CHAR) : IdList
* IdList = IDENT {,IDENT}
* Stmt = AssignStmt | IfStmt | PrintStmt | ReadStmt
* PrintStmt = PRINT, ExprList
* IfStmt = IF (LogicExpr) THEN {Stmt} END IF
* AssignStmt = Var = Expr
* ReadStmt = READ, VarList
* VarList = Var {,Var}
* ExprList = Expr {,Expr}
* Expr = Term{(+|-) Term}
* Term = SFactor {(*|/) SFactor}
* SFactor = Sign Factor | Factor
* LogicExpr = Expr (== | < | > | <= | >= | !=) Expr
* Var = IDENT
* Sign + | -
* Factor = IDENT | ICONST | RCONST | SCONST | (Expr)
----------------------------------------------------------
<h2>How to Install:
<h3><b>Windows:</b>
<ol>
<li>Install any version of MinGw
<li>Navigate to source code with Windows Command Prompt
<li>Compile source code: g++ -std=c++11 driver.cpp -o [name you want of the compiled program but without the [] ]
<li>Then move your code .txt file to where your source code is located.
<li>Run your code with this command: ./"nameofcompiledsource" "filename" without the quotes
</ol>
<li>So for example this would be a clean compilation:
<li>g++ -std=c++11 driver.cpp -o wompz
<li>./wompz code.txt

<h3><b>Linux and MacOS:</b></h3>
<ol>
<li>Get g++.
<ul><li>sudo apt-get install g++(Debian/Ubuntu)
<li>sudo dnf install g++(Redhat/Fedora)
<li>sudo pacman install g++(Arch)
<li>brew install gcc48(MacOs)
<li>Your computer may have g++ preinstalled already. You can check by running g++ command.
</ul>
<li>Navigate to source code with terminal
<li>Compile source code. g++ -std=c++11 driver.cpp -o [name you want of the compilded program but without the [] ]
<li>Move code over to source code location.
<li>Run code with compiled program: ./"nameofcompiledsource" "filename" without the quotes
</ol>
<ul>
<li>So for example this would be a clean compilation:
<li>g++ -std=c++11 driver.cpp -o wompz
<li>./wompz code