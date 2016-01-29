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

#ifndef _LIB_NOVEL_VALUE_H_
#define _LIB_NOVEL_VALUE_H_

#include "Novel.h"
#include "Type.h"

namespace libnovel
{
	class Visitor;
	enum class Traversal;
	
	class Value //: public Novel
	{
	public:
		enum ID
		{ USER
		
		, MODULE
		
		, MEMORY

		, CALLABLE_UNIT
		, COMPONENT
		, FUNCTION
		
		, REFERENCE
		, STRUCTURE

		, CONSTANT
		, IDENTIFIER
		  
		, BLOCK
		  
		, SCOPE
		, PARALLEL_SCOPE
	    , SEQUENTIAL_SCOPE
		  
		, STATEMENT
		, TRIVIAL_STATEMENT
		, BRANCH_STATEMENT
		
		, INSTRUCTION
		, UNARY_INSTRUCTION
		, BINARY_INSTRUCTION
		
		, LOAD_INSTRUCTION
		, READ_INSTRUCTION
		, STORE_INSTRUCTION
		, WRITE_INSTRUCTION

		, CALL_INSTRUCTION
		  
		, AND_INSTRUCTION
				
		, ADDS_INSTRUCTION
		, ADDU_INSTRUCTION
		};
		
		typedef std::unordered_map
		< const char*
		, std::unordered_set< Value* >
		, libstdhl::Hash
		, libstdhl::Equal
		> SymbolTable;
		
		static SymbolTable* getSymbols( void )
		{
			static SymbolTable symbols;
			return &symbols;
		}
		
	private:
		const char* name;
		Type* type;		
		ID id;
		u1 type_lock;
		
		std::vector< Type* > parameters;
		
	public:
		Value( const char* name, Type* type, ID id );
		
		~Value();
		
		const char* getName( void ) const;
	    
		Type* getType( void ) const;
	protected:
		void setType( Type* type );
		
	public:
		ID getValueID() const;
		
		void debug( void ) const;
		void dump( void ) const;
		
		static inline bool classof( Value const* )
		{
			return true;
		}
		
		template< class TO >
		static inline bool isa( Value* value )
		{
			return TO::classof( value );
		}
		
		template< class TO >
		static inline bool isa( const Value* value )
		{
			return isa< TO >( (Value*)value );
		}
		
	    virtual void iterate
		( Traversal order, Visitor* visitor = 0, std::function< void( Value* ) > action  = []( Value* ){ } ) final;
	};
}

#endif /* _LIB_NOVEL_VALUE_H_ */

//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  
