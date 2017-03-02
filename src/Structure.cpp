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

Structure::Structure(
    const std::string& name, const std::vector< StructureElement >& elements )
: User( name, libstdhl::get< VoidType >(), classid() )
, m_elements( elements )
{
    if( elements.size() == 0 )
    {
        throw std::domain_error(
            "element size of structure '" + name + "' cannot be '0'" );
    }

    for( std::size_t c = 0; c < elements.size(); c++ )
    {
        const auto element = std::get< 1 >( elements[ c ] );

        if( not m_element2index.emplace( element, c ).second )
        {
            throw std::domain_error(
                "structure '" + name + "' already has an element '" + element
                + "'" );
        }
    }
}

StructureElement Structure::element( std::size_t index ) const
{
    if( index >= m_elements.size() )
    {
        throw std::domain_error( "structure does not have a element at index '"
                                 + std::to_string( index )
                                 + "'" );
    }

    return m_elements[ index ];
}

StructureElement Structure::element( const std::string& name ) const
{
    const auto result = m_element2index.find( name );
    if( result == m_element2index.end() )
    {
        throw std::domain_error(
            "structure '" + this->name() + "' does not have a element '" + name
            + "'" );
    }

    return m_elements[ result->second ];
}

std::vector< StructureElement > Structure::elements( void ) const
{
    return m_elements;
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
