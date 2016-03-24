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
	else if( Value::isa< Function >( value ) )
	{
		content[ Function::classid() ].push_back( value );
		//value->setRef< Function >( this );
	}
	else if( Value::isa< Component >( value ) )
	{
		content[ Component::classid() ].push_back( value );
		//value->setRef< Component >( this );
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
