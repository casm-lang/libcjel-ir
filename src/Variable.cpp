//
//  Copyright (c) 2015-2016 Philipp Paulweber
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                https://github.com/casm-lang/libnovel
//
//  This file is part of libnovel.
//
//  libnovel is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  libnovel is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with libnovel. If not, see <http://www.gnu.org/licenses/>.
//

#include "Variable.h"

using namespace libnovel;


u64 Variable::allocation_cnt = 0;


Variable::Variable( Type* type, Value* expression, const char* ident )
: User( ".variable", type, Value::VARIABLE )
, allocation_id( 0 )
, expression( expression )
, ident( ident )
{
	assert( type );
	assert( expression && Value::isa< Constants >(expression) );

	allocation_id = BitConstant::create( allocation_cnt, TypeId.getBitsize() );
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


void Variable::dump( void ) const
{
	printf( "[Variable ] " );
	debug();
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
