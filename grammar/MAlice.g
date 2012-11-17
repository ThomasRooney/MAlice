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
	|	IDENTIFIER '\'s' ('0'..'9') 'piece'
	;

additive_expr
	:	(multiplicactive_expr) (('+'|'-') multiplicactive_expr)*
	;


multiplicactive_expr
	:	(bitwise_expr) (('*'|'/'|'%') bitwise_expr)*
	;

bitwise_expr
	:	(unary_expr) (('^'|'|'|'&') unary_expr)*
	;

unary_expr
	:	'~' unary_expr
	|	constant
//	|	IDENTIFIER
	|	LPAREN additive_expr RPAREN
	;

split 	:	('.' | ','|'then'|'and'|'but');

/*boolean_expression
	:	(expression boolean_comparator expression) (boolean_logic boolean_expression)*
	;

boolean_logic 
	:	('&&'|'||');
	
boolean_comparator
	:	'=='|'!=';
*/	
/*
declaration 
	:	'was a' type declarationleft;
declarationleft
	:	ID declaration;
	
declarationlist 
	:	declaration split declarationlist;
*/
type 	:	'number'
        |   	'letter'
        |	'sentence'
        ;

// Programs and functions
program	:	function+;



function:	'The' function_type IDENTIFIER LPAREN function_argument_list? RPAREN return_type_clause? block_unit;
function_type
	:	'room'
	| 	'looking-glass'
	;
function_argument_list
	:	(function_argument ',')* function_argument
	;
function_argument
	:	'spider' type IDENTIFIER
	;
return_type_clause
	:	'contained a' type
	;
block_unit
	:	'opened' (statement_list | function)? 'closed';
	
function_invocation
	:	IDENTIFIER LPAREN function_invocation_argument_list? RPAREN
	;

constant:	NON_ZERO_DECIMAL_NUMBER
	|	ZERO_NUMBER
	|	CHARACTER_LITERAL
	;
	


// Programs and functions
function_invocation_argument_list
	:	(constant ',')* constant
	;
//Expression

assignment
	:	IDENTIFIER 'became' expression
	;

// Statements
statement_list
	:	return_statement
	|	while_loop
	|	if_block
	|	input_statement
	|	increment_statement
	|	decrement_statement
	|	'.'
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
	:	IDENTIFIER 'was a' type ('of' (expression))?;
	
input_statement
	:	'what was' IDENTIFIER '?';
	
spoke_statement
	:	expression 'spoke';

increment_statement
	:	IDENTIFIER 'ate';
	
decrement_statement
	:	IDENTIFIER 'drank';
	
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
	
	
// This is a silly solution so antlr interpreter works correctly
ZERO_NUMBER
	:	'0'
	;
	
NON_ZERO_DECIMAL_NUMBER
	:	'1'..'9' '0'..'9'*
	;
	
// Based on the ANSI-C whitespace grammar.
WS	:	 (' ' | '\t' | '\r' | '\n') {$channel=HIDDEN;}
    	;

// Based on the ANSI-C line comment grammar.
COMMENT	:	'###' ~('\n'|'\r')* NEWLINE {$channel=HIDDEN;};
NEWLINE	:	'\r'? '\n' ;
LPAREN	:	'(';
RPAREN	:	')';