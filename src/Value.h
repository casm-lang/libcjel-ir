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

#ifndef _LIB_CSELIR_VALUE_H_
#define _LIB_CSELIR_VALUE_H_

#include "CselIR.h"

#include "Type.h"

namespace libcsel_ir
{
    class Visitor;
    class Context;
    enum Traversal : u8;

    class Module;

    class Value : public CselIR
    {
      public:
        using Ptr = std::shared_ptr< Value >;

        enum ID
        {
            VALUE,
            USER

            ,
            MODULE,
            INTERCONNECT

            ,
            MEMORY

            ,
            CALLABLE_UNIT,
            INTRINSIC,
            FUNCTION

            ,
            VARIABLE,
            REFERENCE,
            STRUCTURE

            ,
            CONSTANT,
            VOID_CONSTANT,
            BIT_CONSTANT,
            STRUCTURE_CONSTANT,
            STRING_CONSTANT

            ,
            IDENTIFIER

            ,
            BLOCK

            ,
            SCOPE,
            PARALLEL_SCOPE,
            SEQUENTIAL_SCOPE

            ,
            STATEMENT,
            TRIVIAL_STATEMENT,
            BRANCH_STATEMENT,
            LOOP_STATEMENT

            ,
            INSTRUCTION,
            UNARY_INSTRUCTION

            ,
            BINARY_INSTRUCTION

            ,
            NOP_INSTRUCTION,
            ALLOC_INSTRUCTION

            ,
            ID_INSTRUCTION,
            CAST_INSTRUCTION,
            EXTRACT_INSTRUCTION

            ,
            LOAD_INSTRUCTION,
            READ_INSTRUCTION,
            STORE_INSTRUCTION,
            WRITE_INSTRUCTION

            ,
            CALL_INSTRUCTION,
            ID_CALL_INSTRUCTION

            ,
            STREAM_INSTRUCTION

            ,
            OPERATOR_INSTRUCTION

            ,
            ARITHMETIC_INSTRUCTION

            ,
            NOT_INSTRUCTION,
            LNOT_INSTRUCTION,
            AND_INSTRUCTION,
            OR_INSTRUCTION,
            XOR_INSTRUCTION

            ,
            ADDS_INSTRUCTION,
            ADDU_INSTRUCTION,
            DIVS_INSTRUCTION,
            MODU_INSTRUCTION

            ,
            LOGICAL_INSTRUCTION,
            COMPARE_INSTRUCTION,
            EQU_INSTRUCTION,
            NEQ_INSTRUCTION

            ,
            ZEXT_INSTRUCTION,
            TRUNC_INSTRUCTION
        };

        Value( const std::string& name, const Type::Ptr& type, ID id );

        ~Value( void );

        std::string name( void ) const;

        const Type& type( void ) const;

        Type::Ptr ptr_type( void ) const;

        ID id() const;

        std::string description( void ) const;

        std::string dump( void ) const;

        std::string make_hash( void ) const;

        std::string label( void ) const;

        void setModule( std::shared_ptr< Module > );

        const Module& module( void ) const;

        std::shared_ptr< Module > ptr_module( void ) const;

        inline u1 operator==( const Value& rhs ) const
        {
            if( this != &rhs )
            {
                if( this->id() != rhs.id() or this->name().compare( rhs.name() )
                    or this->type() != rhs.type() )
                {
                    return false;
                }
            }
            return true;
        }

        inline u1 operator!=( const Value& rhs ) const
        {
            return !operator==( rhs );
        }

        static inline ID classid( void )
        {
            return Value::VALUE;
        }

        static inline bool classof( Value const* )
        {
            return true;
        }

        virtual void iterate( Traversal order, Visitor* visitor = nullptr,
            Context* context = nullptr,
            std::function< void( Value& ) > action = []( Value& ) {} ) final;

        virtual void iterate(
            Traversal order, std::function< void( Value& ) > action ) final;

      private:
        static std::unordered_map< u8, std::unordered_set< Value* > >&
        m_id2objs( void )
        {
            static std::unordered_map< u8, std::unordered_set< Value* > > cache;
            return cache;
        }

      protected:
        std::string m_name;

      private:
        Type::Ptr m_type;

        ID m_id;

        std::weak_ptr< Module > m_module;

        // Value* m_next; // TODO: PPA: use a std::weak_ptr here?
    };

    using Values = libstdhl::List< Value >;
}

// namespace std
// {
//     template <>
//     struct hash< libcsel_ir::Value* >
//     {
//       public:
//         size_t operator()( const libcsel_ir::Value* obj ) const
//         {
//             assert( obj and " invalid pointer! " );
//             return std::hash< std::string >( obj->make_hash() );
//         }
//     };

//     template <>
//     struct equal_to< libcsel_ir::Value* >
//     {
//       public:
//         bool operator()(
//             const libcsel_ir::Value* lhs, const libcsel_ir::Value* rhs )
//             const
//         {
//             assert( lhs and rhs and " invalid pointers " );
//             return *rhs == *lhs;
//         }
//     };
// }

#endif // _LIB_CSELIR_VALUE_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
