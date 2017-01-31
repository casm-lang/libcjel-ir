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
}

Module::~Module( void )
{
}

void Module::add( Value* value )
{
    assert( value );

    Module* m = value->ref< Module >();
    if( m )
    {
        assert( m != this and " 'value' can only be part of one module" );
    }

    if( isa< Structure >( value ) )
    {
        m_content[ Structure::classid() ].push_back( value );
    }
    else if( isa< Constant >( value ) )
    {
        m_content[ Constant::classid() ].push_back( value );
    }
    else if( isa< Variable >( value ) )
    {
        m_content[ Variable::classid() ].push_back( value );
    }
    else if( isa< Memory >( value ) )
    {
        m_content[ Memory::classid() ].push_back( value );
    }
    else if( isa< Intrinsic >( value ) )
    {
        m_content[ Intrinsic::classid() ].push_back( value );
    }
    else if( isa< Function >( value ) )
    {
        m_content[ Function::classid() ].push_back( value );
    }
    else if( isa< Interconnect >( value ) )
    {
        m_content[ Interconnect::classid() ].push_back( value );
    }
    else
    {
        assert( !"unsupported Module content found!" );
    }

    value->setRef< Module >( this );
}

bool Module::classof( Value const* obj )
{
    return obj->id() == classid();
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
