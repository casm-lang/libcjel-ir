//  
//  Copyright (c) 2016 Philipp Paulweber
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

#include "NovelDumpPass.h"

using namespace libnovel;


char NovelDumpPass::id = 0;

static libpass::PassRegistration< NovelDumpPass > PASS
( "NOVEL Dumping Pass"
, "generates ASCII representation of the NOVEL IR"
, 0
, 0
);


bool NovelDumpPass::run( libpass::PassResult& pr )
{
    Module* value = (Module*)pr.getResult< NovelDumpPass >();
	assert( value );
    
	value->iterate
	( Traversal::PREORDER
	, this
	);
    
	return false;
}


#define DUMP_PREFIX  printf( "%-14s: %p, %s, %s ", __FUNCTION__, &value, value.getLabel(), value.getName() )
#define DUMP_POSTFIX printf( "\n" );


void NovelDumpPass::visit_prolog( Module& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( Module& value ) {}

void NovelDumpPass::visit_prolog( Function& value )   { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_interlog( Function& value ) {}
void NovelDumpPass::visit_epilog( Function& value )   {}

void NovelDumpPass::visit_prolog( Intrinsic& value )   { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_interlog( Intrinsic& value ) {}
void NovelDumpPass::visit_epilog( Intrinsic& value )   {}

void NovelDumpPass::visit_prolog( Reference& value )
{
	DUMP_PREFIX;
	printf( "%s, %s", value.getIdentifier()->getName(), value.isInput() ? "in" : "out" );
	DUMP_POSTFIX;
}
void NovelDumpPass::visit_epilog( Reference& value ) {}

void NovelDumpPass::visit_prolog( Structure& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( Structure& value ) {}

void NovelDumpPass::visit_prolog( Variable& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( Variable& value ) {}

void NovelDumpPass::visit_prolog( Memory& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( Memory& value ) {}

void NovelDumpPass::visit_prolog( ParallelScope& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( ParallelScope& value ) {}
		
void NovelDumpPass::visit_prolog( SequentialScope& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( SequentialScope& value ) {}
		
void NovelDumpPass::visit_prolog( TrivialStatement& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( TrivialStatement& value ) {}
		
void NovelDumpPass::visit_prolog( BranchStatement& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_interlog( BranchStatement& value ) {}
void NovelDumpPass::visit_epilog( BranchStatement& value ) {}
		
void NovelDumpPass::visit_prolog( LoopStatement& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_interlog( LoopStatement& value ) {}
void NovelDumpPass::visit_epilog( LoopStatement& value ) {}
		
void NovelDumpPass::visit_prolog( NopInstruction& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( NopInstruction& value ) {}

void NovelDumpPass::visit_prolog( AllocInstruction& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( AllocInstruction& value ) {}

void NovelDumpPass::visit_prolog( IdInstruction& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( IdInstruction& value ) {}
		
void NovelDumpPass::visit_prolog( CallInstruction& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( CallInstruction& value ) {}

void NovelDumpPass::visit_prolog( ExtractInstruction& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( ExtractInstruction& value ) {}

void NovelDumpPass::visit_prolog( LoadInstruction& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( LoadInstruction& value ) {}

void NovelDumpPass::visit_prolog( StoreInstruction& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( StoreInstruction& value ) {}

void NovelDumpPass::visit_prolog( AndInstruction& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( AndInstruction& value ) {}
		
void NovelDumpPass::visit_prolog( AddSignedInstruction& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( AddSignedInstruction& value ) {}

void NovelDumpPass::visit_prolog( DivSignedInstruction& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( DivSignedInstruction& value ) {}

void NovelDumpPass::visit_prolog( EquUnsignedInstruction& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( EquUnsignedInstruction& value ) {}

void NovelDumpPass::visit_prolog( NeqUnsignedInstruction& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( NeqUnsignedInstruction& value ) {}


void NovelDumpPass::visit_prolog( BitConstant& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( BitConstant& value ) {}

void NovelDumpPass::visit_prolog( StructureConstant& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( StructureConstant& value ) {}


//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  

