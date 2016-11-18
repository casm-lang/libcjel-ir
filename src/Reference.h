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

#ifndef _LIB_NOVEL_REFERENCE_H_
#define _LIB_NOVEL_REFERENCE_H_

#include "Value.h"
#include "User.h"
#include "Intrinsic.h"
#include "Constant.h"
#include "Structure.h"

namespace libnovel
{
	class Reference : public User
	{
	public:
		enum Kind
		{ INPUT = 0
		, OUTPUT
		, LINKAGE
		};
		
	private:
		Identifier* identifier;
		CallableUnit* callable;
		Kind kind;
		Structure* structure;
	    
	public:
		Reference( const char* name, Type* type, CallableUnit* callable = 0, Kind kind = INPUT );
		
		~Reference( void );
		
		const Identifier* getIdentifier( void ) const;
		
		const CallableUnit* getCallableUnit( void ) const;
		void setCallableUnit( CallableUnit* value );

		const u1 isInput( void ) const;
		const u1 isOutput( void ) const;
		const u1 isLinkage( void ) const;
		
		const Structure* getStructure( void ) const;
		const u1 isStructure( void ) const;
				
		void dump( void ) const;

		static inline Value::ID classid( void ) { return Value::REFERENCE; };
		static bool classof( Value const* obj );
	};
}


#endif /* _LIB_NOVEL_REFERENCE_H_ */

//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  
