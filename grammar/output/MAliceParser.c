/** \file
 *  This C source file was generated by $ANTLR version 3.4
 *
 *     -  From the grammar source file : /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g
 *     -                            On : 2012-11-16 15:44:32
 *     -                for the parser : MAliceParserParser
 *
 * Editing it, at least manually, is not wise.
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
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

/* -----------------------------------------
 * Include the ANTLR3 generated header file.
 */
#include    "MAliceParser.h"
/* ----------------------------------------- */





/* MACROS that hide the C interface implementations from the
 * generated code, which makes it a little more understandable to the human eye.
 * I am very much against using C pre-processor macros for function calls and bits
 * of code as you cannot see what is happening when single stepping in debuggers
 * and so on. The exception (in my book at least) is for generated code, where you are
 * not maintaining it, but may wish to read and understand it. If you single step it, you know that input()
 * hides some indirect calls, but is always referring to the input stream. This is
 * probably more readable than ctx->input->istream->input(snarfle0->blarg) and allows me to rejig
 * the runtime interfaces without changing the generated code too often, without
 * confusing the reader of the generated output, who may not wish to know the gory
 * details of the interface inheritance.
 */

#define		CTX	ctx

/* Aids in accessing scopes for grammar programmers
 */
#undef	SCOPE_TYPE
#undef	SCOPE_STACK
#undef	SCOPE_TOP
#define	SCOPE_TYPE(scope)   pMAliceParser_##scope##_SCOPE
#define SCOPE_STACK(scope)  pMAliceParser_##scope##Stack
#define	SCOPE_TOP(scope)    ctx->pMAliceParser_##scope##Top
#define	SCOPE_SIZE(scope)		ctx->pMAliceParser_##scope##Stack_limit
#define SCOPE_INSTANCE(scope, i)	(ctx->SCOPE_STACK(scope)->get(ctx->SCOPE_STACK(scope),i))

/* Macros for accessing things in the parser
 */

#undef	    PARSER
#undef	    RECOGNIZER
#undef	    HAVEPARSEDRULE
#undef		MEMOIZE
#undef	    INPUT
#undef	    STRSTREAM
#undef	    HASEXCEPTION
#undef	    EXCEPTION
#undef	    MATCHT
#undef	    MATCHANYT
#undef	    FOLLOWSTACK
#undef	    FOLLOWPUSH
#undef	    FOLLOWPOP
#undef	    PRECOVER
#undef	    PREPORTERROR
#undef	    LA
#undef	    LT
#undef	    CONSTRUCTEX
#undef	    CONSUME
#undef	    MARK
#undef	    REWIND
#undef	    REWINDLAST
#undef	    PERRORRECOVERY
#undef	    HASFAILED
#undef	    FAILEDFLAG
#undef	    RECOVERFROMMISMATCHEDSET
#undef	    RECOVERFROMMISMATCHEDELEMENT
#undef		INDEX
#undef      ADAPTOR
#undef		SEEK
#undef	    RULEMEMO
#undef		DBG

#define	    PARSER				ctx->pParser
#define	    RECOGNIZER				PARSER->rec
#define	    PSRSTATE				RECOGNIZER->state
#define	    HAVEPARSEDRULE(r)			RECOGNIZER->alreadyParsedRule(RECOGNIZER, r)
#define	    MEMOIZE(ri,si)			RECOGNIZER->memoize(RECOGNIZER, ri, si)
#define	    INPUT				PARSER->tstream
#define	    STRSTREAM				INPUT
#define	    ISTREAM				INPUT->istream
#define	    INDEX()				ISTREAM->index(INPUT->istream)
#define	    HASEXCEPTION()			(PSRSTATE->error == ANTLR3_TRUE)
#define	    EXCEPTION				PSRSTATE->exception
#define	    MATCHT(t, fs)			RECOGNIZER->match(RECOGNIZER, t, fs)
#define	    MATCHANYT()				RECOGNIZER->matchAny(RECOGNIZER)
#define	    FOLLOWSTACK				PSRSTATE->following
#ifdef  SKIP_FOLLOW_SETS
#define	    FOLLOWPUSH(x)
#define	    FOLLOWPOP()
#else
#define	    FOLLOWPUSH(x)			FOLLOWSTACK->push(FOLLOWSTACK, ((void *)(&(x))), NULL)
#define	    FOLLOWPOP()				FOLLOWSTACK->pop(FOLLOWSTACK)
#endif
#define	    PRECOVER()				RECOGNIZER->recover(RECOGNIZER)
#define	    PREPORTERROR()			RECOGNIZER->reportError(RECOGNIZER)
#define	    LA(n)				INPUT->istream->_LA(ISTREAM, n)
#define	    LT(n)				INPUT->_LT(INPUT, n)
#define	    CONSTRUCTEX()			RECOGNIZER->exConstruct(RECOGNIZER)
#define	    CONSUME()				ISTREAM->consume(ISTREAM)
#define	    MARK()				ISTREAM->mark(ISTREAM)
#define	    REWIND(m)				ISTREAM->rewind(ISTREAM, m)
#define	    REWINDLAST()			ISTREAM->rewindLast(ISTREAM)
#define	    SEEK(n)				ISTREAM->seek(ISTREAM, n)
#define	    PERRORRECOVERY			PSRSTATE->errorRecovery
#define	    FAILEDFLAG				PSRSTATE->failed
#define	    HASFAILED()				(FAILEDFLAG == ANTLR3_TRUE)
#define	    BACKTRACKING			PSRSTATE->backtracking
#define	    RECOVERFROMMISMATCHEDSET(s)		RECOGNIZER->recoverFromMismatchedSet(RECOGNIZER, s)
#define	    RECOVERFROMMISMATCHEDELEMENT(e)	RECOGNIZER->recoverFromMismatchedElement(RECOGNIZER, s)
#define     ADAPTOR                         ctx->adaptor
#define		RULEMEMO						PSRSTATE->ruleMemo
#define		DBG								RECOGNIZER->debugger


