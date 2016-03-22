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

#include "Value.h"
#include "Visitor.h"

#include "Scope.h"
#include "Statement.h"
#include "Instruction.h"

using namespace libnovel;


Value::Value( const char* name, Type* type, Value::ID id )
: name( name )
, type( type )
, id( id )
, type_lock( false )
{
	SymbolTable& symbols = *getSymbols();
	symbols[ name ].insert( this );
	printf( "[Value] created '%s' @ %p", name, this );
	if( type )
	{
		printf( " of type '%s' (=0x%lx)", type->getName(), type->getID() );
	}
	printf( "\n" );
}

Value::~Value()
{
	SymbolTable& symbols = *getSymbols();
	symbols[ name ].erase( this );
	printf( "[Value] deleted '%s' @ %p of type %p\n", name, this, type );
}

const char* Value::getName( void ) const
{
	return name;
}

Type* Value::getType( void ) const
{
	return type;
}

void Value::setType( Type* type )
{
    assert( !type_lock );
	type_lock = true;
	
	this->type = type;
}


Value::ID Value::getValueID() const
{
	return id;
}


// TODO: FIXME: PPA: maybe a better solution than this!!!
void Value::setNext( Value* value )
{
	next = value;
}

Value* Value::getNext( void ) const
{
	return next;
}


void Value::debug( void ) const
{
	printf( "DEBUG: %p '%s' : ", this, getName() );
	if( getType() )
	{
		printf( "%s", getType()->getName() );
	}
	printf( "\n" );
}

void Value::dump( void ) const
{    
	switch( this->getValueID() )
	{
	    case Value::SEQUENTIAL_SCOPE:
			((SequentialScope*)this)->dump(); break;
	    case Value::PARALLEL_SCOPE:
			((ParallelScope*)this)->dump(); break;

	    case Value::TRIVIAL_STATEMENT:
			((TrivialStatement*)this)->dump(); break;
		
	// case Value::IDENTIFIER:
	// 	((Identifier*)this)->dump(); break;
	// case Value::INTEGER_CONSTANT:
	// 	((IntegerConstant*)this)->dump(); break;	
	default:
		if( Value::isa< Instruction >( this ) )
		{
			((Instruction*)this)->dump();
		}
		else
		{
			debug();
		}
	}
}


void Value::iterate( Traversal order, Visitor* visitor, std::function< void( Value* ) > action )
{
	if( Value::isa< Structure >( this ) and ((Structure*)this)->getElements().size() == 0 )
	{
		return;
	}
	
	if( order == Traversal::PREORDER )
	{
	    action( /*order, */ this );
	}
	
	if( visitor )
	{
		visitor->dispatch( Visitor::Stage::PROLOG, this );
	}

	if( Value::isa< Module >( this ) )
	{
	    Module* obj = ((Module*)this);
		const std::vector< Value* > empty = {};
		
		for( Value* p : (obj->has< Structure >() ? obj->get< Structure >() : empty ) )
		{
			p->iterate( order, visitor, action );
		}
		
		for( Value* p : (obj->has< Constants >() ? obj->get< Constants >() : empty ) )
		{
			p->iterate( order, visitor, action );
		}
		
		for( Value* p : (obj->has< Variable >() ? obj->get< Variable >() : empty ) )
		{
			p->iterate( order, visitor, action );
		}
		
		for( Value* p : (obj->has< Memory >() ? obj->get< Memory >() : empty ) )
		{
			p->iterate( order, visitor, action );
		}
		
		for( Value* p : (obj->has< Function >() ? obj->get< Function >() : empty ) )
		{
			p->iterate( order, visitor, action );
		}
		
		for( Value* p : (obj->has< Component >() ? obj->get< Component >() : empty ) )
		{
			p->iterate( order, visitor, action );
		}
	}
	else if( Value::isa< Structure >( this ) )
	{
	    Structure* obj = ((Structure*)this);
		
		for( Value* p : obj->getElements() )
		{				
			p->iterate( order, visitor, action );
		}
	}
	else if( Value::isa< StructureConstant >( this ) )
	{
	    StructureConstant* obj = ((StructureConstant*)this);
		
		for( Value* p : obj->getElements() )
		{
			p->iterate( order, visitor, action );
		}
	}
	else if( Value::isa< CallableUnit >( this ) )
	{
		CallableUnit* obj = ((CallableUnit*)this);
		
		for( Value* p : obj->getInParameters() )
		{
			p->iterate( order, visitor, action );
		}
		
		for( Value* p : obj->getOutParameters() )
		{
			p->iterate( order, visitor, action );
		}
		
		if( visitor )
		{
			visitor->dispatch( Visitor::Stage::INTERLOG, this );
		}

		Value* context = obj->getContext();
		assert( context );
		
	    context->iterate( order, visitor, action );
	}
	else if( Value::isa< TrivialStatement >( this ) )
	{
		for( Value* instr : ((TrivialStatement*)this)->getInstructions() )
		{
			assert( instr );
			instr->iterate( order, visitor, action );
		}
	}
	else if( Value::isa< Scope >( this ) )
	{
		for( Block* block : ((Scope*)this)->getBlocks() )
		{
			assert( block );
			block->iterate( order, visitor, action );
		}
	}
	
	if( visitor )
	{
		visitor->dispatch( Visitor::Stage::EPILOG, this );
	}
	
	if( order == Traversal::POSTORDER )
	{
	    action( /*order, */ this );
	}
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
