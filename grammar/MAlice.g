grammar MAlice;

options {
	language=C;
	output=AST;
}

tokens {
	PROGRAM;
	DECLS;
	PARAMS;
	BODY;
	IFSTATEMENT;
	PRINTSTATEMENT;
	INCREMENTSTATEMENT;
	DECREMENTSTATEMENT;
	WHILESTATEMENT;
}

// Programs, procedures and functions
program	:	(variable_declaration statement_inner_separator)* (i+=function|i+=procedure)+
		-> ^(PROGRAM ^(DECLS variable_declaration* $i+))
	;


// Types and constants


type 	:	NUMBER_TYPE
        |   	LETTER_TYPE
        |	SENTENCE_TYPE
        ;
       
constant:	NUMBER_LITERAL
	|	CHARACTER_LITERAL
	;


function:	THEROOM IDENTIFIER LPAREN declaration_argument_list? RPAREN CONTAINEDA type block
		-> ^(IDENTIFIER declaration_argument_list? type block)
	;
procedure
	:	THELOOKINGGLASS IDENTIFIER LPAREN declaration_argument_list? RPAREN block
		-> ^(IDENTIFIER declaration_argument_list? block)
	;

declaration_argument_list
	:	(declaration_argument COMMA)* declaration_argument
		-> ^(PARAMS declaration_argument+)
	;

block	:	OPENED body CLOSED
		-> ^(BODY body)
	;
body
	: body_declarations? statement_list;

body_declarations
	:	(i+=variable_declaration statement_inner_separator | i+=procedure | i+=function)+
		-> ^(DECLS $i+)
	;
	
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
	| 	(stdout_lvalue (SAIDALICE | SPOKE)) => print_statement statement_inner_separator
	|	(IDENTIFIER LPAREN) => proc_func_invocation (options{greedy=true;} : FULL_STOP)?
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
		-> ^(WHILESTATEMENT boolean_expression ^(BODY statement_list))
	;
	
if_block
	:	PERHAPS boolean_expression SO statement_list else_block* ALICEWASUNSURE
		-> ^(IFSTATEMENT boolean_expression statement_list)
	|	EITHER boolean_expression SO statement_list OR statement_list ALICEWASUNSURE
		-> ^(IFSTATEMENT boolean_expression statement_list statement_list)
	;
	
else_block
	:	OR (MAYBE boolean_expression SO)? statement_list
	;
	
variable_declaration
	:	IDENTIFIER (WASA type (OF expression)? | HAD expression type) TOO?
	;

print_statement
	:	stdout_lvalue (SPOKE | SAIDALICE)
		-> ^(PRINTSTATEMENT stdout_lvalue)
	;

input_statement
	:	WHATWAS lvalue QUESTION_MARK
	;

stdout_lvalue
	:	STRING_LITERAL
	|	expression
	;
	
increment_statement
	:	lvalue ATE
		-> ^(INCREMENTSTATEMENT lvalue)
	;
	
decrement_statement
	:	lvalue DRANK
		-> ^(DECREMENTSTATEMENT lvalue)
	;
	

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
	:	multiplicactive_expr (additive_operator multiplicactive_expr)*
	;
	
additive_operator
	:	(PLUS | MINUS);

multiplicactive_expr
	:	bitwise_expr (multiplicative_operator bitwise_expr)*
	;
	
multiplicative_operator
	:	('*' | '/' | '%');
	
bitwise_expr
	:	unary_expr (bitwise_operator unary_expr)*
	;
	
bitwise_operator
	:	('^' | '|' | '&');

unary_expr
	:	(IDENTIFIER LPAREN) => proc_func_invocation
	| 	unary_operator unary_expr
		-> ^(unary_operator unary_expr)
	|	constant
	|	lvalue
	|	LPAREN additive_expr RPAREN
		-> additive_expr
	;
	
unary_operator
	:	(PLUS | MINUS | TILDE | BANG);

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

NUMBER_TYPE
	:	'number';
LETTER_TYPE
	:	'letter';
SENTENCE_TYPE
	:	'sentence';
	
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