#define		TOKTEXT(tok, txt)				tok, (pANTLR3_UINT8)txt

/* The 4 tokens defined below may well clash with your own #defines or token types. If so
 * then for the present you must use different names for your defines as these are hard coded
 * in the code generator. It would be better not to use such names internally, and maybe
 * we can change this in a forthcoming release. I deliberately do not #undef these
 * here as this will at least give you a redefined error somewhere if they clash.
 */
#define	    UP	    ANTLR3_TOKEN_UP
#define	    DOWN    ANTLR3_TOKEN_DOWN
#define	    EOR	    ANTLR3_TOKEN_EOR
#define	    INVALID ANTLR3_TOKEN_INVALID


/* =============================================================================
 * Functions to create and destroy scopes. First come the rule scopes, followed
 * by the global declared scopes.
 */



/* ============================================================================= */

/* =============================================================================
 * Start of recognizer
 */


/** \brief Table of all token names in symbolic order, mainly used for
 *         error reporting.
 */
pANTLR3_UINT8   MAliceParserTokenNames[22+4]
     = {
        (pANTLR3_UINT8) "<invalid>",       /* String to print to indicate an invalid token */
        (pANTLR3_UINT8) "<EOR>",
        (pANTLR3_UINT8) "<DOWN>",
        (pANTLR3_UINT8) "<UP>",
        (pANTLR3_UINT8) "CHARACTER_LITERAL",
        (pANTLR3_UINT8) "DECIMAL_NUMBER",
        (pANTLR3_UINT8) "LETTER",
        (pANTLR3_UINT8) "NEWLINE",
        (pANTLR3_UINT8) "'('",
        (pANTLR3_UINT8) "')'",
        (pANTLR3_UINT8) "','",
        (pANTLR3_UINT8) "'.'",
        (pANTLR3_UINT8) "'0'",
        (pANTLR3_UINT8) "'9'",
        (pANTLR3_UINT8) "'The'",
        (pANTLR3_UINT8) "'and'",
        (pANTLR3_UINT8) "'but'",
        (pANTLR3_UINT8) "'closed'",
        (pANTLR3_UINT8) "'contained a'",
        (pANTLR3_UINT8) "'letter'",
        (pANTLR3_UINT8) "'looking-glass'",
        (pANTLR3_UINT8) "'number'",
        (pANTLR3_UINT8) "'opened'",
        (pANTLR3_UINT8) "'room'",
        (pANTLR3_UINT8) "'then'",
        (pANTLR3_UINT8) "'~'"
       };



// Forward declare the locally static matching functions we have generated.
//
static 
 void
	identifier    (pMAliceParser ctx);
static 
 void
	split    (pMAliceParser ctx);
static 
 void
	type    (pMAliceParser ctx);
static 
 void
	unary_operator    (pMAliceParser ctx);
static 
 void
	constant    (pMAliceParser ctx);
static 
 void
	program    (pMAliceParser ctx);
static 
 void
	function    (pMAliceParser ctx);
static 
 void
	function_type    (pMAliceParser ctx);
static 
 void
	function_argument_list    (pMAliceParser ctx);
static 
 void
	function_argument    (pMAliceParser ctx);
static 
 void
	return_type_clause    (pMAliceParser ctx);
static 
 void
	block_unit    (pMAliceParser ctx);
static void	MAliceParserFree(pMAliceParser ctx);
static void     MAliceParserReset (pMAliceParser ctx);

