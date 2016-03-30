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

#include "Statement.h"

using namespace libnovel;


Statement::Statement( const char* name, Type* type, Value* parent, Value::ID id )
: Block( name, type, parent, false, id )
{
	assert( parent );
	
	if( Value::isa< Scope >( parent ) )
	{
		((Scope*)parent)->add( this );
	}
	else if( Value::isa< Intrinsic >( parent ) )
	{
		((Intrinsic*)parent)->setContext( this );
	}
	else if( Value::isa< BranchStatement >( parent ) )
	{
		((BranchStatement*)parent)->addBlock( this );
	}
	else if( Value::isa< LoopStatement >( parent ) )
	{
		((LoopStatement*)parent)->addBlock( this );
	}
	else
	{
		assert( !"invalid parent pointer!" );
	}
    
	printf( "[Statement] '%s' %p\n", name, parent );
}


const u1 Statement::isParallel( void ) const
{
	const Value* parent = getParent();
	
	if( Value::isa< Scope >( parent ) )
	{
		return ((Scope*)parent)->isParallel();
	}
	else if( Value::isa< Intrinsic >( parent ) )
	{
		return true;
	}
	else
	{
		assert( !"invalid parent pointer!" );
	}
	return false;
}

// ExecutionSemanticsBlock* Statement::getBlock( void ) const
// {
// 	return scope;
// }

const std::vector< Value* >& Statement::getInstructions( void ) const
{
	return instructions;
}

void Statement::add( Value* instruction )
{
	assert( instruction );

	// if( Value::isa< ConstantValue >( instruction ) )
	// {
	// 	printf( "%s: %p --> Constant, omitted\n", __FUNCTION__, instruction );
	// 	return;
	// }
	
	if( Value::isa< Instruction >( instruction ) )
	{
	    static_cast< Instruction* >( instruction )->setStatement( this );
	}
	else
	{
		assert(0);
	}
	
	if( instructions.size() > 0 )
	{
		instructions.back()->setNext( instruction );
	}
	
	instructions.push_back( instruction );
	printf( "[Stmt] add: %p\n", instruction );	
}

void Statement::addBlock( Block* block )
{
	assert( block );
	
	if( Value::isa< TrivialStatement >( this ) )
	{
		assert( !" trivial statements are not allowed to have inside blocks! " );		
	}
	else if( Value::isa< LoopStatement >( this ) )
	{
		assert( blocks.size() < 1 );		
	}
	
	blocks.push_back( block );	
}

const std::vector< Block* >& Statement::getBlocks( void ) const
{
	if( Value::isa< TrivialStatement >( this ) )
	{
		assert( !" trivial statements do not contain inside blocks! " );		
	}

	return blocks;
}

void Statement::dump( void ) const
{
	for( auto instr : instructions )
	{
		((Value*)instr)->dump();
	}
}

bool Statement::classof( Value const* obj )
{
	return obj->getValueID() == classid()
		or TrivialStatement::classof( obj )
		or BranchStatement::classof( obj )
		or LoopStatement::classof( obj )
		;
}

TrivialStatement::TrivialStatement( Value* parent )
: Statement( ".statement", 0, parent, Value::TRIVIAL_STATEMENT )
{
}

void TrivialStatement::dump( void ) const
{
	const Value* parent = getParent();
	
	printf( "[Statment] %p", this );
	if( parent )
	{
		printf( " @ %p", parent );
	}
	printf( "\n" );
	
	((Statement*)this)->dump();
}

bool TrivialStatement::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}


BranchStatement::BranchStatement( Value* parent )
: Statement( ".branch", 0, parent, Value::BRANCH_STATEMENT )
{
}

void BranchStatement::dump( void ) const
{
	const Value* parent = getParent();
	
	printf( "[Branch] %p", this );
	if( parent )
	{
		printf( " @ %p", parent );
	}
	printf( "\n" );
	
	((Statement*)this)->dump();
}

bool BranchStatement::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}


LoopStatement::LoopStatement( Value* parent )
: Statement( ".loop", 0, parent, Value::LOOP_STATEMENT )
{
}

void LoopStatement::dump( void ) const
{
	const Value* parent = getParent();
	
	printf( "[Loop] %p", this );
	if( parent )
	{
		printf( " @ %p", parent );
	}
	printf( "\n" );
	
	((Statement*)this)->dump();
}

bool LoopStatement::classof( Value const* obj )
{
	return obj->getValueID() == classid();
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
