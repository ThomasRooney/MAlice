/** \file
 *  This C header file was generated by $ANTLR version 3.4
 *
 *     -  From the grammar source file : /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g
 *     -                            On : 2012-11-17 15:12:41
 *     -                 for the lexer : MAliceLexerLexer
 *
 * Editing it, at least manually, is not wise.
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
 * The lexer 
MAliceLexer

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
 * a parser context typedef pMAliceLexer, which is returned from a call to MAliceLexerNew().
 *
 * As this is a generated lexer, it is unlikely you will call it 'manually'. However
 * the methods are provided anyway.
 *
 * The methods in pMAliceLexer are  as follows:
 *
 *  - 
 void
      pMAliceLexer->T__14(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->T__15(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->T__16(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->T__17(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->T__18(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->T__19(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->T__20(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->T__21(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->T__22(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->T__23(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->T__24(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->T__25(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->T__26(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->T__27(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->T__28(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->T__29(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->T__30(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->T__31(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->T__32(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->T__33(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->T__34(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->T__35(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->T__36(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->T__37(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->T__38(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->T__39(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->T__40(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->T__41(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->T__42(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->T__43(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->T__44(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->LETTER(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->IDENTIFIER(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->CHARACTER_LITERAL(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->ZERO_NUMBER(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->NON_ZERO_DECIMAL_NUMBER(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->WS(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->COMMENT(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->NEWLINE(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->LPAREN(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->RPAREN(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->Tokens(pMAliceLexer)
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

#ifndef	_MAliceLexer_H
#define _MAliceLexer_H
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
typedef struct MAliceLexer_Ctx_struct MAliceLexer, * pMAliceLexer;



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
MAliceLexer

 */
struct MAliceLexer_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_LEXER    pLexer;

     void
     (*mT__14)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mT__15)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mT__16)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mT__17)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mT__18)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mT__19)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mT__20)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mT__21)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mT__22)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mT__23)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mT__24)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mT__25)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mT__26)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mT__27)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mT__28)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mT__29)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mT__30)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mT__31)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mT__32)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mT__33)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mT__34)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mT__35)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mT__36)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mT__37)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mT__38)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mT__39)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mT__40)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mT__41)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mT__42)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mT__43)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mT__44)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mLETTER)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mIDENTIFIER)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mCHARACTER_LITERAL)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mZERO_NUMBER)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mNON_ZERO_DECIMAL_NUMBER)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mWS)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mCOMMENT)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mNEWLINE)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mLPAREN)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mRPAREN)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mTokens)	(struct MAliceLexer_Ctx_struct * ctx);
    const char * (*getGrammarFileName)();
    void            (*reset)  (struct MAliceLexer_Ctx_struct * ctx);
    void	    (*free)   (struct MAliceLexer_Ctx_struct * ctx);
};

// Function protoypes for the constructor functions that external translation units
// such as delegators and delegates may wish to call.
//
ANTLR3_API pMAliceLexer MAliceLexerNew         (
pANTLR3_INPUT_STREAM
 instream);
ANTLR3_API pMAliceLexer MAliceLexerNewSSD      (
pANTLR3_INPUT_STREAM
 instream, pANTLR3_RECOGNIZER_SHARED_STATE state);

/** Symbolic definitions of all the tokens that the 
lexer
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
#define COMMENT      5
#define IDENTIFIER      6
#define LETTER      7
#define LPAREN      8
#define NEWLINE      9
#define NON_ZERO_DECIMAL_NUMBER      10
#define RPAREN      11
#define WS      12
#define ZERO_NUMBER      13
#ifdef	EOF
#undef	EOF
#define	EOF	ANTLR3_TOKEN_EOF
#endif

#ifndef TOKENSOURCE
#define TOKENSOURCE(lxr) lxr->pLexer->rec->state->tokSource
#endif

/* End of token definitions for MAliceLexer
 * =============================================================================
 */
/** } */

#ifdef __cplusplus
}
#endif

#endif

/* END - Note:Keep extra line feed to satisfy UNIX systems */