/* For use in tree output where we are accumulating rule labels via label += ruleRef
 * we need a function that knows how to free a return scope when the list is destroyed.
 * We cannot just use ANTLR3_FREE because in debug tracking mode, this is a macro.
 */
static	void ANTLR3_CDECL freeScope(void * scope)
{
    ANTLR3_FREE(scope);
}

/** \brief Name of the grammar file that generated this code
 */
static const char fileName[] = "/Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g";

/** \brief Return the name of the grammar file that generated this code.
 */
static const char * getGrammarFileName()
{
	return fileName;
}
/** \brief Create a new MAliceParser parser and return a context for it.
 *
 * \param[in] instream Pointer to an input stream interface.
 *
 * \return Pointer to new parser context upon success.
 */
ANTLR3_API pMAliceParser
MAliceParserNew   (pANTLR3_COMMON_TOKEN_STREAM instream)
{
	// See if we can create a new parser with the standard constructor
	//
	return MAliceParserNewSSD(instream, NULL);
}

/** \brief Create a new MAliceParser parser and return a context for it.
 *
 * \param[in] instream Pointer to an input stream interface.
 *
 * \return Pointer to new parser context upon success.
 */
ANTLR3_API pMAliceParser
MAliceParserNewSSD   (pANTLR3_COMMON_TOKEN_STREAM instream, pANTLR3_RECOGNIZER_SHARED_STATE state)
{
    pMAliceParser ctx;	    /* Context structure we will build and return   */

    ctx	= (pMAliceParser) ANTLR3_CALLOC(1, sizeof(MAliceParser));

    if	(ctx == NULL)
    {
		// Failed to allocate memory for parser context
		//
        return  NULL;
    }

    /* -------------------------------------------------------------------
     * Memory for basic structure is allocated, now to fill in
     * the base ANTLR3 structures. We initialize the function pointers
     * for the standard ANTLR3 parser function set, but upon return
     * from here, the programmer may set the pointers to provide custom
     * implementations of each function.
     *
     * We don't use the macros defined in MAliceParser.h here, in order that you can get a sense
     * of what goes where.
     */

    /* Create a base parser/recognizer, using the supplied token stream
     */
    ctx->pParser	    = antlr3ParserNewStream(ANTLR3_SIZE_HINT, instream->tstream, state);
    /* Install the implementation of our MAliceParser interface
     */
    ctx->identifier	= identifier;
    ctx->split	= split;
    ctx->type	= type;
    ctx->unary_operator	= unary_operator;
    ctx->constant	= constant;
    ctx->program	= program;
    ctx->function	= function;
    ctx->function_type	= function_type;
    ctx->function_argument_list	= function_argument_list;
    ctx->function_argument	= function_argument;
    ctx->return_type_clause	= return_type_clause;
    ctx->block_unit	= block_unit;
    ctx->free			= MAliceParserFree;
    ctx->reset			= MAliceParserReset;
    ctx->getGrammarFileName	= getGrammarFileName;

    /* Install the scope pushing methods.
     */

    /* Install the token table
     */
    PSRSTATE->tokenNames   = MAliceParserTokenNames;


    /* Return the newly built parser to the caller
     */
    return  ctx;
}

static void
MAliceParserReset (pMAliceParser ctx)
{
    RECOGNIZER->reset(RECOGNIZER);
}

/** Free the parser resources
 */
 static void
 MAliceParserFree(pMAliceParser ctx)
 {
    /* Free any scope memory
     */

	// Free this parser
	//
    ctx->pParser->free(ctx->pParser);


    ANTLR3_FREE(ctx);

    /* Everything is released, so we can return
     */
    return;
 }

/** Return token names used by this 
parser

 *
 * The returned pointer is used as an index into the token names table (using the token
 * number as the index).
 *
 * \return Pointer to first char * in the table.
 */
static pANTLR3_UINT8    *getTokenNames()
{
        return MAliceParserTokenNames;
}


/* Declare the bitsets
 */
