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

#include "NovelToC11Pass.h"

using namespace libnovel;


char NovelToC11Pass::id = 0;

static libpass::PassRegistration< NovelToC11Pass > PASS
( "NOVEL IR to C11"
, "generates C11 code out of the NOVEL IR"
, 0
, 0
);

static FILE* stream = stderr;


bool NovelToC11Pass::run( libpass::PassResult& pr )
{
    Module* value = (Module*)pr.getResult< NovelDumpPass >();
	assert( value );
	
	string fn = "obj/" + string( value->getName() ) + ".c"; 
	stream = fopen( fn.c_str(), "w" );
		
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
		u16 bitsize = type->getBitsize();
		u16 bitsize_type = 8;

		while( bitsize > bitsize_type )
		{
		    bitsize_type *= 2;
		}
		
		assert( bitsize_type <= 64 );
		
		string t = "uint" + to_string( bitsize_type ) + "_t";
		return libstdhl::Allocator::string( t );
	}
	else if( type->getIDKind() == Type::ID::STRUCTURE )
	{
		Value* ty = type->getBound();
		assert(  Value::isa< Structure >( ty ) );
		string t = "struct " + string( ((Structure*)ty)->getName() );
	    return libstdhl::Allocator::string( t );
	}
	else
	{
		assert( !"unimplemented or unsupported type to convert!" );
	}
}


//
// Module
//

void NovelToC11Pass::visit_prolog( Module& value )
{
	std::chrono::time_point<std::chrono::system_clock> stamp;
	stamp = std::chrono::system_clock::now();
	std::time_t timestamp = std::chrono::system_clock::to_time_t( stamp );
	
	fprintf
	( stream
	, "// Generated At: %s"
	  "// Module: '%s'\n"
	  "\n"
	, std::ctime( &timestamp )
	, value.getName()
	);
}
void NovelToC11Pass::visit_epilog( Module& value )
{}


//
// Function
//

void NovelToC11Pass::visit_prolog( Function& value )
{
	fprintf
	( stream
	, "// Function '%s'\n"
	  "void %s\n"
	  "( "
	, value.getLabel()
	, value.getName()
	);
}
void NovelToC11Pass::visit_interlog( Function& value )
{
	fprintf
	( stream
	, "\n"
	  ")\n"
	//"{\n"
	);
}
void NovelToC11Pass::visit_epilog( Function& value )
{
	fprintf
	( stream
	,//"}\n"
	  "\n"
	);
}


//
// Intrinsic
//

void NovelToC11Pass::visit_prolog( Intrinsic& value )
{
	fprintf
	( stream
	, "// Intrinsic '%s'\n"
	  "static inline void %s\n"
	  "( "
	, value.getLabel()
	, value.getName()
	);	
}
void NovelToC11Pass::visit_interlog( Intrinsic& value )
{
	visit_interlog( *((Function*)(&value)) );
}
void NovelToC11Pass::visit_epilog( Intrinsic& value )
{
	visit_epilog( *((Function*)(&value)) );
}


//
// Reference
//

void NovelToC11Pass::visit_prolog( Reference& value )
{
	fprintf
	( stream
	, "%s* %s // %s%s"
	, getTypeString( value )
	, value.getLabel()
	, value.getIdentifier()->getName()
	, ( value.getCallableUnit()->isLastParameter( &value ) ? "" : "\n, " )
	);
}
void NovelToC11Pass::visit_epilog( Reference& value )
{}


//
// Structure
//

void NovelToC11Pass::visit_prolog( Structure& value )
{
	Module* m = value.getRef<Module>();
	
	if( m->get< Structure >().front() == &value )
	{
		fprintf
		( stream
	    , "// Structures\n"
	    );
	}
	
    fprintf
	( stream
	, "%s\n"
	  "{ "
	, getTypeString( value )
    );
	
	for( const Structure* s : value.getElements() )
	{
        fprintf
	    ( stream
		, "%s%s %s\n; "
		, getTypeString( *((Value*)s) )
		, s->getElements().size() > 0 ? "*" : ""
		, s->getIdentifier()->getName()
		);
	}
	
	fprintf
	( stream
	, "};\n"
	  "\n"
    );
}
void NovelToC11Pass::visit_epilog( Structure& value )
{
}


