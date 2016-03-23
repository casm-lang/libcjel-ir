//  
//  Copyright (c) 2015 Philipp Paulweber
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/ppaulweber/libnovel
//  
//  Permission is hereby granted, free of charge, to any person obtaining a 
//  copy of this software and associated documentation files (the "Software"), 
//  to deal with the Software without restriction, including without limitation 
//  the rights to use, copy, modify, merge, publish, distribute, sublicense, 
//  and/or sell copies of the Software, and to permit persons to whom the 
//  Software is furnished to do so, subject to the following conditions:
//  
//  * Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimers.
//  
//  * Redistributions in binary form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimers in the 
//    documentation and/or other materials provided with the distribution.
//  
//  * Neither the names of the copyright holders, nor the names of its 
//    contributors may be used to endorse or promote products derived from 
//    this Software without specific prior written permission.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
//  CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
//  WITH THE SOFTWARE.
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
		Type* ty = new Type( Type::STRUCTURE, -1, Type::STATE::LOCKED );
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
