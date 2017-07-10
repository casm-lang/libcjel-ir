//
//  Copyright (c) 2015-2017 Philipp Paulweber
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                https://github.com/casm-lang/libcjel-ir
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

#include "Interconnect.h"

#include "Variable.h"

#include "../stdhl/cpp/Math.h"

using namespace libcjel_ir;

Interconnect::Interconnect( const std::string& name )
: User( name, libstdhl::get< InterconnectType >(), classid() )
, m_bs_max( 0 )
{
}

void Interconnect::add( const Value::Ptr& value )
{
    assert( isa< Variable >( value ) );

    m_objects.add( value );

    m_bs_max = std::max( m_bs_max, value->type().bitsize() );
}

Values Interconnect::objects( void ) const
{
    return m_objects;
}

u64 Interconnect::bitsizeMax( void ) const
{
    return m_bs_max;
}

bool Interconnect::classof( Value const* obj )
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
