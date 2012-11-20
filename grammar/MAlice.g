grammar MAlice;

options {
	language=C;
}

// Types and constants
// $<Group

type 	:	'number'
        |   	'letter'
        |	'sentence'
        ;

// $>Group 
        
constant:	NUMBER_LITERAL
	|	CHARACTER_LITERAL
	;

// Programs, procedures and functions
program	:	(function|procedure)+;

function:	THEROOM IDENTIFIER LPAREN declaration_argument_list? RPAREN 'contained a' type block_unit;


declaration_argument_list
	:	(declaration_argument ',')* declaration_argument
	;
procedure
	:	THELOOKINGGLASS IDENTIFIER LPAREN declaration_argument_list? RPAREN block_unit
	;
	
	
declaration_argument
	:	type IDENTIFIER
	;
	
block_unit
options {
	k = 2;
}
	:	OPENED (declaration_list)? statement_list CLOSED;


proc_func_invocation
	:	IDENTIFIER LPAREN proc_func_invocation_argument_list? RPAREN
	;

// Programs and functions
proc_func_invocation_argument_list
	:	(constant ',')* constant
	;
//Expression

assignment
	:	lvalue BECAME expression
	;

// Statements

declaration_split 	:	(COMMA | THEN | AND | BUT);
split	:	'.';

statement_component
	:	return_statement
	|	while_loop
	|	if_block	
	|	assignment_expr
	|	input_statement
	|	'.'
	;
statement_list
	:	(statement_component) (split statement_component)*
	;
	
	

return_statement
	:	ALICEFOUND constant
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
	
declaration_list
options {
	k = 2;
}
	: 	(IDENTIFIER WASA) => variable_declaration declaration_list?
	|	('The') => (procedure | function) declaration_split declaration_list?
	; 
	
variable_declaration
	:	IDENTIFIER WASA type ('of' expression)? 'too'? declaration_split declaration_list?;
	
input_statement
	:	'what was' lvalue '?';
	
spoke_statement
	:	expression SPOKE;

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
	:	multiplicactive_expr (('+'|'-') multiplicactive_expr)*
	;

multiplicactive_expr
	:	bitwise_expr (('*'|'/'|'%') bitwise_expr)*
	;

bitwise_expr
	:	unary_expr (('^'|'|'|'&') unary_expr)*
	;

unary_expr
	:	('+' | '-' | '~' | '!') unary_expr
	|	constant
	|	lvalue
	|	LPAREN additive_expr RPAREN
	;

boolean_expression
	:	single_boolean_expression (('&&' | '||') single_boolean_expression)*
	;
	
single_boolean_expression
	:	 expression ('==' | '!=' | '<' | '<=' | '>' | '>=' ) expression
	;
	

	
// Lexer rules
fragment LETTER	:	('a'..'z' | 'A'..'Z')
	;

CHARACTER_LITERAL
	:	'\'' LETTER '\''
	;

IDENTIFIER
	:	LETTER (LETTER | '0'..'9' | '-' | '_')*;
	
STRING_LITERAL
	:	'"' ~('"')* '"';
	
NUMBER_LITERAL
	:	'0' | '1'..'9' '0'..'9'*;
	
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
	:	'alice found';
COMMA	:	',';
THEN	:	'then';
AND	:	'and';
BUT	:	'but';