//  
//  Copyright (c) 2016 Philipp Paulweber
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/ppaulweber/libnovel
//  
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program. If not, see <http://www.gnu.org/licenses/>.
//  

#include "Memory.h"

using namespace libnovel;


Memory::Memory( Structure* structure, u32 size )
: User( ".memory", 0, Value::MEMORY )
, structure( structure )
, size( size )
{
	assert( structure );
	assert( size > 0 );
}

Memory::~Memory( void )
{			
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
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  
