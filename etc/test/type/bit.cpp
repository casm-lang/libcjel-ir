//
//  Copyright (C) 2015-2023 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber et al.
//  <https://github.com/casm-lang/libcjel-ir/graphs/contributors>
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

#include "../main.h"

using namespace libcjel_ir;

TEST( libcjel_ir__type_bit, size_less_one_is_invalid )
{
    EXPECT_THROW( { BitType( 0 ); }, std::domain_error );
}

TEST( libcjel_ir__type_bit, size_greater_SizeMax_is_invalid )
{
    EXPECT_THROW( { BitType( BitType::SizeMax + 1 ); }, std::domain_error );
}

TEST( libcjel_ir__type_bit, from_one_to_SizeMax )
{
    for( u16 c = 1; c <= BitType::SizeMax; c++ )
    {
        auto stack = BitType( c );

        auto heap_a = libstdhl::Memory::make< BitType >( c );
        auto heap_b = libstdhl::Memory::make< BitType >( c );

        ASSERT_TRUE( heap_a != nullptr );
        ASSERT_TRUE( heap_b != nullptr );
        EXPECT_TRUE( heap_a != heap_b );
        EXPECT_TRUE( *heap_a == *heap_b );

        auto cache_a = libstdhl::Memory::get< BitType >( c );
        auto cache_b = libstdhl::Memory::get< BitType >( c );

        ASSERT_TRUE( cache_a != nullptr );
        ASSERT_TRUE( cache_b != nullptr );
        EXPECT_TRUE( cache_a == cache_b );
        EXPECT_TRUE( *cache_a == *cache_b );
    }
}

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
