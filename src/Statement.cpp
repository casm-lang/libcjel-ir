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

//
// Statement
//

Statement::Statement(
    const std::string& name, const Type::Ptr& type, Value::ID id )
: Block( name, type, false, id )
{
}

Instructions Statement::instructions( void ) const
{
    return m_instructions;
}

Instruction::Ptr Statement::add( const Instruction::Ptr& instruction )
{
    if( not instruction )
    {
        throw std::domain_error(
            "cannot add a null pointer instruction to a statement block" );
    }

    m_instructions.add( instruction );

    return instruction;
}

void Statement::add( const Scope::Ptr& scope )
{
    if( not scope )
    {
        throw std::domain_error(
            "cannot add a null pointer instruction to a statement block" );
    }

    if( isa< TrivialStatement >( this ) )
    {
        throw std::domain_error(
            "trivial statements are not allowed to have inside scopes" );
    }
    else if( isa< LoopStatement >( this ) )
    {
        if( m_scopes.size() >= 1 )
        {
            throw std::domain_error(
                "loop statements are not allowed to have multiple inside "
                "scopes" );
        }
    }

    m_scopes.add( scope );
}

Scopes Statement::scopes( void ) const
{
    return m_scopes;
}

bool Statement::classof( Value const* obj )
{
    return obj->id() == classid() or TrivialStatement::classof( obj )
           or BranchStatement::classof( obj ) or LoopStatement::classof( obj );
}

//
// Trivial Statement
//

TrivialStatement::TrivialStatement( Value* parent )
: Statement( "stmt", libstdhl::get< LabelType >(), classid() )
{
}

bool TrivialStatement::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Branch Statement
//

BranchStatement::BranchStatement( Value* parent )
: Statement( "branch", libstdhl::get< LabelType >(), classid() )
{
}

bool BranchStatement::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Loop Statement
//

LoopStatement::LoopStatement( Value* parent )
: Statement( "loop", libstdhl::get< LabelType >(), classid() )
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
