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

bool NovelToC11Pass::run( libpass::PassResult& pr )
{
    Module* value = (Module*)pr.getResult< NovelDumpPass >();
	assert( value );
    
	value->iterate
	( Traversal::PREORDER
	, this
	);
    
	return false;
}


void NovelToC11Pass::visit_prolog( Module& value )
{
	fprintf( stdout, "// module begin: '%s'\n", value.getName() );
}
void NovelToC11Pass::visit_epilog( Module& value )
{
	fprintf( stdout, "// module end: '%s'\n\n", value.getName() );
}


void NovelToC11Pass::visit_prolog( Function& value )
{
	fprintf( stdout, "void %s // component\n( ", value.getName() );
}
void NovelToC11Pass::visit_interlog( Function& value )
{
	fprintf( stdout, "\n)\n{\n");
}
void NovelToC11Pass::visit_epilog( Function& value )
{
	fprintf( stdout, "}\n\n");
}


void NovelToC11Pass::visit_prolog( Intrinsic& value )
{
	fprintf( stdout, "void %s // intrinsic\n( ", value.getName() );
}
void NovelToC11Pass::visit_interlog( Intrinsic& value )
{
	fprintf( stdout, "\n)\n{\n");
}
void NovelToC11Pass::visit_epilog( Intrinsic& value )
{
	fprintf( stdout, "}\n\n");
}

void NovelToC11Pass::visit_prolog( Reference& value )
{
	fprintf
	( stdout
	, "%s* %s // %s%s"
	, getTypeString( value )
	, value.getLabel()
	, value.getIdentifier()->getName()
	, ( value.getCallableUnit()->isLastParameter( &value ) ? "" : "\n, " )
	);
}
void NovelToC11Pass::visit_epilog( Reference& value )
{
}

void NovelToC11Pass::visit_prolog( Structure& value )
{
    fprintf
	( stdout
	, "// structure begin: '%s'\n"
	  "struct %s\n"
	  "{ "
    , value.getIdentifier()->getName()
    , value.getIdentifier()->getName()
	);
	
	for( const Structure* s : value.getElements() )
	{
        fprintf
	    ( stdout
		, "%s%s %s\n; "
		, getTypeString( *((Value*)s) )
		, s->getElements().size() > 0 ? "*" : ""
		, s->getIdentifier()->getName()
		);
	}
	
	fprintf
	( stdout
	, "};\n"
	  "// structure end: '%s'\n"
	  "\n"
    , value.getIdentifier()->getName()
	);
}
void NovelToC11Pass::visit_epilog( Structure& value )
{}


void NovelToC11Pass::visit_prolog( Variable& value )
{
	fprintf
	( stdout
	, "%s %s = { 0 }; // variable\n"
	, getTypeString( *value.getType()->getBound() ) //->getName()
	, value.getLabel()
	  //, value.getExpression()->getLabel()
	);
}
void NovelToC11Pass::visit_epilog( Variable& value )
{}

void NovelToC11Pass::visit_prolog( Memory& value )
{
	TODO;
}
void NovelToC11Pass::visit_epilog( Memory& value )
{}


void NovelToC11Pass::visit_prolog( ParallelScope& value )
{
	fprintf( stdout, "  { // par begin\n" );
}
void NovelToC11Pass::visit_epilog( ParallelScope& value )		
{
	fprintf( stdout, "  } // par end\n" );
}


void NovelToC11Pass::visit_prolog( SequentialScope& value )
{
	fprintf( stdout, "  { // seq begin\n" );
}
void NovelToC11Pass::visit_epilog( SequentialScope& value )
{
	fprintf( stdout, "  } // seq end\n" );
}


void NovelToC11Pass::visit_prolog( TrivialStatement& value )
{
	fprintf( stdout, "    { // stmt begin\n" );
}
void NovelToC11Pass::visit_epilog( TrivialStatement& value )
{
	fprintf( stdout, "    } // stmt end\n" );
}


void NovelToC11Pass::visit_prolog( CallInstruction& value )
{	
	// TODO: FIXME: register notion!!! shall be added to NOVEL directly!!!
	fprintf
	( stdout
	, "      "
	  "%s %p// call\n"
	  , value.getValue(0)->getName()
	  , &value
	);
}
void NovelToC11Pass::visit_epilog( CallInstruction& value )
{}

void NovelToC11Pass::visit_prolog( AllocInstruction& value )
{
	TODO;
}
void NovelToC11Pass::visit_epilog( AllocInstruction& value )
{}

void NovelToC11Pass::visit_prolog( IdInstruction& value )
{
	fprintf
	( stdout
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

void NovelToC11Pass::visit_prolog( ExtractInstruction& value )
{
	TODO;
}
void NovelToC11Pass::visit_epilog( ExtractInstruction& value )
{}

void NovelToC11Pass::visit_prolog( LoadInstruction& value )
{
	TODO;
}
void NovelToC11Pass::visit_epilog( LoadInstruction& value )
{}

void NovelToC11Pass::visit_prolog( StoreInstruction& value )
{
	fprintf
	( stdout
	, "      "
	  "*%s = %s;// store\n"
	  , value.getValue(1)->getLabel()
	  , value.getValue(0)->getLabel()
	);
}
void NovelToC11Pass::visit_epilog( StoreInstruction& value )
{}


void NovelToC11Pass::visit_prolog( AndInstruction& value )
{
	TODO;	
}
void NovelToC11Pass::visit_epilog( AndInstruction& value )
{}


void NovelToC11Pass::visit_prolog( AddSignedInstruction& value )
{
	TODO;	
}
void NovelToC11Pass::visit_epilog( AddSignedInstruction& value )
{}

void NovelToC11Pass::visit_prolog( DivSignedInstruction& value )
{
	TODO;	
}
void NovelToC11Pass::visit_epilog( DivSignedInstruction& value )
{}




void NovelToC11Pass::visit_prolog( BitConstant& value )
{
	StructureConstant* sc = 0;
	if( value.isBound() )
	{
		sc = value.getBound();
		u1 last = sc->getElements().back() == &value;
	    
		fprintf
		( stdout
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

void NovelToC11Pass::visit_prolog( StructureConstant& value )
{
	fprintf
	( stdout
	, "%s abc = { "
	, getTypeString( value )
	);
}
void NovelToC11Pass::visit_epilog( StructureConstant& value )
{
	fprintf
	( stdout
	, " };\n"
	  "\n"
	);
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

