/** \file
 *  This C header file was generated by $ANTLR version 3.4
 *
 *     -  From the grammar source file : /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g
 *     -                            On : 2012-11-16 15:40:10
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
 MAliceParser_primary_expr_return
      pMAliceParser->primary_expr(pMAliceParser)
 *  - 
 MAliceParser_expression_return
      pMAliceParser->expression(pMAliceParser)
 *  - 
 MAliceParser_assignment_expr_return
      pMAliceParser->assignment_expr(pMAliceParser)
 *  - 
 MAliceParser_lvalue_return
      pMAliceParser->lvalue(pMAliceParser)
 *  - 
 MAliceParser_additive_expr_return
      pMAliceParser->additive_expr(pMAliceParser)
 *  - 
 MAliceParser_multiplicactive_expr_return
      pMAliceParser->multiplicactive_expr(pMAliceParser)
 *  - 
 MAliceParser_bitwise_expr_return
      pMAliceParser->bitwise_expr(pMAliceParser)
 *  - 
 MAliceParser_unary_expr_return
      pMAliceParser->unary_expr(pMAliceParser)
 *  - 
 MAliceParser_identifier_return
      pMAliceParser->identifier(pMAliceParser)
 *  - 
 MAliceParser_split_return
      pMAliceParser->split(pMAliceParser)
 *  - 
 MAliceParser_type_return
      pMAliceParser->type(pMAliceParser)
 *  - 
 MAliceParser_unary_operator_return
      pMAliceParser->unary_operator(pMAliceParser)
 *  - 
 MAliceParser_constant_return
      pMAliceParser->constant(pMAliceParser)
 *  - 
 MAliceParser_program_return
      pMAliceParser->program(pMAliceParser)
 *  - 
 MAliceParser_function_return
      pMAliceParser->function(pMAliceParser)
 *  - 
 MAliceParser_function_type_return
      pMAliceParser->function_type(pMAliceParser)
 *  - 
 MAliceParser_function_argument_list_return
      pMAliceParser->function_argument_list(pMAliceParser)
 *  - 
 MAliceParser_function_argument_return
      pMAliceParser->function_argument(pMAliceParser)
 *  - 
 MAliceParser_return_type_clause_return
      pMAliceParser->return_type_clause(pMAliceParser)
 *  - 
 MAliceParser_block_unit_return
      pMAliceParser->block_unit(pMAliceParser)
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

typedef struct MAliceParser_primary_expr_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    CommonTree	tree;

}
    MAliceParser_primary_expr_return;



typedef struct MAliceParser_expression_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    CommonTree	tree;

}
    MAliceParser_expression_return;



typedef struct MAliceParser_assignment_expr_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    CommonTree	tree;

}
    MAliceParser_assignment_expr_return;



typedef struct MAliceParser_lvalue_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    CommonTree	tree;

}
    MAliceParser_lvalue_return;



typedef struct MAliceParser_additive_expr_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    CommonTree	tree;

}
    MAliceParser_additive_expr_return;



typedef struct MAliceParser_multiplicactive_expr_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    CommonTree	tree;

}
    MAliceParser_multiplicactive_expr_return;



typedef struct MAliceParser_bitwise_expr_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    CommonTree	tree;

}
    MAliceParser_bitwise_expr_return;



typedef struct MAliceParser_unary_expr_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    CommonTree	tree;

}
    MAliceParser_unary_expr_return;



typedef struct MAliceParser_identifier_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    CommonTree	tree;

}
    MAliceParser_identifier_return;



typedef struct MAliceParser_split_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    CommonTree	tree;

}
    MAliceParser_split_return;



typedef struct MAliceParser_type_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    CommonTree	tree;

}
    MAliceParser_type_return;



typedef struct MAliceParser_unary_operator_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    CommonTree	tree;

}
    MAliceParser_unary_operator_return;



typedef struct MAliceParser_constant_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    CommonTree	tree;

}
    MAliceParser_constant_return;



typedef struct MAliceParser_program_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    CommonTree	tree;

}
    MAliceParser_program_return;



typedef struct MAliceParser_function_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    CommonTree	tree;

}
    MAliceParser_function_return;



typedef struct MAliceParser_function_type_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    CommonTree	tree;

}
    MAliceParser_function_type_return;



typedef struct MAliceParser_function_argument_list_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    CommonTree	tree;

}
    MAliceParser_function_argument_list_return;



typedef struct MAliceParser_function_argument_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    CommonTree	tree;

}
    MAliceParser_function_argument_return;



typedef struct MAliceParser_return_type_clause_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    CommonTree	tree;

}
    MAliceParser_return_type_clause_return;



typedef struct MAliceParser_block_unit_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    CommonTree	tree;

}
    MAliceParser_block_unit_return;




/** Context tracking structure for 
MAliceParser

 */
struct MAliceParser_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_PARSER   pParser;

     MAliceParser_primary_expr_return
     (*primary_expr)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_expression_return
     (*expression)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_assignment_expr_return
     (*assignment_expr)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_lvalue_return
     (*lvalue)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_additive_expr_return
     (*additive_expr)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_multiplicactive_expr_return
     (*multiplicactive_expr)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_bitwise_expr_return
     (*bitwise_expr)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_unary_expr_return
     (*unary_expr)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_identifier_return
     (*identifier)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_split_return
     (*split)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_type_return
     (*type)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_unary_operator_return
     (*unary_operator)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_constant_return
     (*constant)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_program_return
     (*program)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_function_return
     (*function)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_function_type_return
     (*function_type)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_function_argument_list_return
     (*function_argument_list)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_function_argument_return
     (*function_argument)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_return_type_clause_return
     (*return_type_clause)	(struct MAliceParser_Ctx_struct * ctx);

     MAliceParser_block_unit_return
     (*block_unit)	(struct MAliceParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred1_MAlice)	(struct MAliceParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred2_MAlice)	(struct MAliceParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred9_MAlice)	(struct MAliceParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred10_MAlice)	(struct MAliceParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred11_MAlice)	(struct MAliceParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred22_MAlice)	(struct MAliceParser_Ctx_struct * ctx);
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
#define T__8      8
#define T__9      9
#define T__10      10
#define T__11      11
#define T__12      12
#define T__13      13
#define T__14      14
#define T__15      15
#define T__16      16
#define T__17      17
#define T__18      18
#define T__19      19
#define T__20      20
#define T__21      21
#define T__22      22
#define T__23      23
#define T__24      24
#define T__25      25
#define T__26      26
#define T__27      27
#define T__28      28
#define T__29      29
#define T__30      30
#define T__31      31
#define T__32      32
#define T__33      33
#define T__34      34
#define CHARACTER_LITERAL      4
#define DECIMAL_NUMBER      5
#define LETTER      6
#define NEWLINE      7
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
