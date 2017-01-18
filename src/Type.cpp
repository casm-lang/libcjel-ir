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

Type::Type( const char* name, const char* description, u64 size, Type::ID id )
: name( name )
, description( description )
, size( size )
, id( id )
{
}

const Type::ID Type::getID( void ) const
{
    return id;
}

u1 Type::isLabel( void ) const
{
    return getID() == Type::LABEL;
}
u1 Type::isBit( void ) const
{
    return getID() == Type::BIT;
}
u1 Type::isString( void ) const
{
    return getID() == Type::STRING;
}
u1 Type::isVector( void ) const
{
    return getID() == Type::VECTOR;
}
u1 Type::isStructure( void ) const
{
    return getID() == Type::STRUCTURE;
}
u1 Type::isRelation( void ) const
{
    return getID() == Type::RELATION;
}
u1 Type::isInterconnect( void ) const
{
    return getID() == Type::INTERCONNECT;
}

Type* Type::getLabel( void )
{
    static LabelType cache = LabelType();
    return str2obj().emplace( cache.getName(), &cache ).first->second;
}

Type* Type::getTypeID( void )
{
    return getBit( 64 );
}

Type* Type::getBit( u16 size )
{
    BitType tmp( size );

    auto cache = str2obj().find( tmp.getName() );
    if( cache != str2obj().end() )
    {
        return cache->second;
    }

    Type* ptr = new BitType( tmp );
    str2obj()[ tmp.getName() ] = ptr;
    return ptr;
}

Type* Type::getString( void )
{
    static StringType cache = StringType();
    return str2obj().emplace( cache.getName(), &cache ).first->second;
}

Type* Type::getVector( Type* type, u16 length )
{
    VectorType tmp = VectorType( type, length );

    auto cache = str2obj().find( tmp.getName() );
    if( cache != str2obj().end() )
    {
        return cache->second;
    }

    Type* ptr = new VectorType( tmp );
    str2obj()[ tmp.getName() ] = ptr;
    return ptr;
}

Type* Type::getStructure( std::vector< StructureElement > elements )
{
    StructureType tmp( elements );

    auto cache = str2obj().find( tmp.getName() );
    if( cache != str2obj().end() )
    {
        return cache->second;
    }

    Type* ptr = new StructureType( tmp );
    str2obj()[ tmp.getName() ] = ptr;
    return ptr;
}

Type* Type::getRelation(
    std::vector< Type* > result, std::vector< Type* > arguments )
{
    RelationType tmp( result, arguments );

    auto cache = str2obj().find( tmp.getName() );
    if( cache != str2obj().end() )
    {
        return cache->second;
    }

    Type* ptr = new RelationType( tmp );
    str2obj()[ tmp.getName() ] = ptr;
    return ptr;
}

Type* Type::getInterconnect( void )
{
    static InterconnectType cache = InterconnectType();
    return str2obj().emplace( cache.getName(), &cache ).first->second;
}

//
// PrimitiveType
//

PrimitiveType::PrimitiveType(
    const char* name, const char* description, u64 size, Type::ID id )
: Type( name, description, size, id )
{
}

const u64 PrimitiveType::getSize( void )
{
    return size;
}

const char* PrimitiveType::getName( void )
{
    return name;
}

const char* PrimitiveType::getDescription( void )
{
    return description;
}

const std::vector< Type* >& PrimitiveType::getResults( void )
{
    static std::vector< Type* > single = { this };
    return single;
}

const std::vector< Type* >& PrimitiveType::getArguments( void )
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
// BitType
//

BitType::BitType( u16 size )
: PrimitiveType( libstdhl::Allocator::string( "u" + std::to_string( size ) ),
      libstdhl::Allocator::string( "Bit(" + std::to_string( size ) + ")" ),
      size, Type::BIT )
{
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
    const char* name, const char* description, u64 size, Type::ID id )
: Type( name, description, size, id )
{
}

//
// VectorType
//

VectorType::VectorType( Type* type, u16 length )
: AggregateType( "v", "Vector", type->getSize() * length, Type::VECTOR )
, type( type )
, length( length )
{
}

const u64 VectorType::getSize( void )
{
    return size;
}

const char* VectorType::getName( void )
{
    if( not name )
    {
        std::string tmp = "< ";
        tmp += type->getName();
        tmp += " x ";
        tmp += std::to_string( length );
        tmp += ">";
        name = libstdhl::Allocator::string( tmp );
    }

    return name;
}

