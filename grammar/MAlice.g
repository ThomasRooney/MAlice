grammar MAlice;

options {
	language=C;
}

// Programs, procedures and functions
program	:	(variable_declaration declaration_separator)* (function|procedure)+;


// Types and constants


type 	:	'number'
        |   	'letter'
        |	'sentence'
        ;
       
constant:	number_literal
	|	CHARACTER_LITERAL
	;


function:	THEROOM identifier LPAREN declaration_argument_list? RPAREN 'contained a' type block
	;
procedure
	:	THELOOKINGGLASS identifier LPAREN declaration_argument_list? RPAREN OPENED body CLOSED
	;

declaration_argument_list
	:	(declaration_argument ',')* declaration_argument
	;
	
	

block	:	OPENED body CLOSED;
body
	: (variable_declaration declaration_separator | procedure | function)* statement_list;
declaration_argument
	:	type identifier
	;



proc_func_invocation
	:	identifier LPAREN (expression (',' expression)*)? RPAREN
	;

//Expression

assignment
	:	lvalue BECAME expression
	;

// Statements

declaration_separator 	:	(COMMA | THEN | AND | BUT | FULL_STOP);

statement_list
	:	statement_component+
	;
statement_component
	:	(identifier LPAREN) => proc_func_invocation (options{greedy=true;} : FULL_STOP)?
	|	(stdout_lvalue (SAIDALICE | SPOKE)) => io_statement FULL_STOP
	|	(ALICEFOUND expression) => return_statement FULL_STOP
	|	FULL_STOP
	|	while_loop (options{greedy=true;} : FULL_STOP)?
	|	if_block (options{greedy=true;} : FULL_STOP)?
	|	assignment_expr FULL_STOP
	|	input_statement	FULL_STOP
	|	block (options{greedy=true;} : FULL_STOP)?
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
	:	identifier (WASA type ('of' expression)? | 'had' expression type) 'too'?;
	
input_statement
	:	'what was' lvalue '?';

io_statement
	:	print_statement ((COMMA | AND) print_statement)* (',' stdin_statement)?
//	:	stdout_lvalue ((SPOKE|SAIDALICE) (AND stdout_lvalue)?)* (SAIDALICE | ',' stdin_statement) 
	;
	
print_statement
	:	stdout_lvalue (SPOKE | SAIDALICE);

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
	:	multiplicactive_expr ((PLUS|MINUS) multiplicactive_expr)*
	;

multiplicactive_expr
	:	bitwise_expr (('*'|'/'|'%') bitwise_expr)*
	;

bitwise_expr
	:	unary_expr (('^'|'|'|'&') unary_expr)*
	;

unary_expr
	:	(identifier LPAREN) => proc_func_invocation
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

identifier
	:	LETTER (LETTER | '0'..'9' | UNDERSCORE)*;	
	
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
BUT	:	'but';
FULL_STOP
	:	'.' ;

PLUS
	:	'+'
	;

MINUS
	:	'-'
	;

TILDE
	:	'~'
	;

BANG
	:	'!'
	;

UNDERSCORE
	:	'_'
	;
