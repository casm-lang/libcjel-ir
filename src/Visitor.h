//  
//  Copyright (c) 2015 Philipp Paulweber
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/ppaulweber/libnovel
//  
//  Permission is hereby granted, free of charge, to any person obtaining a 
//  copy of this software and associated documentation files (the "Software"), 
//  to deal with the Software without restriction, including without limitation 
//  the rights to use, copy, modify, merge, publish, distribute, sublicense, 
//  and/or sell copies of the Software, and to permit persons to whom the 
//  Software is furnished to do so, subject to the following conditions:
//  
//  * Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimers.
//  
//  * Redistributions in binary form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimers in the 
//    documentation and/or other materials provided with the distribution.
//  
//  * Neither the names of the copyright holders, nor the names of its 
//    contributors may be used to endorse or promote products derived from 
//    this Software without specific prior written permission.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
//  CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
//  WITH THE SOFTWARE.
//  

#ifndef _LIB_NOVEL_VISITOR_H_
#define _LIB_NOVEL_VISITOR_H_

//#include "Novel.h"

#include "Instruction.h"
#include "Scope.h"
#include "Module.h"
#include "Component.h"
#include "Function.h"
#include "Reference.h"
#include "Variable.h"
#include "Memory.h"
#include "Constant.h"


namespace libnovel
{
	enum class Traversal
	{ PREORDER
	, POSTORDER
    };
	
	class Visitor //: public Novel
	{
    public:
		enum class Stage
		{ PROLOG
		, INTERLOG
		, EPILOG
		};
		
		virtual void dispatch( Stage stage, Value* value ) final;
	    
#define LIB_NOVEL_VISITOR_INTERFACE_( PREFIX, POSTFIX )					\
		PREFIX void visit_prolog( Module& value ) POSTFIX;				\
		PREFIX void visit_epilog( Module& value ) POSTFIX;				\
																		\
		PREFIX void visit_prolog( Component& value ) POSTFIX;			\
		PREFIX void visit_interlog( Component& value ) POSTFIX;			\
		PREFIX void visit_epilog( Component& value ) POSTFIX;			\
																		\
		PREFIX void visit_prolog( Function& value ) POSTFIX;			\
		PREFIX void visit_interlog( Function& value ) POSTFIX;			\
		PREFIX void visit_epilog( Function& value ) POSTFIX;			\
																		\
		PREFIX void visit_prolog( Reference& value ) POSTFIX;			\
		PREFIX void visit_epilog( Reference& value ) POSTFIX;			\
																		\
		PREFIX void visit_prolog( Structure& value ) POSTFIX;			\
		PREFIX void visit_epilog( Structure& value ) POSTFIX;			\
																		\
		PREFIX void visit_prolog( Variable& value ) POSTFIX;			\
		PREFIX void visit_epilog( Variable& value ) POSTFIX;			\
																		\
		PREFIX void visit_prolog( Memory& value ) POSTFIX;				\
		PREFIX void visit_epilog( Memory& value ) POSTFIX;				\
																		\
		PREFIX void visit_prolog( ParallelScope& value ) POSTFIX;		\
		PREFIX void visit_epilog( ParallelScope& value ) POSTFIX;		\
																		\
		PREFIX void visit_prolog( SequentialScope& value ) POSTFIX;		\
		PREFIX void visit_epilog( SequentialScope& value ) POSTFIX;		\
																		\
		PREFIX void visit_prolog( TrivialStatement& value ) POSTFIX;	\
		PREFIX void visit_epilog( TrivialStatement& value ) POSTFIX;	\
																		\
		PREFIX void visit_prolog( CallInstruction& value ) POSTFIX;		\
		PREFIX void visit_epilog( CallInstruction& value ) POSTFIX;		\
																		\
		PREFIX void visit_prolog( IdInstruction& value ) POSTFIX;		\
		PREFIX void visit_epilog( IdInstruction& value ) POSTFIX;		\
																		\
		PREFIX void visit_prolog( ExtractInstruction& value ) POSTFIX;	\
		PREFIX void visit_epilog( ExtractInstruction& value ) POSTFIX;	\
																		\
		PREFIX void visit_prolog( LoadInstruction& value ) POSTFIX;		\
		PREFIX void visit_epilog( LoadInstruction& value ) POSTFIX;		\
																		\
		PREFIX void visit_prolog( StoreInstruction& value ) POSTFIX;	\
		PREFIX void visit_epilog( StoreInstruction& value ) POSTFIX;	\
																		\
		PREFIX void visit_prolog( AndInstruction& value ) POSTFIX;		\
		PREFIX void visit_epilog( AndInstruction& value ) POSTFIX;		\
																		\
		PREFIX void visit_prolog( AddSignedInstruction& value ) POSTFIX; \
		PREFIX void visit_epilog( AddSignedInstruction& value ) POSTFIX; \
																		\
		PREFIX void visit_prolog( BitConstant& value ) POSTFIX;			\
		PREFIX void visit_epilog( BitConstant& value ) POSTFIX;			\
																		\
		PREFIX void visit_prolog( StructureConstant& value ) POSTFIX;	\
		PREFIX void visit_epilog( StructureConstant& value ) POSTFIX
		
		
		LIB_NOVEL_VISITOR_INTERFACE_( virtual, = 0 );
	};
}

#define LIB_NOVEL_VISITOR_INTERFACE LIB_NOVEL_VISITOR_INTERFACE_(,)





#endif /* _LIB_NOVEL_VISITOR_H_ */

//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  
