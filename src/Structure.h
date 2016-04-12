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

#ifndef _LIB_NOVEL_STRUCTURE_H_
#define _LIB_NOVEL_STRUCTURE_H_

#include "Value.h"
#include "Module.h"
#include "User.h"
#include "Intrinsic.h"
#include "Constant.h"

namespace libnovel
{
	class Structure : public User
	{
	private:
		Identifier* identifier;
		std::vector< Structure* > element;
		Structure* parent;
		
	public:
		Structure( const char* name, Type* type = 0, Structure* parent = 0 );
		
		~Structure( void );
		
		const Identifier* getIdentifier( void ) const;

		void add( Value* value );
		
		Value* get( u16 index ) const; 
		const std::vector< Structure* >& getElements( void ) const;

		void setParent( Structure* value );
		Structure* getParent( void ) const;
		
		void dump( void ) const;
		
		static inline Value::ID classid( void ) { return Value::STRUCTURE; };
		static bool classof( Value const* obj );
	};
}


#endif /* _LIB_NOVEL_STRUCTURE_H_ */

//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  
