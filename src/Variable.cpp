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

#include "Variable.h"

using namespace libcjel_ir;

u64 Variable::m_allocation_cnt = 0;

Variable::Variable( const Type::Ptr& type, const Value::Ptr& expression,
    const std::string& ident )
: User( ident, type, Value::VARIABLE )
, m_expression( expression )
, m_allocation_id( libstdhl::make< BitConstant >( 64, m_allocation_cnt ) )
{
    assert( expression && isa< Constant >( expression ) );

    m_allocation_cnt++;
}

BitConstant::Ptr Variable::allocId( void )
{
    return m_allocation_id;
}

Value::Ptr Variable::expression( void ) const
{
    return m_expression;
}

bool Variable::classof( Value const* obj )
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
