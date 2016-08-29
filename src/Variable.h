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

#ifndef _LIB_NOVEL_VARIABLE_H_
#define _LIB_NOVEL_VARIABLE_H_

#include "Value.h"
#include "User.h"
#include "Constant.h"
#include "Instruction.h"

namespace libnovel
{
	class Variable : public User
	{
	private:
		static u64 allocation_cnt;
		BitConstant* allocation_id;		
		
		Value* expression;
		const char* ident;
		
	public:
		Variable( Type* type, Value* expression, const char* ident = 0 );
		
		~Variable( void );

		BitConstant* getAllocationID( void );

		Value* getExpression( void ) const;
		const char* getIdent( void ) const;
		
		void dump( void ) const;

		static inline ID classid( void ) { return Value::VARIABLE; };
		static bool classof( Value const* obj );
	};
}


#endif /* _LIB_NOVEL_VARIABLE_H_ */

//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  
