//
//  Copyright (c) 2015-2016 Philipp Paulweber
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

#include "cpp/Binding.h"

#include "Type.h"
#include "User.h"
#include "Value.h"

namespace libcsel_ir
{
    class Structure;

    template < typename V >
    class Constant : public libcsel_ir::User
    {
      protected:
        V value;
        Constant( const char* name, Type* type, V value,
            Value::ID id = Value::CONSTANT );

      public:
        ~Constant( void );

        const V getValue( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::CONSTANT;
        };
        static bool classof( Value const* obj );

      protected:
        void setValue( V val );
    };

    class Constants : public Constant< u1 >
    {
      public:
        static inline Value::ID classid( void )
        {
            return Value::CONSTANT;
        };
        static bool classof( Value const* obj );
    };

    class StructureConstant;

    class BitConstant : public Constant< Type::Bit >,
                        public libstdhl::Binding< StructureConstant >
    {
      private:
        u64 value[ 1 ];
        BitConstant( Type* type, u64 value );

      public:
        static BitConstant* create( u64 value, u16 bitsize );

        void dump( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::BIT_CONSTANT;
        };
        static bool classof( Value const* obj );

        static BitConstant TRUE;
        static BitConstant FALSE;
        static BitConstant NIL;
    };

    class StructureConstant : public Constant< std::vector< Value* > >,
                              public libstdhl::Binding< StructureConstant >
    {
      private:
        StructureConstant( Type* type, std::vector< Value* > value );

      public:
        static StructureConstant* create(
            Structure* kind, const std::vector< Value* >& value = {} );

        const std::vector< Value* >& getElements( void ) const;

        void dump( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::STRUCTURE_CONSTANT;
        };
        static bool classof( Value const* obj );
    };

    class Identifier : public Constant< const char* >
    {
      private:
        Identifier( Type* type, const char* value );

      public:
        ~Identifier( void );

        static Identifier* create(
            Type* type, std::string value, Value* scope = 0 );
        static Identifier* create(
            Type* type, const char* value, Value* scope = 0 );

        static void forgetSymbol( const char* value );

        void dump( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::IDENTIFIER;
        };
        static bool classof( Value const* obj );
    };

    class StringConstant : public Constant< Type::String >,
                           public libstdhl::Binding< StructureConstant >
    {
      public:
        StringConstant( Type* type, Type::String value );
        static StringConstant* create( Type::String value );

        void dump( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::STRING_CONSTANT;
        };
        static bool classof( Value const* obj );

        static StringConstant LF;
    };
}

#endif /* _LIB_CSELIR_CONSTANT_H_ */

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
