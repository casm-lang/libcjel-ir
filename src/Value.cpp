//
//  Copyright (C) 2015-2019 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                <https://github.com/casm-lang/libcjel-ir>
//
//  This file is part of libcjel-ir.
//
//  libcjel-ir is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  libcjel-ir is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with libcjel-ir. If not, see <http://www.gnu.org/licenses/>.
//
//  Additional permission under GNU GPL version 3 section 7
//
//  libcjel-ir is distributed under the terms of the GNU General Public License
//  with the following clarification and special exception: Linking libcjel-ir
//  statically or dynamically with other modules is making a combined work
//  based on libcjel-ir. Thus, the terms and conditions of the GNU General
//  Public License cover the whole combination. As a special exception,
//  the copyright holders of libcjel-ir give you permission to link libcjel-ir
//  with independent modules to produce an executable, regardless of the
//  license terms of these independent modules, and to copy and distribute
//  the resulting executable under terms of your choice, provided that you
//  also meet, for each linked independent module, the terms and conditions
//  of the license of that module. An independent module is a module which
//  is not derived from or based on libcjel-ir. If you modify libcjel-ir, you
//  may extend this exception to your version of the library, but you are
//  not obliged to do so. If you do not wish to do so, delete this exception
//  statement from your version.
//

#include "Value.h"

#include <libcjel-ir/Constant>
#include <libcjel-ir/Function>
#include <libcjel-ir/Instruction>
#include <libcjel-ir/Interconnect>
#include <libcjel-ir/Intrinsic>
#include <libcjel-ir/Memory>
#include <libcjel-ir/Module>
#include <libcjel-ir/Reference>
#include <libcjel-ir/Scope>
#include <libcjel-ir/Statement>
#include <libcjel-ir/Structure>
#include <libcjel-ir/Variable>
#include <libcjel-ir/Visitor>

#include <cassert>

using namespace libcjel_ir;

Value::Value( const std::string& name, const Type::Ptr& type, Value::ID id )
: m_name( name )
, m_type( type )
, m_id( id )
{
    m_id2objs()[ m_id ].insert( this );
}

Value::~Value( void )
{
    m_id2objs()[ m_id ].erase( this );
}

std::string Value::name( void ) const
{
    if( isa< BitConstant >( this ) )
    {
        auto c = static_cast< const BitConstant* >( this );
        return c->literal();
    }

    return m_name;
}

const Type& Value::type( void ) const
{
    return *m_type.get();
}

Type::Ptr Value::ptr_type( void ) const
{
    return m_type;
}

Value::ID Value::id( void ) const
{
    return m_id;
}

std::string Value::description( void ) const
{
    return type().name() + " " + name();
}

std::string Value::dump( void ) const
{
    std::string tmp = "[" + type().name() + "] " + label() + " = ";

    if( isa< Constant >( this ) )
    {
        tmp += type().name() + " ";
    }

    tmp += name();

    if( auto instr = cast< Instruction >( this ) )
    {
        u1 first = true;
        for( auto operand : instr->operands() )
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

            tmp += operand->type().name() + " " + operand->label();
        }
    }

    return tmp;
}

std::string Value::make_hash( void ) const
{
    return "v:" + std::to_string( id() ) + ":" + description();
}

std::string Value::label( void ) const
{
    return name();
}

void Value::iterate(
    Traversal order, Visitor* visitor, Context* context, std::function< void( Value& ) > action )
{
    static Context default_context = Context();

    Context* cxt = context ? context : &default_context;

    Value& value = static_cast< Value& >( *this );

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

        for( auto p : module.get< Structure >() )
        {
            p->iterate( order, visitor, cxt, action );
        }

        for( auto p : module.get< Constant >() )
        {
            p->iterate( order, visitor, cxt, action );
        }

        for( auto p : module.get< Variable >() )
        {
            p->iterate( order, visitor, cxt, action );
        }

        for( auto p : module.get< Memory >() )
        {
            p->iterate( order, visitor, cxt, action );
        }

        for( auto p : module.get< Interconnect >() )
        {
            p->iterate( order, visitor, cxt, action );
        }

        for( auto p : module.get< Intrinsic >() )
        {
            p->iterate( order, visitor, cxt, action );
        }

        for( auto p : module.get< Function >() )
        {
            p->iterate( order, visitor, cxt, action );
        }
    }
    else if( isa< StructureConstant >( value ) )
    {
        StructureConstant& obj = static_cast< StructureConstant& >( value );

        for( auto p : obj.value() )
        {
            p.iterate( order, visitor, cxt, action );
        }
    }
    else if( isa< CallableUnit >( value ) )
    {
        CallableUnit& obj = static_cast< CallableUnit& >( value );

        for( auto p : obj.inputs() )
        {
            p->iterate( order, visitor, cxt, action );
        }

        for( auto p : obj.outputs() )
        {
            p->iterate( order, visitor, cxt, action );
        }

        if( visitor )
        {
            visitor->dispatch( Visitor::Stage::INTERLOG, value, *cxt );
        }

        auto context = obj.context();
        assert( context );

        context->iterate( order, visitor, cxt, action );
    }
    else if( isa< Statement >( value ) )
    {
        Statement& stmt = static_cast< Statement& >( value );

        assert(
            stmt.instructions().size() > 0 and
            " a statement must contain at least one instruction " );

        for( auto instr : stmt.instructions() )
        {
            assert( instr );
            instr->iterate( order, visitor, cxt, action );
        }

        if( visitor && not isa< TrivialStatement >( value ) )
        {
            visitor->dispatch( Visitor::Stage::INTERLOG, value, *cxt );

            for( auto sco : stmt.scopes() )
            {
                assert( sco );
                sco->iterate( order, visitor, cxt, action );
            }
        }
    }
    else if( isa< Scope >( value ) )
    {
        Scope& scope = static_cast< Scope& >( value );

        for( auto block : scope.blocks() )
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