/** Bitset defining follow set for error recovery in rule state: FOLLOW_LETTER_in_identifier27  */
static	ANTLR3_BITWORD FOLLOW_LETTER_in_identifier27_bits[]	= { ANTLR3_UINT64_LIT(0x0000000000000042) };
static  ANTLR3_BITSET_LIST FOLLOW_LETTER_in_identifier27	= { FOLLOW_LETTER_in_identifier27_bits, 1	};
/** Bitset defining follow set for error recovery in rule state: FOLLOW_25_in_unary_operator123  */
static	ANTLR3_BITWORD FOLLOW_25_in_unary_operator123_bits[]	= { ANTLR3_UINT64_LIT(0x0000000000000002) };
static  ANTLR3_BITSET_LIST FOLLOW_25_in_unary_operator123	= { FOLLOW_25_in_unary_operator123_bits, 1	};
/** Bitset defining follow set for error recovery in rule state: FOLLOW_function_in_program151  */
static	ANTLR3_BITWORD FOLLOW_function_in_program151_bits[]	= { ANTLR3_UINT64_LIT(0x0000000000004002) };
static  ANTLR3_BITSET_LIST FOLLOW_function_in_program151	= { FOLLOW_function_in_program151_bits, 1	};
/** Bitset defining follow set for error recovery in rule state: FOLLOW_14_in_function159  */
static	ANTLR3_BITWORD FOLLOW_14_in_function159_bits[]	= { ANTLR3_UINT64_LIT(0x0000000000900000) };
static  ANTLR3_BITSET_LIST FOLLOW_14_in_function159	= { FOLLOW_14_in_function159_bits, 1	};
/** Bitset defining follow set for error recovery in rule state: FOLLOW_function_type_in_function161  */
static	ANTLR3_BITWORD FOLLOW_function_type_in_function161_bits[]	= { ANTLR3_UINT64_LIT(0x0000000000000040) };
static  ANTLR3_BITSET_LIST FOLLOW_function_type_in_function161	= { FOLLOW_function_type_in_function161_bits, 1	};
/** Bitset defining follow set for error recovery in rule state: FOLLOW_identifier_in_function163  */
static	ANTLR3_BITWORD FOLLOW_identifier_in_function163_bits[]	= { ANTLR3_UINT64_LIT(0x0000000000000100) };
static  ANTLR3_BITSET_LIST FOLLOW_identifier_in_function163	= { FOLLOW_identifier_in_function163_bits, 1	};
/** Bitset defining follow set for error recovery in rule state: FOLLOW_8_in_function165  */
static	ANTLR3_BITWORD FOLLOW_8_in_function165_bits[]	= { ANTLR3_UINT64_LIT(0x0000000000280200) };
static  ANTLR3_BITSET_LIST FOLLOW_8_in_function165	= { FOLLOW_8_in_function165_bits, 1	};
/** Bitset defining follow set for error recovery in rule state: FOLLOW_function_argument_list_in_function167  */
static	ANTLR3_BITWORD FOLLOW_function_argument_list_in_function167_bits[]	= { ANTLR3_UINT64_LIT(0x0000000000000200) };
static  ANTLR3_BITSET_LIST FOLLOW_function_argument_list_in_function167	= { FOLLOW_function_argument_list_in_function167_bits, 1	};
/** Bitset defining follow set for error recovery in rule state: FOLLOW_9_in_function169  */
static	ANTLR3_BITWORD FOLLOW_9_in_function169_bits[]	= { ANTLR3_UINT64_LIT(0x0000000000440000) };
static  ANTLR3_BITSET_LIST FOLLOW_9_in_function169	= { FOLLOW_9_in_function169_bits, 1	};
/** Bitset defining follow set for error recovery in rule state: FOLLOW_return_type_clause_in_function171  */
static	ANTLR3_BITWORD FOLLOW_return_type_clause_in_function171_bits[]	= { ANTLR3_UINT64_LIT(0x0000000000400000) };
static  ANTLR3_BITSET_LIST FOLLOW_return_type_clause_in_function171	= { FOLLOW_return_type_clause_in_function171_bits, 1	};
/** Bitset defining follow set for error recovery in rule state: FOLLOW_block_unit_in_function174  */
static	ANTLR3_BITWORD FOLLOW_block_unit_in_function174_bits[]	= { ANTLR3_UINT64_LIT(0x0000000000000002) };
static  ANTLR3_BITSET_LIST FOLLOW_block_unit_in_function174	= { FOLLOW_block_unit_in_function174_bits, 1	};
/** Bitset defining follow set for error recovery in rule state: FOLLOW_function_argument_in_function_argument_list198  */
static	ANTLR3_BITWORD FOLLOW_function_argument_in_function_argument_list198_bits[]	= { ANTLR3_UINT64_LIT(0x0000000000280002) };
static  ANTLR3_BITSET_LIST FOLLOW_function_argument_in_function_argument_list198	= { FOLLOW_function_argument_in_function_argument_list198_bits, 1	};
/** Bitset defining follow set for error recovery in rule state: FOLLOW_type_in_function_argument209  */
static	ANTLR3_BITWORD FOLLOW_type_in_function_argument209_bits[]	= { ANTLR3_UINT64_LIT(0x0000000000000040) };
static  ANTLR3_BITSET_LIST FOLLOW_type_in_function_argument209	= { FOLLOW_type_in_function_argument209_bits, 1	};
/** Bitset defining follow set for error recovery in rule state: FOLLOW_identifier_in_function_argument211  */
static	ANTLR3_BITWORD FOLLOW_identifier_in_function_argument211_bits[]	= { ANTLR3_UINT64_LIT(0x0000000000000002) };
static  ANTLR3_BITSET_LIST FOLLOW_identifier_in_function_argument211	= { FOLLOW_identifier_in_function_argument211_bits, 1	};
/** Bitset defining follow set for error recovery in rule state: FOLLOW_18_in_return_type_clause221  */
static	ANTLR3_BITWORD FOLLOW_18_in_return_type_clause221_bits[]	= { ANTLR3_UINT64_LIT(0x0000000000280000) };
static  ANTLR3_BITSET_LIST FOLLOW_18_in_return_type_clause221	= { FOLLOW_18_in_return_type_clause221_bits, 1	};
/** Bitset defining follow set for error recovery in rule state: FOLLOW_type_in_return_type_clause223  */
static	ANTLR3_BITWORD FOLLOW_type_in_return_type_clause223_bits[]	= { ANTLR3_UINT64_LIT(0x0000000000000002) };
static  ANTLR3_BITSET_LIST FOLLOW_type_in_return_type_clause223	= { FOLLOW_type_in_return_type_clause223_bits, 1	};
/** Bitset defining follow set for error recovery in rule state: FOLLOW_22_in_block_unit233  */
static	ANTLR3_BITWORD FOLLOW_22_in_block_unit233_bits[]	= { ANTLR3_UINT64_LIT(0x0000000000020000) };
static  ANTLR3_BITSET_LIST FOLLOW_22_in_block_unit233	= { FOLLOW_22_in_block_unit233_bits, 1	};
/** Bitset defining follow set for error recovery in rule state: FOLLOW_17_in_block_unit235  */
static	ANTLR3_BITWORD FOLLOW_17_in_block_unit235_bits[]	= { ANTLR3_UINT64_LIT(0x0000000000000002) };
static  ANTLR3_BITSET_LIST FOLLOW_17_in_block_unit235	= { FOLLOW_17_in_block_unit235_bits, 1	};


