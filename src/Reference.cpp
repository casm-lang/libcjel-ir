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

#include "Reference.h"

using namespace libnovel;


Reference::Reference( const char* name, Type* type, CallableUnit* callable, u1 input )
: User( ".reference", type, Value::REFERENCE )
, identifier( 0 )
, callable( 0 )
, input( input )
, structure( 0 )
{
	assert( name );
	assert( type );

    identifier = Identifier::create( type, name /* scope?!?!*/ );
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
		callable->addParameter( this, input );
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
	return input;
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
