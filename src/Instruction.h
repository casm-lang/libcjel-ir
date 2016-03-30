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

#ifndef _LIB_NOVEL_INSTRUCTION_H_
#define _LIB_NOVEL_INSTRUCTION_H_

#include "User.h"
#include "Statement.h"
#include "Intrinsic.h"
#include "Variable.h"
#include "Value.h"

namespace libnovel
{
	class Statement;
	
	class Instruction : public User
	{
	private:
		Statement* statement;
		std::vector< Value* > values;
		
	public:
		Instruction( const char* name, Type* type, Value::ID id = Value::INSTRUCTION );

		void setStatement( Statement* stmt );
		Statement* getStatement( void ) const;
		
	    void add( Value* value );
		Value* getValue( u8 index ) const;
		const std::vector< Value* >& getValues( void ) const;
		
		void dump( void ) const;
		
		static inline Value::ID classid( void ) { return Value::INSTRUCTION; };
		static bool classof( Value const* obj );
	};
	
	class UnaryInstruction : public Instruction
	{
	public:
		UnaryInstruction( const char* name, Type* type, Value* value
						, Value::ID id = Value::UNARY_INSTRUCTION );
		Value* get( void ) const;
		
		static inline Value::ID classid( void ) { return Value::UNARY_INSTRUCTION; };
		static bool classof( Value const* obj );
	};
	
	class BinaryInstruction : public Instruction
	{
	public:
		BinaryInstruction( const char* name, Type* type, Value* lhs, Value* rhs
						 , Value::ID id = Value::BINARY_INSTRUCTION );
		
		Value* getLHS( void ) const;
		Value* getRHS( void ) const;
				
		static inline Value::ID classid( void ) { return Value::BINARY_INSTRUCTION; };
		static bool classof( Value const* obj );
	};
	
	class ArithmeticInstruction : public BinaryInstruction
	{
	public:
		ArithmeticInstruction
		( const char* name
		, Value* lhs
		, Value* rhs
		, Value::ID id = Value::ARITHMETIC_INSTRUCTION
		);
		
		static inline Value::ID classid( void ) { return Value::ARITHMETIC_INSTRUCTION; };
		static bool classof( Value const* obj );
	};

	class LogicalInstruction : public BinaryInstruction
	{
	public:
		LogicalInstruction
		( const char* name
		, Value* lhs
		, Value* rhs
		, Value::ID id = Value::LOGICAL_INSTRUCTION
		);
		
		static inline Value::ID classid( void ) { return Value::LOGICAL_INSTRUCTION; };
		static bool classof( Value const* obj );
	};
	
	class NopInstruction : public Instruction
	{
	public :
		NopInstruction( void );
		static inline Value::ID classid( void ) { return Value::NOP_INSTRUCTION; };
		static bool classof( Value const* obj );
	};
	
	class AllocInstruction : public Instruction
	{
	public :
		AllocInstruction( Type* type );
		static inline Value::ID classid( void ) { return Value::ALLOC_INSTRUCTION; };
		static bool classof( Value const* obj );
	};
    
	class IdInstruction : public UnaryInstruction
	{
	public :
		IdInstruction( Value* src );
		//void dump( void ) const;
		static inline Value::ID classid( void ) { return Value::ID_INSTRUCTION; };
		static bool classof( Value const* obj );
	};
	
	class LoadInstruction : public UnaryInstruction
	{
	public :
		LoadInstruction( Value* src );
		//void dump( void ) const;
		static inline Value::ID classid( void ) { return Value::LOAD_INSTRUCTION; };
		static bool classof( Value const* obj );
	};
	
	
	class StoreInstruction : public BinaryInstruction
	{
	public:
	    StoreInstruction( Value* src, Value* dst );
		//void dump( void ) const;
		static inline Value::ID classid( void ) { return Value::STORE_INSTRUCTION; };
		static bool classof( Value const* obj );
	};

