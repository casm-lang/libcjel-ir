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
	else if( Value::isa< Function >( parent ) )
	{
		((Function*)parent)->setContext( this );
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
	else if( Value::isa< Function >( parent ) )
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
	
	instructions.push_back( instruction );
	printf( "[Stmt] add: %p\n", instruction );	
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
		//or BranchStatement::classof( obj )
		;
}

TrivialStatement::TrivialStatement( Value* parent )
: Statement( ".statement", 0, parent, Value::TRIVIAL_STATEMENT )
{
}

void TrivialStatement::dump( void ) const
{
	const Value* parent = getParent();
	
	printf( "[TrStm] %p", this );
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






// BranchStatement::BranchStatement( ExecutionSemanticsBlock* scope )
// : Statement( ".branch", 0, scope, Value::BRANCH_STATEMENT )
// {
// }

// void BranchStatement::addBlock( Value* block )
// {
// 	assert( Value::isa< Block >( block ) );
	
// 	blocks.push_back( (Block*)block );
// }

// const std::vector< Block* >& BranchStatement::getBlocks( void ) const
// {
// 	return blocks;
// }

// void BranchStatement::dump( void ) const
// {
// 	printf( "[BranchStatement] %p\n", this );
	
// 	((Statement*)this)->dump();
	
// 	// TODO: here the branches etc.
// }

// bool BranchStatement::classof( Value const* obj )
// {
// 	return obj->getValueID() == Value::BRANCH_STATEMENT;
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
