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

#include "Type.h"

using namespace libnovel;


const char* Type::ID2str[ Type::ID::_TOP_ ] =
{ "Bit"      // BIT
, "Integer"  // INTEGER
//, "String"   // STRING
};


Type::Type( Type::ID id, i16 bitsize, Type::STATE state )
: type_id( id )
, type_uid_hash( 0 )
, type_state( Type::STATE::CHANGED )
, bitsize( bitsize )
{
	getName();
	type_state = state;
}

const Type::ID Type::getIDKind( void ) const
{
	return type_id;
}

const u64 Type::getID( void ) const
{
	return type_uid_hash;
}

const char* Type::getName( void )
{
	if( type_state != Type::STATE::CHANGED )
	{
		return description.c_str();
	}
	
	description.clear();
	if( parameters.size() > 0 )
	{
		u1 flag = 0;
		for( auto parameter : parameters )
		{
			if( flag )
			{
				description.append( " x " );
			}
			else
			{
				flag = 1;
			}
			description.append( parameter->getName() );
		}
		description.append( " -> " );
	}
	
	description.append( ID2str[ type_id ] );
	if( type_id == Type::BIT )
	{
		description.append( "(" );
		description.append( std::to_string( bitsize ) );
		description.append( ")" );
	}
	
	if( subtypes.size() > 0 )
	{
		u1 flag = 0;
		description.append( "( " );
		for( auto subtype : subtypes )
		{
			if( flag )
			{
				description.append( ", " );
			}
			else
			{
				flag = 1;
			}
			description.append( subtype->getName() );
		}
		description.append( " )" );
	}
	
	type_uid_hash = std::hash< std::string >()( description );
	
	return description.c_str();
}

const i16 Type::getBitsize( void )
{
	return bitsize;
}

const std::vector< Type* >& Type::getParameters( void ) const
{
	return parameters;
}

const std::vector< Type* >& Type::getSubTypes( void ) const
{
	return subtypes;
}

void Type::addParameter( Type* parameter )
{
	assert( parameter );
	assert( type_state != Type::STATE::LOCKED );
	parameters.push_back( parameter );
	
	type_state = Type::STATE::CHANGED;
	getName();
	type_state = Type::STATE::UNCHANGED;
}

void Type::addSubType( Type* subtype )
{
	assert( subtype );
	assert( type_state != Type::STATE::LOCKED );
	subtypes.push_back( subtype );

	type_state = Type::STATE::CHANGED;
	getName();
	type_state = Type::STATE::UNCHANGED;
}

Type* Type::getResultType( void )
{
	if( subtypes.size() == 0 )
	{
		if( type_id == Type::BIT )
		{
			return new Type( type_id, bitsize );
		}
		else if( type_id == Type::INTEGER )
		{
			return &IntegerType;
		}
		// else if( type_id == Type::STRING )
		// {
		// 	return &StringType;
		// }
		else
		{
			assert( 0 && "unimplemented result type with subtypes size zero!" );
			return 0;
		}
	}
	else
	{
		Type* t = new Type( type_id );
		for( auto subtype : subtypes )
		{
			t->addSubType( subtype );
		}
		return t;
	}
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
