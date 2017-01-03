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

#ifndef _LIB_CSELIR_STRUCTURE_H_
#define _LIB_CSELIR_STRUCTURE_H_

#include "Constant.h"
#include "Intrinsic.h"
#include "Module.h"
#include "User.h"
#include "Value.h"

namespace libcsel_ir
{
    class Structure : public User
    {
      private:
        Identifier* identifier;
        std::vector< Structure* > element;
        Structure* parent;

      public:
        Structure( const char* name, Type* type = 0, Structure* parent = 0 );

        ~Structure( void );

        const Identifier* getIdentifier( void ) const;

        void add( Value* value );

        Value* get( u16 index ) const;
        const std::vector< Structure* >& getElements( void ) const;

        void setParent( Structure* value );
        Structure* getParent( void ) const;

        void dump( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::STRUCTURE;
        };
        static bool classof( Value const* obj );

        size_t hash( void ) const
        {
            assert( identifier );
            return std::hash< string >()( string( identifier->getName() ) );
        }
    };
}

// TODO: FIXME: PPA: IMPROVEMENT: find a generic solution for the
// hashing/equivalence functionality
namespace std
{
    template <>
    struct hash< libcsel_ir::Structure* >
    {
      public:
        size_t operator()( const libcsel_ir::Structure* obj ) const
        {
            assert( obj and " invalid pointer! " );
            return obj->hash();
        }
    };

    template <>
    struct equal_to< libcsel_ir::Structure* >
    {
      public:
        bool operator()( const libcsel_ir::Structure* lhs,
            const libcsel_ir::Structure* rhs ) const
        {
            return lhs->hash() == rhs->hash();
        }
    };
}

#endif /* _LIB_CSELIR_STRUCTURE_H_ */

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
