//
//  Copyright (C) 2015-2020 CASM Organization <https://casm-lang.org>
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

#ifndef _LIBCJEL_IR_CONSTANT_H_
#define _LIBCJEL_IR_CONSTANT_H_

#include <libcjel-ir/Value>

#include <libstdhl/type/Data>

namespace libcjel_ir
{
    class Structure;

    class Constant : public Value
    {
      public:
        using Ptr = std::shared_ptr< Constant >;

        Constant(
            const std::string& name,
            const Type::Ptr& type,
            const libstdhl::Type::Data& data,
            const std::vector< Constant >& constants,
            Value::ID id = classid() );

        static inline Value::ID classid( void )
        {
            return Value::CONSTANT;
        }

        static bool classof( Value const* obj );

      protected:
        libstdhl::Type::Data m_data;

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

        BitConstant( const Type::Ptr& type, u64 value );

        BitConstant( u16 bitsize, u64 value );

        std::string literal( libstdhl::Type::Radix radix = libstdhl::Type::DECIMAL ) const;

        const libstdhl::Type::Data& value( void ) const;

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

        StructureConstant( const Type::Ptr& type, const std::vector< Constant >& values );

        StructureConstant(
            const std::shared_ptr< Structure >& kind, const std::vector< Constant >& values );

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
}  // namespace libcjel_ir

#endif  // _LIBCJEL_IR_CONSTANT_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
