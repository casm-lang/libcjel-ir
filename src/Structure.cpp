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

#include "Structure.h"

using namespace libcsel_ir;

Structure::Structure( const char* name, Type* type, Structure* parent )
: User( name, type, Value::STRUCTURE )
, m_identifier( 0 )
, m_parent( parent )
{
    assert( name );

    assert( !" PPA: continue here!!! " );
    // if( !type )
    // {
    //     Type* ty = new Type( Type::STRUCTURE );
    //     assert( ty );
    //     setType( ty );
    // }

    m_identifier = Identifier::create( type, name, parent );
    assert( m_identifier );

    if( m_parent )
    {
        m_parent->add( this );
    }
}

Structure::~Structure( void )
{
}

const Identifier* Structure::identifier( void ) const
{
    return m_identifier;
}

void Structure::add( Value* value )
{
    assert( value );
    assert( isa< Structure >( value ) );
    Structure* s = (Structure*)value;
    // s->setParent( this );

    m_element.push_back( s );

    // getType()->addSubType( value->getType() );

    assert( s->parent() == this );
}

Value* Structure::at( u16 index ) const
{
    assert( index < m_element.size() );

    return m_element[ index ];
}

const std::vector< Structure* >& Structure::elements( void ) const
{
    return m_element;
}

void Structure::setParent( Structure* value )
{
    assert( !m_parent );
    m_parent = value;
}

Structure* Structure::parent( void ) const
{
    return m_parent;
}

bool Structure::classof( Value const* obj )
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
