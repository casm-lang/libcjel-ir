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

#include "Type.h"

#include "../stdhl/cpp/Allocator.h"

using namespace libcsel_ir;

Type::Type(
    const char* name, const char* description, u64 bitsize, Type::ID id )
: m_name( name )
, m_description( description )
, m_bitsize( bitsize )
, m_id( id )
, m_hash( 0 )
{
}

const Type::ID Type::id( void ) const
{
    return m_id;
}

const char* Type::make_hash( void )
{
    if( not m_hash )
    {
        std::string tmp;
        tmp += "t:";
        tmp += std::to_string( id() );
        tmp += ":";
        tmp += name();

        m_hash = libstdhl::Allocator::string( tmp );
    }

    return m_hash;
}

u1 Type::isLabel( void ) const
{
    return id() == Type::LABEL;
}
u1 Type::isVoid( void ) const
{
    return id() == Type::VOID;
}
u1 Type::isBit( void ) const
{
    return id() == Type::BIT;
}
u1 Type::isString( void ) const
{
    return id() == Type::STRING;
}
u1 Type::isVector( void ) const
{
    return id() == Type::VECTOR;
}
u1 Type::isStructure( void ) const
{
    return id() == Type::STRUCTURE;
}
u1 Type::isRelation( void ) const
{
    return id() == Type::RELATION;
}
u1 Type::isInterconnect( void ) const
{
    return id() == Type::INTERCONNECT;
}

Type* Type::Label( void )
{
    static LabelType cache = LabelType();
    return m_str2obj().emplace( cache.name(), &cache ).first->second;
}

Type* Type::Void( void )
{
    static VoidType cache = VoidType();
    return m_str2obj().emplace( cache.name(), &cache ).first->second;
}

Type* Type::TypeID( void )
{
    return Bit( 64 );
}

Type* Type::Bit( u16 bitsize )
{
    BitType tmp( bitsize );

    auto cache = m_str2obj().find( tmp.name() );
    if( cache != m_str2obj().end() )
    {
        return cache->second;
    }

    Type* ptr = new BitType( tmp );
    m_str2obj()[ tmp.name() ] = ptr;
    return ptr;
}

Type* Type::String( void )
{
    static StringType cache = StringType();
    return m_str2obj().emplace( cache.name(), &cache ).first->second;
}

Type* Type::Vector( Type* type, u16 length )
{
    VectorType tmp = VectorType( type, length );

    auto cache = m_str2obj().find( tmp.name() );
    if( cache != m_str2obj().end() )
    {
        return cache->second;
    }

    Type* ptr = new VectorType( tmp );
    m_str2obj()[ tmp.name() ] = ptr;
    return ptr;
}

Type* Type::Structure( std::vector< StructureElement > elements )
{
    StructureType tmp( elements );

    auto cache = m_str2obj().find( tmp.name() );
    if( cache != m_str2obj().end() )
    {
        return cache->second;
    }

    Type* ptr = new StructureType( tmp );
    m_str2obj()[ tmp.name() ] = ptr;
    return ptr;
}

Type* Type::Relation(
    std::vector< Type* > result, std::vector< Type* > arguments )
{
    RelationType tmp( result, arguments );

    auto cache = m_str2obj().find( tmp.name() );
    if( cache != m_str2obj().end() )
    {
        return cache->second;
    }

    Type* ptr = new RelationType( tmp );
    m_str2obj()[ tmp.name() ] = ptr;
    return ptr;
}

Type* Type::Interconnect( void )
{
    static InterconnectType cache = InterconnectType();
    return m_str2obj().emplace( cache.name(), &cache ).first->second;
}

//
// PrimitiveType
//

PrimitiveType::PrimitiveType(
    const char* name, const char* description, u64 bitsize, Type::ID id )
: Type( name, description, bitsize, id )
{
}

const u64 PrimitiveType::bitsize( void )
{
    return m_bitsize;
}

const char* PrimitiveType::name( void )
{
    return m_name;
}

const char* PrimitiveType::description( void )
{
    return m_description;
}

const std::vector< Type* >& PrimitiveType::results( void )
{
    if( m_results.size() == 0 )
    {
        m_results.push_back( this );
    }
    return m_results;
}

const std::vector< Type* >& PrimitiveType::arguments( void )
{
    static std::vector< Type* > empty = {};
    return empty;
}

//
// LabelType
//

LabelType::LabelType()
: PrimitiveType( "label", "Label", 0, Type::LABEL )
{
}

//
// VoidType
//

VoidType::VoidType()
: PrimitiveType( "void", "Void", 0, Type::VOID )
{
}

//
// BitType
//

BitType::BitType( u16 bitsize )
: PrimitiveType( libstdhl::Allocator::string( "u" + std::to_string( bitsize ) ),
      libstdhl::Allocator::string( "Bit(" + std::to_string( bitsize ) + ")" ),
      bitsize, Type::BIT )
{
    assert( m_bitsize >= 1 and m_bitsize <= BitType::SizeMax );
}

//
// StringType
//

StringType::StringType()
: PrimitiveType( "s", "String", 0 /*PPA todo*/, Type::STRING )
{
}

//
// AggregateType
//

AggregateType::AggregateType(
    const char* name, const char* description, u64 bitsize, Type::ID id )
: Type( name, description, bitsize, id )
{
}

//
// VectorType
//

