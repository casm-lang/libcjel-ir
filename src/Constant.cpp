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

#include "Constant.h"
#include "Structure.h"

using namespace libnovel;


template< typename V >
Constant< V >::Constant( const char* name, Type* type, V value, Value::ID id )
: User( name, type, id )
, value( value )
{
	(*Value::getSymbols())[ ".constant" ].insert( this );
}

template< typename V >
Constant< V >::~Constant( void )
{
	(*Value::getSymbols())[ ".constant" ].erase( this );	
}

template< typename T >
bool Constant< T >::classof( Value const* obj )
{
	assert( 0 && "invalid to check 'classof' Constant< V > template class, use Constants" );
	return false;
}


template< typename V >
const V Constant< V >::getValue( void ) const
{
	return value;
}

template< typename V >
void Constant< V >::setValue( V val )
{
	value = val;
}

bool Constants::classof( Value const* obj )
{
	return obj->getValueID() == classid()
	    or BitConstant::classof( obj )
		or StructureConstant::classof( obj )
		//or Identifier::classof( obj )
		;
}


BitConstant::BitConstant( Type* type, u64 value )
: Constant< Type::Bit >( ".const_bit", type, this->value, Value::BIT_CONSTANT )
{
	this->value[0] = value;
}

BitConstant* BitConstant::create( u64 value, u16 bitsize )
{
	//static std::unordered_map< u64, BitConstant* > cache[256];
	
	assert( bitsize > 0 and bitsize <= 256 and "invalid 'Bit' constant bit size" );
	
	// TODO: FIXME: improve this to save memory and gain more compilation performance 
	// auto result = cache[bitsize].find( value );
	// if( result != cache[bitsize].end() )
	// {
	// 	assert( result->second );
	// 	printf( "[Const] found %p\n", result->second );
	// 	return result->second;
	// }
	
	BitConstant* obj = new BitConstant( new Type( Type::ID::BIT, bitsize ), value );
	//cache[bitsize][ value ] = obj;
	return obj;
}

void BitConstant::dump( void ) const
{
	printf( "[Const] %p = bit %li\n", this, getValue()[0] );
}

bool BitConstant::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}




StructureConstant::StructureConstant( Type* type, std::vector< Value* > value )
: Constant< std::vector< Value* > >( ".const_struct", type, value, Value::STRUCTURE_CONSTANT )
{
	assert( type );
	libnovel::Value* b = type->getBound();
	assert( b and libnovel::Value::isa< libnovel::Structure >( b ) );
	libnovel::Structure* s = (libnovel::Structure*)b;
		
	if( value.size() == 0 ) 
	{
		if( s->getElements().size() > 0 )
		{
			for( Value* e : s->getElements() )
			{
				if( e->getType()->getIDKind() == Type::BIT )
				{
					
				}
				else
				{
					assert( !"struct in struct in ... not implemented yet!" );
				}
			}
		}
		else
		{
			assert( !"empty structure found, should not be possible!" );
		}
	}
	else
	{
		for( Value* p : value )
		{
			assert( p );
			if( Value::isa< BitConstant >( p ) )
			{
				((BitConstant*)p)->bind( this );
			}
			else if( Value::isa< StructureConstant >( p ) )
			{
				((StructureConstant*)p)->bind( this );
			}
		}
	}
}

StructureConstant* StructureConstant::create( Structure* kind, const std::vector< Value* >& value )
{
	// TODO: FIXME: PPA: silly implmeentation for now!!! must be improved to gain performance and use less memory!!!

	// TODO: FIXME: PPA: check if KIND structure subtypes match the provided value subtypes in the vector!!!

	assert( kind );
	assert( kind->getType() );
	
	StructureConstant* obj = new StructureConstant( kind->getType(), value );
	return obj;
}

const std::vector< Value* >& StructureConstant::getElements( void ) const
{
	return value;
}

void StructureConstant::dump( void ) const
{
	printf( "[Const] %p = structure %p\n", this, &getElements() );
}

bool StructureConstant::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}







Identifier::Identifier( Type* type, const char* value )
: Constant< const char* >( value, type, value, Value::IDENTIFIER )
{
    (*Value::getSymbols())[ ".identifier" ].insert( this );
}

Identifier::~Identifier( void )
{
	(*Value::getSymbols())[ ".identifier" ].erase( this );
}

Identifier* Identifier::create( Type* type, std::string value, Value* scope )
{
	return create( type, value.c_str(), scope );
}

Identifier* Identifier::create( Type* type, const char* value, Value* scope )
{
	SymbolTable& symbols = *getSymbols();

	const char* tmp = value;
	
	if( scope )
	{
		tmp = libstdhl::Allocator::string( string(scope->getName()) + "::" + string(tmp) );
	}
	
	auto result = symbols.find( tmp );
	if( result != symbols.end() )
	{
		assert( result->second.size() == 1 );
		Value* x = *result->second.begin();

		//assert( x->getType()->getID() == type->getID() );
		if( x->getType()->getID() == type->getID() )
		{
			printf( "[Ident] found '%s' of type %lu @ %p\n", value, type->getID(), x );
			return (Identifier*)x;
		}
	}
	


// //const char* tmp_scope = value;
		
// 	if( scope )
// 	{
// 		// std::string tmp;
// 		// tmp.append( scope->getName() );
// 		// tmp.append( "::" );
// 		// tmp.append( value );
// 		// tmp_scope = tmp.c_str();
// 	}
	
// 	auto result = symbols.find( value );
// 	if( result != symbols.end() )
// 	{
// 		assert( result->second.size() == 1 );
// 		Value* x = *result->second.begin();
// 	 	assert( x->getType()->getID() == type->getID() );
// 		printf( "[Ident] found '%s' of type %lu @ %p\n", value, type->getID(), x );
// 		return (Identifier*)x;
// 	}
	
	printf( "[Ident] creating '%s' of type %lu\n", value, type->getID() );
	return new Identifier( type, value );
}


void Identifier::forgetSymbol( const char* value )
{
	printf( "[Ident] forgetting '%s'\n", value );
	getSymbols()->erase( value );
}

void Identifier::dump( void ) const
{
	printf( "[Ident] " );
	debug();
}

bool Identifier::classof( Value const* obj )
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
