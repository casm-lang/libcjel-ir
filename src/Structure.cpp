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

#include "Structure.h"

using namespace libnovel;


Structure::Structure( const char* name, Type* type, Structure* parent )
: User( name, type, Value::STRUCTURE )
, identifier( 0 )
, parent( parent )
{
	assert( name );
	
	if( !type )
	{
		Type* ty = new Type( Type::STRUCTURE );
		assert( ty );
		setType( ty );
	}
	
	identifier = Identifier::create( getType(), name, parent );
	assert( identifier );
	
	if( parent )
	{
		parent->add( this );
	}
	
	(*Value::getSymbols())[ ".structure" ].insert( this );
}

Structure::~Structure( void )
{
	(*Value::getSymbols())[ ".structure" ].erase( this );
}

const Identifier* Structure::getIdentifier( void ) const
{
	return identifier;
}

void Structure::add( Value* value )
{
	assert( value );
	assert( Value::isa< Structure >( value ) );
	Structure* s = (Structure*)value;
	//s->setParent( this );
	
	element.push_back( s );

	getType()->addSubType( value->getType() );
	
	assert( s->getParent() == this );
}

Value* Structure::get( u16 index ) const
{
	assert( index < element.size() );

	return element[ index ];
}

const std::vector< Structure* >& Structure::getElements( void ) const
{
	return element;
}

void Structure::setParent( Structure* value )
{
	assert( !parent );
	parent = value;
}

Structure* Structure::getParent( void ) const
{
	return parent;
}

void Structure::dump( void ) const
{
	printf( "[Structure ] " );
	debug();
}

bool Structure::classof( Value const* obj )
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
