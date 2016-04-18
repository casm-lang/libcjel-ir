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

#include "Visitor.h"

using namespace libnovel;

#define CASE_VALUE( VID, CLASS )									\
	case Value::ID::VID:												    \
	     if( stage == Stage::PROLOG )   visit_prolog(   *((CLASS*)value) ); \
	else if( stage == Stage::EPILOG )   visit_epilog(   *((CLASS*)value) ); \
	else assert( !"invalid visitor stage value!" );						    \
	break

#define CASE_VALUE_INTER( VID, CLASS )									\
	case Value::ID::VID:												    \
	     if( stage == Stage::PROLOG )   visit_prolog(   *((CLASS*)value) ); \
	else if( stage == Stage::INTERLOG ) visit_interlog( *((CLASS*)value) ); \
	else if( stage == Stage::EPILOG )   visit_epilog(   *((CLASS*)value) ); \
	else assert( !"invalid visitor stage value!" );						    \
	break

void Visitor::dispatch( Stage stage, Value* value )
{
	assert( value );
	
	switch( value->getValueID() )
	{
		CASE_VALUE( MODULE,              Module );
		
		CASE_VALUE_INTER( FUNCTION,      Function );
		CASE_VALUE_INTER( INTRINSIC,     Intrinsic );
		
		CASE_VALUE( REFERENCE,           Reference );
		
		CASE_VALUE( VARIABLE,            Variable );
		CASE_VALUE( MEMORY,              Memory );
		CASE_VALUE( STRUCTURE,           Structure );

		CASE_VALUE( BIT_CONSTANT,        BitConstant );
		CASE_VALUE( STRUCTURE_CONSTANT,  StructureConstant );
		
		CASE_VALUE( PARALLEL_SCOPE,      ParallelScope );
		CASE_VALUE( SEQUENTIAL_SCOPE,    SequentialScope );
		
		CASE_VALUE( TRIVIAL_STATEMENT,      TrivialStatement );
		CASE_VALUE_INTER( BRANCH_STATEMENT, BranchStatement );
		CASE_VALUE_INTER( LOOP_STATEMENT,   LoopStatement );
		
		CASE_VALUE( CALL_INSTRUCTION,    CallInstruction );
		CASE_VALUE( STREAM_INSTRUCTION,  StreamInstruction );
		
		CASE_VALUE( NOP_INSTRUCTION,     NopInstruction );
		CASE_VALUE( ALLOC_INSTRUCTION,   AllocInstruction );
		
		CASE_VALUE( ID_INSTRUCTION,      IdInstruction );
		CASE_VALUE( CAST_INSTRUCTION,    CastInstruction );
		CASE_VALUE( EXTRACT_INSTRUCTION, ExtractInstruction );
		
		CASE_VALUE( LOAD_INSTRUCTION,    LoadInstruction );
		CASE_VALUE( STORE_INSTRUCTION,   StoreInstruction );
		
		CASE_VALUE( AND_INSTRUCTION,     AndInstruction );
		CASE_VALUE( ADDS_INSTRUCTION,    AddSignedInstruction );
		CASE_VALUE( DIVS_INSTRUCTION,    DivSignedInstruction );

		CASE_VALUE( EQUU_INSTRUCTION,    EquUnsignedInstruction );
		CASE_VALUE( NEQU_INSTRUCTION,    NeqUnsignedInstruction );
		
	    default:
			printf
			( "%s:%i: warning: unimplemented value ID '%s' to dispatch for stage '%i'\n"
			, __FILE__
			, __LINE__
			, value->getName()
			, stage
			);
 			break;
	}
}




//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  
