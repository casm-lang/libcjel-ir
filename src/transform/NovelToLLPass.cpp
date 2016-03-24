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

#include "NovelToLLPass.h"

using namespace libnovel;


char NovelToLLPass::id = 0;

static libpass::PassRegistration< NovelToLLPass > PASS
( "NOVEL IR to LLVM IR"
, "generates LLVM IR code out of the NOVEL IR"
, 0
, 0
);


static const char* getTypeString( Value& value )
{
	Type* type = value.getType();
	assert( type );
		
	if( type->getIDKind() == Type::ID::BIT )
	{
		string t = "i" + to_string( type->getBitsize() );
		return t.c_str();
	}
	else if( type->getIDKind() == Type::ID::STRUCTURE )
	{
		assert( Value::isa< Structure >( &value ) );
		return ((Structure*)&value)->getIdentifier()->getName();
	}
	else
	{
		assert( !"unimplemented or unsupported type to convert!" );
	}
}

bool NovelToLLPass::run( libpass::PassResult& pr )
{
    Module* value = (Module*)pr.getResult< NovelDumpPass >();
	assert( value );
    
	value->iterate
	( Traversal::PREORDER
	, this
	);
    
	return false;
}


void NovelToLLPass::visit_prolog( Module& value )
{
	fprintf
	( stdout
	, "; ModuleID = '%s'\n"
	  ";; begin of module: '%s'\n"
	  "\n"
	, value.getName()
	, value.getName()
	);
}
void NovelToLLPass::visit_epilog( Module& value )
{
	fprintf
	( stdout
	, ";; end of module: '%s'\n"
	  "\n"
    , value.getName()
	);
}


void NovelToLLPass::visit_prolog( Function& value )
{
	fprintf
	( stdout
    , "define void @%s ;; Function\n"
	  "( "
    , value.getName()
	);
}
void NovelToLLPass::visit_interlog( Function& value )
{
	fprintf
	( stdout
	, "\n)\n"
	  "{\n"
	  "begin:\n"
	);
}
void NovelToLLPass::visit_epilog( Function& value )
{
	fprintf
	( stdout
	, "  ret void\n"
	  "}\n"
	  "\n"
	);
}


void NovelToLLPass::visit_prolog( Intrinsic& value )
{
	fprintf
	( stdout
	, "define void @%s ;; Intrinsic\n"
	  "( "
	, value.getName()
	);
}
void NovelToLLPass::visit_interlog( Intrinsic& value )
{
	fprintf
	( stdout
	, "\n)\n{\nbegin:\n"
	);
}
void NovelToLLPass::visit_epilog( Intrinsic& value )
{
	fprintf
	( stdout
	, "  ret void\n"
	  "}\n"
	  "\n"
	);
}

void NovelToLLPass::visit_prolog( Reference& value )
{
	fprintf
	( stdout
	, "%s %%%s%s"
	, "i32" //value.getType()->getName() // TODO: FIXME!!!
	, value.getIdentifier()->getName()
	, ( value.getCallableUnit()->isLastParameter( &value ) ? "" : "\n, " )
	);
}
void NovelToLLPass::visit_epilog( Reference& value )
{}


void NovelToLLPass::visit_prolog( Structure& value )
{
	if( value.getElements().size() == 0 )
	{
		// all bit types can be represented in LLVM IR directly!
		return;
	}
	
    fprintf
	( stdout
	, ";; begin of structure: '%s'\n"
	  "%%%s = type\n"
	  "{ "
    , value.getIdentifier()->getName()
    , value.getIdentifier()->getName()
	);

	u16 cnt = 0;
	for( const Structure* s : value.getElements() )
	{
		cnt++;
		
        fprintf
	    ( stdout
		, "%s%s ;; %s\n%s"
		, getTypeString( *((Value*)s) )
		, s->getElements().size() > 0 ? "*" : ""
		, s->getIdentifier()->getName()
		, cnt < value.getElements().size() ? ", " : ""
		);
	}
	
	fprintf
	( stdout
	, "}\n"
	  ";; end of structure: '%s'\n"
	  "\n"
    , value.getIdentifier()->getName()
	);	
}
void NovelToLLPass::visit_epilog( Structure& value )
{}


void NovelToLLPass::visit_prolog( Variable& value )
{
	TODO;
}
void NovelToLLPass::visit_epilog( Variable& value )
{}

void NovelToLLPass::visit_prolog( Memory& value )
{
	TODO;
}
void NovelToLLPass::visit_epilog( Memory& value )
{}


void NovelToLLPass::visit_prolog( ParallelScope& value )
{
	TODO;
}
void NovelToLLPass::visit_epilog( ParallelScope& value )		
{}


void NovelToLLPass::visit_prolog( SequentialScope& value )
{
	TODO;
}
void NovelToLLPass::visit_epilog( SequentialScope& value )
{}


void NovelToLLPass::visit_prolog( TrivialStatement& value )
{
	TODO;
}
void NovelToLLPass::visit_epilog( TrivialStatement& value )
{}


void NovelToLLPass::visit_prolog( CallInstruction& value )
{
	TODO;
}
void NovelToLLPass::visit_epilog( CallInstruction& value )	
{}


void NovelToLLPass::visit_prolog( AllocInstruction& value )
{
	TODO;
}
void NovelToLLPass::visit_epilog( AllocInstruction& value )
{}


void NovelToLLPass::visit_prolog( IdInstruction& value )
{
	TODO;
}
void NovelToLLPass::visit_epilog( IdInstruction& value )		
{}

void NovelToLLPass::visit_prolog( ExtractInstruction& value )
{
	TODO;
}
void NovelToLLPass::visit_epilog( ExtractInstruction& value )
{}

void NovelToLLPass::visit_prolog( LoadInstruction& value )
{
	TODO;
}
void NovelToLLPass::visit_epilog( LoadInstruction& value )		
{}

void NovelToLLPass::visit_prolog( StoreInstruction& value )
{
	TODO;
}
void NovelToLLPass::visit_epilog( StoreInstruction& value )		
{}


void NovelToLLPass::visit_prolog( AndInstruction& value )
{
	TODO;
}
void NovelToLLPass::visit_epilog( AndInstruction& value )
{}


void NovelToLLPass::visit_prolog( AddSignedInstruction& value )
{
	TODO;
}
void NovelToLLPass::visit_epilog( AddSignedInstruction& value )
{}

void NovelToLLPass::visit_prolog( DivSignedInstruction& value )
{
	TODO;	
}
void NovelToLLPass::visit_epilog( DivSignedInstruction& value )
{}



void NovelToLLPass::visit_prolog( BitConstant& value )
{
	TODO;
}
void NovelToLLPass::visit_epilog( BitConstant& value )
{}

void NovelToLLPass::visit_prolog( StructureConstant& value )
{
	TODO;
}
void NovelToLLPass::visit_epilog( StructureConstant& value )
{}


//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  

