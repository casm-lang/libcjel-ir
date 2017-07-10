//
//  Copyright (c) 2015-2017 Philipp Paulweber
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                https://github.com/casm-lang/libcjel-ir
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

#include "Type.h"

#include "Structure.h"

using namespace libcjel_ir;

Type::Type( const std::string& name, const std::string& description,
    u64 bitsize, Type::ID id )
: m_name( name )
, m_description( description )
, m_bitsize( bitsize )
, m_id( id )
{
}

std::string Type::name( void ) const
{
    return m_name;
}

std::string Type::description( void ) const
{
    return m_description;
}

Type::ID Type::id( void ) const
{
    return m_id;
}

u64 Type::bitsize( void ) const
{
    return m_bitsize;
}

u64 Type::wordsize( const u64 wordbits ) const
{
    return ( ( bitsize() - 1 ) / wordbits ) + 1;
}

const Types& Type::results( void ) const
{
    return m_results;
}

Types Type::ptr_results( void ) const
{
    return results();
}

const Types& Type::arguments( void ) const
{
    static Types empty = {};

    if( isRelation() )
    {
        return static_cast< const RelationType* >( this )->arguments();
    }

    return empty;
}

Types Type::ptr_arguments( void ) const
{
    return arguments();
}

std::string Type::make_hash( void ) const
{
    return "t:" + std::to_string( id() ) + ":" + description();
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

//
// PrimitiveType
//

PrimitiveType::PrimitiveType( const std::string& name,
    const std::string& description, u64 bitsize, Type::ID id )
: Type( name, description, bitsize, id )
{
}

//
// LabelType
//

LabelType::LabelType( void )
: PrimitiveType( "label", "Label", 0, Type::LABEL )
{
}

//
// VoidType
//

VoidType::VoidType( void )
: PrimitiveType( "void", "Void", 0, Type::VOID )
{
}

//
// BitType
//

BitType::BitType( u16 bitsize )
: PrimitiveType( "u" + std::to_string( bitsize ),
      "Bit(" + std::to_string( bitsize ) + ")", bitsize, Type::BIT )
{
    if( bitsize < 1 )
    {
        throw std::domain_error(
            "bit size of 'BitType' shall be greater or equal than '1'" );
    }
    else if( bitsize > BitType::SizeMax )
    {
        throw std::domain_error(
            "bit size of 'BitType' shall be smaller or equal than '"
            + std::to_string( BitType::SizeMax )
            + "'" );
    }
}

//
// StringType
//

StringType::StringType( void )
: PrimitiveType( "s", "String", 0, Type::STRING )
{
    // TODO: PPA: FIXME: IDEA: strings are either always without a size and it
    // is determined later in the compilation step or every string size
    // (bitsize) will be fixed by construction
}

//
// AggregateType
//

AggregateType::AggregateType( const std::string& name,
    const std::string& description, u64 bitsize, Type::ID id )
: Type( name, description, bitsize, id )
{
}

//
// VectorType
//

VectorType::VectorType( const Type::Ptr& type, u16 length )
: AggregateType( "v", "Vector", type->bitsize() * length, Type::VECTOR )
, m_type( type )
, m_length( length )
{
    if( length < 1 )
    {
        throw std::domain_error(
            "length of 'VectorType' shall be greater or equal than '1'" );
    }

    m_name = "< " + m_type->name() + " x " + std::to_string( m_length ) + " > ";

    m_description = "< " + m_type->description() + " x "
                    + std::to_string( m_length ) + " > ";

    for( u32 i = 0; i < m_length; i++ )
    {
        m_results.add( m_type );
    }
}

//
// StructureType
//

StructureType::StructureType( const Structure::Ptr& kind )
: AggregateType( "", "", 0, Type::STRUCTURE )
, m_kind( kind )
{
    if( not kind )
    {
        throw std::domain_error(
            "structure kind of 'StructureType' cannot be a null pointer" );
    }

    const auto elements = kind->elements();

    m_name = "[";
    m_description = "[";

    m_bitsize = 0;

    u1 first = true;
    for( const auto& element : elements )
    {
        auto t = std::get< 0 >( element );
        auto s = std::get< 1 >( element );

        m_results.add( t );

        m_bitsize += t->bitsize();

        if( not first )
        {
            m_name += ", ";
            m_description += ", ";
        }

        m_name += t->name() + " : " + s;
        m_description += t->description() + " : " + s;

        first = false;
    }

    m_name += "]";
    m_description += "]";

    if( m_bitsize == 0 )
    {
        throw std::domain_error( "bit-size of 'StructureType' cannot be '0'" );
    }
}

//
// RelationType
//

RelationType::RelationType( const std::vector< Type::Ptr >& results,
    const std::vector< Type::Ptr >& arguments )
: Type( "", "", 0, Type::RELATION )
, m_arguments( arguments )
{
    m_results = std::move( results );

    if( results.size() == 0 )
    {
        throw std::domain_error(
            "results size of 'RelationType' cannot be '0'" );
    }

    m_name = "(";
    m_description = "(";

    u1 first = true;
    for( auto argument : m_arguments )
    {
        if( not first )
        {
            m_name += ", ";
            m_description += " x ";
        }

        m_name += argument->name();
        m_description += argument->description();

        first = false;
    }

    m_name += " -> ";
    m_description += " -> ";

    m_bitsize = 0;

    first = true;
    for( auto result : m_results )
    {
        m_bitsize += result->bitsize();

        if( not first )
        {
            m_name += ", ";
            m_description += " x ";
        }

        m_name += result->name();
        m_description += result->description();

        first = false;
    }

    m_name += ")";
    m_description += ")";

    if( m_bitsize == 0 )
    {
        throw std::domain_error( "bit-size of 'StructureType' cannot be '0'" );
    }
}

const Types& RelationType::arguments( void ) const
{
    return m_arguments;
}

//
// SyntheticType
//

SyntheticType::SyntheticType( const std::string& name,
    const std::string& description, u64 bitsize, Type::ID id )
: Type( name, description, bitsize, id )
{
}

//
// InterconnectType
//

InterconnectType::InterconnectType( void )
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