VectorType::VectorType( Type* type, u16 length )
: AggregateType( "v", "Vector", type->bitsize() * length, Type::VECTOR )
, m_type( type )
, m_length( length )
{
}

const u64 VectorType::bitsize( void )
{
    return m_bitsize;
}

const char* VectorType::name( void )
{
    if( not m_name )
    {
        std::string tmp = "< ";
        tmp += m_type->name();
        tmp += " x ";
        tmp += std::to_string( m_length );
        tmp += ">";
        m_name = libstdhl::Allocator::string( tmp );
    }

    return m_name;
}

const char* VectorType::description( void )
{
    if( not m_description )
    {
        std::string tmp = "< ";
        tmp += m_type->description();
        tmp += " x ";
        tmp += std::to_string( m_length );
        tmp += ">";
        m_description = libstdhl::Allocator::string( tmp );
    }

    return m_description;
}

const std::vector< Type* >& VectorType::results( void )
{
    if( m_results.size() == 0 )
    {
        for( u32 i = 0; i < m_length; i++ )
        {
            m_results.push_back( m_type );
        }
    }
    return m_results;
}

const std::vector< Type* >& VectorType::arguments( void )
{
    static std::vector< Type* > empty = {};
    return empty;
}

//
// StructureType
//

StructureType::StructureType( std::vector< StructureElement > elements )
: AggregateType( 0, 0, 0, Type::STRUCTURE )
, m_elements( elements )
{
}

const u64 StructureType::bitsize( void )
{
    if( not m_bitsize )
    {
        u64 tmp = 0;

        for( auto element : m_elements )
        {
            tmp += element.type->bitsize();
        }

        assert( tmp != 0 );
        m_bitsize = tmp;
    }

    return m_bitsize;
}

const char* StructureType::name( void )
{
    if( not m_name )
    {
        u1 first = true;
        std::string tmp = "[";
        for( auto element : m_elements )
        {
            if( not first )
            {
                tmp += ", ";
            }
            tmp += element.type->name();
            tmp += " : ";
            tmp += element.name;
            first = false;
        }
        tmp += "]";
        m_name = libstdhl::Allocator::string( tmp );
    }

    return m_name;
}

const char* StructureType::description( void )
{
    if( not m_description )
    {
        u1 first = true;
        std::string tmp = "[";
        for( auto element : m_elements )
        {
            if( not first )
            {
                tmp += ", ";
            }
            tmp += element.type->description();
            tmp += " : ";
            tmp += element.name;
            first = false;
        }
        tmp += "]";
        m_description = libstdhl::Allocator::string( tmp );
    }

    return m_description;
}

const std::vector< StructureElement >& StructureType::elements( void ) const
{
    return m_elements;
}

const std::vector< Type* >& StructureType::results( void )
{
    if( m_results.size() == 0 )
    {
        for( auto element : m_elements )
        {
            m_results.push_back( element.type );
        }
    }
    return m_results;
}

const std::vector< Type* >& StructureType::arguments( void )
{
    static std::vector< Type* > empty = {};
    return empty;
}

//
// RelationType
//

RelationType::RelationType(
    std::vector< Type* > results, std::vector< Type* > arguments )
: Type( 0, 0, 0, Type::RELATION )
, m_arguments( arguments )
{
    m_results = std::move( results );
}

const u64 RelationType::bitsize( void )
{
    return m_bitsize;
}

const char* RelationType::name( void )
{
    if( not m_name )
    {
        u1 first = true;
        std::string tmp = "(";
        for( auto argument : m_arguments )
        {
            if( not first )
            {
                tmp += ", ";
            }
            tmp += argument->name();
            first = false;
        }
        tmp += " -> ";

        first = true;
        for( auto result : m_results )
        {
            if( not first )
            {
                tmp += ", ";
            }
            tmp += result->name();
            first = false;
        }
        tmp += ")";

        m_name = libstdhl::Allocator::string( tmp );
    }

    return m_name;
}

const char* RelationType::description( void )
{
    if( not m_description )
    {
        u1 first = true;
        std::string tmp = "(";
        for( auto argument : m_arguments )
        {
            if( not first )
            {
                tmp += " x ";
            }
            tmp += argument->description();
            first = false;
        }

        tmp += " -> ";

        first = true;
        for( auto result : m_results )
        {
            if( not first )
            {
                tmp += ", ";
            }
            tmp += result->description();
            first = false;
        }
        tmp += ")";

        m_description = libstdhl::Allocator::string( tmp );
    }

    return m_description;
}

const std::vector< Type* >& RelationType::results( void )
{
    return m_results;
}

const std::vector< Type* >& RelationType::arguments( void )
{
    return m_arguments;
}

//
// SyntheticType
//

SyntheticType::SyntheticType(
    const char* name, const char* description, u64 bitsize, Type::ID id )
: Type( name, description, bitsize, id )
{
}

const u64 SyntheticType::bitsize( void )
{
    return m_bitsize;
}

const char* SyntheticType::name( void )
{
    return m_name;
}

const char* SyntheticType::description( void )
{
    return m_description;
}

const std::vector< Type* >& SyntheticType::results( void )
{
    static std::vector< Type* > empty = {};
    return empty;
}

const std::vector< Type* >& SyntheticType::arguments( void )
{
    static std::vector< Type* > empty = {};
    return empty;
}

//
// InterconnectType
//

InterconnectType::InterconnectType()
: SyntheticType( "x", "Interconnect", 0, Type::INTERCONNECT )
{
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
