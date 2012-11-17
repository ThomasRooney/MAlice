/** \file
 *  This C header file was generated by $ANTLR version 3.4
 *
 *     -  From the grammar source file : /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g
 *     -                            On : 2012-11-17 14:47:24
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
 void
      pMAliceParser->expression(pMAliceParser)
 *  - 
 void
      pMAliceParser->assignment_expr(pMAliceParser)
 *  - 
 void
      pMAliceParser->lvalue(pMAliceParser)
 *  - 
 void
      pMAliceParser->additive_expr(pMAliceParser)
 *  - 
 void
      pMAliceParser->multiplicactive_expr(pMAliceParser)
 *  - 
 void
      pMAliceParser->bitwise_expr(pMAliceParser)
 *  - 
 void
      pMAliceParser->unary_expr(pMAliceParser)
 *  - 
 void
      pMAliceParser->identifier(pMAliceParser)
 *  - 
 void
      pMAliceParser->split(pMAliceParser)
 *  - 
 void
      pMAliceParser->type(pMAliceParser)
 *  - 
 void
      pMAliceParser->unary_operator(pMAliceParser)
 *  - 
 void
      pMAliceParser->program(pMAliceParser)
 *  - 
 void
      pMAliceParser->function(pMAliceParser)
 *  - 
 void
      pMAliceParser->function_type(pMAliceParser)
 *  - 
 void
      pMAliceParser->function_argument_list(pMAliceParser)
 *  - 
 void
      pMAliceParser->function_argument(pMAliceParser)
 *  - 
 void
      pMAliceParser->return_type_clause(pMAliceParser)
 *  - 
 void
      pMAliceParser->block_unit(pMAliceParser)
 *  - 
 void
      pMAliceParser->function_invocation(pMAliceParser)
 *  - 
 void
      pMAliceParser->constant(pMAliceParser)
 *  - 
 void
      pMAliceParser->function_invocation_argument_list(pMAliceParser)
 *  - 
 void
      pMAliceParser->assignment(pMAliceParser)
 *  - 
 void
      pMAliceParser->statement_list(pMAliceParser)
 *  - 
 void
      pMAliceParser->return_statement(pMAliceParser)
 *  - 
 void
      pMAliceParser->comment(pMAliceParser)
 *  - 
 void
      pMAliceParser->while_loop(pMAliceParser)
 *  - 
 void
      pMAliceParser->if_block(pMAliceParser)
 *  - 
 void
      pMAliceParser->else_block(pMAliceParser)
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

/** Context tracking structure for 
MAliceParser

 */
struct MAliceParser_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_PARSER   pParser;

     void
     (*expression)	(struct MAliceParser_Ctx_struct * ctx);

     void
     (*assignment_expr)	(struct MAliceParser_Ctx_struct * ctx);

     void
     (*lvalue)	(struct MAliceParser_Ctx_struct * ctx);

     void
     (*additive_expr)	(struct MAliceParser_Ctx_struct * ctx);

     void
     (*multiplicactive_expr)	(struct MAliceParser_Ctx_struct * ctx);

     void
     (*bitwise_expr)	(struct MAliceParser_Ctx_struct * ctx);

     void
     (*unary_expr)	(struct MAliceParser_Ctx_struct * ctx);

     void
     (*identifier)	(struct MAliceParser_Ctx_struct * ctx);

     void
     (*split)	(struct MAliceParser_Ctx_struct * ctx);

     void
     (*type)	(struct MAliceParser_Ctx_struct * ctx);

     void
     (*unary_operator)	(struct MAliceParser_Ctx_struct * ctx);

     void
     (*program)	(struct MAliceParser_Ctx_struct * ctx);

     void
     (*function)	(struct MAliceParser_Ctx_struct * ctx);

     void
     (*function_type)	(struct MAliceParser_Ctx_struct * ctx);

     void
     (*function_argument_list)	(struct MAliceParser_Ctx_struct * ctx);

     void
     (*function_argument)	(struct MAliceParser_Ctx_struct * ctx);

     void
     (*return_type_clause)	(struct MAliceParser_Ctx_struct * ctx);

     void
     (*block_unit)	(struct MAliceParser_Ctx_struct * ctx);

     void
     (*function_invocation)	(struct MAliceParser_Ctx_struct * ctx);

     void
     (*constant)	(struct MAliceParser_Ctx_struct * ctx);

     void
     (*function_invocation_argument_list)	(struct MAliceParser_Ctx_struct * ctx);

     void
     (*assignment)	(struct MAliceParser_Ctx_struct * ctx);

     void
     (*statement_list)	(struct MAliceParser_Ctx_struct * ctx);

     void
     (*return_statement)	(struct MAliceParser_Ctx_struct * ctx);

     void
     (*comment)	(struct MAliceParser_Ctx_struct * ctx);

     void
     (*while_loop)	(struct MAliceParser_Ctx_struct * ctx);

     void
     (*if_block)	(struct MAliceParser_Ctx_struct * ctx);

     void
     (*else_block)	(struct MAliceParser_Ctx_struct * ctx);
    // Delegated rules

    const char * (*getGrammarFileName)();
    void            (*reset)  (struct MAliceParser_Ctx_struct * ctx);
    void	    (*free)   (struct MAliceParser_Ctx_struct * ctx);
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
#define T__35      35
#define T__36      36
#define T__37      37
#define T__38      38
#define T__39      39
#define T__40      40
#define T__41      41
#define T__42      42
#define T__43      43
#define T__44      44
#define CHARACTER_LITERAL      4
#define LETTER      5
#define LPAREN      6
#define NEWLINE      7
#define NON_ZERO_DECIMAL_NUMBER      8
#define RPAREN      9
#define ZERO_NUMBER      10
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
