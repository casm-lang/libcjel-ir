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

#include "Reference.h"

using namespace libcsel_ir;

Reference::Reference(
    const char* name, Type* type, CallableUnit* callable, Kind kind )
: User( ".reference", type, Value::REFERENCE )
, m_identifier( 0 )
, m_callable( 0 )
, m_kind( kind )
, m_structure( 0 )
{
    assert( name );
    assert( type );

    // m_identifier = Identifier::create( type, name, callable /* scope?!?!*/ );
    // assert( m_identifier );

    if( callable )
    {
        if( kind != LINKAGE )
        {
            callable->addParameter( this, kind == INPUT );
        }
        else
        {
            callable->addLinkage( this );
        }
    }
}

Reference::~Reference( void )
{
}

const Identifier* Reference::identifier( void ) const
{
    assert( m_identifier );
    return m_identifier;
}

const CallableUnit* Reference::callableUnit( void ) const
{
    return m_callable;
}

void Reference::setCallableUnit( CallableUnit* value )
{
    assert( !m_callable );
    m_callable = value;
}

const u1 Reference::isInput( void ) const
{
    return m_kind == INPUT;
}

const u1 Reference::isOutput( void ) const
{
    return m_kind == OUTPUT;
}

const u1 Reference::isLinkage( void ) const
{
    return m_kind == LINKAGE;
}

const Structure* Reference::structure( void ) const
{
    assert( m_structure );
    return m_structure;
}

const u1 Reference::isStructure( void ) const
{
    return m_structure != 0;
}

bool Reference::classof( Value const* obj )
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
