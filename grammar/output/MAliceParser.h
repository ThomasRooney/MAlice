/** \file
 *  This C header file was generated by $ANTLR version 3.4
 *
 *     -  From the grammar source file : /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g
 *     -                            On : 2012-11-26 15:14:08
 *     -                for the parser : MAliceParserParser
 *
 * Editing it, at least manually, is not wise.
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
 * The parser 
MAliceParser

has the callable functions (rules) shown below,
 * which will invoke the code for the associated rule in the source grammar
 * assuming that the input stream is pointing to a token/text stream that could begin
 * this rule.
 *
 * For instance if you call the first (topmost) rule in a parser grammar, you will
 * get the results of a full parse, but calling a rule half way through the grammar will
 * allow you to pass part of a full token stream to the parser, such as for syntax checking
 * in editors and so on.
 *
 * The parser entry points are called indirectly (by function pointer to function) via
 * a parser context typedef pMAliceParser, which is returned from a call to MAliceParserNew().
 *
 * The methods in pMAliceParser are  as follows:
 *
 *  - 
 MAliceParser_program_return
      pMAliceParser->program(pMAliceParser)
 *  - 
 MAliceParser_type_return
      pMAliceParser->type(pMAliceParser)
 *  - 
 MAliceParser_constant_return
      pMAliceParser->constant(pMAliceParser)
 *  - 
 MAliceParser_function_return
      pMAliceParser->function(pMAliceParser)
 *  - 
 MAliceParser_procedure_return
      pMAliceParser->procedure(pMAliceParser)
 *  - 
 MAliceParser_declaration_argument_list_return
      pMAliceParser->declaration_argument_list(pMAliceParser)
 *  - 
 MAliceParser_block_return
      pMAliceParser->block(pMAliceParser)
 *  - 
 MAliceParser_func_proc_block_return
      pMAliceParser->func_proc_block(pMAliceParser)
 *  - 
 MAliceParser_body_return
      pMAliceParser->body(pMAliceParser)
 *  - 
 MAliceParser_body_declarations_return
      pMAliceParser->body_declarations(pMAliceParser)
 *  - 
 MAliceParser_declaration_argument_return
      pMAliceParser->declaration_argument(pMAliceParser)
 *  - 
 MAliceParser_proc_func_invocation_return
      pMAliceParser->proc_func_invocation(pMAliceParser)
 *  - 
 MAliceParser_assignment_return
      pMAliceParser->assignment(pMAliceParser)
 *  - 
 MAliceParser_statement_inner_separator_return
      pMAliceParser->statement_inner_separator(pMAliceParser)
 *  - 
 MAliceParser_statement_list_return
      pMAliceParser->statement_list(pMAliceParser)
 *  - 
 MAliceParser_statement_component_return
      pMAliceParser->statement_component(pMAliceParser)
 *  - 
 MAliceParser_return_statement_return
      pMAliceParser->return_statement(pMAliceParser)
 *  - 
 MAliceParser_while_loop_return
      pMAliceParser->while_loop(pMAliceParser)
 *  - 
 MAliceParser_if_block_return
      pMAliceParser->if_block(pMAliceParser)
 *  - 
 MAliceParser_else_block_return
      pMAliceParser->else_block(pMAliceParser)
 *  - 
 MAliceParser_variable_declaration_return
      pMAliceParser->variable_declaration(pMAliceParser)
 *  - 
 MAliceParser_print_statement_return
      pMAliceParser->print_statement(pMAliceParser)
 *  - 
 MAliceParser_input_statement_return
      pMAliceParser->input_statement(pMAliceParser)
 *  - 
 MAliceParser_stdout_lvalue_return
      pMAliceParser->stdout_lvalue(pMAliceParser)
 *  - 
 MAliceParser_increment_statement_return
      pMAliceParser->increment_statement(pMAliceParser)
 *  - 
 MAliceParser_decrement_statement_return
      pMAliceParser->decrement_statement(pMAliceParser)
 *  - 
 MAliceParser_expression_return
      pMAliceParser->expression(pMAliceParser)
 *  - 
 MAliceParser_lvalue_return
      pMAliceParser->lvalue(pMAliceParser)
 *  - 
 MAliceParser_additive_expr_return
      pMAliceParser->additive_expr(pMAliceParser)
 *  - 
 MAliceParser_additive_operator_return
      pMAliceParser->additive_operator(pMAliceParser)
 *  - 
 MAliceParser_multiplicative_expr_return
      pMAliceParser->multiplicative_expr(pMAliceParser)
 *  - 
 MAliceParser_multiplicative_operator_return
      pMAliceParser->multiplicative_operator(pMAliceParser)
 *  - 
 MAliceParser_bitwise_expr_return
      pMAliceParser->bitwise_expr(pMAliceParser)
 *  - 
 MAliceParser_bitwise_operator_return
      pMAliceParser->bitwise_operator(pMAliceParser)
 *  - 
 MAliceParser_unary_expr_return
      pMAliceParser->unary_expr(pMAliceParser)
 *  - 
 MAliceParser_unary_operator_return
      pMAliceParser->unary_operator(pMAliceParser)
 *  - 
 MAliceParser_null_statement_return
      pMAliceParser->null_statement(pMAliceParser)
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 *
 * The return type for any particular rule is of course determined by the source
 * grammar file.
 */
