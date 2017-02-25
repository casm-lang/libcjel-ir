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

#include "Constant.h"
#include "Function.h"
#include "Interconnect.h"
#include "Memory.h"
#include "Structure.h"
#include "Variable.h"

using namespace libcsel_ir;

Module::Module( const std::string& name )
: User( name, libstdhl::get< VoidType >(), Value::MODULE )
{
}

void Module::add( const Value::Ptr& value )
{
    if( isa< Structure >( value ) )
    {
        m_content[ Structure::classid() ].add( value );
    }
    else if( isa< Constant >( value ) )
    {
        m_content[ Constant::classid() ].add( value );
    }
    else if( isa< Variable >( value ) )
    {
        m_content[ Variable::classid() ].add( value );
    }
    else if( isa< Memory >( value ) )
    {
        m_content[ Memory::classid() ].add( value );
    }
    else if( isa< Intrinsic >( value ) )
    {
        m_content[ Intrinsic::classid() ].add( value );
    }
    else if( isa< Function >( value ) )
    {
        m_content[ Function::classid() ].add( value );
    }
    else if( isa< Interconnect >( value ) )
    {
        m_content[ Interconnect::classid() ].add( value );
    }
    else
    {
        assert( !"unsupported Module content found!" );
    }
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
