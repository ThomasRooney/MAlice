grammar MAlice;

options {
	language=C;
	output=AST;
	ASTLabelType=CommonTree;
}

expr	:	'+' term expr_left | '-' term expr_left | term;
expr_left
	:	expr expr_left | '';
	
term 	:	('*' | '/' | '%') bit_term term_left | bit_term;
term_left
	:	term term_left | '';
	
bit_term:	('^' | '|' | '&') factor bit_term_left | factor;
bit_term_left
	:	bit_term bit_term_left | '';
factor	:	ID | NUMBER | '~' factor;


/*prog	: ( stat {print $stat.tree.toStringTree();} )+ ;

stat	:	expr NEWLINE        -> expr
	|	ID '=' expr NEWLINE -> ^('=' ID expr)
	|	NEWLINE             ->
	;

expr	:	multExpr (('+'^|'-'^) multExpr)*
	;

multExpr
	:	atom ('*'^ atom)*
	;

atom	:	INT
	|	ID
	|	'('! expr ')'!
	;*/
	
//Expression
//	:	Expression '+' Term | Expression '-' Term | Term;
//Term	:	Term '*' BitTerm | BitTerm;
//BitTerm	:	BitTerm '^' Factor | Factor;
//Factor 	:	ID | NUMBER;

// Types
ID	:	('a'..'z'|'A'..'Z')+ ;

NUMBER	:	'0'..'9'+ ;

NEWLINE	:	'\r'? '\n' ;

WS	:	(' '|'\t'|'\n'|'\r')+ {self.skip()} ;