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

#include "CallableUnit.h"
#include "Function.h"
#include "Intrinsic.h"
#include "Reference.h"

using namespace libcsel_ir;

u64 CallableUnit::m_allocation_cnt = 0;

CallableUnit::CallableUnit( const char* name, Type* type, Value::ID id )
: User( name, type, id )
, m_context( 0 )
, m_identifier( 0 )
, m_allocation_id( BitConstant( Type::TypeID(), m_allocation_cnt ) )
{
    m_allocation_cnt++;
}

CallableUnit::~CallableUnit( void )
{
}

BitConstant& CallableUnit::allocId( void )
{
    return m_allocation_id;
}

Block* CallableUnit::context( void ) const
{
    return m_context;
}

void CallableUnit::setContext( Block* scope )
{
    assert( scope );

    m_context = scope;

    scope->setParent( this );
}

const Identifier* CallableUnit::identifier( void ) const
{
    assert( m_identifier );
    return m_identifier;
}

Reference* CallableUnit::add(
    const char* ref_name, Type* ref_type, u8 ref_kind )
{
    Reference* ref
        = new Reference( ref_name, ref_type, this, (Reference::Kind)ref_kind );
    assert( ref );

    m_name2ref[ ref_name ] = ref;

    return ref;
}

Reference* CallableUnit::in( const char* ref_name, Type* ref_type )
{
    return add( ref_name, ref_type, Reference::INPUT );
}

Reference* CallableUnit::out( const char* ref_name, Type* ref_type )
{
    return add( ref_name, ref_type, Reference::OUTPUT );
}

Reference* CallableUnit::link( const char* ref_name, Type* ref_type )
{
    return add( ref_name, ref_type, Reference::LINKAGE );
}

void CallableUnit::addParameter( Value* value, u1 input )
{
    assert( value );
    assert( isa< Reference >( value ) );

    Reference* ref = (Reference*)value;
    ref->setCallableUnit( this );

    if( input )
    {
        m_parameter2index[ value ] = m_parameter_in.size();
        m_parameter_in.push_back( value );
    }
    else
    {
        m_parameter2index[ value ] = m_parameter_out.size();
        m_parameter_out.push_back( value );
    }
}

void CallableUnit::addLinkage( Value* value )
{
    assert( value );
    assert( isa< Reference >( value ) );

    Reference* ref = (Reference*)value;
    ref->setCallableUnit( this );

    m_linkage.push_back( value );
}

const std::vector< Value* >& CallableUnit::inParameters( void ) const
{
    return m_parameter_in;
}

const std::vector< Value* >& CallableUnit::outParameters( void ) const
{
    return m_parameter_out;
}

const i16 CallableUnit::indexOfParameter( Value* value ) const
{
    auto result = m_parameter2index.find( value );
    if( result != m_parameter2index.end() )
    {
        return result->second;
    }

    return -1;
}

const u1 CallableUnit::isLastParameter( Value* value ) const
{
    assert( isa< Reference >( value ) );
    Reference* ref = (Reference*)value;

    i16 index = indexOfParameter( ref );
    assert( index >= 0 );

    i16 total = parameterLength();

    if( not ref->isInput() )
    {
        index += m_parameter_in.size();
    }

    return index >= ( total - 1 );
}

const i16 CallableUnit::parameterLength( void ) const
{
    return m_parameter_in.size() + m_parameter_out.size();
}

const std::vector< Value* >& CallableUnit::linkage( void ) const
{
    return m_linkage;
}

const Reference* CallableUnit::reference( const char* name ) const
{
    auto result = m_name2ref.find( name );
    if( result != m_name2ref.end() )
    {
        assert( isa< Reference >( result->second ) );
        return (const Reference*)result->second;
    }

    return 0;
}

bool CallableUnit::classof( Value const* obj )
{
    return obj->id() == classid() or Intrinsic::classof( obj )
           or Function::classof( obj );
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
