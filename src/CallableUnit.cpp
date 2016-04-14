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
//  along with this program. If not, see <http://www.gnu.org/licenses/>.
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

const i16 CallableUnit::getParameterLength( void ) const
{
	return parameter_in.size() + parameter_out.size();
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
