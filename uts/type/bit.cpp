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

TEST( libcsel_ir__type_bit, size_less_one_is_invalid )
{
    EXPECT_THROW( { BitType( 0 ); }, std::domain_error );
}

TEST( libcsel_ir__type_bit, size_greater_SizeMax_is_invalid )
{
    EXPECT_THROW( { BitType( BitType::SizeMax + 1 ); }, std::domain_error );
}

TEST( libcsel_ir__type_bit, from_one_to_SizeMax )
{
    for( u16 c = 1; c <= BitType::SizeMax; c++ )
    {
        auto stack = BitType( c );

        auto heap_a = libstdhl::make< BitType >( c );
        auto heap_b = libstdhl::make< BitType >( c );

        ASSERT_TRUE( heap_a != nullptr );
        ASSERT_TRUE( heap_b != nullptr );
        EXPECT_TRUE( heap_a != heap_b );
        EXPECT_TRUE( *heap_a == *heap_b );

        auto cache_a = libstdhl::get< BitType >( c );
        auto cache_b = libstdhl::get< BitType >( c );

        ASSERT_TRUE( cache_a != nullptr );
        ASSERT_TRUE( cache_b != nullptr );
        EXPECT_TRUE( cache_a == cache_b );
        EXPECT_TRUE( *cache_a == *cache_b );
    }
}
