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

#include "../stdhl/cpp/Labeling.h"

#include "Type.h"

namespace libcsel_ir
{
    enum class Traversal;
    class Context;
    class Visitor;

    class Value : public CselIR, public libstdhl::Labeling
    {
      public:
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
            ARITHMETIC_INSTRUCTION

            ,
            NOT_INSTRUCTION,
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
            EQUU_INSTRUCTION,
            NEQU_INSTRUCTION

            ,
            ZEXT_INSTRUCTION,
            TRUNC_INSTRUCTION
        };

        typedef std::unordered_map< const char*, std::unordered_set< Value* >,
            libstdhl::Hash, libstdhl::Equal >
            SymbolTable;

        static SymbolTable* getSymbols( void )
        {
            static SymbolTable symbols;
            return &symbols;
        }

      private:
        const char* name;
        Type* type;
        ID id;
        u1 type_lock;

        std::unordered_map< u32, Value* > references;

        Value* next;

      protected:
        static std::unordered_map< u8, std::unordered_set< Value* > >& id2objs(
            void )
        {
            static std::unordered_map< u8, std::unordered_set< Value* > > cache;
            return cache;
        }

      public:
        Value( const char* name, Type* type, ID id );

        ~Value();

        const char* getName( void ) const;

        Type* getType( void ) const;

      protected:
        void setType( Type* type );

      public:
        ID getValueID() const;

        void debug( void ) const;
        void dump( void ) const;

        void setNext( Value* value );
        Value* getNext( void ) const;

        template < class C >
        C* getRef( void )
        {
            auto result = references.find( C::classid() );
            if( result != references.end() )
            {
                assert( isa< C >( result->second ) );
                return (C*)result->second;
            }

            return 0;
        }

        template < class C >
        void setRef( Value* reference )
        {
            // printf( "[[[ %p ]]]: classID/referenceCNT = %u %u\n"
            //, reference, C::classid(), references.count( C::classid() ) );
            assert( references.count( C::classid() ) == 0 );
            references[ C::classid() ] = reference;
        }

        static inline ID classid( void )
        {
            return Value::VALUE;
        };

        static inline bool classof( Value const* )
        {
            return true;
        }

        virtual void iterate( Traversal order, Visitor* visitor = nullptr,
            Context* context = nullptr,
            std::function< void( Value& ) > action = []( Value& ) {} ) final;

        virtual void iterate(
            Traversal order, std::function< void( Value& ) > action ) final;
    };
}

#endif /* _LIB_CSELIR_VALUE_H_ */

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
