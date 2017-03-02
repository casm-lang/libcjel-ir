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

#include "uts/main.h"

using namespace libcsel_ir;

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
    EXPECT_STREQ(
        v->name().c_str(), std::to_string( v->value()[ 0 ] ).c_str() );

    EXPECT_EQ( v->value()[ 0 ], i_mask );

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

TEST( libcsel_ir__constant_bit, example )
{
    test_constant_bit( 1234, BitType::SizeMax );
}

TEST( libcsel_ir__constant_bit, create_range )
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

TEST( libcsel_ir__constant_bit, create_random )
{
    for( u16 s = 1; s <= BitType::SizeMax; s++ )
    {
        for( u32 c = 0; c < 16; c++ )
        {
            auto i = libstdhl::Random::uniform< u64 >(
                0, (u64)std::floor( std::pow( (long double)2, (long double)s ) )
                       - 1 );

            test_constant_bit( i, s );
        }
    }
}
