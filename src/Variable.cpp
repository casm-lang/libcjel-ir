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

#include "Variable.h"

using namespace libcsel_ir;

u64 Variable::allocation_cnt = 0;

Variable::Variable( Type* type, Value* expression, const char* ident )
: User( ".variable", type, Value::VARIABLE )
, allocation_id( 0 )
, expression( expression )
, ident( ident )
{
    assert( type );
    assert( expression && isa< Constant >( expression ) );

    allocation_id = cast< BitConstant >(
        Constant::getBit( Type::getTypeID(), allocation_cnt ) );
    allocation_cnt++;
    assert( allocation_id );
}

Variable::~Variable( void )
{
}

BitConstant* Variable::getAllocationID( void )
{
    assert( allocation_id );
    return allocation_id;
}

Value* Variable::getExpression( void ) const
{
    return expression;
}

const char* Variable::getIdent( void ) const
{
    return ident;
}

bool Variable::classof( Value const* obj )
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
