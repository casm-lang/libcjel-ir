//
//  Copyright (c) 2015-2016 Philipp Paulweber
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                https://github.com/casm-lang/libnovel
//
//  This file is part of libnovel.
//
//  libnovel is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  libnovel is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with libnovel. If not, see <http://www.gnu.org/licenses/>.
//

#include "Memory.h"

using namespace libnovel;

Memory::Memory( Structure* structure, u32 size )
: User( ".memory", &TypeMemory, Value::MEMORY )
, structure( structure )
, size( size )
{
    assert( structure );
    assert( size > 0 );

    // setType( structure->getType() );
}

Memory::~Memory( void )
{
}

Structure* Memory::getStructure( void ) const
{
    assert( structure );
    return structure;
}

const u32 Memory::getSize( void ) const
{
    return size;
}

// ParallelBlock* Memory::getContext( void ) const
// {
// 	return context;
// }

// void Memory::setContext( ParallelBlock* scope )
// {
// 	assert( scope );
// 	context = scope;
// }

void Memory::dump( void ) const
{
    printf( "[Memory ] " );
    debug();

    // if( context )
    // {
    // 	context->dump();
    // }
    // else
    // {
    // 	printf( "('context' not set)\n" );
    // }
}

bool Memory::classof( Value const* obj )
{
    return obj->getValueID() == classid();
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
