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

#include "../stdhl/cpp/Allocator.h"

namespace libcsel_ir
{
    class Structure;

    class Constant : public User
    {
      private:
        static std::unordered_map< std::string, Value* >& m_str2obj( void )
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
        }

        static bool classof( Value const* obj );

        virtual const char* labelName( void ) override final
        {
            return "@c";
        }

        virtual u64 labelId( void ) override final
        {
            static u64 cnt = 0;
            return cnt++;
        }

        static inline Value* TRUE( void )
        {
            return Bit( Type::Bit( 1 ), 1 );
        }

        static inline Value* FALSE( void )
        {
            return Bit( Type::Bit( 1 ), 0 );
        }

        static inline Value* NIL( void )
        {
            return Bit( Type::TypeID(), 0 );
        }

        static Value* Bit( Type* result, u64 value );

        static Value* String( const char* value );

        static Value* Structure( Type* result, std::vector< Value* > values );

        static Value* StructureZero( Type& result );
    };

    template < typename V >
    class ConstantOf : public Constant
    {
      private:
        V m_value;
        const char* m_description;

      protected:
        ConstantOf( const char* name, Type* type, V value,
            Value::ID id = Value::CONSTANT )
        : Constant( name, type, id )
        , m_value( value )
        , m_description( 0 )
        {
        }

      public:
        ~ConstantOf( void )
        {
        }

        const V value( void ) const
        {
            return m_value;
        }

        const char* description( void )
        {
            if( not m_description )
            {
                std::string tmp = "";
                tmp += type().name();
                tmp += " ";
                tmp += name();

                m_description = libstdhl::Allocator::string( tmp );
            }

            return m_description;
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
        void setValue( V value )
        {
            m_value = value;
        }
    };

    class BitConstant : public ConstantOf< Type::BitTy >
    {
      public:
        using Ptr = std::shared_ptr< BitConstant >;
        
        BitConstant( Type* result, u64 value );

        static inline Value::ID classid( void )
        {
            return Value::BIT_CONSTANT;
        }

        static bool classof( Value const* obj );
    };

    class StringConstant : public ConstantOf< Type::StringTy >
    {
      public:
        using Ptr = std::shared_ptr< StringConstant >;
        
        StringConstant( Type::StringTy value );
        StringConstant( const char* value );

        static inline Value::ID classid( void )
        {
            return Value::STRING_CONSTANT;
        };
        static bool classof( Value const* obj );
    };

    class StructureConstant : public ConstantOf< Type::StructTy >
    {
      public:
        using Ptr = std::shared_ptr< StructureConstant >;
        
        StructureConstant( Type* type,
            std::vector< Value* >
                value ); // PPA: optimize here with const <>&

        static inline Value::ID classid( void )
        {
            return Value::STRUCTURE_CONSTANT;
        };
        static bool classof( Value const* obj );
    };

    class Identifier : public ConstantOf< const char* >
    {
      private:
        using Ptr = std::shared_ptr< Identifier >;
        
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
