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

#include "NovelDumpPass.h"

using namespace libnovel;


char NovelDumpPass::id = 0;

static libpass::PassRegistration< NovelDumpPass > PASS
( "NOVEL Dumping Pass"
, "generates ASCII representation of the NOVEL IR"
, 0
, 0
);

static const char* default_output_name = "stdout";



bool NovelDumpPass::run( libpass::PassResult& pr )
{
	assert( !"not implemented yet!" );

    Module* value = (Module*)pr.getResult< NovelDumpPass >();
	assert( value );
    
	value->iterate
	( Traversal::PREORDER
	, this
	);
    
	return false;
}


#define DUMP_PREFIX  printf( "%-14s: %p, %s ", __FUNCTION__, &value, value.getName() )
#define DUMP_POSTFIX printf( "\n" );

void NovelDumpPass::visit_prolog( Module& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
void NovelDumpPass::visit_epilog( Module& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}

void NovelDumpPass::visit_prolog( Component& value )   { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_interlog( Component& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( Component& value )   { DUMP_PREFIX; DUMP_POSTFIX; }

void NovelDumpPass::visit_prolog( Function& value )   { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_interlog( Function& value ) {	DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( Function& value )   {	DUMP_PREFIX; DUMP_POSTFIX; }

void NovelDumpPass::visit_prolog( Reference& value )
{
	DUMP_PREFIX;
	printf( "%s, %s", value.getIdentifier()->getName(), value.isInput() ? "in" : "out" );
	DUMP_POSTFIX;
}
void NovelDumpPass::visit_epilog( Reference& value )  { DUMP_PREFIX; DUMP_POSTFIX; }

void NovelDumpPass::visit_prolog( Memory& value )     { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( Memory& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
		
void NovelDumpPass::visit_prolog( ParallelScope& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
void NovelDumpPass::visit_epilog( ParallelScope& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
		
void NovelDumpPass::visit_prolog( SequentialScope& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
void NovelDumpPass::visit_epilog( SequentialScope& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
		
void NovelDumpPass::visit_prolog( TrivialStatement& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
void NovelDumpPass::visit_epilog( TrivialStatement& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
	    
void NovelDumpPass::visit_prolog( CallInstruction& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
void NovelDumpPass::visit_epilog( CallInstruction& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
		
void NovelDumpPass::visit_prolog( LoadInstruction& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
void NovelDumpPass::visit_epilog( LoadInstruction& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
		
void NovelDumpPass::visit_prolog( StoreInstruction& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
void NovelDumpPass::visit_epilog( StoreInstruction& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
		
void NovelDumpPass::visit_prolog( AndInstruction& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
void NovelDumpPass::visit_epilog( AndInstruction& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
		
void NovelDumpPass::visit_prolog( AddSignedInstruction& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
}
void NovelDumpPass::visit_epilog( AddSignedInstruction& value )
{
	DUMP_PREFIX; DUMP_POSTFIX;
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

