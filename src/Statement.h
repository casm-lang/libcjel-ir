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

#ifndef _LIB_CSELIR_STATEMENT_H_
#define _LIB_CSELIR_STATEMENT_H_

#include "Block.h"

#include "Constant.h"
#include "Instruction.h"
#include "Scope.h"
#include "Value.h"

namespace libcsel_ir
{
    class Statement : public Block
    {
      private:
        std::vector< Value* > instructions;
        std::vector< Scope* > scopes;

      public:
        Statement( const char* name, Type* type, Value* parent,
            Value::ID id = Value::STATEMENT );

        const std::vector< Value* >& getInstructions( void ) const;

        virtual const u1 isParallel( void ) const;

        void add( Value* instruction );

        void addScope( Scope* block );
        const std::vector< Scope* >& getScopes( void ) const;

        template < class C >
        bool consistsOnlyOf( void )
        {
            for( auto instr : instructions )
            {
                if( not Value::isa< C >( instr ) )
                {
                    return false;
                }
            }

            return true;
        };

        void dump( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::STATEMENT;
        };
        static bool classof( Value const* obj );
    };

    class TrivialStatement : public Statement
    {
      public:
        TrivialStatement( Value* parent = 0 );

        void dump( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::TRIVIAL_STATEMENT;
        };
        static bool classof( Value const* obj );
    };

    class BranchStatement : public Statement
    {
      public:
        BranchStatement( Value* parent = 0 );

        void dump( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::BRANCH_STATEMENT;
        };
        static bool classof( Value const* obj );
    };

    class LoopStatement : public Statement
    {
      public:
        LoopStatement( Value* parent = 0 );

        void dump( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::LOOP_STATEMENT;
        };
        static bool classof( Value const* obj );
    };
}

#endif /* _LIB_CSELIR_STATEMENT_H_ */

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
