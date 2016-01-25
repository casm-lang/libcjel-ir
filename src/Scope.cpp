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


SequentialScope::SequentialScope()
: Block( ".seq", 0, 0, false, Value::ID::SEQUENTIAL_SCOPE )
{
}

void SequentialScope::dump( void ) const
{
	// printf( "[Scope] %p\n", this );
	((Value*)this)->dump();
}

bool SequentialScope::classof( Value const* obj )
{
	return obj->getValueID() == Value::SEQUENTIAL_SCOPE
		//or ???::classof( obj )
		;
}





// ExecutionSemanticsScope::ExecutionSemanticsScope
// ( const char* name
// , Type* type
// , const u1 is_parallel
// , ExecutionSemanticsScope* parent
// , Value::ID id
// )
// : Scope( name, type, id )
// , is_parallel( is_parallel )
// , pseudo_state( 0 )
// , parent( parent )
// {
// 	setParent( parent );
// }

// const u1 ExecutionSemanticsScope::isParallel( void ) const
// {
// 	return is_parallel;
// }
		
// const u64 ExecutionSemanticsScope::getPseudoState( void ) const
// {
// 	return pseudo_state;
// }
		
// ExecutionSemanticsScope* ExecutionSemanticsScope::getParent( void ) const
// {
// 	return parent;
// }
		
// void ExecutionSemanticsScope::setParent( ExecutionSemanticsScope* parent_block )
// {
// 	parent = parent_block;
			
// 	if( parent )
// 	{
// 		pseudo_state = parent->getPseudoState();

// 		if( parent->isParallel() != this->isParallel() )
// 		{
// 			pseudo_state++;
// 		}
// 	}
// }

// const std::vector< Scope* >& ExecutionSemanticsScope::getScopes( void ) const
// {
// 	return blocks;
// }
		
// void ExecutionSemanticsScope::add( Scope* block )
// {
// 	assert( block );
			
			
// 	if( Value::isa< ExecutionSemanticsScope >( block ) )
// 	{
// 		ExecutionSemanticsScope* inner = static_cast< ExecutionSemanticsScope* >( block );
// 		inner->setParent( this );
// 	}
			
// 	blocks.push_back( block );
// }
		
// void ExecutionSemanticsScope::dump( void ) const
// {
// 	printf( "[ESBlk] %p, %p, %u @ %lu\n"
// 			, this, parent, isParallel(), getPseudoState() );
			
// 	for( Scope* block : blocks )
// 	{
// 		assert( block );

// 		block->dump();
// 	}
// }
		




// ParallelScope::ParallelScope( ExecutionSemanticsScope* parent )
// : ExecutionSemanticsScope( "par", 0, true, parent, Value::PARALLEL_SCOPE )
// {
// }

// void ParallelScope::dump( void ) const
// {
// 	((ExecutionSemanticsScope*)this)->dump();
// }
		


// SequentialScope::SequentialScope( ExecutionSemanticsScope* parent )
// : ExecutionSemanticsScope( "seq", 0, false, parent, Value::SEQUENTIAL_SCOPE )
// {
// }
		
// void SequentialScope::dump( void ) const
// {
// 	((ExecutionSemanticsScope*)this)->dump();
// }





// bool ExecutionSemanticsScope::classof( Value const* obj )
// {
// 	return obj->getValueID() == Value::EXECUTION_SEMANTICS_SCOPE
// 		or ParallelScope::classof( obj )
// 		or SequentialScope::classof( obj );
// }

// bool ParallelScope::classof( Value const* obj )
// {
// 	return obj->getValueID() == Value::PARALLEL_SCOPE;
// }

// bool SequentialScope::classof( Value const* obj )
// {
// 	return obj->getValueID() == Value::SEQUENTIAL_SCOPE;
// }



//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  
