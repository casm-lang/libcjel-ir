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

#include "Scope.h"
#include "Statement.h"

using namespace libnovel;


Scope::Scope( const char* name, Type* type, Value* parent, u1 is_parallel, Value::ID id )
: Block( name, type, parent, is_parallel, id )
{
	if( parent and Value::isa< CallableUnit >( parent ) )
	{
		((CallableUnit*)parent)->setContext( this );
	}
}

void Scope::add( Value* element )
{
	assert( Value::isa< Block >( element ) );

	// if( Value::isa< Scope >( element ) )
	// {
	// 	((Scope*)element)->setParent( this );
	// }
	
	block.push_back( (Block*)element );
}

const std::vector< Block* >& Scope::getBlocks( void ) const
{
	return block;
}

void Scope::dump( void ) const
{
	printf( "[%sScope] %p, %p\n", isParallel() ? "Par" : "Seq", this, getParent() );
	
	for( Block* blk : getBlocks() )
	{
		assert( blk );
	    blk->dump();
		//assert(0);
	}
}

bool Scope::classof( Value const* obj )
{
	return obj->getValueID() == classid()
		or SequentialScope::classof( obj )
		or ParallelScope::classof( obj )
		;
}


SequentialScope::SequentialScope( Value* parent )
: Scope( ".seq", 0, parent, false, Value::ID::SEQUENTIAL_SCOPE )
{
}

void SequentialScope::dump( void ) const
{
	((Scope*)this)->dump();
}

bool SequentialScope::classof( Value const* obj )
{
	return obj->getValueID() == classid()
		//or ???::classof( obj )
		;
}




ParallelScope::ParallelScope( Value* parent )
: Scope( ".par", 0, parent, false, Value::ID::PARALLEL_SCOPE )
{
}

void ParallelScope::dump( void ) const
{
	((Scope*)this)->dump();
}

bool ParallelScope::classof( Value const* obj )
{
	
	return obj->getValueID() == classid()
		//or ???::classof( obj )
		;
}



//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  
