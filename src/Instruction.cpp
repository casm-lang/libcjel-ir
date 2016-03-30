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

#include "Instruction.h"

using namespace libnovel;


Instruction::Instruction( const char* name, Type* type, Value::ID id )
: User( name, type, id )
, statement( 0 )
{
}

void Instruction::setStatement( Statement* stmt )
{
	statement = stmt;
	
	for( Value* value : values )
	{
		if( not Value::isa< Instruction >( value ) )
		{
			continue;
		}

		Instruction* instr = (Instruction*)value;

		if( instr->getStatement() == 0 )
		{
			stmt->add( value );
		}
		
		if( instr->getStatement() != stmt )
		{
			fprintf
				( stderr
				  , "error: %s:%i: Instruction %p does belong to a different Statement block\n"
				  , __FUNCTION__, __LINE__, value
					);
			assert(0);
		}
	}
}

Statement* Instruction::getStatement( void ) const
{
	return statement;
}

void Instruction::add( Value* value )
{
	assert( value );
	
	if( Value::isa< UnaryInstruction >( this ) )
	{
		assert( values.size() < 1 );	
	}
	else if( Value::isa< BinaryInstruction >( this ) )
	{
		assert( values.size() < 2 );		
	}
	
	values.push_back( value );
}

Value* Instruction::getValue( u8 index ) const
{
	// TODO: IDEA: assert( getType() ); // to force the type check!
	assert( index < values.size() );
	return values[ index ];
}

const std::vector< Value* >& Instruction::getValues( void ) const
{
	// TODO: IDEA: assert( getType() ); // to force the type check!
	return values;
}


void Instruction::dump( void ) const
{
	printf( "[Instr] %p %s ", this, &getName()[1] );
	u1 flag = 0;
	for( auto instr : values )
	{
		if( flag )
		{
			printf( ", " );
		}
		else
		{
			flag = 1;
		}
		printf( "%p", instr );
	}
	printf( "\n" );
}

bool Instruction::classof( Value const* obj )
{
	return obj->getValueID() == classid()
		or UnaryInstruction::classof( obj )
		or BinaryInstruction::classof( obj )
		or NopInstruction::classof( obj )
		or AllocInstruction::classof( obj )
		or CallInstruction::classof( obj )
		;
}


UnaryInstruction::UnaryInstruction( const char* name, Type* type, Value* value, Value::ID id )
: Instruction( name, type, id )
{
	if( not type )
	{
		assert(  value->getType() );
		setType( value->getType() );
	}
	
	add( value );
}

Value* UnaryInstruction::get( void ) const
{
	return getValue( 0 );
}

bool UnaryInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid()
		or IdInstruction::classof( obj )
		or LoadInstruction::classof( obj )
		;
}


BinaryInstruction::BinaryInstruction( const char* name, Type* type, Value* lhs, Value* rhs, Value::ID id )
: Instruction( name, type, id )
{
	add( lhs );
	add( rhs );
}	  

Value* BinaryInstruction::getLHS( void ) const
{
	return getValue( 0 );
}

Value* BinaryInstruction::getRHS( void ) const
{
	return getValue( 1 );
}

bool BinaryInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid()
	    or ArithmeticInstruction::classof( obj )
	    or LogicalInstruction::classof( obj )
	    or StoreInstruction::classof( obj )
		or ExtractInstruction::classof( obj )
		;
}



ArithmeticInstruction::ArithmeticInstruction( const char* name, Value* lhs, Value* rhs, Value::ID id )
: BinaryInstruction( name, 0, lhs, rhs, id )
{
	Type* lhs_ty = getLHS()->getType();
	Type* rhs_ty = getRHS()->getType();
	
	assert( lhs_ty );
	assert( rhs_ty );
	assert( lhs_ty->getID() == rhs_ty->getID() );
	
	assert( !getType() );
	setType( lhs->getType() );	
}

bool ArithmeticInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid()
	    or AndInstruction::classof( obj )
	    or AddSignedInstruction::classof( obj )
	    or DivSignedInstruction::classof( obj )
	    ;
}

