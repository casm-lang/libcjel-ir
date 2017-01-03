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

#include "Block.h"
#include "Statement.h"

using namespace libcsel_ir;

Block::Block(
    const char* name, Type* type, Value* parent, u1 is_parallel, Value::ID id )
: Value( name, type, id )
, parent( parent )
, is_parallel( is_parallel )
{
}

void Block::setParent( Value* parent )
{
    assert( parent );
    this->parent = parent;
}

const Value* Block::getParent( void ) const
{
    return parent;
}

const u1 Block::isParallel( void ) const
{
    return is_parallel;
}

void Block::dump( void ) const
{
    // printf( "[Block] %p\n", this );
    ( (Value*)this )->dump();
}

bool Block::classof( Value const* obj )
{
    return obj->getValueID() == classid() or Scope::classof( obj )
           or Statement::classof( obj );
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
