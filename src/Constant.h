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

#ifndef _LIB_NOVEL_CONSTANT_H_
#define _LIB_NOVEL_CONSTANT_H_

#include "stdhl/cpp/Binding.h"

#include "Value.h"
#include "Type.h"
#include "User.h"

namespace libnovel
{
	class Structure;

	template< typename V >
	class Constant : public libnovel::User
	{
    protected:
		V value;
		Constant( const char* name, Type* type, V value, Value::ID id = Value::CONSTANT );

	public:
		~Constant( void );
		
	    const V getValue( void ) const;
		
		static inline Value::ID classid( void ) { return Value::CONSTANT; };
		static bool classof( Value const* obj );

	protected:
	    void setValue( V val );
	};
	
	
	class Constants : public Constant< u1 >
	{
	public:
		static inline Value::ID classid( void ) { return Value::CONSTANT; };
		static bool classof( Value const* obj );
	};

	
	class StructureConstant;
	
	class BitConstant : public Constant< Type::Bit >, public libstdhl::Binding< StructureConstant >
	{
	private:
		u64 value[1];
		BitConstant( Type* type, u64 value );
		
	public:
		static BitConstant* create( u64 value, u16 bitsize );
		
		void dump( void ) const;
		
		static inline Value::ID classid( void ) { return Value::BIT_CONSTANT; };
		static bool classof( Value const* obj );
	};
	
	
	class StructureConstant : public Constant< std::vector< Value* > >, public libstdhl::Binding< StructureConstant >
	{
	private:
		StructureConstant( Type* type, std::vector< Value* > value );
		
	public:
		static StructureConstant* create( Structure* kind, const std::vector< Value* >& value = {} );

		const std::vector< Value* >& getElements( void ) const;
		
		void dump( void ) const;
		
		static inline Value::ID classid( void ) { return Value::STRUCTURE_CONSTANT; };
		static bool classof( Value const* obj );
	};
	
	
	class Identifier : public Constant< const char* >
	{
	private:
		Identifier( Type* type, const char* value );
				
	public:
	    ~Identifier( void );

		static Identifier* create( Type* type, std::string value, Value* scope = 0 );
		static Identifier* create( Type* type, const char* value, Value* scope = 0 );
		
		static void forgetSymbol( const char* value );
		
		void dump( void ) const;

		static inline Value::ID classid( void ) { return Value::IDENTIFIER; };
		static bool classof( Value const* obj );
	};
}


#endif /* _LIB_NOVEL_CONSTANT_H_ */

//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  
