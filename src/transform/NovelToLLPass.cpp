//  
//  Copyright (c) 2016 Philipp Paulweber
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/ppaulweber/libnovel
//  
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program. If not, see <http://www.gnu.org/licenses/>.
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

static FILE* stream = stderr;


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


void NovelToLLPass::visit_prolog( BranchStatement& value )
{
	TODO;
}
void NovelToLLPass::visit_interlog( BranchStatement& value )
{}
void NovelToLLPass::visit_epilog( BranchStatement& value )
{}


void NovelToLLPass::visit_prolog( LoopStatement& value )
{
	TODO;
}
void NovelToLLPass::visit_interlog( LoopStatement& value )
{}
void NovelToLLPass::visit_epilog( LoopStatement& value )
{}


void NovelToLLPass::visit_prolog( CallInstruction& value )
{
	TODO;
}
void NovelToLLPass::visit_epilog( CallInstruction& value )	
{}


void NovelToLLPass::visit_prolog( NopInstruction& value )
{
	TODO;
	fprintf
	( stream
	, "    ;; nop\n"
	);
}
void NovelToLLPass::visit_epilog( NopInstruction& value )
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

void NovelToLLPass::visit_prolog( EquUnsignedInstruction& value ) { TODO; }
void NovelToLLPass::visit_epilog( EquUnsignedInstruction& value ) {}

void NovelToLLPass::visit_prolog( NeqUnsignedInstruction& value ) { TODO; }
void NovelToLLPass::visit_epilog( NeqUnsignedInstruction& value ) {}



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

