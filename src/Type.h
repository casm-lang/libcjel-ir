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

#ifndef _LIBCJEL_IR_TYPE_H_
#define _LIBCJEL_IR_TYPE_H_

#include <libcjel-ir/CjelIR>

#include <libstdhl/List>
#include <libstdhl/Log>

#include <functional>
#include <unordered_map>
#include <unordered_set>

namespace libcjel_ir
{
    class Structure;

    class Type;
    using Types = libstdhl::List< Type >;

    class Type : public CjelIR
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

        Type( const std::string& name, const std::string& description, u64 bitsize, ID id );

        ~Type() = default;

        std::string name( void ) const;

        std::string description( void ) const;

        ID id( void ) const;

        u64 bitsize( void ) const;

        u64 wordsize( const u64 wordbits = 64 ) const;

        const Types& results( void ) const;

        Types ptr_results( void ) const;

        const Types& arguments( void ) const;

        Types ptr_arguments( void ) const;

        virtual std::size_t hash( void ) const = 0;

        inline u1 operator==( const Type& rhs ) const
        {
            if( this != &rhs )
            {
                if( this->id() != rhs.id() or this->name().compare( rhs.name() ) )
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

        inline std::unordered_map< std::size_t, Type::Ptr >& cache( void )
        {
            return s_cache();
        }

      protected:
        std::string m_name;

        std::string m_description;

        u64 m_bitsize;

        Types m_results;

      private:
        ID m_id;

        // std::weak_ptr< Type > m_self;

      public:
        static std::unordered_map< std::size_t, Type::Ptr >& s_cache( void )
        {
            static std::unordered_map< std::size_t, Type::Ptr > obj = {};
            return obj;
        }
        static std::unordered_map< std::size_t, Type::Ptr >& s_registered_type_hash2ptr( void )
        {
            static std::unordered_map< std::size_t, Type::Ptr > obj = {};
            return obj;
        }
        static std::unordered_map< u64, std::size_t >& s_registered_type_id2hash( void )
        {
            static std::unordered_map< u64, std::size_t > obj = {};
            return obj;
        }
    };

    class PrimitiveType : public Type
    {
      public:
        using Ptr = std::shared_ptr< PrimitiveType >;

        PrimitiveType(
            const std::string& name, const std::string& description, u64 bitsize, Type::ID id );
    };

    class AggregateType : public Type
    {
      public:
        using Ptr = std::shared_ptr< AggregateType >;

        AggregateType(
            const std::string& name, const std::string& description, u64 bitsize, Type::ID id );
    };

    class SyntheticType : public Type
    {
      public:
        using Ptr = std::shared_ptr< SyntheticType >;

        SyntheticType(
            const std::string& name, const std::string& description, u64 bitsize, Type::ID id );
    };

    class LabelType : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< LabelType >;

        LabelType( void );

        std::size_t hash( void ) const override;
    };

    class VoidType : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< VoidType >;

        VoidType( void );

        std::size_t hash( void ) const override;
    };

    class BitType : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< BitType >;

        static const u16 SizeMax = 512;

        BitType( u16 bitsize );

        std::size_t hash( void ) const override;
    };

    class StringType : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< StringType >;

        StringType( void );

        std::size_t hash( void ) const override;
    };

    class VectorType : public AggregateType
    {
      public:
        using Ptr = std::shared_ptr< VectorType >;

        VectorType( const Type::Ptr& type, u16 length );

        std::size_t hash( void ) const override;

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

        std::size_t hash( void ) const override;

      private:
        std::shared_ptr< Structure > m_kind;
    };

    class RelationType : public Type
    {
      public:
        using Ptr = std::shared_ptr< RelationType >;

        RelationType(
            const std::vector< Type::Ptr >& results, const std::vector< Type::Ptr >& arguments );

        const Types& arguments( void ) const;

        std::size_t hash( void ) const override;

      private:
        Types m_arguments;
    };

    class InterconnectType : public SyntheticType
    {
      public:
        InterconnectType( void );

        std::size_t hash( void ) const override;
    };
}

#endif  // _LIBCJEL_IR_TYPE_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
