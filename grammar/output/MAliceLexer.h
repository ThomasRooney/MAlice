/** \file
 *  This C header file was generated by $ANTLR version 3.4
 *
 *     -  From the grammar source file : /homes/anr11/CS2/Labs/MAlice/malice/grammar/MAlice.g
 *     -                            On : 2012-11-23 13:31:36
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
      pMAliceLexer->CHARACTER_LITERAL(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->ESCAPE(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->QUOTE(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->STRING_LITERAL(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->NUMBER_LITERAL(pMAliceLexer)
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
      pMAliceLexer->OPENED(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->CLOSED(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->SPIDER(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->BUT(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->THELOOKINGGLASS(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->THEROOM(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->SPOKE(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->WASA(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->BECAME(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->ALICEFOUND(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->BECAUSE(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->SAIDALICE(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->COMMA(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->THEN(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->AND(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->FULL_STOP(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->CONTAINEDA(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->EVENTUALLY(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->ENOUGHTIMES(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->ALICEWASUNSURE(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->OR(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->SO(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->PIECE(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->WHATWAS(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->OF(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->TOO(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->MAYBE(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->PERHAPS(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->EITHER(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->HAD(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->QUESTION_MARK(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->ATE(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->DRANK(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->APOSTROPHE_S(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->NUMBER_TYPE(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->LETTER_TYPE(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->SENTENCE_TYPE(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->IDENTIFIER(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->PLUS(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->MINUS(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->TILDE(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->BANG(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->MULTIPLY(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->DIVIDE(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->MODULO(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->EQUALS(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->NOTEQUAL(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->LESSTHAN(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->LESSTHANEQUAL(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->GREATERTHAN(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->GREATERTHANEQUAL(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->LOGICALAND(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->LOGICALOR(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->BITWISEAND(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->BITWISEOR(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->BITWISEXOR(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->UNDERSCORE(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->LETTER(pMAliceLexer)
 *  - 
 void
      pMAliceLexer->DIGIT(pMAliceLexer)
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
     (*mCHARACTER_LITERAL)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mESCAPE)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mQUOTE)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mSTRING_LITERAL)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mNUMBER_LITERAL)	(struct MAliceLexer_Ctx_struct * ctx);

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
     (*mOPENED)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mCLOSED)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mSPIDER)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mBUT)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mTHELOOKINGGLASS)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mTHEROOM)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mSPOKE)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mWASA)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mBECAME)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mALICEFOUND)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mBECAUSE)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mSAIDALICE)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mCOMMA)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mTHEN)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mAND)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mFULL_STOP)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mCONTAINEDA)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mEVENTUALLY)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mENOUGHTIMES)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mALICEWASUNSURE)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mOR)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mSO)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mPIECE)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mWHATWAS)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mOF)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mTOO)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mMAYBE)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mPERHAPS)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mEITHER)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mHAD)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mQUESTION_MARK)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mATE)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mDRANK)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mAPOSTROPHE_S)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mNUMBER_TYPE)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mLETTER_TYPE)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mSENTENCE_TYPE)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mIDENTIFIER)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mPLUS)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mMINUS)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mTILDE)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mBANG)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mMULTIPLY)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mDIVIDE)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mMODULO)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mEQUALS)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mNOTEQUAL)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mLESSTHAN)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mLESSTHANEQUAL)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mGREATERTHAN)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mGREATERTHANEQUAL)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mLOGICALAND)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mLOGICALOR)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mBITWISEAND)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mBITWISEOR)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mBITWISEXOR)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mUNDERSCORE)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mLETTER)	(struct MAliceLexer_Ctx_struct * ctx);

     void
     (*mDIGIT)	(struct MAliceLexer_Ctx_struct * ctx);

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
#define WASA      90
#define WHATWAS      91
#define WHILESTATEMENT      92
#define WS      93
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
