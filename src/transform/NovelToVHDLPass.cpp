//  
//  Copyright (c) 2015 Philipp Paulweber
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/ppaulweber/libcasm-be
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

#include "NovelToVHDLPass.h"

using namespace libnovel;


char NovelToVHDLPass::id = 0;

static libpass::PassRegistration< NovelToVHDLPass > PASS
( "NOVEL IR to VHDL"
, "generates VDHL code out of the NOVEL IR"
, 0
, 0
);

// static const char* default_output_name = "stdout";


bool NovelToVHDLPass::run( libpass::PassResult& pr )
{
	assert( !"not implemented yet!" );
	return false;
}


void NovelToVHDLPass::visit_prolog( Function& value )
{

}
void NovelToVHDLPass::visit_epilog( Function& value )
{

}

void NovelToVHDLPass::visit_prolog( Memory& value )
{

}
void NovelToVHDLPass::visit_epilog( Memory& value )
{

}

void NovelToVHDLPass::visit_prolog( ParallelScope& value )
{

}
void NovelToVHDLPass::visit_epilog( ParallelScope& value )		
{

}

void NovelToVHDLPass::visit_prolog( SequentialScope& value )
{

}
void NovelToVHDLPass::visit_epilog( SequentialScope& value )
{

}

void NovelToVHDLPass::visit_prolog( TrivialStatement& value )
{

}
void NovelToVHDLPass::visit_epilog( TrivialStatement& value )
{

}

void NovelToVHDLPass::visit_prolog( CallInstruction& value )
{

}
void NovelToVHDLPass::visit_epilog( CallInstruction& value )	
{

}

void NovelToVHDLPass::visit_prolog( LoadInstruction& value )
{

}
void NovelToVHDLPass::visit_epilog( LoadInstruction& value )		
{

}

void NovelToVHDLPass::visit_prolog( StoreInstruction& value )
{

}
void NovelToVHDLPass::visit_epilog( StoreInstruction& value )		
{

}

void NovelToVHDLPass::visit_prolog( AndInstruction& value )
{

}
void NovelToVHDLPass::visit_epilog( AndInstruction& value )
{

}

void NovelToVHDLPass::visit_prolog( AddSignedInstruction& value )
{

}
void NovelToVHDLPass::visit_epilog( AddSignedInstruction& value )
{

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

