grammar MAlice;

options {
	language=C;
}

// Programs, procedures and functions
program	:	(variable_declaration statement_inner_separator)* (function|procedure)+;


// Types and constants


type 	:	'number'
        |   	'letter'
        |	'sentence'
        ;
       
constant:	NUMBER_LITERAL
	|	CHARACTER_LITERAL
	;


function:	THEROOM IDENTIFIER LPAREN declaration_argument_list? RPAREN 'contained a' type block
	;
procedure
	:	THELOOKINGGLASS IDENTIFIER LPAREN declaration_argument_list? RPAREN OPENED body CLOSED
	;

declaration_argument_list
	:	(declaration_argument ',')* declaration_argument
	;
	
	

block	:	OPENED body CLOSED;
body
	: (variable_declaration statement_inner_separator | procedure | function)* statement_list;
declaration_argument
	:	SPIDER? type IDENTIFIER
	;



proc_func_invocation
	:	IDENTIFIER LPAREN (expression (',' expression)*)? RPAREN
	;

//Expression

assignment
	:	lvalue BECAME expression
	;

// Statements

statement_inner_separator 	:	(COMMA | THEN | AND | BUT | FULL_STOP);

statement_list
	:	statement_component+
	;
statement_component
	:	(IDENTIFIER LPAREN) => proc_func_invocation (options{greedy=true;} : FULL_STOP)?
	|	(stdout_lvalue (SAIDALICE | SPOKE)) => print_statement statement_inner_separator
	|	input_statement (options{greedy=true;} : FULL_STOP)?
	|	(ALICEFOUND expression) => return_statement FULL_STOP
	|	FULL_STOP
	|	while_loop (options{greedy=true;} : FULL_STOP)?
	|	if_block (options{greedy=true;} : FULL_STOP)?
	|	assignment_expr FULL_STOP
	|	block (options{greedy=true;} : FULL_STOP)?
	;

	
	
	
return_statement
	:	ALICEFOUND expression
	;
	
while_loop
	:	'eventually' boolean_expression BECAUSE statement_list 'enough times'
	;
	
if_block
	:	'perhaps' boolean_expression 'so' statement_list else_block* 'because Alice was unsure which'
	;

else_block
	:	'or' ('maybe' boolean_expression 'so')? statement_list
	;
	
variable_declaration
	:	IDENTIFIER (WASA type ('of' expression)? | 'had' expression type) 'too'?;

print_statement
	:	stdout_lvalue (SPOKE | SAIDALICE);

input_statement
	:	'what was' lvalue '?'
	;

stdout_lvalue
	:	STRING_LITERAL
	|	expression
	;

// Expressions
expression
	:	additive_expr
	;

assignment_expr
	:	lvalue 'became' expression
	;	

lvalue	:	IDENTIFIER ('\'s' expression 'piece')?
	;

additive_expr
	:	multiplicactive_expr ((PLUS|MINUS) multiplicactive_expr)*
	;

multiplicactive_expr
	:	bitwise_expr (('*'|'/'|'%') bitwise_expr)*
	;

bitwise_expr
	:	unary_expr (('^'|'|'|'&') unary_expr)*
	;

unary_expr
	:	(IDENTIFIER LPAREN) => proc_func_invocation
	| 	(PLUS | MINUS | TILDE | BANG) unary_expr
	|	constant
	|	lvalue
	|	LPAREN additive_expr RPAREN
	;

boolean_expression
	:	single_boolean_expression (('&&' | '||') single_boolean_expression)*
	;
	
single_boolean_expression
	:	 LPAREN expression ('==' | '!=' | '<' | '<=' | '>' | '>=' ) expression RPAREN
	;
		
null_statement
	:	'.';

// Lexer rules
CHARACTER_LITERAL
	:	'\'' LETTER '\''
	;
STRING_LITERAL
	:	'"' ~('"')* '"';
NUMBER_LITERAL
	:	'0' | '1'..'9' DIGIT*
	;
	
// Based on the ANSI-C whitespace grammar.
WS	:	 (' ' | '\t' | '\r' | '\n') {$channel=HIDDEN;}
    	;

// Based on the ANSI-C line comment grammar.
COMMENT	:	'###' ~('\n'|'\r')* NEWLINE {$channel=HIDDEN;};
NEWLINE	:	'\r'? '\n' ;
LPAREN	:	'(';
RPAREN	:	')';
OPENED 	:	'opened';
CLOSED	:	'closed';
SPIDER	:	'spider';
BUT	:	'but';	
THELOOKINGGLASS
	:	'The looking-glass';
THEROOM	:	'The room';
SPOKE	:	'spoke';
WASA	:	'was a';
BECAME	:	'became';
ALICEFOUND
	:	'Alice found';
BECAUSE	:	'because';
SAIDALICE
	:	'said Alice';
COMMA	:	',';
THEN	:	'then';
AND	:	'and';
FULL_STOP
	:	'.' ;
	
IDENTIFIER
	:	LETTER (LETTER | DIGIT | UNDERSCORE)*;

PLUS	:	'+';
MINUS	:	'-';
TILDE	:	'~';
BANG	:	'!';
UNDERSCORE
	:	'_';

fragment LETTER
	:	('a'..'z' | 'A'..'Z')
	;
fragment DIGIT
	:	'0'..'9'
	;