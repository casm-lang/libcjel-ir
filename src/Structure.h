//
//  Copyright (c) 2015-2016 Philipp Paulweber
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                https://github.com/casm-lang/libnovel
//
//  This file is part of libnovel.
//
//  libnovel is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  libnovel is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with libnovel. If not, see <http://www.gnu.org/licenses/>.
//

#ifndef _LIB_NOVEL_STRUCTURE_H_
#define _LIB_NOVEL_STRUCTURE_H_

#include "Constant.h"
#include "Intrinsic.h"
#include "Module.h"
#include "User.h"
#include "Value.h"

namespace libnovel
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
    struct hash< libnovel::Structure* >
    {
      public:
        size_t operator()( const libnovel::Structure* obj ) const
        {
            assert( obj and " invalid pointer! " );
            return obj->hash();
        }
    };

    template <>
    struct equal_to< libnovel::Structure* >
    {
      public:
        bool operator()( const libnovel::Structure* lhs,
            const libnovel::Structure* rhs ) const
        {
            return lhs->hash() == rhs->hash();
        }
    };
}

#endif /* _LIB_NOVEL_STRUCTURE_H_ */

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
