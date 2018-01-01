//
//  Copyright (C) 2015-2018 CASM Organization <https://casm-lang.org>
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

#include "Constant.h"

#include "Structure.h"

#include "../stdhl/cpp/Integer.h"

using namespace libcjel_ir;

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

BitConstant::BitConstant( const Type::Ptr& type, u64 value )
: Constant( "", type, libstdhl::Type( value, type->bitsize() ), {}, classid() )
{
    if( not type->isBit() )
    {
        throw std::domain_error(
            "invalid type '" + type->name() + "' for a bit constant" );
    }

    assert( m_data.size() <= type->wordsize() );

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

const libstdhl::Type& BitConstant::value( void ) const
{
    return m_data;
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
    const Type::Ptr& type, const std::vector< Constant >& values )
: Constant( "", type, libstdhl::Type(), values, classid() )
{
    assert( this->type().results().size() == values.size() );

    if( not type->isStructure() )
    {
        throw std::domain_error(
            "invalid type '" + type->name() + "' for a structure constant" );
    }

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
