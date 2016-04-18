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

#ifndef _LIB_NOVEL_INSTRUCTION_H_
#define _LIB_NOVEL_INSTRUCTION_H_

#include "User.h"
#include "Statement.h"
#include "Intrinsic.h"
#include "Function.h"
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
		static inline Value::ID classid( void ) { return Value::ID_INSTRUCTION; };
		static bool classof( Value const* obj );
	};
	
    class CastInstruction : public UnaryInstruction
	{
	public :
		CastInstruction( Value::ID kind, Value* src );
		static inline Value::ID classid( void ) { return Value::CAST_INSTRUCTION; };
		static bool classof( Value const* obj );
	};
	
	class LoadInstruction : public UnaryInstruction
	{
	public :
		LoadInstruction( Value* src );
		static inline Value::ID classid( void ) { return Value::LOAD_INSTRUCTION; };
		static bool classof( Value const* obj );
	};
	
	
	class StoreInstruction : public BinaryInstruction
	{
	public:
	    StoreInstruction( Value* src, Value* dst );
		static inline Value::ID classid( void ) { return Value::STORE_INSTRUCTION; };
		static bool classof( Value const* obj );
	};

	class ExtractInstruction : public BinaryInstruction
	{
	public:
	    ExtractInstruction( Value* ref, Value* element );
		static inline Value::ID classid( void ) { return Value::EXTRACT_INSTRUCTION; };
		static bool classof( Value const* obj );
	};
    
	class CallInstruction : public Instruction
	{
	public:
		CallInstruction( Value* symbol );
    	static inline Value::ID classid( void ) { return Value::CALL_INSTRUCTION; };
		static bool classof( Value const* obj );
	};
	
	class StreamInstruction : public Instruction
	{
	public:
		enum Channel
		{ INPUT
		, OUTPUT
		, ERROR
		, WARNING
		};

	private:
		Channel channel;
		
	public:
		StreamInstruction( Channel channel );
		const Channel getChannel( void ) const;
    	static inline Value::ID classid( void ) { return Value::STREAM_INSTRUCTION; };
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