LogicalInstruction::LogicalInstruction( const char* name, Value* lhs, Value* rhs, Value::ID id )
: BinaryInstruction( name, &TypeB1, lhs, rhs, id )
{
	Type* lhs_ty = getLHS()->getType();
	Type* rhs_ty = getRHS()->getType();
	
	assert( lhs_ty );
	assert( rhs_ty );
	assert( lhs_ty->getID() == rhs_ty->getID() );
}

bool LogicalInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid()
	    or EquUnsignedInstruction::classof( obj )
	    or NeqUnsignedInstruction::classof( obj )
	    ;
}



// -----------------------------------------------------------------------------
// INSTRUCTIONS
// -----------------------------------------------------------------------------

NopInstruction::NopInstruction( void )
: Instruction( ".nop", 0, Value::NOP_INSTRUCTION )
{
}
bool NopInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}


AllocInstruction::AllocInstruction( Type* type )
: Instruction( ".alloc", type, Value::ALLOC_INSTRUCTION )
{
}
bool AllocInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}


CallInstruction::CallInstruction( Value* symbol )
: Instruction( ".call", 0, Value::CALL_INSTRUCTION )
{
	add( symbol );

	assert( Value::isa< Intrinsic >( symbol ) );
	
	//assert( symbol->getType() ); // TODO: FIXME: PPA: MARK:
	//setType( symbol->getType()->getResultType() );
}
bool CallInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}



// -----------------------------------------------------------------------------
// UNARY INSTRUCTIONS
// -----------------------------------------------------------------------------

IdInstruction::IdInstruction( Value* src )
: UnaryInstruction( ".id", &TypeId, src, Value::ID_INSTRUCTION )
{
	assert( src and Value::isa< Variable >( src ) );
}
bool IdInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}


LoadInstruction::LoadInstruction( Value* src )
: UnaryInstruction( ".load", 0, src, Value::LOAD_INSTRUCTION )
{
}
bool LoadInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}



// -----------------------------------------------------------------------------
// BINARY INSTRUCTIONS
// -----------------------------------------------------------------------------

StoreInstruction::StoreInstruction( Value* src, Value* dst )
: BinaryInstruction( ".store", 0, src, dst, Value::STORE_INSTRUCTION )
{
}
bool StoreInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}


ExtractInstruction::ExtractInstruction( Value* src, Value* dst )
: BinaryInstruction( ".extract", 0, src, dst, Value::EXTRACT_INSTRUCTION )
{
	assert( src );
	assert( dst );

	assert( src->getType() );
	assert( dst->getType() );

	// TODO: IDEA: FIXME: PPA: possible check to implement if 'dst' is inside 'src'
	
	setType( dst->getType() );
}
bool ExtractInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}





// -----------------------------------------------------------------------------
// ARITHMETIC INSTRUCTIONS
// -----------------------------------------------------------------------------

AndInstruction::AndInstruction( Value* lhs, Value* rhs )
: ArithmeticInstruction( ".and", lhs, rhs, Value::AND_INSTRUCTION )
{	
}
bool AndInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}


AddSignedInstruction::AddSignedInstruction( Value* lhs, Value* rhs )
: ArithmeticInstruction( ".adds", lhs, rhs, Value::ADDS_INSTRUCTION )
{
}
bool AddSignedInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}

DivSignedInstruction::DivSignedInstruction( Value* lhs, Value* rhs )
: ArithmeticInstruction( ".divs", lhs, rhs, Value::DIVS_INSTRUCTION )
{
}
bool DivSignedInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}



// -----------------------------------------------------------------------------
// LOGICAL INSTRUCTIONS
// -----------------------------------------------------------------------------

EquUnsignedInstruction::EquUnsignedInstruction( Value* lhs, Value* rhs )
: LogicalInstruction( ".equu", lhs, rhs, Value::EQUU_INSTRUCTION )
{	
}
bool EquUnsignedInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}

NeqUnsignedInstruction::NeqUnsignedInstruction( Value* lhs, Value* rhs )
: LogicalInstruction( ".nequ", lhs, rhs, Value::NEQU_INSTRUCTION )
{
}
bool NeqUnsignedInstruction::classof( Value const* obj )
{
	return obj->getValueID() == classid();
}










