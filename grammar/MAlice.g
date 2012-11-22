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


function:	THEROOM IDENTIFIER LPAREN declaration_argument_list? RPAREN CONTAINEDA type block
	;
procedure
	:	THELOOKINGGLASS IDENTIFIER LPAREN declaration_argument_list? RPAREN OPENED body CLOSED
	;

declaration_argument_list
	:	(declaration_argument COMMA)* declaration_argument
	;
	
	

block	:	OPENED body CLOSED;
body
	: (variable_declaration statement_inner_separator | procedure | function)* statement_list;
declaration_argument
	:	SPIDER? type IDENTIFIER
	;



proc_func_invocation
	:	IDENTIFIER LPAREN (expression (COMMA expression)*)? RPAREN
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
	:	(EVENTUALLY) => while_loop (options{greedy=true;} : FULL_STOP)?
	|	(IDENTIFIER LPAREN) => proc_func_invocation (options{greedy=true;} : FULL_STOP)?
	|	(stdout_lvalue (SAIDALICE | SPOKE)) => print_statement statement_inner_separator
	|	(lvalue ATE) => increment_statement
	|	(lvalue DRANK) => decrement_statement
	|	input_statement (options{greedy=true;} : FULL_STOP)?
	|	(ALICEFOUND expression) => return_statement FULL_STOP
	|	FULL_STOP
	|	if_block (options{greedy=true;} : FULL_STOP)?
	|	assignment_expr FULL_STOP
	|	block (options{greedy=true;} : FULL_STOP)?
	;

return_statement
	:	ALICEFOUND expression
	;
	
while_loop
	:	EVENTUALLY boolean_expression BECAUSE statement_list ENOUGHTIMES
	;
	
if_block
	:	PERHAPS boolean_expression SO statement_list else_block* ALICEWASUNSURE
	|	EITHER boolean_expression SO statement_list OR statement_list ALICEWASUNSURE
	;

else_block
	:	OR (MAYBE boolean_expression SO)? statement_list
	;
	
variable_declaration
	:	IDENTIFIER (WASA type (OF expression)? | HAD expression type) TOO?;

print_statement
	:	stdout_lvalue (SPOKE | SAIDALICE);

input_statement
	:	WHATWAS lvalue QUESTION_MARK
	;

stdout_lvalue
	:	STRING_LITERAL
	|	expression
	;
	
increment_statement
	:	lvalue ATE
	;
	
decrement_statement
	:	lvalue DRANK;
	

// Expressions
expression
	:	additive_expr
	;

assignment_expr
	:	lvalue BECAME expression
	;	

lvalue	:	IDENTIFIER ('\'s' expression PIECE)?
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
	:	'.'
	;
CONTAINEDA
	:	'contained a'
	;
EVENTUALLY
	:	'eventually'
	;
ENOUGHTIMES
	:	'enough times'
	;
ALICEWASUNSURE
	:	'because Alice was unsure which'
	;
OR	:	'or';
SO	:	'so';
PIECE	:	'piece';
WHATWAS	:	'what was';
OF	:	'of';
TOO	:	'too';
MAYBE	:	'maybe';
PERHAPS	:	'perhaps';
EITHER	:	'either';
HAD	:	'had';
QUESTION_MARK
	:	'?';
ATE	:	'ate';
DRANK	:	'drank';
	
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