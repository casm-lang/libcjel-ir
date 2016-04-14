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

#ifndef _LIB_NOVEL_MODULE_H_
#define _LIB_NOVEL_MODULE_H_

#include "Value.h"
#include "User.h"
#include "Function.h"
#include "Intrinsic.h"
#include "Constant.h"
#include "Structure.h"
#include "Memory.h"

namespace libnovel
{
	class Module : public User
	{
	private:
		std::unordered_map< u32, std::vector< Value* > > content;
		
	public:
		Module( const char* name );
		
		~Module( void );
		
		void add( Value* value );

		//const std::unordered_map< u8, std::vector< Value* > >& getContent( void ) const;
		
		template< class C >
		bool has( void ) const
		{
			return content.count( C::classid() ) > 0;
		}
		
		template< class C >
		const std::vector< Value* >& get( void ) const
		{
			auto result = content.find( C::classid() );
			assert( result != content.end() );
			return result->second;
		}
		
		void dump( void ) const;
			    
		static inline Value::ID classid( void ) { return Value::MODULE; };
		static bool classof( Value const* obj );
	};
}


#endif /* _LIB_NOVEL_MODULE_H_ */

//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  
