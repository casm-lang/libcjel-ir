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

#include "Scope.h"
#include "Statement.h"

using namespace libnovel;


Scope::Scope( const char* name, Type* type, Value* parent, u1 is_parallel, Value::ID id )
: Block( name, type, parent, is_parallel, id )
{
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


SequentialScope::SequentialScope()
: Scope( ".seq", 0, 0, false, Value::ID::SEQUENTIAL_SCOPE )
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




ParallelScope::ParallelScope()
: Scope( ".par", 0, 0, false, Value::ID::PARALLEL_SCOPE )
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
