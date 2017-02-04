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

#ifndef _LIB_CSELIR_CSELIR_H_
#define _LIB_CSELIR_CSELIR_H_

#include "../stdhl/cpp/Default.h"
#include "../stdhl/cpp/Type.h"

namespace libcsel_ir
{
    class CselIR
    {
    };

    class Value;

    //
    // is-a relation
    //

    template < typename T >
    static inline u1 isa( Value* value )
    {
        assert( value );
        return T::classof( value );
    }

    template < typename T >
    static inline u1 isa( const Value* value )
    {
        return isa< T >( (Value*)value );
    }

    template < typename T >
    static inline u1 isa( Value& value )
    {
        return isa< T >( &value );
    }

    template < typename T >
    static inline u1 isa( const Value& value )
    {
        return isa< T >( &value );
    }

    //
    // casting utility
    //

    template < typename T >
    static inline T* cast( Value* value )
    {
        if( isa< T >( value ) )
        {
            return static_cast< T* >( value );
        }
        else
        {
            return 0;
        }
    }

    template < typename T >
    static inline const T* cast( const Value* value )
    {
        if( isa< T >( value ) )
        {
            return static_cast< const T* >( value );
        }
        else
        {
            return 0;
        }
    }

    template < typename T >
    static inline T* cast( Value& value )
    {
        return cast< T >( &value );
    }

    template < typename T >
    static inline const T* cast( const Value& value )
    {
        return cast< T >( &value );
    }
}

#endif /* _LIB_CSELIR_CSELIR_H_ */

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
