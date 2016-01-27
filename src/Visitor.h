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

#include "Function.h"
#include "Memory.h"

namespace libnovel
{
	enum class Traversal
	{ PREORDER
	, POSTORDER
	, INORDER
	};

	class Visitor //: public Novel
	{
    public:
		virtual void visit_prolog( Function& value ) = 0;
		virtual void visit_epilog( Function& value ) = 0;
		
		virtual void visit_prolog( Memory& value ) = 0;
		virtual void visit_epilog( Memory& value ) = 0;
		
		virtual void visit_prolog( ParallelScope& value ) = 0;
		virtual void visit_epilog( ParallelScope& value ) = 0;
		
		virtual void visit_prolog( SequentialScope& value ) = 0;
		virtual void visit_epilog( SequentialScope& value ) = 0;
		
		virtual void visit_prolog( TrivialStatement& value ) = 0;
		virtual void visit_epilog( TrivialStatement& value ) = 0;
	    
		virtual void visit_prolog( CallInstruction& value ) = 0;
		virtual void visit_epilog( CallInstruction& value ) = 0;
		
		virtual void visit_prolog( LoadInstruction& value ) = 0;
		virtual void visit_epilog( LoadInstruction& value ) = 0;
		
		virtual void visit_prolog( StoreInstruction& value ) = 0;
		virtual void visit_epilog( StoreInstruction& value ) = 0;
		
		virtual void visit_prolog( AndInstruction& value ) = 0;
		virtual void visit_epilog( AndInstruction& value ) = 0;
		
		virtual void visit_prolog( AddSignedInstruction& value ) = 0;
		virtual void visit_epilog( AddSignedInstruction& value ) = 0;
		
		// virtual void visit_prolog( & value ) = 0;
		// virtual void visit_epilog( & value ) = 0;
		
		virtual void dispatch( u1 prolog, Value* value ) final;
	};
}


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
