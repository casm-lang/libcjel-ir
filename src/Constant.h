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

#include "User.h"

namespace libcsel_ir
{
    class Structure;

    class Constant : public User
    {
      private:
        static std::unordered_map< std::string, Value* >& str2obj( void )
        {
            static std::unordered_map< std::string, Value* > cache;
            return cache;
        };

      public:
        Constant( const char* name, Type* type, Value::ID id = Value::CONSTANT )
        : User( name, type, id ){};

        static inline Value::ID classid( void )
        {
            return Value::CONSTANT;
        };
        static bool classof( Value const* obj );

        virtual const char* getLabelName( void ) override final
        {
            return "@c";
        }

        virtual u64 getLabelID( void ) override final
        {
            static u64 cnt = 0;
            return cnt++;
        }

        static Value* getBit( Type* result, u64 value );
        static Value* getString( const char* value );
        // static Value* get( void );
    };

    template < typename V >
    class ConstantOf : public Constant
    {
      private:
        V value;
        const char* description;

      protected:
        ConstantOf( const char* name, Type* type, V value,
            Value::ID id = Value::CONSTANT )
        : Constant( name, type, id )
        , value( value )
        , description( 0 )
        {
        }

      public:
        ~ConstantOf( void )
        {
        }

        const V getValue( void ) const
        {
            return value;
        }

        const char* getDescription( void )
        {
            if( not description )
            {
                std::string tmp = "";
                tmp += getType()->getName();
                tmp += " ";
                tmp += getName();

                description = libstdhl::Allocator::string( tmp );
            }

            return description;
        }

        static inline Value::ID classid( void )
        {
            return Value::CONSTANT;
        }

        static bool classof( Value const* obj )
        {
            return Constant::classof( obj );
        }

      protected:
        void setValue( V val )
        {
            value = val;
        }
    };

    class BitConstant : public ConstantOf< Type::Bit >
    {
      private:
        u64 value[ 1 ];

      public:
        BitConstant( Type* result, u64 value );

        void dump( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::BIT_CONSTANT;
        };
        static bool classof( Value const* obj );
    };

    class StringConstant : public ConstantOf< Type::String >
    {
      public:
        StringConstant( Type::String value );
        StringConstant( const char* value );

        void dump( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::STRING_CONSTANT;
        };
        static bool classof( Value const* obj );
    };

    class StructureConstant : public ConstantOf< Type::Struct >
    {
      public:
        StructureConstant( Type* type, std::vector< Value* > value );

        const std::vector< Value* >& getElements( void ) const;

        void dump( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::STRUCTURE_CONSTANT;
        };
        static bool classof( Value const* obj );
    };

    class Identifier : public ConstantOf< const char* >
    {
      private:
        static std::unordered_map< std::string, Identifier* >& ident2obj( void )
        {
            static std::unordered_map< std::string, Identifier* > cache;
            return cache;
        }

        Identifier( Type* type, const char* value );

      public:
        ~Identifier( void );

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

    //     template < typename V >
    // class Constant : public libcsel_ir::User
    // {
    //   protected:
    //     V value;
    //     Constant( const char* name, Type* type, V value,
    //         Value::ID id = Value::CONSTANT );

    //   public:
    //     ~Constant( void );

    //     const V getValue( void ) const;

    //     static inline Value::ID classid( void )
    //     {
    //         return Value::CONSTANT;
    //     };
    //     static bool classof( Value const* obj );

    //   protected:
    //     void setValue( V val );
    // };

    // class Constants : public Constant< u1 >
    // {
    //   public:
    //     static inline Value::ID classid( void )
    //     {
    //         return Value::CONSTANT;
    //     };
    //     static bool classof( Value const* obj );
    // };

    // class StructureConstant;

    // class BitConstant : public Constant< Type::Bit >,
    //                     public libstdhl::Binding< StructureConstant >
    // {
    //   private:
    //     u64 value[ 1 ];
    //     BitConstant( Type* type, u64 value );

    //   public:
    //     static BitConstant* create( u64 value, u16 bitsize );

    //     void dump( void ) const;

    //     static inline Value::ID classid( void )
    //     {
    //         return Value::BIT_CONSTANT;
    //     };
    //     static bool classof( Value const* obj );

    //     static BitConstant TRUE;
    //     static BitConstant FALSE;
    //     static BitConstant NIL;
    // };

    // class StructureConstant : public Constant< std::vector< Value* > >,
    //                           public libstdhl::Binding< StructureConstant >
    // {
    //   private:
    //     StructureConstant( Type* type, std::vector< Value* > value );

    //   public:
    //     static StructureConstant* create(
    //         Structure* kind, const std::vector< Value* >& value = {} );

    //     const std::vector< Value* >& getElements( void ) const;

    //     void dump( void ) const;

    //     static inline Value::ID classid( void )
    //     {
    //         return Value::STRUCTURE_CONSTANT;
    //     };
    //     static bool classof( Value const* obj );
    // };

    // class Identifier : public Constant< const char* >
    // {
    //   private:
    //     Identifier( Type* type, const char* value );

    //   public:
    //     ~Identifier( void );

    //     static Identifier* create(
    //         Type* type, std::string value, Value* scope = 0 );
    //     static Identifier* create(
    //         Type* type, const char* value, Value* scope = 0 );

    //     static void forgetSymbol( const char* value );

    //     void dump( void ) const;

    //     static inline Value::ID classid( void )
    //     {
    //         return Value::IDENTIFIER;
    //     };
    //     static bool classof( Value const* obj );
    // };

    // class StringConstant : public Constant< Type::String >,
    //                        public libstdhl::Binding< StructureConstant >
    // {
    //   public:
    //     StringConstant( Type* type, Type::String value );
    //     static StringConstant* create( Type::String value );

    //     void dump( void ) const;

    //     static inline Value::ID classid( void )
    //     {
    //         return Value::STRING_CONSTANT;
    //     };
    //     static bool classof( Value const* obj );

    //     static StringConstant LF;
    // };
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
