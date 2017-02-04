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

#include "gtest/gtest.h"

#include "libcsel-ir.h"

#include "libstdhl.h"

using namespace libcsel_ir;

static void test_constant_bit( Type::BitTy i, i16 s )
{
    auto t = Type::Bit( s );

    auto v = libstdhl::make< BitConstant >( t, i );

    EXPECT_STREQ( v->name(), std::to_string( i ).c_str() );
    EXPECT_STREQ( v->name(), std::to_string( v->value() ).c_str() );
    EXPECT_EQ( v->type().bitsize(), s );

    auto w = libstdhl::make< BitConstant >( t, i );

    EXPECT_TRUE( v != w );
    EXPECT_TRUE( *w == *w );

    auto a = libstdhl::get< BitConstant >( t, i );
    auto b = libstdhl::get< BitConstant >( t, i );

    EXPECT_TRUE( a == b );
    EXPECT_TRUE( *a == *b );
}

TEST( libcsel_ir__Constant_Bit, create_range )
{
    for( u16 s = 1; s < BitType::SizeMax; s++ )
    {
        for( Type::BitTy i = -128; i < 128; i++ )
        {
            test_constant_bit( i, s );
        }
    }
}

TEST( libcsel_ir__Constant_Bit, create_random )
{
    for( u16 s = 1; s < BitType::SizeMax; s++ )
    {
        for( u32 c = 0; c < 16; c++ )
        {
            Type::BitTy i = libstdhl::Random::uniform< Type::BitTy >();

            test_constant_bit( i, s );
        }
    }
}
