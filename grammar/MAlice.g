grammar MAlice;

options {
	language=C;
}

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
	:	'~' unary_expr
	|	constant
//	|	IDENTIFIER
	|	LPAREN additive_expr RPAREN
	;


boolean_expression
	:	single_boolean_expression (('&&' | '||') single_boolean_expression)*
	;
	
single_boolean_expression
	:	expression ('==' | '!=') expression
	;
	
type 	:	'number'
        |   	'letter'
        |	'sentence'
        ;

// Programs and functions
program	:	function+;



function:	'The' function_declaration block_unit;

// This has been written in this form to match the reference compiler; it is a parse error for 'room' functions to not be followed
// by a return_type_clause.
function_declaration
	:	'room' 		IDENTIFIER LPAREN function_argument_list? RPAREN return_type_clause 
	|	'looking-glass' IDENTIFIER LPAREN function_argument_list? RPAREN
	;
	
function_argument_list
	:	(function_argument ',')* function_argument
	;
function_argument
	:	type IDENTIFIER
	;
return_type_clause
	:	'contained a' type
	;
block_unit
	:	'opened' (statement_list | function)+ 'closed';
	
function_invocation
	:	IDENTIFIER LPAREN function_invocation_argument_list? RPAREN
	;

constant:	NUMBER_LITERAL
	|	CHARACTER_LITERAL
	;
	


// Programs and functions
function_invocation_argument_list
	:	(constant ',')* constant
	;
//Expression

assignment
	:	lvalue 'became' expression
	;

// Statements
statement_list
	:	(statement_component split)* statement_component '.'
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
	:	'eventually' 'because' statement_list 'enough times'
	;
	
if_block
	:	'perhaps' expression 'so' statement_list else_block* 'because Alice was unsure which' '.'?
	;

else_block
	:	'or' ('maybe' expression 'so')? statement_list
	;
	
variable_declaration
	:	IDENTIFIER 'was a' type ('of' (expression))? 'too'?;
	
input_statement
	:	'what was' lvalue '?';
	
spoke_statement
	:	expression 'spoke';

	
// Types
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