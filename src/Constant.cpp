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

#include "Constant.h"

#include "Structure.h"

#include "../stdhl/cpp/Integer.h"

using namespace libcsel_ir;

Constant::Constant( const std::string& name, const Type::Ptr& type,
    const libstdhl::Type& data, const std::vector< Constant >& constants,
    Value::ID id )
: Value( name, type, id )
, m_data( data )
, m_constants( constants )
{
}

bool Constant::classof( Value const* obj )
{
    return obj->id() == classid() or VoidConstant::classof( obj )
           or BitConstant::classof( obj ) or StringConstant::classof( obj )
           or StructureConstant::classof( obj ) or Identifier::classof( obj );
}

//
// Constants
//

//
// Void Constant
//

VoidConstant::VoidConstant( void )
: Constant(
      "void", libstdhl::get< VoidType >(), libstdhl::Type(), {}, classid() )
{
}

bool VoidConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Bit Constant
//

BitConstant::BitConstant( const BitType::Ptr& type, u64 value )
: Constant( "", type, libstdhl::Type( value, type->bitsize() ), {}, classid() )
{
    assert( m_data.words().size() == type->wordsize() );

    if( type->bitsize() > BitType::SizeMax )
    {
        throw std::domain_error( "invalid bit size '"
                                 + std::to_string( type->bitsize() )
                                 + "' to create 'BitConstant'" );
    }
}

BitConstant::BitConstant( u16 bitsize, u64 value )
: BitConstant( libstdhl::get< BitType >( bitsize ), value )
{
}

std::string BitConstant::literal( libstdhl::Type::Radix radix ) const
{
    return m_data.to_string( radix );
}

const std::vector< u64 >& BitConstant::value( void ) const
{
    return m_data.words();
}

bool BitConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// String Constant
//

StringConstant::StringConstant( const std::string& value )
: Constant(
      value, libstdhl::get< StringType >(), libstdhl::Type(), {}, classid() )
{
}

std::string StringConstant::value( void ) const
{
    return name();
}

bool StringConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Structure Constant
//

StructureConstant::StructureConstant(
    const StructureType::Ptr& type, const std::vector< Constant >& values )
: Constant( "", type, libstdhl::Type(), values, classid() )
{
    assert( type->results().size() == values.size() );

    m_name = "{";

    for( u32 i = 0; i < values.size(); i++ )
    {
        assert( *type->results()[ i ] == values[ i ].type() );

        if( i > 0 )
        {
            m_name += ", ";
        }

        m_name += values[ i ].name();
    }

    m_name += "}";
}

StructureConstant::StructureConstant(
    const Structure::Ptr& kind, const std::vector< Constant >& values )
: StructureConstant( libstdhl::get< StructureType >( kind ), values )
{
}

std::vector< Constant > StructureConstant::value( void ) const
{
    return m_constants;
}

bool StructureConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Identifier (Constant)
//

Identifier::Identifier( const Type::Ptr& type, const std::string& value )
: Constant( value, type, libstdhl::Type(), {}, classid() )
{
}

bool Identifier::classof( Value const* obj )
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
