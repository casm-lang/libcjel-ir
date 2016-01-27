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

#include "Visitor.h"

using namespace libnovel;

#define CASE_VALUE( VID, CLASS )									\
	case Value::ID::VID: prolog										\
	? visit_prolog( *((CLASS*)value) )								\
	: visit_epilog( *((CLASS*)value) ) ; break

void Visitor::dispatch( u1 prolog, Value* value )
{
	assert( value );
	
	switch( value->getValueID() )
	{
		CASE_VALUE( FUNCTION,          Function );
		CASE_VALUE( MEMORY,            Memory );
		
		CASE_VALUE( PARALLEL_SCOPE,    ParallelScope );
		CASE_VALUE( SEQUENTIAL_SCOPE,  SequentialScope );
		
		CASE_VALUE( TRIVIAL_STATEMENT, TrivialStatement );
		
		CASE_VALUE( CALL_INSTRUCTION,  CallInstruction );
		
		CASE_VALUE( LOAD_INSTRUCTION,  LoadInstruction );
		CASE_VALUE( STORE_INSTRUCTION, StoreInstruction );
		
		CASE_VALUE( AND_INSTRUCTION,   AndInstruction );
		CASE_VALUE( ADDS_INSTRUCTION,  AddSignedInstruction );
		
	    default:
			printf( "unimplemented value ID '%s' to dispatch\n", value->getName() );
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
