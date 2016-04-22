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
		
		for( Value* p : (obj->has< Intrinsic >() ? obj->get< Intrinsic >() : empty ) )
		{
			p->iterate( order, visitor, action );
		}
		
		for( Value* p : (obj->has< Function >() ? obj->get< Function >() : empty ) )
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
	else if( Value::isa< Statement >( this ) )
	{
		Statement* stmt = (Statement*)this;		
		assert( stmt->getInstructions().size() > 0 and " a statement must contain at least one instruction " );
		
		for( Value* instr : stmt->getInstructions() )
		{
			assert( instr );
			instr->iterate( order, visitor, action );
		}
		
		if( visitor && not Value::isa< TrivialStatement >( this ) )
		{
			visitor->dispatch( Visitor::Stage::INTERLOG, this );

			for( Scope* sco : stmt->getScopes() )
			{
				assert( sco );
				sco->iterate( order, visitor, action );
			}
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
