//
//  Copyright (C) 2015-2020 CASM Organization <https://casm-lang.org>
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

#include "uts/main.h"

using namespace libcjel_ir;

static void test_constant_bit( u64 i, i16 s )
{
    u64 i_mask = i;

    auto t = libstdhl::make< BitType >( s );

    auto v = libstdhl::make< BitConstant >( t, i );

    EXPECT_EQ( v->type().bitsize(), s );

    if( s < 64 )
    {
        i_mask = i & ( ~( ( (u64)UINT64_MAX ) << s ) );
    }

    EXPECT_STREQ( v->name().c_str(), std::to_string( i_mask ).c_str() );

    if( v->name().compare( std::to_string( i_mask ) ) )
    {
        printf( "input: '%lu' with bitsize: '%lu'\n", i, v->type().bitsize() );
    }

    auto w = libstdhl::make< BitConstant >( t, i );

    EXPECT_TRUE( v != w );
    EXPECT_TRUE( *w == *w );

    auto a = libstdhl::get< BitConstant >( t, i );
    auto b = libstdhl::get< BitConstant >( t, i );

    EXPECT_TRUE( a == b );
    EXPECT_TRUE( *a == *b );
}

TEST( libcjel_ir__constant_bit, example )
{
    test_constant_bit( 1234, BitType::SizeMax );
}

TEST( libcjel_ir__constant_bit, create_range )
{
    for( u16 s = 1; s <= BitType::SizeMax; s++ )
    {
        test_constant_bit( 0, s );
        test_constant_bit( 1, s );

        for( u64 i = 2; i < s; i *= 3 )
        {
            test_constant_bit( i, s );
        }
    }
}

TEST( libcjel_ir__constant_bit, create_random )
{
    for( u16 s = 1; s <= BitType::SizeMax; s++ )
    {
        for( u32 c = 0; c < 16; c++ )
        {
            auto i = libstdhl::Random::uniform< u64 >( 0,
                (u64)std::floor( std::pow( (long double)2, (long double)s ) )
                    - 1 );

            test_constant_bit( i, s );
        }
    }
}
