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
//  along with libnovel. If not, see <http://www.gnu.org/licenses/>.
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

void NovelDumpPass::visit_prolog( CastInstruction& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( CastInstruction& value ) {}
		
void NovelDumpPass::visit_prolog( CallInstruction& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( CallInstruction& value ) {}
		
void NovelDumpPass::visit_prolog( StreamInstruction& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( StreamInstruction& value ) {}

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

void NovelDumpPass::visit_prolog( ZeroExtendInstruction& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( ZeroExtendInstruction& value ) {}

void NovelDumpPass::visit_prolog( TruncationInstruction& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( TruncationInstruction& value ) {}

void NovelDumpPass::visit_prolog( ModUnsignedInstruction& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( ModUnsignedInstruction& value ) {}


void NovelDumpPass::visit_prolog( BitConstant& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( BitConstant& value ) {}

void NovelDumpPass::visit_prolog( StructureConstant& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( StructureConstant& value ) {}


void NovelDumpPass::visit_prolog( Interconnect& value ) { DUMP_PREFIX; DUMP_POSTFIX; }
void NovelDumpPass::visit_epilog( Interconnect& value ) {}


//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  

