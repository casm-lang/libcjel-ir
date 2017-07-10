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

#include "Reference.h"

#include "CallableUnit.h"

using namespace libcjel_ir;

Reference::Reference(
    const std::string& name, const Type::Ptr& type, Kind kind )
: User( name, type, Value::REFERENCE )
, m_kind( kind )
, m_callable()
{
}

void Reference::setCallable( const CallableUnit::Ptr& callable )
{
    if( not callable )
    {
        throw std::domain_error(
            "cannot set callable of reference to null pointer" );
    }

    m_callable = callable;
}

CallableUnit::Ptr Reference::callable( void ) const
{
    return m_callable.lock();
}

Reference::Kind Reference::kind( void ) const
{
    return m_kind;
}

u1 Reference::isInput( void ) const
{
    return m_kind == INPUT;
}

u1 Reference::isOutput( void ) const
{
    return m_kind == OUTPUT;
}

u1 Reference::isLinkage( void ) const
{
    return m_kind == LINKAGE;
}

bool Reference::classof( Value const* obj )
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
