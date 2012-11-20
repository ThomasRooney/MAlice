grammar MAlice;

options {
	language=C;
}

// Types and constants
type 	:	'number'
        |   	'letter'
        |	'sentence'
        ;
        
constant:	NUMBER_LITERAL
	|	CHARACTER_LITERAL
	;

// Programs, procedures and functions
program	:	function+;

function:	'The room' IDENTIFIER LPAREN declaration_argument_list? RPAREN 'contained a' type block_unit;
procedure
	:	'The looking-glass' IDENTIFIER LPAREN declaration_argument_list? RPAREN block_unit
	;
	
declaration_argument_list
	:	(declaration_argument ',')* declaration_argument
	;
	
declaration_argument
	:	type IDENTIFIER
	;
	
block_unit
	:	'opened' (statement_list | function)+ 'closed'
;

proc_func_invocation
	:	IDENTIFIER LPAREN proc_func_invocation_argument_list? RPAREN
	;

// Programs and functions
proc_func_invocation_argument_list
	:	(constant ',')* constant
	;
//Expression

assignment
	:	lvalue 'became' expression
	;

// Statements
statement_list
	:	(statement_component split)* statement_component split
	|	'.'
	;
	
split 	:	(',' | 'then' | 'and' | 'but');
	
statement_component
	:	return_statement
	|	while_loop
	|	if_block
	|	input_statement
	;

return_statement
	:	'Alice found' constant
	;
	
while_loop
	:	'eventually' boolean_expression 'because' statement_list 'enough times'
	;
	
if_block
	:	'perhaps' boolean_expression 'so' statement_list else_block* 'because Alice was unsure which' '.'?
	;

else_block
	:	'or' ('maybe' boolean_expression 'so')? statement_list
	;
	
variable_declaration
	:	IDENTIFIER 'was a' type ('of' expression)? 'too'?;
	
input_statement
	:	'what was' lvalue '?';
	
spoke_statement
	:	expression 'spoke';

// Expressions
expression
	:	additive_expr
	;

assignment_expr
	:	lvalue 'became' expression
	;	

lvalue	:	IDENTIFIER
	|	IDENTIFIER '\'s' expression 'piece'
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
LETTER	:	('a'..'z' | 'A'..'Z')
	;
	
IDENTIFIER
	:	LETTER (LETTER | '0'..'9' | '-' | '_')*;

CHARACTER_LITERAL
	:	'\'' LETTER '\''
	;
	
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