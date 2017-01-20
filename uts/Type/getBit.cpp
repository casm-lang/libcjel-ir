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

using namespace libcsel_ir;

TEST( libcsel_ir__Type_getBit, size_less_one_is_invalid )
{
    EXPECT_EXIT( Type::getBit( 0 ), ::testing::KilledBySignal( SIGABRT ), "" );
}

TEST( libcsel_ir__Type_getBit, size_greater_SizeMax_is_invalid )
{
    EXPECT_EXIT( Type::getBit( BitType::SizeMax + 1 ),
        ::testing::KilledBySignal( SIGABRT ), "" );
}

TEST( libcsel_ir__Type_getBit, from_one_to_SizeMax )
{
    Type* t = 0;

    for( u16 c = 1; c <= BitType::SizeMax; c++ )
    {
        Type::getBit( c );
    }
}