//
// Variable
//

void NovelToC11Pass::visit_prolog( Variable& value )
{
	Module* m = value.getRef< Module >();
	
	if( m->get< Variable >().front() == &value )
	{
		fprintf
		( stream
	    , "// Variables\n"
	    );
	}
	
	fprintf
	( stream
	, "%s %s = %s;\n"
	, getTypeString( *value.getType()->getBound() )
	, value.getLabel()
	, value.getExpression()->getLabel()
	);
}
void NovelToC11Pass::visit_epilog( Variable& value )
{
	Module* m = value.getRef< Module >();
	
	if( m->get< Variable >().back() == &value )
	{
		fprintf
		( stream
	    , "\n"
	    );
	}
}


//
// Memory
//

void NovelToC11Pass::visit_prolog( Memory& value )
{
	TODO;
}
void NovelToC11Pass::visit_epilog( Memory& value )
{
}


//
// ParallelScope
//

void NovelToC11Pass::visit_prolog( ParallelScope& value )
{
	fprintf
	( stream
	, "%s{ // par '%s'\n"
	, "" // TODO: FIXME: indention!
	, value.getLabel()
	);
}
void NovelToC11Pass::visit_epilog( ParallelScope& value )		
{
	fprintf
	( stream
	, "}\n"
	);
}


//
// SequentialScope
//

void NovelToC11Pass::visit_prolog( SequentialScope& value )
{
	fprintf
	( stream
	, "%s{ // seq '%s'\n"
	, "" // TODO: FIXME: indention!
	, value.getLabel()
	);
}
void NovelToC11Pass::visit_epilog( SequentialScope& value )
{
	visit_epilog( *((ParallelScope*)(&value)) );
}


//
// TrivialStatement
//

void NovelToC11Pass::visit_prolog( TrivialStatement& value )
{
	fprintf
	( stream
	, "%s// stmt '%s'\n"
	, "" // TODO: FIXME: indention!
	, value.getLabel()
	);
}
void NovelToC11Pass::visit_epilog( TrivialStatement& value )
{}


//
// BranchStatement
//

void NovelToC11Pass::visit_prolog( BranchStatement& value )
{
	fprintf
	( stream
	, "%s// branch '%s'\n"
	, "" // TODO: FIXME: indention!
	, value.getLabel()
	);
}
void NovelToC11Pass::visit_interlog( BranchStatement& value )
{
	TODO;
}
void NovelToC11Pass::visit_epilog( BranchStatement& value )
{
	TODO;
}


//
// LoopStatement
//

void NovelToC11Pass::visit_prolog( LoopStatement& value )
{
	fprintf
	( stream
	, "%s// loop '%s'\n"
	, "" // TODO: FIXME: indention!
	, value.getLabel()
	);
	TODO;
}
void NovelToC11Pass::visit_interlog( LoopStatement& value )
{
	TODO;
}
void NovelToC11Pass::visit_epilog( LoopStatement& value )
{
	TODO;
}


//
// CallInstruction
//

void NovelToC11Pass::visit_prolog( CallInstruction& value )
{	
	// TODO: FIXME: register notion!!! shall be added to NOVEL directly!!!
	fprintf
	( stream
	, "      "
	  "%s %p// call\n"
	  , value.getValue(0)->getName()
	  , &value
	);
}
void NovelToC11Pass::visit_epilog( CallInstruction& value )
{}


//
// NopInstruction
//

void NovelToC11Pass::visit_prolog( NopInstruction& value )
{
	fprintf
	( stream
	, "      // nop\n"
	);
}
void NovelToC11Pass::visit_epilog( NopInstruction& value )
{}


