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
	STATEMENTLIST;
	PRINTSTATEMENT;
	INCREMENTSTATEMENT;
	DECREMENTSTATEMENT;
	WHILESTATEMENT;
	RETURNSTATEMENT;
	ASSIGNMENTSTATEMENT;
	INVOCATION;
	FUNCDEFINITION;
	PROCDEFINITION;
	INPUTSTATEMENT;
	EXPRESSION;
	BYREFERENCE;
	BYVALUE;
	ARRAY;
	ARRAYSUBSCRIPT;
	IFSTATEMENT;
	VARDECLARATION;
	BLOCK;
}

@parser::includes
{
	#include "ErrorReporter.h"	
}

@parser::apifuncs
{
	RECOGNIZER->displayRecognitionError = handleParserError;
}

@lexer::includes
{
	#include "ErrorReporter.h"	
}

@lexer::apifuncs
{
	RECOGNIZER->displayRecognitionError = handleLexerError;
}

// Programs, procedures and functions
program	:	(variable_declaration statement_inner_separator)* (func+=function|proc+=procedure)*
		-> ^(PROGRAM ^(DECLS variable_declaration* $func* $proc*))
	;


// Types and constants


type 	:	NUMBER_TYPE
        |   	LETTER_TYPE
        |	SENTENCE_TYPE
        ;
       
constant:	NUMBER_LITERAL
	|	CHARACTER_LITERAL
	|	STRING_LITERAL
	;


function:	THEROOM IDENTIFIER LPAREN declaration_argument_list? RPAREN CONTAINEDA type block
		-> ^(FUNCDEFINITION IDENTIFIER declaration_argument_list? type block)
	;
procedure
	:	THELOOKINGGLASS IDENTIFIER LPAREN declaration_argument_list? RPAREN block
		-> ^(PROCDEFINITION IDENTIFIER declaration_argument_list? block)
	;

declaration_argument_list
	:	(declaration_argument COMMA)* declaration_argument
		-> ^(PARAMS declaration_argument+)
	;

block	:	OPENED body? CLOSED
		-> ^(BODY body?)
	;
	
arbitraryblock
	:	OPENED body? CLOSED
		-> ^(BLOCK body?)
	;
	
body
	: body_declarations? statement_list;

body_declarations
	:	(var_decl+=variable_declaration statement_inner_separator | proc+=procedure | func+=function)+
		-> ^(DECLS $var_decl* $proc* $func*)
	;
	
declaration_argument
	:	SPIDER type IDENTIFIER
		-> ^(BYREFERENCE type IDENTIFIER)
	|	type IDENTIFIER
		-> ^(BYVALUE type IDENTIFIER)
	;



proc_func_invocation
	:	IDENTIFIER LPAREN (i+=expression (COMMA i+=expression)*)? RPAREN
		-> ^(INVOCATION ^(IDENTIFIER $i*))
	;

//Expression

assignment
	:	e1=expression BECAME e2=expression
		-> ^(ASSIGNMENTSTATEMENT $e1 $e2)
	;

// Statements

statement_inner_separator 
	:	(COMMA | THEN | AND | BUT | FULL_STOP);

statement_list
	:	statement_component+
		-> ^(STATEMENTLIST statement_component+)
	;
statement_component
	:	(EVENTUALLY) => while_loop (options{greedy=true;} : FULL_STOP!)?
	| 	(stdout_lvalue (SAIDALICE | SPOKE)) => print_statement statement_inner_separator!
	|	(IDENTIFIER LPAREN) => proc_func_invocation statement_inner_separator!
	|	(lvalue ATE) => increment_statement
	|	(lvalue DRANK) => decrement_statement
	|	input_statement (options{greedy=true;} : FULL_STOP!)?
	|	(ALICEFOUND expression) => return_statement FULL_STOP!
	|	null_statement
	|	if_block (options{greedy=true;} : FULL_STOP!)?
	|	assignment statement_inner_separator!
	|	arbitraryblock (options{greedy=true;} : FULL_STOP!)?
	;

return_statement
	:	ALICEFOUND expression
		-> ^(RETURNSTATEMENT expression)
	;
	
while_loop
	:	EVENTUALLY expression BECAUSE statement_list ENOUGHTIMES
		-> ^(WHILESTATEMENT expression ^(BODY statement_list))
	;
	
if_block
	:	PERHAPS LPAREN expression RPAREN SO statement_list else_block* BECAUSE ALICEWASUNSURE
		-> ^(IFSTATEMENT expression statement_list else_block*)
	|	EITHER LPAREN expression RPAREN SO statement_list OR statement_list BECAUSE ALICEWASUNSURE
		-> ^(IFSTATEMENT expression statement_list statement_list)
	;
	
