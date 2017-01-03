//
//  Copyright (c) 2015-2017 Philipp Paulweber
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                https://github.com/casm-lang/libcsel-ir
//
//  This file is part of libcsel-ir.
//
//  libcsel-ir is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  libcsel-ir is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with libcsel-ir. If not, see <http://www.gnu.org/licenses/>.
//

#include "Module.h"
#include "Interconnect.h"
#include "Variable.h"

using namespace libcsel_ir;

Module::Module( const char* name )
: User( name, 0, Value::MODULE )
{
    assert( name );

    // assert(0);

    ( *Value::getSymbols() )[ ".module" ].insert( this );
}

Module::~Module( void )
{
    ( *Value::getSymbols() )[ ".module" ].erase( this );
}

void Module::add( Value* value )
{
    assert( value );

    Module* m = value->getRef< Module >();
    if( m )
    {
        assert( m != this and " 'value' can only be part of one module" );
    }

    if( Value::isa< Structure >( value ) )
    {
        content[ Structure::classid() ].push_back( value );
    }
    else if( Value::isa< Constants >( value ) )
    {
        content[ Constants::classid() ].push_back( value );
    }
    else if( Value::isa< Variable >( value ) )
    {
        content[ Variable::classid() ].push_back( value );
    }
    else if( Value::isa< Memory >( value ) )
    {
        content[ Memory::classid() ].push_back( value );
    }
    else if( Value::isa< Intrinsic >( value ) )
    {
        content[ Intrinsic::classid() ].push_back( value );
    }
    else if( Value::isa< Function >( value ) )
    {
        content[ Function::classid() ].push_back( value );
    }
    else if( Value::isa< Interconnect >( value ) )
    {
        content[ Interconnect::classid() ].push_back( value );
    }
    else
    {
        assert( !"unsupported Module content found!" );
    }

    value->setRef< Module >( this );
}

// const std::unordered_map< u8, std::vector< Value* > >& Module::getContent(
// void ) const
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
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
