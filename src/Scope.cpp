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

#include "Scope.h"

using namespace libcsel_ir;

//
// Scope
//

Scope::Scope(
    const std::string& name, const Type::Ptr& type, u1 parallel, Value::ID id )
: Block( name, type, parallel, id )
{
}

void Scope::add( const Block::Ptr& block )
{
    if( not block )
    {
        throw std::domain_error( "cannot add a null pointer block to a scope" );
    }

    m_blocks.add( block );
}

Blocks Scope::blocks( void ) const
{
    return m_blocks;
}

bool Scope::classof( Value const* obj )
{
    return obj->id() == classid() or SequentialScope::classof( obj )
           or ParallelScope::classof( obj );
}

//
// Sequential Scope
//

SequentialScope::SequentialScope( void )
: Scope( "seq", libstdhl::get< LabelType >(), false, classid() )
{
}

bool SequentialScope::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Parallel Scope
//

ParallelScope::ParallelScope( void )
: Scope( "par", libstdhl::get< LabelType >(), false, classid() )
{
}

bool ParallelScope::classof( Value const* obj )
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
