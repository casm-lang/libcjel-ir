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

#include "hayai.hpp"

#include "libcsel-ir.h"

using namespace libcsel_ir;

BENCHMARK( libcsel_ir__Type, make_BitType, 5, 5 )
{
    for( u16 c = 1; c <= BitType::SizeMax; c++ )
    {
        libstdhl::make< BitType >( c );
    }
}

BENCHMARK( libcsel_ir__Type, get_BitType, 5, 5 )
{
    for( u16 c = 1; c <= BitType::SizeMax; c++ )
    {
        libstdhl::get< BitType >( c );
    }
}

BENCHMARK( libcsel_ir__Type, old_BitType, 5, 5 )
{
    for( u16 c = 1; c <= BitType::SizeMax; c++ )
    {
        Type::Bit( c );
    }
}
