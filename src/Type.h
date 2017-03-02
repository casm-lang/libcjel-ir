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

#ifndef _LIB_CSELIR_TYPE_H_
#define _LIB_CSELIR_TYPE_H_

#include "CselIR.h"

#include "../stdhl/cpp/List.h"
#include "../stdhl/cpp/Log.h"

namespace libcsel_ir
{
    class Structure;

    class Type;
    using Types = libstdhl::List< Type >;

    class Type : public CselIR
    {
      public:
        using Ptr = std::shared_ptr< Type >;

        enum ID : u8
        {
            _BOTTOM_ = 0,
            LABEL,
            VOID,

            BIT,
            VECTOR,
            INTERCONNECT,
            STRING,
            RELATION,
            STRUCTURE,
            _TOP_
        };

        Type( const std::string& name, const std::string& description,
            u64 bitsize, ID id );

        ~Type() = default;

        std::string name( void ) const;

        std::string description( void ) const;

        ID id( void ) const;

        u64 bitsize( void ) const;

        u64 wordsize( const u64 wordbits = 64 ) const;

        const Types& results( void ) const;

        Types ptr_results( void ) const;

        std::vector< Type* > arguments( void ) const;

        std::vector< Type::Ptr > ptr_arguments( void ) const;

        std::string make_hash( void ) const;

        inline u1 operator==( const Type& rhs ) const
        {
            if( this != &rhs )
            {
                if( this->id() != rhs.id()
                    or this->name().compare( rhs.name() ) )
                {
                    return false;
                }
            }
            return true;
        }

        inline u1 operator!=( const Type& rhs ) const
        {
            return !operator==( rhs );
        }

        u1 isLabel( void ) const;
        u1 isVoid( void ) const;
        u1 isBit( void ) const;
        u1 isString( void ) const;
        u1 isVector( void ) const;
        u1 isStructure( void ) const;
        u1 isRelation( void ) const;
        u1 isInterconnect( void ) const;

      protected:
        std::string m_name;
        std::string m_description;
        u64 m_bitsize;

        Types m_results;

      private:
        ID m_id;

        // std::weak_ptr< Type > m_self;

      public:
        std::unordered_map< std::string, Type::Ptr >& make_cache( void )
        {
            static std::unordered_map< std::string, Type::Ptr > cache;
            return cache;
        }
    };

    class PrimitiveType : public Type
    {
      public:
        using Ptr = std::shared_ptr< PrimitiveType >;

        PrimitiveType( const std::string& name, const std::string& description,
            u64 bitsize, Type::ID id );
    };

    class AggregateType : public Type
    {
      public:
        using Ptr = std::shared_ptr< AggregateType >;

        AggregateType( const std::string& name, const std::string& description,
            u64 bitsize, Type::ID id );
    };

    class SyntheticType : public Type
    {
      public:
        using Ptr = std::shared_ptr< SyntheticType >;

        SyntheticType( const std::string& name, const std::string& description,
            u64 bitsize, Type::ID id );
    };

    class LabelType : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< LabelType >;

        LabelType( void );
    };

    class VoidType : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< VoidType >;

        VoidType( void );
    };

    class BitType : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< BitType >;

        static const u16 SizeMax = 512;

        BitType( u16 bitsize );
    };

    class StringType : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< StringType >;

        StringType( void );
    };

    class VectorType : public AggregateType
    {
      public:
        using Ptr = std::shared_ptr< VectorType >;

        VectorType( const Type::Ptr& type, u16 length );

      private:
        Type::Ptr m_type;
        u16 m_length;
    };

    class StructureType : public AggregateType
    {
      public:
        using Ptr = std::shared_ptr< StructureType >;

        StructureType( const std::shared_ptr< Structure >& kind );

        Structure& kind( void ) const;

        std::shared_ptr< Structure > ptr_kind( void ) const;

      private:
        std::shared_ptr< Structure > m_kind;
    };

    class RelationType : public Type
    {
      public:
        using Ptr = std::shared_ptr< RelationType >;

        RelationType( const std::vector< Type::Ptr >& results,
            const std::vector< Type::Ptr >& arguments );

      private:
        Types m_arguments;
    };

    class InterconnectType : public SyntheticType
    {
      public:
        InterconnectType( void );
    };
}

#endif // _LIB_CSELIR_TYPE_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