/* ==============================================
 * Parsing rules
 */
/**
 * $ANTLR start identifier
 * /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:46:1: identifier : LETTER ( LETTER | '0' .. '9' )* ;
 */
static void
identifier(pMAliceParser ctx)
{
    /* Initialize rule variables
     */

    {
        // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:47:2: ( LETTER ( LETTER | '0' .. '9' )* )
        // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:47:4: LETTER ( LETTER | '0' .. '9' )*
        {
             MATCHT(LETTER, &FOLLOW_LETTER_in_identifier27);
            if  (HASEXCEPTION())
            {
                goto ruleidentifierEx;
            }


            // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:47:11: ( LETTER | '0' .. '9' )*

            for (;;)
            {
                int alt1=2;
                switch ( LA(1) )
                {
                case LETTER:
                	{
                		alt1=1;
                	}
                    break;

                }

                switch (alt1)
                {
            	case 1:
            	    // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:
            	    {
            	        if ( LA(1) == LETTER )
            	        {
            	            CONSUME();
            	            PERRORRECOVERY=ANTLR3_FALSE;
            	        }
            	        else
            	        {
            	            CONSTRUCTEX();
            	            EXCEPTION->type         = ANTLR3_MISMATCHED_SET_EXCEPTION;
            	            EXCEPTION->name         = (void *)ANTLR3_MISMATCHED_SET_NAME;
            	            EXCEPTION->expectingSet = NULL;

            	            goto ruleidentifierEx;
            	        }


            	    }
            	    break;

            	default:
            	    goto loop1;	/* break out of the loop */
            	    break;
                }
            }
            loop1: ; /* Jump out to here if this rule does not match */


        }

    }

    // This is where rules clean up and exit
    //
    goto ruleidentifierEx; /* Prevent compiler warnings */
    ruleidentifierEx: ;

            if (HASEXCEPTION())
            {
                PREPORTERROR();
                PRECOVER();
            }
    return ;
}
/* $ANTLR end identifier */

/**
 * $ANTLR start split
 * /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:67:1: split : ( '.' | ',' | 'then' | 'and' | 'but' ) ;
 */