//
// AllocInstruction
//

void NovelToC11Pass::visit_prolog( AllocInstruction& value )
{
	TODO;
}
void NovelToC11Pass::visit_epilog( AllocInstruction& value )
{}


//
// IdInstruction
//

void NovelToC11Pass::visit_prolog( IdInstruction& value )
{
	fprintf
	( stream
	, "      "
	  "%s %s = (%s)&%s;// id\n"
	  , getTypeString( value )
	  , value.getLabel()
	  , getTypeString( value )
	  , value.get()->getLabel()
	);
}
void NovelToC11Pass::visit_epilog( IdInstruction& value )
{}


//
// ExtractInstruction
//

void NovelToC11Pass::visit_prolog( ExtractInstruction& value )
{
	TODO;
}
void NovelToC11Pass::visit_epilog( ExtractInstruction& value )
{}


//
// LoadInstruction
//

void NovelToC11Pass::visit_prolog( LoadInstruction& value )
{
	TODO;
}
void NovelToC11Pass::visit_epilog( LoadInstruction& value )
{}


//
// StoreInstruction
//

void NovelToC11Pass::visit_prolog( StoreInstruction& value )
{
	fprintf
	( stream
	, "      "
	  "*%s = %s;// store\n"
	  , value.getValue(1)->getLabel()
	  , value.getValue(0)->getLabel()
	);
}
void NovelToC11Pass::visit_epilog( StoreInstruction& value )
{}


//
// AndInstruction
//

void NovelToC11Pass::visit_prolog( AndInstruction& value )
{
	TODO;	
}
void NovelToC11Pass::visit_epilog( AndInstruction& value )
{}


//
// AddSignedInstruction
//

void NovelToC11Pass::visit_prolog( AddSignedInstruction& value )
{
	TODO;	
}
void NovelToC11Pass::visit_epilog( AddSignedInstruction& value )
{}


//
// DivSignedInstruction
//

void NovelToC11Pass::visit_prolog( DivSignedInstruction& value )
{
	TODO;	
}
void NovelToC11Pass::visit_epilog( DivSignedInstruction& value )
{}


//
// EquUnsignedInstruction
//

void NovelToC11Pass::visit_prolog( EquUnsignedInstruction& value )
{
	TODO;
}
void NovelToC11Pass::visit_epilog( EquUnsignedInstruction& value )
{
}


//
// NeqUnsignedInstruction
//

void NovelToC11Pass::visit_prolog( NeqUnsignedInstruction& value )
{
	TODO;
}
void NovelToC11Pass::visit_epilog( NeqUnsignedInstruction& value )
{}


//
// BitConstant
//

void NovelToC11Pass::visit_prolog( BitConstant& value )
{
	StructureConstant* sc = 0;
	if( value.isBound() )
	{
		sc = value.getBound();
		u1 last = sc->getElements().back() == &value;
	    
		fprintf
		( stream
		, "%lu%s"
		, value.getValue()[0]
		, last ? "" : ", "
		);
	}
	else
	{
		assert( !" unimplemented !!! " );
	}
}
void NovelToC11Pass::visit_epilog( BitConstant& value )
{}


//
// StructureConstant
//

void NovelToC11Pass::visit_prolog( StructureConstant& value )
{
	Module* m = value.getRef<Module>();
	
	if( m->get< Constants >().front() == &value )
	{
		fprintf
		( stream
	    , "// Constants\n"
	    );
	}
	
	fprintf
	( stream
	, "%s %s = { "
	, getTypeString( value )
	, value.getLabel()
	);
}
void NovelToC11Pass::visit_epilog( StructureConstant& value )
{
	fprintf
	( stream
	, " };\n"
	);

	Module* m = value.getRef<Module>();	
	if( m->get< Constants >().back() == &value )
	{
		fprintf
		( stream
	    , "\n"
	    );
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

