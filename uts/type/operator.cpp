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

TEST( libcsel_ir__Type_operator, equal )
{
    for( u16 x = 1; x <= BitType::SizeMax; x++ )
    {
        for( u16 y = 1; y <= BitType::SizeMax; y++ )
        {
            if( x == y )
            {
                EXPECT_TRUE( Type::Bit( x ) == Type::Bit( y ) );
            }
            else
            {
                EXPECT_FALSE( Type::Bit( x ) == Type::Bit( y ) );
            }
        }
    }
}

TEST( libcsel_ir__Type_operator, not_equal )
{
    for( u16 x = 1; x <= BitType::SizeMax; x++ )
    {
        for( u16 y = 1; y <= BitType::SizeMax; y++ )
        {
            if( x != y )
            {
                EXPECT_TRUE( Type::Bit( x ) != Type::Bit( y ) );
            }
            else
            {
                EXPECT_FALSE( Type::Bit( x ) != Type::Bit( y ) );
            }
        }
    }
}
