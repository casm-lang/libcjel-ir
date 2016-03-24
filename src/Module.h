//  
//  Copyright (c) 2015 Philipp Paulweber
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/ppaulweber/libnovel
//  
//  Permission is hereby granted, free of charge, to any person obtaining a 
//  copy of this software and associated documentation files (the "Software"), 
//  to deal with the Software without restriction, including without limitation 
//  the rights to use, copy, modify, merge, publish, distribute, sublicense, 
//  and/or sell copies of the Software, and to permit persons to whom the 
//  Software is furnished to do so, subject to the following conditions:
//  
//  * Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimers.
//  
//  * Redistributions in binary form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimers in the 
//    documentation and/or other materials provided with the distribution.
//  
//  * Neither the names of the copyright holders, nor the names of its 
//    contributors may be used to endorse or promote products derived from 
//    this Software without specific prior written permission.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
//  CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
//  WITH THE SOFTWARE.
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