else_block
	:	OR (MAYBE LPAREN expression RPAREN SO)? statement_list
		-> expression? statement_list
	;
	
variable_declaration
	:	IDENTIFIER WASA type TOO?
		-> ^(VARDECLARATION IDENTIFIER type)
	|	IDENTIFIER WASA type OF expression TOO?
		-> ^(VARDECLARATION IDENTIFIER type expression)
	|	IDENTIFIER HAD expression type TOO?
		-> ^(ARRAY ^(IDENTIFIER type expression))
	;

print_statement
	:	stdout_lvalue (SPOKE | SAIDALICE)
		-> ^(PRINTSTATEMENT stdout_lvalue)
	;

input_statement
	:	WHATWAS lvalue QUESTION_MARK
		-> ^(INPUTSTATEMENT lvalue)
	;

stdout_lvalue
	:	expression
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
	:	boolean_combinator_expr
		-> ^(EXPRESSION boolean_combinator_expr)
	;

lvalue	:	(IDENTIFIER APOSTROPHE_S) => IDENTIFIER (APOSTROPHE_S expression PIECE)?
		-> ^(ARRAYSUBSCRIPT IDENTIFIER expression)
	|	IDENTIFIER
	;
	
boolean_combinator_expr
	:	boolean_expr (boolean_combinator_operator^ boolean_expr)*
	;
	
boolean_combinator_operator
	:	(LOGICALAND | LOGICALOR)
	;

boolean_expr 
	:	additive_expr (boolean_operator^ additive_expr)*
	;
	
boolean_operator
	:	(EQUALS | NOTEQUAL | LESSTHAN | LESSTHANEQUAL | GREATERTHAN | GREATERTHANEQUAL)
	;

additive_expr
	:	multiplicative_expr (additive_operator^ multiplicative_expr)*
	;
	
additive_operator
	:	(PLUS | MINUS)
	;
	


multiplicative_expr
	:	bitwise_expr (multiplicative_operator^ bitwise_expr)*
	;
	
multiplicative_operator
	:	(MULTIPLY | DIVIDE | MODULO)
	;
	
	
bitwise_expr
	:	unary_expr (bitwise_operator^ unary_expr)*
	;
	
bitwise_operator
	:	(BITWISEXOR | BITWISEOR | BITWISEAND)
	;

unary_expr
	:	(IDENTIFIER LPAREN) => proc_func_invocation
	|	constant
	| 	unary_operator^ unary_expr
	|	lvalue
	|	LPAREN boolean_combinator_expr RPAREN
		-> boolean_combinator_expr
	;
	
unary_operator
	:	(PLUS | MINUS | TILDE | BANG)
	;
	
		
null_statement
	:	FULL_STOP;

// Lexer rules
// Based on the ANSI-C whitespace grammar.
WS    :    (' '|'\t'|NEWLINE)+ {$channel=HIDDEN;};
    	
// Based on the ANSI-C line comment grammar.
COMMENT	:	'###' ~('\n'|'\r')* NEWLINE {$channel=HIDDEN;};

CHARACTER_LITERAL
	:	'\'' LETTER '\''
	;
	
STRING_LITERAL
	:	QUOTE (ESCAPE | ~(QUOTE))* QUOTE
	;
NUMBER_LITERAL
	:	'0' | ('-')? '1'..'9' DIGIT*
	;
	
fragment NEWLINE
	:	'\r'? '\n' ;
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
SAIDALICE
	:	'said Alice';
COMMA	:	',';
THEN	:	'then';
AND	:	'and';
FULL_STOP
	:	'.'
	;
BECAUSE
	:	'because';
ALICEWASUNSURE
	:	'Alice was unsure which'
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
APOSTROPHE_S
	:	'\'s';

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
MULTIPLY:	'*';
DIVIDE	:	'/';
MODULO	:	'%';
EQUALS	:	'==';
NOTEQUAL:	'!=';
LESSTHAN	
	:	'<';
LESSTHANEQUAL	
	:	'<=';
GREATERTHAN
	:	'>';
GREATERTHANEQUAL
	:	'>=';
LOGICALAND
	:	'&&'
	;
LOGICALOR
	:	'||'
	;
BITWISEAND
	:	'&';
BITWISEOR
	:	'|';
BITWISEXOR
	:	'^';
	
UNDERSCORE
	:	'_';

fragment LETTER
	:	('a'..'z' | 'A'..'Z')
	;
fragment DIGIT
	:	'0'..'9'
	;
	
fragment ESCAPE
	:	'\\"'
	;
fragment QUOTE
	:	'"';