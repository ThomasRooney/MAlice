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
	

lvalue	:	identifier
	;

additive_expr
	:	(multiplicactive_expr) ('+' multiplicactive_expr  | '-' multiplicactive_expr)*
	;

multiplicactive_expr
	:	(bitwise_expr) ('*' bitwise_expr | '/' bitwise_expr | '%' bitwise_expr)*
	;

bitwise_expr
	:	unary_expr //(unary_expr) ('^' unary_expr | '|' unary_expr |'&' unary_expr)*
	;

unary_expr
	:	unary_operator bitwise_expr
	|	constant
	|	LPAREN additive_expr RPAREN
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
	


// Programs and functions
program	:	function+;



function:	'The' function_type identifier LPAREN function_argument_list? RPAREN return_type_clause? block_unit;
function_type
	:	'room'
	| 	'looking-glass'
	;
function_argument_list
	:	(function_argument ',')* function_argument
	;
function_argument
	:	type identifier
	;
return_type_clause
	:	'contained a' type
	;
block_unit
	:	'opened' (statement_list | function)? 'closed';
	
function_invocation
	:	identifier LPAREN function_invocation_argument_list? RPAREN
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
	:	identifier 'became' expression
	;

// Statements
statement_list
	:	return_statement
	|	while_loop
	|	if_block
	;

return_statement
	:	'Alice found' constant
	;
	
while_loop
	:	'eventually' 'because' statement_list 'enough times'
	;
	
if_block
	:	'perhaps' expression 'so' statement_list else_block* 'because Alice was unsure which'
	;

else_block
	:	'or' ('maybe' expression 'so')? statement_list
	;
	
// Types
LETTER	:	('a'..'z' | 'A'..'Z')
	;

CHARACTER_LITERAL
	:	'\'' LETTER '\''
	;
	
	
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