static void
split(pMAliceParser ctx)
{
    /* Initialize rule variables
     */

    {
        // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:67:8: ( ( '.' | ',' | 'then' | 'and' | 'but' ) )
        // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:
        {
            if ( ((LA(1) >= 10) && (LA(1) <= 11)) || ((LA(1) >= 15) && (LA(1) <= 16)) || LA(1) == 24 )
            {
                CONSUME();
                PERRORRECOVERY=ANTLR3_FALSE;
            }
            else
            {
                CONSTRUCTEX();
                EXCEPTION->type         = ANTLR3_MISMATCHED_SET_EXCEPTION;
                EXCEPTION->name         = (void *)ANTLR3_MISMATCHED_SET_NAME;
                EXCEPTION->expectingSet = NULL;

                goto rulesplitEx;
            }


        }

    }

    // This is where rules clean up and exit
    //
    goto rulesplitEx; /* Prevent compiler warnings */
    rulesplitEx: ;

            if (HASEXCEPTION())
            {
                PREPORTERROR();
                PRECOVER();
            }
    return ;
}
/* $ANTLR end split */

/**
 * $ANTLR start type
 * /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:77:1: type : ( 'number' | 'letter' );
 */
static void
type(pMAliceParser ctx)
{
    /* Initialize rule variables
     */

    {
        // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:77:7: ( 'number' | 'letter' )
        // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:
        {
            if ( LA(1) == 19 || LA(1) == 21 )
            {
                CONSUME();
                PERRORRECOVERY=ANTLR3_FALSE;
            }
            else
            {
                CONSTRUCTEX();
                EXCEPTION->type         = ANTLR3_MISMATCHED_SET_EXCEPTION;
                EXCEPTION->name         = (void *)ANTLR3_MISMATCHED_SET_NAME;
                EXCEPTION->expectingSet = NULL;

                goto ruletypeEx;
            }


        }

    }

    // This is where rules clean up and exit
    //
    goto ruletypeEx; /* Prevent compiler warnings */
    ruletypeEx: ;

            if (HASEXCEPTION())
            {
                PREPORTERROR();
                PRECOVER();
            }
    return ;
}
/* $ANTLR end type */

/**
 * $ANTLR start unary_operator
 * /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:81:1: unary_operator : '~' ;
 */
static void
unary_operator(pMAliceParser ctx)
{
    /* Initialize rule variables
     */

    {
        // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:82:2: ( '~' )
        // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:82:4: '~'
        {
             MATCHT(25, &FOLLOW_25_in_unary_operator123);
            if  (HASEXCEPTION())
            {
                goto ruleunary_operatorEx;
            }


        }

    }

    // This is where rules clean up and exit
    //
    goto ruleunary_operatorEx; /* Prevent compiler warnings */
    ruleunary_operatorEx: ;

            if (HASEXCEPTION())
            {
                PREPORTERROR();
                PRECOVER();
            }
    return ;
}
/* $ANTLR end unary_operator */

/**
 * $ANTLR start constant
 * /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:85:1: constant : ( DECIMAL_NUMBER | CHARACTER_LITERAL );
 */
static void
constant(pMAliceParser ctx)
{
    /* Initialize rule variables
     */

    {
        // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:85:9: ( DECIMAL_NUMBER | CHARACTER_LITERAL )
        // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:
        {
            if ( ((LA(1) >= CHARACTER_LITERAL) && (LA(1) <= DECIMAL_NUMBER)) )
            {
                CONSUME();
                PERRORRECOVERY=ANTLR3_FALSE;
            }
            else
            {
                CONSTRUCTEX();
                EXCEPTION->type         = ANTLR3_MISMATCHED_SET_EXCEPTION;
                EXCEPTION->name         = (void *)ANTLR3_MISMATCHED_SET_NAME;
                EXCEPTION->expectingSet = NULL;

                goto ruleconstantEx;
            }


        }

    }

    // This is where rules clean up and exit
    //
    goto ruleconstantEx; /* Prevent compiler warnings */
    ruleconstantEx: ;

            if (HASEXCEPTION())
            {
                PREPORTERROR();
                PRECOVER();
            }
    return ;
}
/* $ANTLR end constant */

/**
 * $ANTLR start program
 * /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:92:1: program : ( function )+ ;
 */
static void
program(pMAliceParser ctx)
{
    /* Initialize rule variables
     */

    {
        // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:92:9: ( ( function )+ )
        // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:92:11: ( function )+
        {
            // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:92:11: ( function )+
            {
                int cnt2=0;

                for (;;)
                {
                    int alt2=2;
            	switch ( LA(1) )
            	{
            	case 14:
            		{
            			alt2=1;
            		}
            	    break;

            	}

            	switch (alt2)
            	{
            	    case 1:
            	        // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:92:11: function
            	        {
            	            FOLLOWPUSH(FOLLOW_function_in_program151);
            	            function(ctx);

            	            FOLLOWPOP();
            	            if  (HASEXCEPTION())
            	            {
            	                goto ruleprogramEx;
            	            }


            	        }
            	        break;

            	    default:

            		if ( cnt2 >= 1 )
            		{
            		    goto loop2;
            		}
            		/* mismatchedSetEx()
            		 */
            		CONSTRUCTEX();
            		EXCEPTION->type = ANTLR3_EARLY_EXIT_EXCEPTION;
            		EXCEPTION->name = (void *)ANTLR3_EARLY_EXIT_NAME;


            		goto ruleprogramEx;
            	}
            	cnt2++;
                }
                loop2: ;	/* Jump to here if this rule does not match */
            }

        }

    }

    // This is where rules clean up and exit
    //
    goto ruleprogramEx; /* Prevent compiler warnings */
    ruleprogramEx: ;

            if (HASEXCEPTION())
            {
                PREPORTERROR();
                PRECOVER();
            }
    return ;
}
/* $ANTLR end program */

