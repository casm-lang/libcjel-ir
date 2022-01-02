//
//  Copyright (C) 2015-2022 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber et al.
//                <https://github.com/casm-lang/libcjel-ir/graphs/contributors>
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

#include "Type.h"

#include <libcjel-ir/Structure>

using namespace libcjel_ir;

Type::Type( const std::string& name, const std::string& description, u64 bitsize, Type::ID id )
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

PrimitiveType::PrimitiveType(
    const std::string& name, const std::string& description, u64 bitsize, Type::ID id )
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

std::size_t LabelType::hash( void ) const
{
    return std::hash< std::string >()( "t:" + std::to_string( id() ) + ":" + description() );
}

//
// VoidType
//

VoidType::VoidType( void )
: PrimitiveType( "void", "Void", 0, Type::VOID )
{
}

std::size_t VoidType::hash( void ) const
{
    return std::hash< std::string >()( "t:" + std::to_string( id() ) + ":" + description() );
}

//
// BitType
//

BitType::BitType( u16 bitsize )
: PrimitiveType(
      "u" + std::to_string( bitsize ),
      "Bit(" + std::to_string( bitsize ) + ")",
      bitsize,
      Type::BIT )
{
    if( bitsize < 1 )
    {
        throw std::domain_error( "bit size of 'BitType' shall be greater or equal than '1'" );
    }
    else if( bitsize > BitType::SizeMax )
    {
        throw std::domain_error(
            "bit size of 'BitType' shall be smaller or equal than '" +
            std::to_string( BitType::SizeMax ) + "'" );
    }
}

std::size_t BitType::hash( void ) const
{
    return std::hash< std::string >()( "t:" + std::to_string( id() ) + ":" + description() );
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

std::size_t StringType::hash( void ) const
{
    return std::hash< std::string >()( "t:" + std::to_string( id() ) + ":" + description() );
}

//
// AggregateType
//

AggregateType::AggregateType(
    const std::string& name, const std::string& description, u64 bitsize, Type::ID id )
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
        throw std::domain_error( "length of 'VectorType' shall be greater or equal than '1'" );
    }

    m_name = "< " + m_type->name() + " x " + std::to_string( m_length ) + " > ";

    m_description = "< " + m_type->description() + " x " + std::to_string( m_length ) + " > ";

    for( u32 i = 0; i < m_length; i++ )
    {
        m_results.add( m_type );
    }
}

std::size_t VectorType::hash( void ) const
{
    return std::hash< std::string >()( "t:" + std::to_string( id() ) + ":" + description() );
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
        throw std::domain_error( "structure kind of 'StructureType' cannot be a null pointer" );
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

std::size_t StructureType::hash( void ) const
{
    return std::hash< std::string >()( "t:" + std::to_string( id() ) + ":" + description() );
}

//
// RelationType
//

RelationType::RelationType(
    const std::vector< Type::Ptr >& results, const std::vector< Type::Ptr >& arguments )
: Type( "", "", 0, Type::RELATION )
, m_arguments( arguments )
{
    m_results = std::move( results );

    if( results.size() == 0 )
    {
        throw std::domain_error( "results size of 'RelationType' cannot be '0'" );
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

std::size_t RelationType::hash( void ) const
{
    return std::hash< std::string >()( "t:" + std::to_string( id() ) + ":" + description() );
}

//
// SyntheticType
//

SyntheticType::SyntheticType(
    const std::string& name, const std::string& description, u64 bitsize, Type::ID id )
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

std::size_t InterconnectType::hash( void ) const
{
    return std::hash< std::string >()( "t:" + std::to_string( id() ) + ":" + description() );
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
