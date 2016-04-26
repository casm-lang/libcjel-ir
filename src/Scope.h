//  
//  Copyright (c) 2015-2016 Philipp Paulweber
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/ppaulweber/libnovel
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

#ifndef _LIB_NOVEL_SCOPE_H_
#define _LIB_NOVEL_SCOPE_H_

#include "Value.h"
#include "Block.h"

namespace libnovel
{
	class Scope : public Block
	{
		std::vector< Block* > block;
	public:
		Scope
		( const char* name
		, Type* type
		, Value* parent
		, u1 is_parallel
		, Value::ID id = Value::ID::SCOPE
		);

		void add( Value* element );
		
		const std::vector< Block* >& getBlocks( void ) const;
		
		void dump( void ) const;
		
		static inline Value::ID classid( void ) { return Value::SCOPE; };
		static bool classof( Value const* obj );
	};
	
	
	class SequentialScope : public Scope
	{
	public:
		SequentialScope( Value* parent = 0 );
		
		void dump( void ) const;
		
		static inline Value::ID classid( void ) { return Value::SEQUENTIAL_SCOPE; };
		static bool classof( Value const* obj );		
	};

	class ParallelScope : public Scope
	{
	public:
	    ParallelScope( Value* parent = 0 );
		
		void dump( void ) const;
		
		static inline Value::ID classid( void ) { return Value::PARALLEL_SCOPE; };
		static bool classof( Value const* obj );		
	};
}


#endif /* _LIB_NOVEL_SCOPE_H_ */

//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  