// [The "BSD license"]
// Copyright (c) 2005-2009 Jim Idle, Temporal Wave LLC
// http://www.temporal-wave.com
// http://www.linkedin.com/in/jimidle
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. The name of the author may not be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef	_MAliceParser_H
#define _MAliceParser_H
/* =============================================================================
 * Standard antlr3 C runtime definitions
 */
#include    <antlr3.h>

/* End of standard antlr 3 runtime definitions
 * =============================================================================
 */

	#include "ErrorReporter.h"	


#ifdef __cplusplus
extern "C" {
#endif

// Forward declare the context typedef so that we can use it before it is
// properly defined. Delegators and delegates (from import statements) are
// interdependent and their context structures contain pointers to each other
// C only allows such things to be declared if you pre-declare the typedef.
//
typedef struct MAliceParser_Ctx_struct MAliceParser, * pMAliceParser;



#ifdef	ANTLR3_WINDOWS
// Disable: Unreferenced parameter,							- Rules with parameters that are not used
//          constant conditional,							- ANTLR realizes that a prediction is always true (synpred usually)
//          initialized but unused variable					- tree rewrite variables declared but not needed
//          Unreferenced local variable						- lexer rule declares but does not always use _type
//          potentially unitialized variable used			- retval always returned from a rule
//			unreferenced local function has been removed	- susually getTokenNames or freeScope, they can go without warnigns
//
// These are only really displayed at warning level /W4 but that is the code ideal I am aiming at
// and the codegen must generate some of these warnings by necessity, apart from 4100, which is
// usually generated when a parser rule is given a parameter that it does not use. Mostly though
// this is a matter of orthogonality hence I disable that one.
//
#pragma warning( disable : 4100 )
#pragma warning( disable : 4101 )
#pragma warning( disable : 4127 )
#pragma warning( disable : 4189 )
#pragma warning( disable : 4505 )
#pragma warning( disable : 4701 )
#endif

/* ========================
 * BACKTRACKING IS ENABLED
 * ========================
 */

typedef struct MAliceParser_program_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_program_return;



typedef struct MAliceParser_type_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_type_return;



typedef struct MAliceParser_constant_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_constant_return;



typedef struct MAliceParser_function_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_function_return;



typedef struct MAliceParser_procedure_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_procedure_return;



typedef struct MAliceParser_declaration_argument_list_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_declaration_argument_list_return;



typedef struct MAliceParser_block_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_block_return;



typedef struct MAliceParser_func_proc_block_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_func_proc_block_return;



typedef struct MAliceParser_body_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_body_return;



typedef struct MAliceParser_body_declarations_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_body_declarations_return;



typedef struct MAliceParser_declaration_argument_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_declaration_argument_return;



typedef struct MAliceParser_proc_func_invocation_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_proc_func_invocation_return;



typedef struct MAliceParser_assignment_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_assignment_return;



typedef struct MAliceParser_statement_inner_separator_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_statement_inner_separator_return;



typedef struct MAliceParser_statement_list_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_statement_list_return;



typedef struct MAliceParser_statement_component_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_statement_component_return;



typedef struct MAliceParser_return_statement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_return_statement_return;



typedef struct MAliceParser_while_loop_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_while_loop_return;



typedef struct MAliceParser_if_block_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_if_block_return;



typedef struct MAliceParser_else_block_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_else_block_return;



typedef struct MAliceParser_variable_declaration_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_variable_declaration_return;



typedef struct MAliceParser_print_statement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_print_statement_return;



typedef struct MAliceParser_input_statement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_input_statement_return;



typedef struct MAliceParser_stdout_lvalue_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_stdout_lvalue_return;



typedef struct MAliceParser_increment_statement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_increment_statement_return;



typedef struct MAliceParser_decrement_statement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_decrement_statement_return;



typedef struct MAliceParser_expression_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_expression_return;



typedef struct MAliceParser_lvalue_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_lvalue_return;



typedef struct MAliceParser_additive_expr_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_additive_expr_return;



typedef struct MAliceParser_additive_operator_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_additive_operator_return;



typedef struct MAliceParser_multiplicative_expr_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_multiplicative_expr_return;



typedef struct MAliceParser_multiplicative_operator_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_multiplicative_operator_return;



typedef struct MAliceParser_bitwise_expr_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_bitwise_expr_return;



typedef struct MAliceParser_bitwise_operator_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_bitwise_operator_return;



typedef struct MAliceParser_unary_expr_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_unary_expr_return;



typedef struct MAliceParser_unary_operator_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_unary_operator_return;



typedef struct MAliceParser_null_statement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    MAliceParser_null_statement_return;




/** Context tracking structure for 
MAliceParser

 */
struct MAliceParser_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_PARSER   pParser;

     MAliceParser_program_return
     (*program)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_type_return
     (*type)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_constant_return
     (*constant)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_function_return
     (*function)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_procedure_return
     (*procedure)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_declaration_argument_list_return
     (*declaration_argument_list)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_block_return
     (*block)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_func_proc_block_return
     (*func_proc_block)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_body_return
     (*body)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_body_declarations_return
     (*body_declarations)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_declaration_argument_return
     (*declaration_argument)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_proc_func_invocation_return
     (*proc_func_invocation)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_assignment_return
     (*assignment)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_statement_inner_separator_return
     (*statement_inner_separator)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_statement_list_return
     (*statement_list)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_statement_component_return
     (*statement_component)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_return_statement_return
     (*return_statement)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_while_loop_return
     (*while_loop)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_if_block_return
     (*if_block)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_else_block_return
     (*else_block)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_variable_declaration_return
     (*variable_declaration)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_print_statement_return
     (*print_statement)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_input_statement_return
     (*input_statement)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_stdout_lvalue_return
     (*stdout_lvalue)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_increment_statement_return
     (*increment_statement)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_decrement_statement_return
     (*decrement_statement)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_expression_return
     (*expression)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_lvalue_return
     (*lvalue)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_additive_expr_return
     (*additive_expr)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_additive_operator_return
     (*additive_operator)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_multiplicative_expr_return
     (*multiplicative_expr)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_multiplicative_operator_return
     (*multiplicative_operator)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_bitwise_expr_return
     (*bitwise_expr)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_bitwise_operator_return
     (*bitwise_operator)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_unary_expr_return
     (*unary_expr)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_unary_operator_return
     (*unary_operator)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_null_statement_return
     (*null_statement)	(struct MAliceParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred1_MAlice)	(struct MAliceParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred2_MAlice)	(struct MAliceParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred3_MAlice)	(struct MAliceParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred4_MAlice)	(struct MAliceParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred5_MAlice)	(struct MAliceParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred6_MAlice)	(struct MAliceParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred7_MAlice)	(struct MAliceParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred8_MAlice)	(struct MAliceParser_Ctx_struct * ctx);
    // Delegated rules

    const char * (*getGrammarFileName)();
    void            (*reset)  (struct MAliceParser_Ctx_struct * ctx);
    void	    (*free)   (struct MAliceParser_Ctx_struct * ctx);
/* @headerFile.members() */
pANTLR3_BASE_TREE_ADAPTOR	adaptor;
pANTLR3_VECTOR_FACTORY		vectors;
/* End @headerFile.members() */
};