/**
 * $ANTLR start function
 * /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:94:1: function : 'The' function_type identifier '(' function_argument_list ')' ( return_type_clause )? block_unit ;
 */
static void
function(pMAliceParser ctx)
{
    /* Initialize rule variables
     */

    {
        // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:94:9: ( 'The' function_type identifier '(' function_argument_list ')' ( return_type_clause )? block_unit )
        // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:94:11: 'The' function_type identifier '(' function_argument_list ')' ( return_type_clause )? block_unit
        {
             MATCHT(14, &FOLLOW_14_in_function159);
            if  (HASEXCEPTION())
            {
                goto rulefunctionEx;
            }


            FOLLOWPUSH(FOLLOW_function_type_in_function161);
            function_type(ctx);

            FOLLOWPOP();
            if  (HASEXCEPTION())
            {
                goto rulefunctionEx;
            }


            FOLLOWPUSH(FOLLOW_identifier_in_function163);
            identifier(ctx);

            FOLLOWPOP();
            if  (HASEXCEPTION())
            {
                goto rulefunctionEx;
            }


             MATCHT(8, &FOLLOW_8_in_function165);
            if  (HASEXCEPTION())
            {
                goto rulefunctionEx;
            }


            FOLLOWPUSH(FOLLOW_function_argument_list_in_function167);
            function_argument_list(ctx);

            FOLLOWPOP();
            if  (HASEXCEPTION())
            {
                goto rulefunctionEx;
            }


             MATCHT(9, &FOLLOW_9_in_function169);
            if  (HASEXCEPTION())
            {
                goto rulefunctionEx;
            }


            // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:94:73: ( return_type_clause )?
            {
                int alt3=2;
                switch ( LA(1) )
                {
                    case 18:
                    	{
                    		alt3=1;
                    	}
                        break;
                }

                switch (alt3)
                {
            	case 1:
            	    // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:94:73: return_type_clause
            	    {
            	        FOLLOWPUSH(FOLLOW_return_type_clause_in_function171);
            	        return_type_clause(ctx);

            	        FOLLOWPOP();
            	        if  (HASEXCEPTION())
            	        {
            	            goto rulefunctionEx;
            	        }


            	    }
            	    break;

                }
            }

            FOLLOWPUSH(FOLLOW_block_unit_in_function174);
            block_unit(ctx);

            FOLLOWPOP();
            if  (HASEXCEPTION())
            {
                goto rulefunctionEx;
            }


        }

    }

    // This is where rules clean up and exit
    //
    goto rulefunctionEx; /* Prevent compiler warnings */
    rulefunctionEx: ;

            if (HASEXCEPTION())
            {
                PREPORTERROR();
                PRECOVER();
            }
    return ;
}
/* $ANTLR end function */

/**
 * $ANTLR start function_type
 * /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:95:1: function_type : ( 'room' | 'looking-glass' );
 */
static void
function_type(pMAliceParser ctx)
{
    /* Initialize rule variables
     */

    {
        // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:96:2: ( 'room' | 'looking-glass' )
        // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:
        {
            if ( LA(1) == 20 || LA(1) == 23 )
            {
                CONSUME();
                PERRORRECOVERY=ANTLR3_FALSE;
            }
            else
            {
                CONSTRUCTEX();
                EXCEPTION->type         = ANTLR3_MISMATCHED_SET_EXCEPTION;
                EXCEPTION->name         = (void *)ANTLR3_MISMATCHED_SET_NAME;
                EXCEPTION->expectingSet = NULL;

                goto rulefunction_typeEx;
            }


        }

    }

    // This is where rules clean up and exit
    //
    goto rulefunction_typeEx; /* Prevent compiler warnings */
    rulefunction_typeEx: ;

            if (HASEXCEPTION())
            {
                PREPORTERROR();
                PRECOVER();
            }
    return ;
}
/* $ANTLR end function_type */

/**
 * $ANTLR start function_argument_list
 * /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:99:1: function_argument_list : ( function_argument )* ;
 */
