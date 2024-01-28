//
//  Copyright (C) 2015-2024 CASM Organization <https://casm-lang.org>
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

#include "Constant.h"

#include <cassert>
#include <libcjel-ir/Structure>
#include <libcjel-ir/Type>
#include <libstdhl/Memory>
#include <libstdhl/data/type/Integer>

using namespace libcjel_ir;

static const auto VOID_TYPE = libstdhl::Memory::make< VoidType >();

//
//
// Constant
//

Constant::Constant(
    const std::string& name,
    const Type::Ptr& type,
    const libstdhl::Type::Data& data,
    const std::vector< Constant >& constants,
    Value::ID id )
: Value( name, type, id )
, m_data( data )
, m_constants( constants )
{
}

std::size_t Constant::hash( void ) const
{
    switch( id() )
    {
        case Value::VOID_CONSTANT:
        {
            return static_cast< const VoidConstant* >( this )->hash();
        }
        case Value::BIT_CONSTANT:
        {
            return static_cast< const BitConstant* >( this )->hash();
        }
        case Value::STRING_CONSTANT:
        {
            return static_cast< const StringConstant* >( this )->hash();
        }
        case Value::STRUCTURE_CONSTANT:
        {
            return static_cast< const StructureConstant* >( this )->hash();
        }
        default:
        {
            throw std::domain_error( "unsupported" );
            return 0;
        }
    }
}

bool Constant::classof( Value const* obj )
{
    return obj->id() == classid() or VoidConstant::classof( obj ) or BitConstant::classof( obj ) or
           StringConstant::classof( obj ) or StructureConstant::classof( obj ) or
           Identifier::classof( obj );
}

//
// Void Constant
//

VoidConstant::VoidConstant( void )
: Constant( "void", VOID_TYPE, libstdhl::Type::Data(), {}, classid() )
{
}

std::size_t VoidConstant::hash( void ) const
{
    const auto h = ( (std::size_t)classid() ) << 1;
    return h;
}

bool VoidConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Bit Constant
//

BitConstant::BitConstant( const Type::Ptr& type, u64 value )
: Constant( "", type, libstdhl::Type::Data( value, 0 ), {}, classid() )
{
    if( not type->isBit() )
    {
        throw std::domain_error( "invalid type '" + type->name() + "' for a bit constant" );
    }

    if( type->bitsize() > BitType::SizeMax )
    {
        throw std::domain_error(
            "invalid bit size '" + std::to_string( type->bitsize() ) +
            "' to create 'BitConstant'" );
    }
}

BitConstant::BitConstant( u16 bitsize, u64 value )
: BitConstant( libstdhl::Memory::get< BitType >( bitsize ), value )
{
}

std::string BitConstant::literal( libstdhl::Type::Radix radix ) const
{
    return m_data.to_string( radix );
}

const libstdhl::Type::Data& BitConstant::value( void ) const
{
    return m_data;
}

std::size_t BitConstant::hash( void ) const
{
    const auto h = ( (std::size_t)classid() ) << 1;
    return h;
}

bool BitConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// String Constant
//

StringConstant::StringConstant( const std::string& value )
: Constant( value, libstdhl::Memory::get< StringType >(), libstdhl::Type::Data(), {}, classid() )
{
}

std::string StringConstant::value( void ) const
{
    return name();
}

std::size_t StringConstant::hash( void ) const
{
    const auto h = ( (std::size_t)classid() ) << 1;
    return h;
}

bool StringConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Structure Constant
//

StructureConstant::StructureConstant( const Type::Ptr& type, const std::vector< Constant >& values )
: Constant( "", type, libstdhl::Type::Data(), values, classid() )
{
    assert( this->type().results().size() == values.size() );

    if( not type->isStructure() )
    {
        throw std::domain_error( "invalid type '" + type->name() + "' for a structure constant" );
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
: StructureConstant( libstdhl::Memory::get< StructureType >( kind ), values )
{
}

std::vector< Constant > StructureConstant::value( void ) const
{
    return m_constants;
}

std::size_t StructureConstant::hash( void ) const
{
    const auto h = ( (std::size_t)classid() ) << 1;
    return h;
}

bool StructureConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Identifier (Constant)
//

Identifier::Identifier( const Type::Ptr& type, const std::string& value )
: Constant( value, type, libstdhl::Type::Data(), {}, classid() )
{
}

std::size_t Identifier::hash( void ) const
{
    const auto h = ( (std::size_t)classid() ) << 1;
    return h;
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
