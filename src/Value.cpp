//
//  Copyright (c) 2015-2017 Philipp Paulweber
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                https://github.com/casm-lang/libcsel-ir
//
//  This file is part of libcsel-ir.
//
//  libcsel-ir is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  libcsel-ir is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with libcsel-ir. If not, see <http://www.gnu.org/licenses/>.
//

#include "Value.h"
#include "Visitor.h"

#include "Instruction.h"
#include "Scope.h"
#include "Statement.h"

using namespace libcsel_ir;

Value::Value( const char* name, Type* type, Value::ID id )
: name( name )
, type( type )
, id( id )
, type_lock( false )
, next( 0 )
{
    SymbolTable& symbols = *getSymbols();
    symbols[ name ].insert( this );
    // printf( "[Value] created '%s' @ %p", name, this );
    // if( type )
    // {
    // 	printf( " of type '%s' (=0x%lx)", type->getName(), type->getID() );
    // }
    // printf( "\n" );
}

Value::~Value()
{
    SymbolTable& symbols = *getSymbols();
    symbols[ name ].erase( this );
    // printf( "[Value] deleted '%s' @ %p of type %p\n", name, this, type );
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
            ( (SequentialScope*)this )->dump();
            break;
        case Value::PARALLEL_SCOPE:
            ( (ParallelScope*)this )->dump();
            break;

        case Value::TRIVIAL_STATEMENT:
            ( (TrivialStatement*)this )->dump();
            break;

        // case Value::IDENTIFIER:
        // 	((Identifier*)this)->dump(); break;
        // case Value::INTEGER_CONSTANT:
        // 	((IntegerConstant*)this)->dump(); break;
        default:
            if( isa< Instruction >( this ) )
            {
                ( (Instruction*)this )->dump();
            }
            else
            {
                debug();
            }
    }
}

void Value::iterate( Traversal order,
    Visitor* visitor,
    Context* context,
    std::function< void( Value& ) >
        action )
{
    static Context default_context = Context();

    Context* cxt = context ? context : &default_context;

    Value& value = static_cast< Value& >( *this );

    if( isa< Structure >( value )
        and ( static_cast< Structure& >( value ) ).getElements().size() == 0 )
    {
        // PPA: CHECK if this is still necessary
        return;
    }

    if( order == Traversal::PREORDER )
    {
        action( /*order, */ value );
    }

    if( visitor )
    {
        visitor->dispatch( Visitor::Stage::PROLOG, value, *cxt );
    }

    if( isa< Module >( value ) )
    {
        Module& module = static_cast< Module& >( value );
        const std::vector< Value* > empty = {};

        for( Value* p :
            ( module.has< Structure >() ? module.get< Structure >() : empty ) )
        {
            p->iterate( order, visitor, cxt, action );
        }

        for( Value* p :
            ( module.has< Constant >() ? module.get< Constant >() : empty ) )
        {
            p->iterate( order, visitor, cxt, action );
        }

        for( Value* p :
            ( module.has< Variable >() ? module.get< Variable >() : empty ) )
        {
            p->iterate( order, visitor, cxt, action );
        }

        for( Value* p :
            ( module.has< Memory >() ? module.get< Memory >() : empty ) )
        {
            p->iterate( order, visitor, cxt, action );
        }

        for( Value* p :
            ( module.has< Interconnect >() ? module.get< Interconnect >()
                                           : empty ) )
        {
            p->iterate( order, visitor, cxt, action );
        }

        for( Value* p :
            ( module.has< Intrinsic >() ? module.get< Intrinsic >() : empty ) )
        {
            p->iterate( order, visitor, cxt, action );
        }

        for( Value* p :
            ( module.has< Function >() ? module.get< Function >() : empty ) )
        {
            p->iterate( order, visitor, cxt, action );
        }
    }
    else if( isa< Structure >( value ) )
    {
        Structure& obj = static_cast< Structure& >( value );

        for( Value* p : obj.getElements() )
        {
            p->iterate( order, visitor, cxt, action );
        }
    }
    else if( isa< StructureConstant >( value ) )
    {
        StructureConstant& obj = static_cast< StructureConstant& >( value );

        for( Value* p : obj.getValue() )
        {
            p->iterate( order, visitor, cxt, action );
        }
    }
    else if( isa< CallableUnit >( value ) )
    {
        CallableUnit& obj = static_cast< CallableUnit& >( value );

        for( Value* p : obj.getInParameters() )
        {
            p->iterate( order, visitor, cxt, action );
        }

        for( Value* p : obj.getOutParameters() )
        {
            p->iterate( order, visitor, cxt, action );
        }

        if( visitor )
        {
            visitor->dispatch( Visitor::Stage::INTERLOG, value, *cxt );
        }

        Value* context = obj.getContext();
        assert( context );

        context->iterate( order, visitor, cxt, action );
    }
    else if( isa< Statement >( value ) )
    {
        Statement& stmt = static_cast< Statement& >( value );

        assert( stmt.getInstructions().size() > 0
                and " a statement must contain at least one instruction " );

        for( Value* instr : stmt.getInstructions() )
        {
            assert( instr );
            instr->iterate( order, visitor, cxt, action );
        }

        if( visitor && not isa< TrivialStatement >( value ) )
        {
            visitor->dispatch( Visitor::Stage::INTERLOG, value, *cxt );

            for( Scope* sco : stmt.getScopes() )
            {
                assert( sco );
                sco->iterate( order, visitor, cxt, action );
            }
        }
    }
    else if( isa< Scope >( value ) )
    {
        Scope& scope = static_cast< Scope& >( value );

        for( Block* block : scope.getBlocks() )
        {
            assert( block );
            block->iterate( order, visitor, cxt, action );
        }
    }

    if( visitor )
    {
        visitor->dispatch( Visitor::Stage::EPILOG, value, *cxt );
    }

    if( order == Traversal::POSTORDER )
    {
        action( /*order, */ value );
    }
}

void Value::iterate( Traversal order, std::function< void( Value& ) > action )
{
    iterate( order, 0, 0, action );
}

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