// Function protoypes for the constructor functions that external translation units
// such as delegators and delegates may wish to call.
//
ANTLR3_API pMAliceParser MAliceParserNew         (
pANTLR3_COMMON_TOKEN_STREAM
 instream);
ANTLR3_API pMAliceParser MAliceParserNewSSD      (
pANTLR3_COMMON_TOKEN_STREAM
 instream, pANTLR3_RECOGNIZER_SHARED_STATE state);

/** Symbolic definitions of all the tokens that the 
parser
 will work with.
 * \{
 *
 * Antlr will define EOF, but we can't use that as it it is too common in
 * in C header files and that would be confusing. There is no way to filter this out at the moment
 * so we just undef it here for now. That isn't the value we get back from C recognizers
 * anyway. We are looking for ANTLR3_TOKEN_EOF.
 */
#ifdef	EOF
#undef	EOF
#endif
#ifdef	Tokens
#undef	Tokens
#endif
#define EOF      -1
#define ALICEFOUND      4
#define ALICEWASUNSURE      5
#define AND      6
#define APOSTROPHE_S      7
#define ARRAY      8
#define ARRAYSUBSCRIPT      9
#define ASSIGNMENTSTATEMENT      10
#define ATE      11
#define BANG      12
#define BECAME      13
#define BECAUSE      14
#define BITWISEAND      15
#define BITWISEOR      16
#define BITWISEXOR      17
#define BODY      18
#define BUT      19
#define BYREFERENCE      20
#define BYVALUE      21
#define CHARACTER_LITERAL      22
#define CLOSED      23
#define COMMA      24
#define COMMENT      25
#define CONTAINEDA      26
#define DECLS      27
#define DECREMENTSTATEMENT      28
#define DIGIT      29
#define DIVIDE      30
#define DRANK      31
#define EITHER      32
#define ENOUGHTIMES      33
#define EQUALS      34
#define ESCAPE      35
#define EVENTUALLY      36
#define EXPRESSION      37
#define FULL_STOP      38
#define FUNCDEFINITION      39
#define GREATERTHAN      40
#define GREATERTHANEQUAL      41
#define HAD      42
#define IDENTIFIER      43
#define IFSTATEMENT      44
#define INCREMENTSTATEMENT      45
#define INPUTSTATEMENT      46
#define INVOCATION      47
#define LESSTHAN      48
#define LESSTHANEQUAL      49
#define LETTER      50
#define LETTER_TYPE      51
#define LOGICALAND      52
#define LOGICALOR      53
#define LPAREN      54
#define MAYBE      55
#define MINUS      56
#define MODULO      57
#define MULTIPLY      58
#define NEWLINE      59
#define NOTEQUAL      60
#define NUMBER_LITERAL      61
#define NUMBER_TYPE      62
#define OF      63
#define OPENED      64
#define OR      65
#define PARAMS      66
#define PERHAPS      67
#define PIECE      68
#define PLUS      69
#define PRINTSTATEMENT      70
#define PROCDEFINITION      71
#define PROGRAM      72
#define QUESTION_MARK      73
#define QUOTE      74
#define RETURNSTATEMENT      75
#define RPAREN      76
#define SAIDALICE      77
#define SENTENCE_TYPE      78
#define SO      79
#define SPIDER      80
#define SPOKE      81
#define STATEMENTLIST      82
#define STRING_LITERAL      83
#define THELOOKINGGLASS      84
#define THEN      85
#define THEROOM      86
#define TILDE      87
#define TOO      88
#define UNDERSCORE      89
#define VARDECLARATION      90
#define WASA      91
#define WHATWAS      92
#define WHILESTATEMENT      93
#define WS      94
#ifdef	EOF
#undef	EOF
#define	EOF	ANTLR3_TOKEN_EOF
#endif

#ifndef TOKENSOURCE
#define TOKENSOURCE(lxr) lxr->pLexer->rec->state->tokSource
#endif

/* End of token definitions for MAliceParser
 * =============================================================================
 */
/** } */

#ifdef __cplusplus
}
#endif

#endif

/* END - Note:Keep extra line feed to satisfy UNIX systems */
