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

#ifndef _LIB_NOVEL_STATEMENT_H_
#define _LIB_NOVEL_STATEMENT_H_

#include "Block.h"

#include "Value.h"
#include "Constant.h"
#include "Instruction.h"
#include "Scope.h"

namespace libnovel
{
	class Statement : public Block
	{
    // protected:
	// 	Block* scope;
		
	private:
		std::vector< Value* > instructions;
		
	public:
		Statement( const char* name, Type* type, Value* parent, Value::ID id = Value::STATEMENT );
	    
	// 	ExecutionSemanticsBlock* getBlock( void ) const;

		const std::vector< Value* >& getInstructions( void ) const;
		
		virtual const u1 isParallel( void ) const;

		void add( Value* instruction );
		
		void dump( void ) const;
		
		static bool classof( Value const* obj );
	};
	
	class TrivialStatement : public Statement
	{
	public:
		TrivialStatement( Value* parent = 0 );

		void dump( void ) const;
		
		static bool classof( Value const* obj );
	};
	
	
	
	// class BranchStatement : public Statement
	// {
	// private:
	// 	std::vector< Block* > blocks;
		
	// public:
	// 	BranchStatement( ExecutionSemanticsBlock* scope = 0 );
	    
	// 	void addBlock( Value* block );

	// 	const std::vector< Block* >& getBlocks( void ) const;
		
	// 	void dump( void ) const;
		
	// 	static bool classof( Value const* obj );
	// };

}


#endif /* _LIB_NOVEL_STATEMENT_H_ */

//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  