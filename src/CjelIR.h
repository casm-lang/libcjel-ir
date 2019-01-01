//
//  Copyright (C) 2015-2019 CASM Organization <https://casm-lang.org>
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

#ifndef _LIBCJEL_IR_CJELIR_H_
#define _LIBCJEL_IR_CJELIR_H_

#include <libstdhl/Type>

#include <memory>

namespace libcjel_ir
{
    using u1 = libstdhl::u1;
    using u8 = libstdhl::u8;
    using u16 = libstdhl::u16;
    using u32 = libstdhl::u32;
    using u64 = libstdhl::u64;

    using i8 = libstdhl::i8;
    using i16 = libstdhl::i16;
    using i32 = libstdhl::i32;
    using i64 = libstdhl::i64;

    class CjelIR
    {
    };

    class Value;

    //
    // is-a relation
    //

    template < typename T >
    static inline u1 isa( Value* value )
    {
        if( value )
        {
            return T::classof( value );
        }
        else
        {
            return false;
        }
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

    template < typename T >
    static inline u1 isa( const std::shared_ptr< Value >& value )
    {
        if( auto ptr = value.get() )
        {
            return isa< T >( ptr );
        }
        return false;
    }

    //
    // casting utility
    //

    template < typename T >
    static inline T* cast( Value* value )
    {
        if( isa< T >( value ) )
        {
            return reinterpret_cast< T* >( value );
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
            return reinterpret_cast< const T* >( value );
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

    template < typename T >
    static inline const T* cast( const std::shared_ptr< Value >& value )
    {
        if( auto ptr = value.get() )
        {
            return cast< T >( ptr );
        }
        return nullptr;
    }
}  // namespace libcjel_ir

#endif  // _LIBCJEL_IR_CJELIR_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