// SubInstruction::SubInstruction( Value* lhs, Value* rhs )
// : OperatorInstruction( ".sub", 0, lhs, rhs, Value::SUB_INSTRUCTION )
// {	
// }
// bool SubInstruction::classof( Value const* obj )
// {
// 	return obj->getValueID() == Value::SUB_INSTRUCTION;
// }

// MulInstruction::MulInstruction( Value* lhs, Value* rhs )
// : OperatorInstruction( ".mul", 0, lhs, rhs, Value::MUL_INSTRUCTION )
// {	
// }
// bool MulInstruction::classof( Value const* obj )
// {
// 	return obj->getValueID() == Value::MUL_INSTRUCTION;
// }

// DivInstruction::DivInstruction( Value* lhs, Value* rhs )
// : OperatorInstruction( ".div", 0, lhs, rhs, Value::DIV_INSTRUCTION )
// {	
// }
// bool DivInstruction::classof( Value const* obj )
// {
// 	return obj->getValueID() == Value::DIV_INSTRUCTION;
// }

// RivInstruction::RivInstruction( Value* lhs, Value* rhs )
// : OperatorInstruction( ".riv", 0, lhs, rhs, Value::RIV_INSTRUCTION )
// {	
// }
// bool RivInstruction::classof( Value const* obj )
// {
// 	return obj->getValueID() == Value::RIV_INSTRUCTION;
// }

// ModInstruction::ModInstruction( Value* lhs, Value* rhs )
// : OperatorInstruction( ".mod", 0, lhs, rhs, Value::MOD_INSTRUCTION )
// {	
// }
// bool ModInstruction::classof( Value const* obj )
// {
// 	return obj->getValueID() == Value::MOD_INSTRUCTION;
// }





// LthInstruction::LthInstruction( Value* lhs, Value* rhs )
// : OperatorInstruction( ".lth", &BooleanType, lhs, rhs, Value::LTH_INSTRUCTION )
// {	
// }
// bool LthInstruction::classof( Value const* obj )
// {
// 	return obj->getValueID() == Value::LTH_INSTRUCTION;
// }

// LeqInstruction::LeqInstruction( Value* lhs, Value* rhs )
// : OperatorInstruction( ".leq", &BooleanType, lhs, rhs, Value::LEQ_INSTRUCTION )
// {	
// }
// bool LeqInstruction::classof( Value const* obj )
// {
// 	return obj->getValueID() == Value::LEQ_INSTRUCTION;
// }

// GthInstruction::GthInstruction( Value* lhs, Value* rhs )
// : OperatorInstruction( ".gth", &BooleanType, lhs, rhs, Value::GTH_INSTRUCTION )
// {	
// }
// bool GthInstruction::classof( Value const* obj )
// {
// 	return obj->getValueID() == Value::GTH_INSTRUCTION;
// }

// GeqInstruction::GeqInstruction( Value* lhs, Value* rhs )
// : OperatorInstruction( ".geq", &BooleanType, lhs, rhs, Value::GEQ_INSTRUCTION )
// {	
// }
// bool GeqInstruction::classof( Value const* obj )
// {
// 	return obj->getValueID() == Value::GEQ_INSTRUCTION;
// }

// MovInstruction::MovInstruction( Value* lhs )
// : UnaryInstruction( ".mov", 0, lhs, Value::MOV_INSTRUCTION )
// {	
// 	assert( get()->getType() );
// 	setType( get()->getType() );
// }
// bool MovInstruction::classof( Value const* obj )
// {
// 	return obj->getValueID() == Value::MOV_INSTRUCTION;
// }



// BranchInstruction::BranchInstruction( Value* condition, Value* case_true, Value* case_false )
// : UnaryInstruction( ".branch", 0, condition, Value::BRANCH_INSTRUCTION )
// , case_true ( case_true  )
// , case_false( case_false )
// {
// 	assert( condition->getType() );
// 	setType( condition->getType()->getResultType() );
// 	assert( getType()->getID() == BooleanType.getID() );



//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  
