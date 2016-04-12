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

#include "Type.h"

using namespace libnovel;


const char* Type::ID2str[ Type::ID::_TOP_ ] =
{ "Bit"        // BIT
, "Structure"  // STRUCTURE
, "Function"   // FUNCTION
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
	assert( !"DEPRECATED: DO NOT USE THIS FUNCTION" );
	
	if( subtypes.size() == 0 )
	{
		if( type_id == Type::BIT )
		{
			return new Type( type_id, bitsize );
		}
		else if( type_id == Type::STRUCTURE )
		{
			return new Type( type_id, bitsize );
		}
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
