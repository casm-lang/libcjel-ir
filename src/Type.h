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

#include "cpp/Binding.h"
#include "cpp/Type.h"

#include "CselIR.h"

namespace libcsel_ir
{
    class Structure;
    struct StructureElement;

    class Type : public CselIR
    {
      public:
        typedef u64* Bit;
        typedef char* String;
        typedef std::vector< Value* > Struct;
        
        enum ID : u8
        {
            _BOTTOM_ = 0,
            LABEL,

            BIT,
            VECTOR,
            INTERCONNECT,
            STRING,
            RELATION,
            STRUCTURE,
            _TOP_
        };

      protected:
        const char* name;
        const char* description;
        u64 size;

      private:
        ID id;

      protected:
        static std::unordered_map< std::string, Type* >& str2obj( void )
        {
            static std::unordered_map< std::string, Type* > cache;
            return cache;
        }

      public:
        Type( const char* name, const char* description, u64 size, ID id );
        ~Type() = default;

        const ID getID( void ) const;

        virtual const u64 getSize( void ) = 0;
        virtual const char* getName( void ) = 0;
        virtual const char* getDescription( void ) = 0;

        Type* getResult( void ) const;

        // PPA: add isTYPEetc. CHECKS

        static Type* getLabel( void );

        static Type* getTypeID( void );

        static Type* getBit( u16 size );
        static Type* getString( void );
        static Type* getVector( Type* type, u16 length );
        static Type* getStructure( std::vector< StructureElement > arguments );
        static Type* getRelation(
            Type* result, std::vector< Type* > arguments );
        static Type* getInterconnect( void );

      private:
        void setID( ID id );
    };

    class PrimitiveType : public Type
    {
      public:
        PrimitiveType(
            const char* name, const char* description, u64 size, Type::ID id );

        const u64 getSize( void ) override final;
        const char* getName( void ) override final;
        const char* getDescription( void ) override final;
    };

    class AggregateType : public Type
    {
      public:
        AggregateType(
            const char* name, const char* description, u64 size, Type::ID id );
    };

    class SyntheticType : public Type
    {
      public:
        SyntheticType(
            const char* name, const char* description, u64 size, Type::ID id );

        const u64 getSize( void ) override final;
        const char* getName( void ) override final;
        const char* getDescription( void ) override final;
    };

    class LabelType : public PrimitiveType
    {
      public:
        LabelType();
    };

    class BitType : public PrimitiveType
    {
      public:
        BitType( u16 size );
    };

    class StringType : public PrimitiveType
    {
      public:
        StringType();
    };

    class InterconnectType : public SyntheticType
    {
      public:
        InterconnectType();
    };

    class RelationType : public Type
    {
      private:
        Type* result;
        std::vector< Type* > arguments;

      public:
        RelationType( Type* result, std::vector< Type* > arguments );

        const u64 getSize( void ) override final;
        const char* getName( void ) override final;
        const char* getDescription( void ) override final;

        const Type* getResult( void ) const;
        const std::vector< Type* >& getArguments( void ) const;
    };

    class VectorType : public AggregateType
    {
      private:
        Type* type;
        u16 length;

      public:
        VectorType( Type* type, u16 length );

        const u64 getSize( void ) override final;
        const char* getName( void ) override final;
        const char* getDescription( void ) override final;
    };

    struct StructureElement
    {
        Type* type;
        const char* name;
    };

    class StructureType : public AggregateType
    {
      private:
        std::vector< StructureElement > elements;

      public:
        StructureType( std::vector< StructureElement > elements );

        const u64 getSize( void ) override final;
        const char* getName( void ) override final;
        const char* getDescription( void ) override final;

        const std::vector< StructureElement >& getElements( void ) const;
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
