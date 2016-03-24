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
