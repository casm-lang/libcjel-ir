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

#include "Module.h"
#include "Variable.h"

using namespace libnovel;


Module::Module( const char* name )
: User( name, 0, Value::MODULE )
{	
	assert( name );

	//assert(0);
	
	(*Value::getSymbols())[ ".module" ].insert( this );
}

Module::~Module( void )
{			
	(*Value::getSymbols())[ ".module" ].erase( this );
}

void Module::add( Value* value )
{
	assert( value );
	
	if( Value::isa< Structure >( value ) )
	{
		content[ Structure::classid() ].push_back( value );
		//value->setRef< Module >( this );
	}
	else if( Value::isa< Constants >( value ) )
	{
		content[ Constants::classid() ].push_back( value );
		//value->setRef< Constants >( this );
	}
	else if( Value::isa< Variable >( value ) )
	{
		content[ Variable::classid() ].push_back( value );
		//value->setRef< Variable >( this );
	}
	else if( Value::isa< Memory >( value ) ) 
	{
		content[ Memory::classid() ].push_back( value );
		//value->setRef< Memory >( this );
	}
	else if( Value::isa< Intrinsic >( value ) )
	{
		content[ Intrinsic::classid() ].push_back( value );
		//value->setRef< Intrinsic >( this );
	}
	else if( Value::isa< Function >( value ) )
	{
		content[ Function::classid() ].push_back( value );
		//value->setRef< Function >( this );
	}
	else
	{
		assert( !"unsupported Module content found!" );
	}

	value->setRef< Module >( this );
}

// const std::unordered_map< u8, std::vector< Value* > >& Module::getContent( void ) const
// {
// 	return content;
// }
		

void Module::dump( void ) const
{
	printf( "[Module ] " );
	debug();
}

bool Module::classof( Value const* obj )
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
