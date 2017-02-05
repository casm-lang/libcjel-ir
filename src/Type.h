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

#include "../stdhl/cpp/Type.h"

#include "CselIR.h"

namespace libcsel_ir
{
    class Structure;
    struct StructureElement;

    class Type : public CselIR
    {
      public:
        typedef u64 BitTy;
        typedef char* StringTy;
        typedef std::vector< Value* > StructTy;

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

      protected:
        static std::unordered_map< std::string, Type* >& m_str2obj( void )
        {
            static std::unordered_map< std::string, Type* > cache;
            return cache;
        }

        const char* m_name;
        const char* m_description;
        u64 m_bitsize;
        std::vector< Type* > m_results;

      private:
        ID m_id;
        const char* m_hash;

      public:
        Type( const char* name, const char* description, u64 bitsize, ID id );
        ~Type() = default;

        const ID id( void ) const;

        const char* make_hash( void );

        virtual const u64 bitsize( void ) = 0;
        virtual const char* name( void ) = 0;
        virtual const char* description( void ) = 0;
        virtual const std::vector< Type* >& results( void ) = 0;
        virtual const std::vector< Type* >& arguments( void ) = 0;

        inline u1 operator==( const Type& rhs )
        {
            if( this != &rhs )
            {
                if( strcmp( this->name(), ( (Type&)rhs ).name() ) )
                {
                    return false;
                }
            }
            return true;
        }
        inline u1 operator!=( const Type& rhs )
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

        static Type* Label( void );
        static Type* Void( void );

        static Type* TypeID( void );

        static Type* Bit( u16 bitsize );
        static Type* String( void );
        static Type* Vector( Type* type, u16 length );
        static Type* Structure( std::vector< StructureElement > arguments );
        static Type* Relation(
            std::vector< Type* > result, std::vector< Type* > arguments );
        static Type* Interconnect( void );
    };

    class PrimitiveType : public Type
    {
      public:
        PrimitiveType( const char* name, const char* description, u64 bitsize,
            Type::ID id );

        const u64 bitsize( void ) override final;
        const char* name( void ) override final;
        const char* description( void ) override final;
        const std::vector< Type* >& results( void ) override final;
        const std::vector< Type* >& arguments( void ) override final;
    };

    class AggregateType : public Type
    {
      public:
        AggregateType( const char* name, const char* description, u64 bitsize,
            Type::ID id );
    };

    class SyntheticType : public Type
    {
      public:
        SyntheticType( const char* name, const char* description, u64 bitsize,
            Type::ID id );

        const u64 bitsize( void ) override final;
        const char* name( void ) override final;
        const char* description( void ) override final;
        const std::vector< Type* >& results( void ) override final;
        const std::vector< Type* >& arguments( void ) override final;
    };

    class LabelType : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< LabelType >;

        LabelType();
    };

    class VoidType : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< VoidType >;

        VoidType();
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

        StringType();
    };

    class VectorType : public AggregateType
    {
      private:
        using Ptr = std::shared_ptr< VectorType >;

        Type* m_type;
        u16 m_length;

      public:
        VectorType( Type* type, u16 length );

        const u64 bitsize( void ) override final;
        const char* name( void ) override final;
        const char* description( void ) override final;
        const std::vector< Type* >& results( void ) override final;
        const std::vector< Type* >& arguments( void ) override final;
    };

    struct StructureElement
    {
        Type* type;
        const char* name;
    };

    class StructureType : public AggregateType
    {
      private:
        using Ptr = std::shared_ptr< StructureType >;

        std::vector< StructureElement > m_elements;
        // Structure* value; // PPA: link to EL instance ?!?!

      public:
        StructureType( std::vector< StructureElement > elements );

        const u64 bitsize( void ) override final;
        const char* name( void ) override final;
        const char* description( void ) override final;
        const std::vector< Type* >& results( void ) override final;
        const std::vector< Type* >& arguments( void ) override final;

        const std::vector< StructureElement >& elements( void ) const;
    };

    class RelationType : public Type
    {
      private:
        using Ptr = std::shared_ptr< RelationType >;

        std::vector< Type* > m_arguments;

      public:
        RelationType(
            std::vector< Type* > results, std::vector< Type* > arguments );

        const u64 bitsize( void ) override final;
        const char* name( void ) override final;
        const char* description( void ) override final;
        const std::vector< Type* >& results( void ) override final;
        const std::vector< Type* >& arguments( void ) override final;
    };

    class InterconnectType : public SyntheticType
    {
      public:
        InterconnectType();
    };
}

#endif /* _LIB_CSELIR_TYPE_H_ */

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
