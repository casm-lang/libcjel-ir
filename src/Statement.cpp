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

#include "Statement.h"

using namespace libcsel_ir;

Statement::Statement(
    const char* name, Type* type, Value* parent, Value::ID id )
: Block( name, type, parent, false, id )
{
    assert( parent );

    if( isa< Scope >( parent ) )
    {
        ( (Scope*)parent )->add( this );
    }
    else if( isa< Intrinsic >( parent ) )
    {
        ( (Intrinsic*)parent )->setContext( this );
    }
    else
    {
        assert( !"invalid parent pointer!" );
    }

    printf( "[Statement] '%s' %p\n", name, parent );
}

const u1 Statement::isParallel( void ) const
{
    const Value* parent = this->parent();

    if( isa< Scope >( parent ) )
    {
        return ( (Scope*)parent )->isParallel();
    }
    else if( isa< Intrinsic >( parent ) )
    {
        return true;
    }
    else
    {
        assert( !"invalid parent pointer!" );
    }
    return false;
}

const std::vector< Value* >& Statement::instructions( void ) const
{
    return m_instructions;
}

Value* Statement::add( Value* instruction )
{
    assert( instruction );

    if( isa< Instruction >( instruction ) )
    {
        static_cast< Instruction* >( instruction )->setStatement( this );
    }
    else
    {
        assert( 0 );
    }

    if( m_instructions.size() > 0 )
    {
        m_instructions.back()->setNext( instruction );
    }

    m_instructions.push_back( instruction );
    printf( "[Stmt] add: %p\n", instruction );

    return instruction;
}

void Statement::addScope( Scope* scope )
{
    assert( scope );

    if( isa< TrivialStatement >( this ) )
    {
        assert(
            !" trivial statements are not allowed to have inside scopes! " );
    }
    else if( isa< LoopStatement >( this ) )
    {
        assert( m_scopes.size() < 1 );
    }

    m_scopes.push_back( scope );

    if( scope->parent() == 0 )
    {
        scope->setParent( this );
    }

    assert( scope->parent() == this && " inconsistent scope nesting! " );
}

const std::vector< Scope* >& Statement::scopes( void ) const
{
    if( isa< TrivialStatement >( this ) )
    {
        assert( !" trivial statements do not contain inside scopes! " );
    }

    return m_scopes;
}

bool Statement::classof( Value const* obj )
{
    return obj->id() == classid() or TrivialStatement::classof( obj )
           or BranchStatement::classof( obj ) or LoopStatement::classof( obj );
}

TrivialStatement::TrivialStatement( Value* parent )
: Statement( ".statement", Type::Label(), parent, Value::TRIVIAL_STATEMENT )
{
}

bool TrivialStatement::classof( Value const* obj )
{
    return obj->id() == classid();
}

BranchStatement::BranchStatement( Value* parent )
: Statement( ".branch", Type::Label(), parent, Value::BRANCH_STATEMENT )
{
}

bool BranchStatement::classof( Value const* obj )
{
    return obj->id() == classid();
}

LoopStatement::LoopStatement( Value* parent )
: Statement( ".loop", Type::Label(), parent, Value::LOOP_STATEMENT )
{
}

bool LoopStatement::classof( Value const* obj )
{
    return obj->id() == classid();
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