static void
function_argument_list(pMAliceParser ctx)
{
    /* Initialize rule variables
     */

    {
        // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:100:2: ( ( function_argument )* )
        // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:100:4: ( function_argument )*
        {
            // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:100:4: ( function_argument )*

            for (;;)
            {
                int alt4=2;
                switch ( LA(1) )
                {
                case 19:
                case 21:
                	{
                		alt4=1;
                	}
                    break;

                }

                switch (alt4)
                {
            	case 1:
            	    // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:100:4: function_argument
            	    {
            	        FOLLOWPUSH(FOLLOW_function_argument_in_function_argument_list198);
            	        function_argument(ctx);

            	        FOLLOWPOP();
            	        if  (HASEXCEPTION())
            	        {
            	            goto rulefunction_argument_listEx;
            	        }


            	    }
            	    break;

            	default:
            	    goto loop4;	/* break out of the loop */
            	    break;
                }
            }
            loop4: ; /* Jump out to here if this rule does not match */


        }

    }

    // This is where rules clean up and exit
    //
    goto rulefunction_argument_listEx; /* Prevent compiler warnings */
    rulefunction_argument_listEx: ;

            if (HASEXCEPTION())
            {
                PREPORTERROR();
                PRECOVER();
            }
    return ;
}
/* $ANTLR end function_argument_list */

/**
 * $ANTLR start function_argument
 * /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:102:1: function_argument : type identifier ;
 */
static void
function_argument(pMAliceParser ctx)
{
    /* Initialize rule variables
     */

    {
        // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:103:2: ( type identifier )
        // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:103:4: type identifier
        {
            FOLLOWPUSH(FOLLOW_type_in_function_argument209);
            type(ctx);

            FOLLOWPOP();
            if  (HASEXCEPTION())
            {
                goto rulefunction_argumentEx;
            }


            FOLLOWPUSH(FOLLOW_identifier_in_function_argument211);
            identifier(ctx);

            FOLLOWPOP();
            if  (HASEXCEPTION())
            {
                goto rulefunction_argumentEx;
            }


        }

    }

    // This is where rules clean up and exit
    //
    goto rulefunction_argumentEx; /* Prevent compiler warnings */
    rulefunction_argumentEx: ;

            if (HASEXCEPTION())
            {
                PREPORTERROR();
                PRECOVER();
            }
    return ;
}
/* $ANTLR end function_argument */

/**
 * $ANTLR start return_type_clause
 * /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:105:1: return_type_clause : 'contained a' type ;
 */
static void
return_type_clause(pMAliceParser ctx)
{
    /* Initialize rule variables
     */

    {
        // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:106:2: ( 'contained a' type )
        // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:106:4: 'contained a' type
        {
             MATCHT(18, &FOLLOW_18_in_return_type_clause221);
            if  (HASEXCEPTION())
            {
                goto rulereturn_type_clauseEx;
            }


            FOLLOWPUSH(FOLLOW_type_in_return_type_clause223);
            type(ctx);

            FOLLOWPOP();
            if  (HASEXCEPTION())
            {
                goto rulereturn_type_clauseEx;
            }


        }

    }

    // This is where rules clean up and exit
    //
    goto rulereturn_type_clauseEx; /* Prevent compiler warnings */
    rulereturn_type_clauseEx: ;

            if (HASEXCEPTION())
            {
                PREPORTERROR();
                PRECOVER();
            }
    return ;
}
/* $ANTLR end return_type_clause */

/**
 * $ANTLR start block_unit
 * /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:108:1: block_unit : 'opened' 'closed' ;
 */
static void
block_unit(pMAliceParser ctx)
{
    /* Initialize rule variables
     */

    {
        // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:109:2: ( 'opened' 'closed' )
        // /Users/alexrozanski/Imperial/CS2/Labs/MAlice/malice/grammar/MAlice.g:109:4: 'opened' 'closed'
        {
             MATCHT(22, &FOLLOW_22_in_block_unit233);
            if  (HASEXCEPTION())
            {
                goto ruleblock_unitEx;
            }


             MATCHT(17, &FOLLOW_17_in_block_unit235);
            if  (HASEXCEPTION())
            {
                goto ruleblock_unitEx;
            }


        }

    }

    // This is where rules clean up and exit
    //
    goto ruleblock_unitEx; /* Prevent compiler warnings */
    ruleblock_unitEx: ;

            if (HASEXCEPTION())
            {
                PREPORTERROR();
                PRECOVER();
            }
    return ;
}
/* $ANTLR end block_unit */
/* End of parsing rules
 * ==============================================
 */

/* ==============================================
 * Syntactic predicates
 */
/* End of syntactic predicates
 * ==============================================
 */






/* End of code
 * =============================================================================
 */
