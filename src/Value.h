//  
//  Copyright (c) 2015-2016 Philipp Paulweber
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/casm-lang/libnovel
//  
//  This file is part of libnovel.
//  
//  libnovel is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  libnovel is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with libnovel. If not, see <http://www.gnu.org/licenses/>.
//  

#ifndef _LIB_NOVEL_VALUE_H_
#define _LIB_NOVEL_VALUE_H_

#include "Novel.h"
#include "Type.h"

#include "cpp/Labeling.h"

namespace libnovel
{
	class Visitor;
	enum class Traversal;
	
	class Value : public libstdhl::Labeling //: public Novel
	{
	public:
		enum ID
		{ VALUE
		, USER
		
		, MODULE
		, INTERCONNECT
		
		, MEMORY
		
		, CALLABLE_UNIT
		, INTRINSIC
		, FUNCTION
		
		, VARIABLE
		, REFERENCE
		, STRUCTURE
		
		, CONSTANT
		, BIT_CONSTANT
		, STRUCTURE_CONSTANT
		, STRING_CONSTANT
		
		, IDENTIFIER
		
		, BLOCK
		
		, SCOPE
		, PARALLEL_SCOPE
	    , SEQUENTIAL_SCOPE
		  
		, STATEMENT
		, TRIVIAL_STATEMENT
		, BRANCH_STATEMENT
		, LOOP_STATEMENT
		
		, INSTRUCTION
		, UNARY_INSTRUCTION

		, BINARY_INSTRUCTION

		, NOP_INSTRUCTION
		, ALLOC_INSTRUCTION
		
		, ID_INSTRUCTION
		, CAST_INSTRUCTION
		, EXTRACT_INSTRUCTION
		
		, LOAD_INSTRUCTION
		, READ_INSTRUCTION
		, STORE_INSTRUCTION
		, WRITE_INSTRUCTION
		
		, CALL_INSTRUCTION
		, ID_CALL_INSTRUCTION
		
		, STREAM_INSTRUCTION
		
		, ARITHMETIC_INSTRUCTION
		  
		, NOT_INSTRUCTION
		, AND_INSTRUCTION
		, OR_INSTRUCTION
		, XOR_INSTRUCTION
	    
		, ADDS_INSTRUCTION
		, ADDU_INSTRUCTION
		, DIVS_INSTRUCTION
		, MODU_INSTRUCTION
		
		, LOGICAL_INSTRUCTION
		, EQUU_INSTRUCTION
		, NEQU_INSTRUCTION
		
	    , ZEXT_INSTRUCTION
		, TRUNC_INSTRUCTION
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

		std::unordered_map< u32, Value* > references;

		Value* next;
		
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

		void setNext( Value* value );
		Value* getNext( void ) const;


		template< class C >
		C* getRef( void )
		{			
			auto result = references.find( C::classid() );
			if( result != references.end() )
			{
				assert( Value::isa< C >( result->second ) );
				return (C*)result->second;
			}
			
			return 0;
		}
		
		template< class C >
		void setRef( Value* reference )
		{
			//printf( "[[[ %p ]]]: classID/referenceCNT = %u %u\n"
			//, reference, C::classid(), references.count( C::classid() ) );
			assert( references.count( C::classid() ) == 0 );
			references[ C::classid() ] = reference;
		}
		
		static inline ID classid( void ) { return Value::VALUE; };

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

		virtual void iterate
		( Traversal order, std::function< void( Value* ) > action ) final;
		
		
		virtual u64 getLabelID( void )
		{
			static u64 cnt = -1;
			cnt++;
			return cnt;
		}
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
