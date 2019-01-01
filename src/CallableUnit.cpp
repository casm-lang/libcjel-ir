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

#include "CallableUnit.h"

#include <libcjel-ir/Constant>
#include <libcjel-ir/Function>
#include <libcjel-ir/Intrinsic>
#include <libcjel-ir/Scope>

#include <libstdhl/Memory>

#include <cassert>

using namespace libcjel_ir;

u64 CallableUnit::m_allocation_cnt = 0;

CallableUnit::CallableUnit( const std::string& name, const Type::Ptr& type, Value::ID id )
: User( name, type, id )
, m_allocation_id( libstdhl::Memory::make< BitConstant >( 64, m_allocation_cnt ) )
{
    if( not type->isRelation() )
    {
        throw std::domain_error(
            "invalid type '" + type->name() + "' for intrinsic, requires 'RelationType'" );
    }

    m_allocation_cnt++;
}

void CallableUnit::setContext( const Scope::Ptr& scope )
{
    assert( scope );

    m_context = scope;
}

Scope::Ptr CallableUnit::context( void ) const
{
    return m_context;
}

BitConstant::Ptr CallableUnit::allocId( void ) const
{
    return m_allocation_id;
}

void CallableUnit::add( const Reference::Ptr& reference )
{
    auto name = reference->name();

    auto result = m_name2ref.find( name );
    if( result != m_name2ref.end() )
    {
        throw std::domain_error(
            "this 'CallableUnit' already has a reference named '" + name + "' of type '" +
            reference->type().name() + "'" );
    }

    m_name2ref[ name ] = reference;

    auto kind = reference->kind();

    m_name2index[ name ] = m_references[ kind ].size();
    m_references[ kind ].push_back( reference );
}

Reference::Ptr CallableUnit::in( const std::string& name, const Type::Ptr& type )
{
    auto ref = libstdhl::Memory::make< Reference >( name, type, Reference::INPUT );
    add( ref );
    return ref;
}

Reference::Ptr CallableUnit::out( const std::string& name, const Type::Ptr& type )
{
    auto ref = libstdhl::Memory::make< Reference >( name, type, Reference::OUTPUT );
    add( ref );
    return ref;
}

Reference::Ptr CallableUnit::link( const std::string& name, const Type::Ptr& type )
{
    auto ref = libstdhl::Memory::make< Reference >( name, type, Reference::LINKAGE );
    add( ref );
    return ref;
}

const std::vector< Reference::Ptr >& CallableUnit::inputs( void ) const
{
    return m_references[ Reference::INPUT ];
}

const std::vector< Reference::Ptr >& CallableUnit::outputs( void ) const
{
    return m_references[ Reference::OUTPUT ];
}

const std::vector< Reference::Ptr >& CallableUnit::linkage( void ) const
{
    return m_references[ Reference::LINKAGE ];
}

u16 CallableUnit::indexOf( const Reference::Ptr& reference ) const
{
    auto result = m_name2index.find( reference->name() );
    if( result == m_name2index.end() )
    {
        throw std::domain_error(
            "reference '" + reference->description() + "' does not belong to this callable '" +
            this->description() + "'" );
    }

    return result->second;
}

u1 CallableUnit::isLast( const Reference::Ptr& reference ) const
{
    i32 index = indexOf( reference );
    i32 total = length();

    if( not reference->isInput() )
    {
        index += m_references[ Reference::INPUT ].size();
    }

    return index >= ( total - 1 );
}

u16 CallableUnit::length( void ) const
{
    return m_references[ Reference::INPUT ].size() + m_references[ Reference::OUTPUT ].size();
}

Reference::Ptr CallableUnit::reference( const std::string& name ) const
{
    auto result = m_name2ref.find( name );
    if( result != m_name2ref.end() )
    {
        return result->second.lock();
    }

    return nullptr;
}

bool CallableUnit::classof( Value const* obj )
{
    return obj->id() == classid() or Intrinsic::classof( obj ) or Function::classof( obj );
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
