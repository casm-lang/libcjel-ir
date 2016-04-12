//  
//  Copyright (c) 2016 Philipp Paulweber
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/ppaulweber/libnovel
//  
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program. If not, see <http://www.gnu.org/licenses/>.
//  

#ifndef _LIB_NOVEL_BLOCK_H_
#define _LIB_NOVEL_BLOCK_H_

#include "Value.h"

namespace libnovel
{
	class Block : public Value
	{
	private:
	    Value* parent;
		u1 is_parallel;
		
  	public:
		Block
		( const char* name
		, Type* type
		, Value* parent
		, u1 is_parallel
		, Value::ID id = Value::BLOCK
		);
		
	    void setParent( Value* parent );
		const Value* getParent( void ) const;
		
		virtual const u1 isParallel( void ) const;
	    
		void dump( void ) const;
		
		static inline bool classid( void ) { return Value::BLOCK; }
		static bool classof( Value const* obj );
	};
}


#endif /* _LIB_NOVEL_BLOCK_H_ */

//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  
