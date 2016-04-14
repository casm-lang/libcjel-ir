//  
//  Copyright (c) 2015-2016 Philipp Paulweber
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/ppaulweber/libnovel
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
//  along with this program. If not, see <http://www.gnu.org/licenses/>.
//  

#ifndef _LIB_NOVEL_CALLABLE_UNIT_H_
#define _LIB_NOVEL_CALLABLE_UNIT_H_

#include "Value.h"
#include "User.h"
#include "Block.h"
#include "Constant.h"

namespace libnovel
{	
	class CallableUnit : public User
	{
	private:
		Block* context;

		Identifier* identifier;
	    std::vector< Value* > parameter_in;
	    std::vector< Value* > parameter_out;

		std::unordered_map< Value*, u16 > parameter2index;

	    std::vector< Value* > linkage;
		
	public:
		CallableUnit( const char* name, Type* type, Value::ID id = CALLABLE_UNIT );
		
		~CallableUnit( void );
		
	    Block* getContext( void ) const;		
		void setContext( Block* scope );

		const Identifier* getIdentifier( void ) const;

		void addParameter( Value* value, u1 input = true );		
	    void addLinkage( Value* value );		
	    
		const std::vector< Value* >& getInParameters( void ) const;
		const std::vector< Value* >& getOutParameters( void ) const;
		
		const i16 getIndexOfParameter( Value* value ) const;
		const u1 isLastParameter( Value* value ) const;
		
		const std::vector< Value* >& getLinkage( void ) const;
		
		void dump( void ) const;
		
		static inline Value::ID classid( void ) { return Value::CALLABLE_UNIT; };
		static bool classof( Value const* obj );
	};
}


#endif /* _LIB_NOVEL_CALLABLE_UNIT_H_ */

//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  
