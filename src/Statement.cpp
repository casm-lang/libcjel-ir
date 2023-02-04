//
//  Copyright (C) 2015-2023 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber et al.
//  <https://github.com/casm-lang/libcjel-ir/graphs/contributors>
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

#include "Statement.h"

#include <libstdhl/Memory>

using namespace libcjel_ir;

//
// Statement
//

Statement::Statement( const std::string& name, const Type::Ptr& type, Value::ID id )
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
        throw std::domain_error( "cannot add a null pointer instruction to a statement block" );
    }

    m_instructions.add( instruction );

    return instruction;
}

void Statement::add( const Scope::Ptr& scope )
{
    if( not scope )
    {
        throw std::domain_error( "cannot add a null pointer instruction to a statement block" );
    }

    if( isa< TrivialStatement >( this ) )
    {
        throw std::domain_error( "trivial statements are not allowed to have inside scopes" );
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
    return obj->id() == classid() or TrivialStatement::classof( obj ) or
           BranchStatement::classof( obj ) or LoopStatement::classof( obj );
}

//
// Trivial Statement
//

TrivialStatement::TrivialStatement( Value* parent )
: Statement( "stmt", libstdhl::Memory::get< LabelType >(), classid() )
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
: Statement( "branch", libstdhl::Memory::get< LabelType >(), classid() )
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
: Statement( "loop", libstdhl::Memory::get< LabelType >(), classid() )
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
