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

#include "Reference.h"

using namespace libnovel;


Reference::Reference( const char* name, Type* type, CallableUnit* callable, Kind kind )
: User( ".reference", type, Value::REFERENCE )
, identifier( 0 )
, callable( 0 )
, kind( kind )
, structure( 0 )
{
	assert( name );
	assert( type );

    identifier = Identifier::create( type, name, callable /* scope?!?!*/ );
	assert( identifier );
	
	if( type->isBound() )
	{
		Value* bind = type->getBound();
		if( bind and libnovel::Value::isa< libnovel::Structure >( bind ) )
		{
			structure = (Structure*)bind;
		}
	}
	
	if( callable )
	{
		if( kind != LINKAGE )
		{
			callable->addParameter( this, kind == INPUT );
		}
		else
		{
			callable->addLinkage( this );
		}
	}
}

Reference::~Reference( void )
{
}

const Identifier* Reference::getIdentifier( void ) const
{
	return identifier;
}

const CallableUnit* Reference::getCallableUnit( void ) const
{
	return callable;
}

void Reference::setCallableUnit( CallableUnit* value )
{
	assert( !callable );
	callable = value;
}

const u1 Reference::isInput( void ) const
{
	return kind == INPUT;
}

const u1 Reference::isOutput( void ) const
{
	return kind == OUTPUT;
}

const u1 Reference::isLinkage( void ) const
{
	return kind == LINKAGE;
}

const Structure* Reference::getStructure( void ) const
{
	assert( structure );
	return structure;
}


const u1 Reference::isStructure( void ) const
{
	return structure != 0;
}


void Reference::dump( void ) const
{
	printf( "[Reference ] " );
	debug();
}

bool Reference::classof( Value const* obj )
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
