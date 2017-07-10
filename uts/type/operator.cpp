//
//  Copyright (c) 2015-2017 Philipp Paulweber
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                https://github.com/casm-lang/libcjel-ir
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

#include "uts/main.h"

using namespace libcjel_ir;

TEST( libcjel_ir__Type_operator, equal )
{
    for( u16 x = 1; x <= BitType::SizeMax; x++ )
    {
        for( u16 y = 1; y <= BitType::SizeMax; y++ )
        {
            if( x == y )
            {
                EXPECT_TRUE( BitType( x ) == BitType( y ) );
            }
            else
            {
                EXPECT_FALSE( BitType( x ) == BitType( y ) );
            }
        }
    }
}

TEST( libcjel_ir__Type_operator, not_equal )
{
    for( u16 x = 1; x <= BitType::SizeMax; x++ )
    {
        for( u16 y = 1; y <= BitType::SizeMax; y++ )
        {
            if( x != y )
            {
                EXPECT_TRUE( BitType( x ) != BitType( y ) );
            }
            else
            {
                EXPECT_FALSE( BitType( x ) != BitType( y ) );
            }
        }
    }
}