const char* VectorType::getDescription( void )
{
    if( not description )
    {
        std::string tmp = "< ";
        tmp += type->getDescription();
        tmp += " x ";
        tmp += std::to_string( length );
        tmp += ">";
        description = libstdhl::Allocator::string( tmp );
    }

    return description;
}

const std::vector< Type* >& VectorType::getResults( void )
{
    static std::vector< Type* > vector = {};

    if( vector.size() == 0 )
    {
        for( u32 i = 0; i < length; i++ )
        {
            vector.push_back( type );
        }
    }

    return vector;
}

const std::vector< Type* >& VectorType::getArguments( void )
{
    static std::vector< Type* > empty = {};
    return empty;
}

//
// StructureType
//

StructureType::StructureType( std::vector< StructureElement > elements )
: AggregateType( 0, 0, 0, Type::STRUCTURE )
, elements( elements )
{
}

const u64 StructureType::getSize( void )
{
    if( not size )
    {
        u64 tmp = 0;

        for( auto element : elements )
        {
            tmp += element.type->getSize();
        }

        assert( tmp != 0 );
        size = tmp;
    }

    return size;
}

const char* StructureType::getName( void )
{
    if( not name )
    {
        u1 first = true;
        std::string tmp = "[";
        for( auto element : elements )
        {
            if( not first )
            {
                tmp += ", ";
            }
            tmp += element.type->getName();
            tmp += " : ";
            tmp += element.name;
            first = false;
        }
        tmp += "]";
        name = libstdhl::Allocator::string( tmp );
    }

    return name;
}

const char* StructureType::getDescription( void )
{
    if( not description )
    {
        u1 first = true;
        std::string tmp = "[";
        for( auto element : elements )
        {
            if( not first )
            {
                tmp += ", ";
            }
            tmp += element.type->getDescription();
            tmp += " : ";
            tmp += element.name;
            first = false;
        }
        tmp += "]";
        description = libstdhl::Allocator::string( tmp );
    }

    return description;
}

const std::vector< StructureElement >& StructureType::getElements( void ) const
{
    return elements;
}

const std::vector< Type* >& StructureType::getResults( void )
{
    static std::vector< Type* > structure = {};

    if( structure.size() == 0 )
    {
        for( auto element : elements )
        {
            structure.push_back( element.type );
        }
    }

    return structure;
}

const std::vector< Type* >& StructureType::getArguments( void )
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
, results( results )
, arguments( arguments )
{
}

const u64 RelationType::getSize( void )
{
    return size;
}

const char* RelationType::getName( void )
{
    if( not name )
    {
        u1 first = true;
        std::string tmp = "(";
        for( auto argument : arguments )
        {
            if( not first )
            {
                tmp += ", ";
            }
            tmp += argument->getName();
            first = false;
        }
        tmp += " -> ";

        first = true;
        for( auto result : results )
        {
            if( not first )
            {
                tmp += ", ";
            }
            tmp += result->getName();
            first = false;
        }
        tmp += ")";

        name = libstdhl::Allocator::string( tmp );
    }

    return name;
}

const char* RelationType::getDescription( void )
{
    if( not description )
    {
        u1 first = true;
        std::string tmp = "(";
        for( auto argument : arguments )
        {
            if( not first )
            {
                tmp += " x ";
            }
            tmp += argument->getDescription();
            first = false;
        }

        tmp += " -> ";

        first = true;
        for( auto result : results )
        {
            if( not first )
            {
                tmp += ", ";
            }
            tmp += result->getDescription();
            first = false;
        }
        tmp += ")";

        description = libstdhl::Allocator::string( tmp );
    }

    return description;
}

const std::vector< Type* >& RelationType::getResults( void )
{
    return results;
}

const std::vector< Type* >& RelationType::getArguments( void )
{
    return arguments;
}

//
// SyntheticType
//

SyntheticType::SyntheticType(
    const char* name, const char* description, u64 size, Type::ID id )
: Type( name, description, size, id )
{
}

const u64 SyntheticType::getSize( void )
{
    return size;
}

const char* SyntheticType::getName( void )
{
    return name;
}

const char* SyntheticType::getDescription( void )
{
    return description;
}

const std::vector< Type* >& SyntheticType::getResults( void )
{
    static std::vector< Type* > empty = {};
    return empty;
}

const std::vector< Type* >& SyntheticType::getArguments( void )
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
