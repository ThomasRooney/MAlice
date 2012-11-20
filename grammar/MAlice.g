grammar MAlice;

options {
	language=Java;
}

// Programs, procedures and functions
program	:	(variable_declaration declaration_split)* (function|procedure)+;


// Types and constants


type 	:	'number'
        |   	'letter'
        |	'sentence'
        ;
       
constant:	number_literal
	|	CHARACTER_LITERAL
	;


function:	THEROOM identifier LPAREN declaration_argument_list? RPAREN 'contained a' type OPENED body CLOSED
	;
procedure
	:	THELOOKINGGLASS identifier LPAREN declaration_argument_list? RPAREN OPENED body CLOSED
	;

declaration_argument_list
	:	(declaration_argument ',')* declaration_argument
	;
	
	
declaration_argument
	:	type identifier
	;
	
body
	: (variable_declaration declaration_split)* statement_list;


proc_func_invocation
	:	identifier LPAREN (expression (',' expression)*)? RPAREN
	;

//Expression

assignment
	:	lvalue BECAME expression
	;

// Statements

declaration_split 	:	(COMMA | THEN | AND | BUT | split);
split	:	'.';

statement_component
	:	(identifier LPAREN) => proc_func_invocation
	|	(stdout_lvalue (SAIDALICE | SPOKE)) => io_statement
	|	(ALICEFOUND expression) => return_statement
	|	while_loop
	|	if_block	
	|	assignment_expr
	|	input_statement	
	|	null_statement
	;
statement_list
	:	statement_component (split statement_component)* split
	;
	
	
	
return_statement
	:	ALICEFOUND expression
	;
	
while_loop
	:	'eventually' boolean_expression 'because' statement_list 'enough times'
	;
	
if_block
	:	'perhaps' boolean_expression 'so' statement_list else_block* 'because Alice was unsure which'
	;

else_block
	:	'or' ('maybe' boolean_expression 'so')? statement_list
	;
	
variable_declaration
	:	identifier WASA type ('of' expression)? 'too'?;
	
input_statement
	:	'what was' lvalue '?';

io_statement
	:	stdout_lvalue (SPOKE (AND stdout_lvalue)?)* (SAIDALICE | ',' stdin_statement) 
	;

stdin_statement
	:	'what was' identifier '?'
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

lvalue	:	identifier ('\'s' expression 'piece')?
	;

additive_expr
	:	multiplicactive_expr (('+'|'-') multiplicactive_expr)*
	;

multiplicactive_expr
	:	bitwise_expr (('*'|'/'|'%') bitwise_expr)*
	;

bitwise_expr
	:	unary_expr (('^'|'|'|'&') unary_expr)*
	;

unary_expr
	:	(identifier LPAREN) => proc_func_invocation
	| 	('+' | '-' | '~' | '!') unary_expr
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

identifier
	:	LETTER (LETTER | '0'..'9' | '_')*;	
	
number_literal 
	:	(ZERO_NUMBER) | (NON_ZERO_NUMBER (ZERO_NUMBER | NON_ZERO_NUMBER)*);
		
null_statement
	:	'.';

// Lexer rules
LETTER	:	('a'..'z' | 'A'..'Z')
	;

CHARACTER_LITERAL
	:	'\'' LETTER '\''
	;


	
STRING_LITERAL
	:	'"' ~('"')* '"';
	
ZERO_NUMBER 
	:	'0';
NON_ZERO_NUMBER
	:	'1'..'9';
	
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

THELOOKINGGLASS
	:	'The looking-glass';
THEROOM	:	'The room';
SPOKE	:	'spoke';
WASA	:	'was a';
BECAME	:	'became';
ALICEFOUND
	:	'Alice found';
SAIDALICE
	:	'said Alice';
COMMA	:	',';
THEN	:	'then';
AND	:	'and';
fragment BUT	:	'but';