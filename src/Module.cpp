//
//  Copyright (C) 2015-2021 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                <https://github.com/casm-lang/libcjel-ir>
//
//  This file is part of libcjel-ir.
//
//  libcjel-ir is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  libcjel-ir is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with libcjel-ir. If not, see <http://www.gnu.org/licenses/>.
//
//  Additional permission under GNU GPL version 3 section 7
//
//  libcjel-ir is distributed under the terms of the GNU General Public License
//  with the following clarification and special exception: Linking libcjel-ir
//  statically or dynamically with other modules is making a combined work
//  based on libcjel-ir. Thus, the terms and conditions of the GNU General
//  Public License cover the whole combination. As a special exception,
//  the copyright holders of libcjel-ir give you permission to link libcjel-ir
//  with independent modules to produce an executable, regardless of the
//  license terms of these independent modules, and to copy and distribute
//  the resulting executable under terms of your choice, provided that you
//  also meet, for each linked independent module, the terms and conditions
//  of the license of that module. An independent module is a module which
//  is not derived from or based on libcjel-ir. If you modify libcjel-ir, you
//  may extend this exception to your version of the library, but you are
//  not obliged to do so. If you do not wish to do so, delete this exception
//  statement from your version.
//

#include "Module.h"

#include <libcjel-ir/Constant>
#include <libcjel-ir/Function>
#include <libcjel-ir/Interconnect>
#include <libcjel-ir/Intrinsic>
#include <libcjel-ir/Memory>
#include <libcjel-ir/Structure>
#include <libcjel-ir/Variable>

#include <libstdhl/Hash>
#include <libstdhl/Memory>

#include <cassert>

using namespace libcjel_ir;

Module::Module( const std::string& name )
: User( name, libstdhl::Memory::get< VoidType >(), Value::MODULE )
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

std::size_t Module::hash( void ) const
{
    return libstdhl::Hash::combine( classid(), std::hash< std::string >()( name() ) );
}

u1 Module::classof( Value const* obj )
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
