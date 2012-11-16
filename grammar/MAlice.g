grammar MAlice;

options {
	language=C;
	output=AST;
	backtrack=true;
	ASTLabelType=CommonTree;
}

//expr	:	'+' term expr_left | '-' term expr_left | term;

primary_expr
	:	constant
	|	CHARACTER_LITERAL
	|	'(' expression ')'
	;


expression 
	:	assignment_expr (split assignment_expr)*	
	;

assignment_expr
	:	lvalue 'became' assignment_expr
	;

lvalue	:	identifier
	;

additive_expr
	:	(multiplicactive_expr) ('+' multiplicactive_expr  |'-' multiplicactive_expr)*
	;

multiplicactive_expr
	:	bitwise_expr ('*' bitwise_expr | '/' bitwise_expr | '%' bitwise_expr)*
	;

bitwise_expr
	:	(unary_expr) ('^' unary_expr | '|' unary_expr |'&' unary_expr)*
	;

unary_expr
	:	unary_operator bitwise_expr
	|	primary_expr
	;

identifier
	:	LETTER (LETTER | '0'..'9')*
	;


/*	
expr_left
	:	(expr expr_left) | '';
	
term 	:	('*' | '/' | '%') bit_term term_left | bit_term;
term_left
	:	(term term_left) | '';
	
bit_term:	('^' | '|' | '&') factor bit_term_left | factor;
bit_term_left
	:	(bit_term bit_term_left) | '';
*/
	
// factor	:	ID | NUMBER | '~' factor;


split 	:	('.' | ','|'then'|'and'|'but');
/*
declaration 
	:	'was a' type declarationleft;
declarationleft
	:	ID declaration;
	
declarationlist 
	:	declaration split declarationlist;
*/
type 	:	'number'
        |       'letter'
        ;
        
unary_operator
	:	'~'
	;

constant:	DECIMAL_NUMBER
	|	CHARACTER_LITERAL
	;
	


// Programs and functions
program	:	function+;

function:	'The' function_type identifier '(' ')' return_type_clause? block_unit;
function_type
	:	'room'
	| 	'looking-glass'
	;
return_type_clause
	:	'contained a' type
	;
block_unit
	:	'opened' 'closed';


/*
argumentlist 
	:	(declaration ('"'|'and'))* declaration;
	
blockunit
	:	'opened' statementlist 'closed';

statementlist 
	:	(statement split statementlist)?;

statement
	:	declaration | assignment;

assignment
	:	ID 'became' (expr | character);

character 
	:	'\'' ['a'..'z'|'A'..'Z'] '\'';
*/	
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
LETTER	:	('a'..'z' | 'A'..'Z')
	;

CHARACTER_LITERAL
	:	'"' LETTER '"'
	;
	
	
DECIMAL_NUMBER
	:	('0' | '1'..'9' '0'..'9'*)
	;

NEWLINE	:	'\r'? '\n' ;