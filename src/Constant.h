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

#ifndef _LIB_CSELIR_CONSTANT_H_
#define _LIB_CSELIR_CONSTANT_H_

#include "Value.h"

namespace libcsel_ir
{
    class Structure;

    class Constant : public Value
    {
      public:
        using Ptr = std::shared_ptr< Constant >;

        Constant( const std::string& name,
            const Type::Ptr& type,
            const libstdhl::Type& data,
            const std::vector< Constant >& constants,
            Value::ID id = classid() );

        static inline Value::ID classid( void )
        {
            return Value::CONSTANT;
        }

        static bool classof( Value const* obj );

      protected:
        libstdhl::Type m_data;

        std::vector< Constant > m_constants;

      public:
        std::unordered_map< std::string, Constant::Ptr >& make_cache( void )
        {
            static std::unordered_map< std::string, Constant::Ptr > cache;
            return cache;
        }
    };

    class VoidConstant : public Constant
    {
      public:
        using Ptr = std::shared_ptr< VoidConstant >;

        VoidConstant( void );

        static inline Value::ID classid( void )
        {
            return Value::VOID_CONSTANT;
        }

        static bool classof( Value const* obj );
    };

    class BitConstant : public Constant
    {
      public:
        using Ptr = std::shared_ptr< BitConstant >;

        BitConstant( const BitType::Ptr& type, u64 value );

        BitConstant( u16 bitsize, u64 value );

        std::string literal(
            libstdhl::Type::Radix radix = libstdhl::Type::DECIMAL ) const;

        const std::vector< u64 >& value( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::BIT_CONSTANT;
        }

        static bool classof( Value const* obj );
    };

    class StringConstant : public Constant
    {
      public:
        using Ptr = std::shared_ptr< StringConstant >;

        StringConstant( const std::string& value );

        std::string value( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::STRING_CONSTANT;
        }

        static bool classof( Value const* obj );
    };

    class StructureConstant : public Constant
    {
      public:
        using Ptr = std::shared_ptr< StructureConstant >;

        StructureConstant( const StructureType::Ptr& type,
            const std::vector< Constant >& values );

        StructureConstant( const std::shared_ptr< Structure >& kind,
            const std::vector< Constant >& values );

        std::vector< Constant > value( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::STRUCTURE_CONSTANT;
        }

        static bool classof( Value const* obj );
    };

    class Identifier : public Constant
    {
      public:
        using Ptr = std::shared_ptr< Identifier >;

        Identifier( const Type::Ptr& type, const std::string& value );

        static inline Value::ID classid( void )
        {
            return Value::IDENTIFIER;
        }

        static bool classof( Value const* obj );
    };
}

#endif // _LIB_CSELIR_CONSTANT_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
