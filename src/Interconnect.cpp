//  
//  Copyright (c) 2015-2016 Philipp Paulweber
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/ppaulweber/libnovel
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

#include "Interconnect.h"

using namespace libnovel;


Interconnect::Interconnect( void )
: User( ".interconnect", &TypeInterconnect, classid() )
, bs_max( 0 )
{
}

Interconnect::~Interconnect( void )
{			
}

void Interconnect::add( Value* object )
{
	assert( Value::isa< Variable >( object ) );
	objects.push_back( object );

	bs_max = std::max( bs_max, object->getType()->getBitsize() );
}

const std::vector< Value* >& Interconnect::getObjects( void ) const
{
	return objects;
}

const i16 Interconnect::getBitsizeMax( void ) const
{
	return bs_max;
}

void Interconnect::dump( void ) const
{
	printf( "[Interconnect ] " );
	debug();
}

bool Interconnect::classof( Value const* obj )
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
