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
#include "Statement.h"

using namespace libcsel_ir;

Scope::Scope(
    const char* name, Type* type, Value* parent, u1 is_parallel, Value::ID id )
: Block( name, type, parent, is_parallel, id )
{
    if( parent and isa< CallableUnit >( parent ) )
    {
        ( (CallableUnit*)parent )->setContext( this );
    }
}

void Scope::add( Value* element )
{
    assert( isa< Block >( element ) );

    // if( isa< Scope >( element ) )
    // {
    // 	((Scope*)element)->setParent( this );
    // }

    m_blocks.push_back( (Block*)element );
}

const std::vector< Block* >& Scope::blocks( void ) const
{
    return m_blocks;
}

bool Scope::classof( Value const* obj )
{
    return obj->id() == classid() or SequentialScope::classof( obj )
           or ParallelScope::classof( obj );
}

SequentialScope::SequentialScope( Value* parent )
: Scope( "seq", Type::Label(), parent, false, Value::ID::SEQUENTIAL_SCOPE )
{
}

bool SequentialScope::classof( Value const* obj )
{
    return obj->id() == classid();
}

ParallelScope::ParallelScope( Value* parent )
: Scope( "par", Type::Label(), parent, false, Value::ID::PARALLEL_SCOPE )
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
