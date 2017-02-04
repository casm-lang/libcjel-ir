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

#include "Instruction.h"
#include "Scope.h"
#include "Statement.h"
#include "Visitor.h"

#include "../stdhl/cpp/Default.h"
#include "../stdhl/cpp/Log.h"

using namespace libcsel_ir;

Value::Value( const char* name, Type* type, Value::ID id )
: m_name( name )
, m_type( type )
, m_id( id )
, m_type_lock( false )
, m_next( 0 )
{
    m_id2objs()[ m_id ].insert( this );
}

Value::~Value()
{
    m_id2objs()[ m_id ].erase( this );
}

const char* Value::name( void ) const
{
    return m_name;
}

Type& Value::type( void ) const
{
    return *m_type;
}

void Value::setType( Type& type )
{
    assert( !m_type_lock );
    m_type_lock = true;

    this->m_type = &type;
}

Value::ID Value::id() const
{
    return m_id;
}

void Value::setNext( Value* value )
{
    m_next = value;
}

Value& Value::next( void ) const
{
    return *m_next;
}

const char* Value::c_str( void )
{
    std::string tmp = "";
    tmp += label();
    tmp += " = ";

    if( isa< Constant >( this ) )
    {
        tmp += type().name();
        tmp += " ";
    }
    tmp += name();

    if( auto instr = cast< Instruction >( this ) )
    {
        u1 first = true;
        for( auto operand : instr->values() )
        {
            if( first )
            {
                first = false;
                tmp += " ";
            }
            else
            {
                tmp += ", ";
            }
            tmp += operand->type().name();
            tmp += " ";
            tmp += operand->label();
        }
    }

    tmp += "    ;; ";
    tmp += type().name();

    return libstdhl::Allocator::string( tmp );
}

void Value::dump( void ) const
{
    libstdhl::Log::info( "%p: '%s' [%u] %s [%u]",
        this,
        this->name(),
        this->id(),
        this->type().name(),
        this->type().id() );
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
        and ( static_cast< Structure& >( value ) ).elements().size() == 0 )
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

        for( Value* p : obj.elements() )
        {
            p->iterate( order, visitor, cxt, action );
        }
    }
    else if( isa< StructureConstant >( value ) )
    {
        StructureConstant& obj = static_cast< StructureConstant& >( value );

        for( Value* p : obj.value() )
        {
            p->iterate( order, visitor, cxt, action );
        }
    }
    else if( isa< CallableUnit >( value ) )
    {
        CallableUnit& obj = static_cast< CallableUnit& >( value );

        for( Value* p : obj.inParameters() )
        {
            p->iterate( order, visitor, cxt, action );
        }

        for( Value* p : obj.outParameters() )
        {
            p->iterate( order, visitor, cxt, action );
        }

        if( visitor )
        {
            visitor->dispatch( Visitor::Stage::INTERLOG, value, *cxt );
        }

        Value* context = obj.context();
        assert( context );

        context->iterate( order, visitor, cxt, action );
    }
    else if( isa< Statement >( value ) )
    {
        Statement& stmt = static_cast< Statement& >( value );

        assert( stmt.instructions().size() > 0
                and " a statement must contain at least one instruction " );

        for( Value* instr : stmt.instructions() )
        {
            assert( instr );
            instr->iterate( order, visitor, cxt, action );
        }

        if( visitor && not isa< TrivialStatement >( value ) )
        {
            visitor->dispatch( Visitor::Stage::INTERLOG, value, *cxt );

            for( Scope* sco : stmt.scopes() )
            {
                assert( sco );
                sco->iterate( order, visitor, cxt, action );
            }
        }
    }
    else if( isa< Scope >( value ) )
    {
        Scope& scope = static_cast< Scope& >( value );

        for( Block* block : scope.blocks() )
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