	class ExtractInstruction : public BinaryInstruction
	{
	public:
	    ExtractInstruction( Value* ref, Value* element );
		//void dump( void ) const;
		static inline Value::ID classid( void ) { return Value::EXTRACT_INSTRUCTION; };
		static bool classof( Value const* obj );
	};
	
	
	class CallInstruction : public Instruction
	{
	public:
		CallInstruction( Value* symbol );
		//void dump( void ) const;
		static inline Value::ID classid( void ) { return Value::CALL_INSTRUCTION; };
		static bool classof( Value const* obj );
	};
	
	class AndInstruction : public ArithmeticInstruction
	{
	public:
		AndInstruction( Value* lhs, Value* rhs );
		static inline Value::ID classid( void ) { return Value::AND_INSTRUCTION; };
		static bool classof( Value const* obj );
	};

	
	class AddSignedInstruction : public ArithmeticInstruction
	{
	public:
		AddSignedInstruction( Value* lhs, Value* rhs );
		static inline Value::ID classid( void ) { return Value::ADDS_INSTRUCTION; };
		static bool classof( Value const* obj );
	};

	class DivSignedInstruction : public ArithmeticInstruction
	{
	public:
		DivSignedInstruction( Value* lhs, Value* rhs );
		static inline Value::ID classid( void ) { return Value::DIVS_INSTRUCTION; };
		static bool classof( Value const* obj );
	};
	
	// Class SubInstruction : public OperatorInstruction
	// {
	// public:
	// 	SubInstruction( Value* lhs, Value* rhs );
	// 	static bool classof( Value const* obj );
	// };

	// class MulInstruction : public OperatorInstruction
	// {
	// public:
	// 	MulInstruction( Value* lhs, Value* rhs );
	// 	static bool classof( Value const* obj );
	// };

	// class DivInstruction : public OperatorInstruction
	// {
	// public:
	// 	DivInstruction( Value* lhs, Value* rhs );
	// 	static bool classof( Value const* obj );
	// };

	// class RivInstruction : public OperatorInstruction
	// {
	// public:
	// 	RivInstruction( Value* lhs, Value* rhs );
	// 	static bool classof( Value const* obj );
	// };

	// class ModInstruction : public OperatorInstruction
	// {
	// public:
	// 	ModInstruction( Value* lhs, Value* rhs );
	// 	static bool classof( Value const* obj );
	// };
	
	class EquUnsignedInstruction : public LogicalInstruction
	{
	public:
		EquUnsignedInstruction( Value* lhs, Value* rhs );
		static inline Value::ID classid( void ) { return Value::EQUU_INSTRUCTION; };
		static bool classof( Value const* obj );
	};
	
	class NeqUnsignedInstruction : public LogicalInstruction
	{
	public:
		NeqUnsignedInstruction( Value* lhs, Value* rhs );
		static inline Value::ID classid( void ) { return Value::NEQU_INSTRUCTION; };
		static bool classof( Value const* obj );
	};
	
	// class LthInstruction : public OperatorInstruction
	// {
	// public:
	// 	LthInstruction( Value* lhs, Value* rhs );
	// 	static bool classof( Value const* obj );
	// };

	// class LeqInstruction : public OperatorInstruction
	// {
	// public:
	// 	LeqInstruction( Value* lhs, Value* rhs );
	// 	static bool classof( Value const* obj );
	// };

	// class GthInstruction : public OperatorInstruction
	// {
	// public:
	// 	GthInstruction( Value* lhs, Value* rhs );
	// 	static bool classof( Value const* obj );
	// };

	// class GeqInstruction : public OperatorInstruction
	// {
	// public:
	// 	GeqInstruction( Value* lhs, Value* rhs );
	// 	static bool classof( Value const* obj );
	// };

	
	// class MovInstruction : public UnaryInstruction
	// {
	// public:
	// 	MovInstruction( Value* lhs );
	// 	static bool classof( Value const* obj );
	// };
	
}


#endif /* _LIB_NOVEL_INSTRUCTION_H_ */

//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  
