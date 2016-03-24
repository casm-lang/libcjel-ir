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

#include "CallableUnit.h"
#include "Reference.h"

#include "Intrinsic.h"
#include "Function.h"

using namespace libnovel;


CallableUnit::CallableUnit( const char* name, Type* type, Value::ID id )
: User( name, type, id )
, context( 0 )
, identifier( 0 )
{
	identifier = Identifier::create( type, name, this );
	assert( identifier );
	
	(*Value::getSymbols())[ ".callableunit" ].insert( this );
}

CallableUnit::~CallableUnit( void )
{			
	(*Value::getSymbols())[ ".callableunit" ].erase( this );
}

Block* CallableUnit::getContext( void ) const
{
	return context;
}

void CallableUnit::setContext( Block* scope )
{
	assert( scope );
	
	context = scope;
	
	scope->setParent( this );
}

const Identifier* CallableUnit::getIdentifier( void ) const
{
	return identifier;
}


void CallableUnit::addParameter( Value* value, u1 input )
{
	assert( value );
	assert( Value::isa< Reference >( value ) );
	
	Reference* ref = (Reference*)value;
	ref->setCallableUnit( this );
	
	if( input )
	{
		parameter2index[ value ] = parameter_in.size();
		parameter_in.push_back( value );
	}
	else
	{
		parameter2index[ value ] = parameter_in.size();
		parameter_out.push_back( value );
	}	
}

void CallableUnit::addLinkage( Value* value )
{
	assert( value );
	assert( Value::isa< Reference >( value ) );
	
	Reference* ref = (Reference*)value;
	ref->setCallableUnit( this );

	linkage.push_back( value );
}

const std::vector< Value* >& CallableUnit::getInParameters( void ) const
{
	return parameter_in;
}

const std::vector< Value* >& CallableUnit::getOutParameters( void ) const
{
	return parameter_out;	
}

const i16 CallableUnit::getIndexOfParameter( Value* value ) const
{
	auto result = parameter2index.find( value );
	if( result != parameter2index.end() )
	{
		return result->second;
	}

	return -1;
}

const u1 CallableUnit::isLastParameter( Value* value ) const
{
	assert( Value::isa< Reference >( value ) );
	Reference* ref = (Reference*)value;
	
	i16 index = getIndexOfParameter( ref );
	assert( index >= 0 );
	
	i16 total = parameter_in.size() + parameter_out.size();

	if( not ref->isInput() )
	{
		index += parameter_in.size();
	}
    
	return index >= (total - 1);
}

const std::vector< Value* >& CallableUnit::getLinkage( void ) const
{
	return linkage;
}


void CallableUnit::dump( void ) const
{
	printf( "[CallableUnit ] " );
	debug();
	
	if( context )
	{
		context->dump();
	}
	else
	{
		printf( "('context' not set)\n" );
	}
}

bool CallableUnit::classof( Value const* obj )
{
	return obj->getValueID() == classid()
		or Intrinsic::classof( obj )
		or Function::classof( obj )
		;
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
