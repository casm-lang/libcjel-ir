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
		//string t = "struct_" + string( ((Structure*)ty)->getName() );
		string t = string( ((Structure*)ty)->getName() );
	    return libstdhl::Allocator::string( t );
	}
	else
	{
		assert( !"unimplemented or unsupported type to convert!" );
	}
}


static const char* indention( Value& value )
{
	string ind = "";
	u8 cnt = 0;
	Value* p = (&value);
	while( p != 0 )
	{
		if( Value::isa< Block >( p ) )
		{
			p = (Value*)((Block*)p)->getParent();
		}
		else if( Value::isa< Instruction >( p ) )
		{
			p = (Value*)((Instruction*)p)->getStatement();
		}
		else
		{
			assert(0);
		}

		if( Value::isa< CallableUnit >( p ) )
		{
		    break;
		}
		
		cnt++;
		ind+="    ";
	}
	
	return libstdhl::Allocator::string( ind );
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
	  "#include <stdio.h>\n"
	  "#include <stdlib.h>\n"
	  "#include <stdint.h>\n"
	  "#include <assert.h>\n"
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
	  "%s %s\n"
	  "( "
	, value.getLabel()
	, ( strcmp( value.getName(), "main" ) == 0 ? "int" : "void" )
	, value.getName()
	);	
	
	if( strcmp( value.getName(), "main" ) == 0 )
	{
	    fprintf
	    ( stream
	    , "int    argc\n"
	      ", char** argv%s"
		, ( value.getParameterLength() == 0 ? "" : "\n, " )
	    );
	}
}
void NovelToC11Pass::visit_interlog( Function& value )
{
	fprintf
	( stream
	, "\n"
	  ")\n"
	);
	
    if( value.getLinkage().size() > 0 )
	{
		fprintf
		( stream
		, "{ // linkage\n"
		);
		
		for( Value* linkage : value.getLinkage() )
		{
			assert( Value::isa< Reference >( linkage ) );
			Reference* ref = (Reference*)linkage;
			
		    Value* origin = ref->getRef< Variable >();
			if( origin )
			{
			    fprintf
			    ( stream
			    , "%s* %s = &%s; // '%s'\n"
			    , getTypeString( *ref )
			    , ref->getIdentifier()->getName()
			    , origin->getLabel() 
			    , ref->getLabel() 
			    );
			}
			origin = ref->getRef< Memory >();
			if( origin )
			{
				Memory* mem = (Memory*)origin;				
			    fprintf
			    ( stream
				, "%s = malloc( sizeof( %s ) * %u );\n"
				  "assert( %s );\n"
				  "%s* %s = %s; // '%s'\n"
				, mem->getLabel()
				, getTypeString( *mem )
				, mem->getSize()
				, mem->getLabel()
				, getTypeString( *mem )
				, ref->getIdentifier()->getName()
				, mem->getLabel()
				, ref->getLabel()
			    );
			}
		    //assert( origin and " internal error! " );
		}
	}
}
void NovelToC11Pass::visit_epilog( Function& value )
{
	if( value.getLinkage().size() > 0 )
	{
		for( Value* linkage : value.getLinkage() )
		{
			assert( Value::isa< Reference >( linkage ) );
			Reference* ref = (Reference*)linkage;
			
		    Value* origin = ref->getRef< Memory >();
			if( origin )
			{
				Memory* mem = (Memory*)origin;
				fprintf
			    ( stream
				, "free( %s ); // '%s'\n"
				, ref->getIdentifier()->getName()
				, mem->getLabel()
				);
			}
		}
		
	    fprintf
	    ( stream
	    , "}\n"
	    );
	}

	fprintf
	( stream
	, "\n"
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
	
	assert( value.getLinkage().size() == 0 );
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
	, value.getIdentifier()->getName()
	, value.getLabel()
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
	, "typedef struct %s_t\n"
	  "{ "
	  , value.getIdentifier()->getName()
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
	, "}\n"
	  "%s;\n"
	  "\n"
	, getTypeString( value )
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
	, "%s %s = { 0 }; // '%s'\n"
	, getTypeString( *value.getType()->getBound() )
	, value.getLabel()
	, value.getIdent()
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
	Module* m = value.getRef< Module >();
	if( m->get< Memory >().front() == &value )
	{
		fprintf
		( stream
	    , "// Memory\n"
	    );
	}
	
	fprintf
	( stream
	, "%s* %s = 0; // size = '%u'\n"
    , getTypeString( value )
	, value.getLabel()
	, value.getSize()
	);
}
void NovelToC11Pass::visit_epilog( Memory& value )
{
	Module* m = value.getRef< Module >();
	if( m->get< Memory >().back() == &value )
	{
		fprintf
		( stream
	    , "\n"
	    );
	}
}


static void non_trivial_statement( Scope& value )
{
	const Value* parent = value.getParent();
	assert( parent );
	
	if( Value::isa< BranchStatement >( parent ) )
	{
		BranchStatement* branch = (BranchStatement*)parent;
		
		Value* expr = (Value*)branch->getInstructions().back();
		assert( expr );
		assert( Value::isa< LogicalInstruction >( expr ) );
		
		if( branch->getScopes().front() == &value )
		{
	        fprintf
	        ( stream
	        , "%sif( %s )\n"
	        , indention( value )
			, expr->getLabel()
	        );
		}
		else if( branch->getScopes().back() == &value )
		{
	        fprintf
	        ( stream
	        , "%selse\n"
	        , indention( value )
	        );
		}
	}	
}


//
// ParallelScope
//

void NovelToC11Pass::visit_prolog( ParallelScope& value )
{
	non_trivial_statement( value );
	
	fprintf
	( stream
	, "%s{ // par '%s'\n"
	, indention( value )
	, value.getLabel()
	);
}
void NovelToC11Pass::visit_epilog( ParallelScope& value )		
{
	fprintf
	( stream
	, "%s}\n"
	, indention( value )
	);
}


//
// SequentialScope
//

void NovelToC11Pass::visit_prolog( SequentialScope& value )
{
	non_trivial_statement( value );
	
	fprintf
	( stream
	, "%s{ // seq '%s'\n"
	, indention( value )
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
	  "%s{\n"
	, indention( value )
	, value.getLabel()
	, indention( value )
	);
}
void NovelToC11Pass::visit_epilog( TrivialStatement& value )
{
	fprintf
	( stream
	, "%s}\n"
	, indention( value )
	);
}


//
// BranchStatement
//

void NovelToC11Pass::visit_prolog( BranchStatement& value )
{
	fprintf
	( stream
	, "%s// branch '%s'\n"
	  "%s{\n"
	, indention( value )
	, value.getLabel()
	, indention( value )
	);
}
void NovelToC11Pass::visit_interlog( BranchStatement& value )
{}
void NovelToC11Pass::visit_epilog( BranchStatement& value )
{
	visit_epilog( *((TrivialStatement*)(&value)) );
}


//
// LoopStatement
//

void NovelToC11Pass::visit_prolog( LoopStatement& value )
{	
	fprintf
	( stream
	, "%s// loop '%s'\n"
	  "%swhile(1)\n"
	  "%s{\n"
    , indention( value )
	, value.getLabel()
	, indention( value )
	, indention( value )
	);
}
void NovelToC11Pass::visit_interlog( LoopStatement& value )
{
	Value* expr = (Value*)value.getInstructions().back();
	assert( expr );
	assert( Value::isa< LogicalInstruction >( expr ) );
	
	fprintf
	( stream
	, "%s    if( !%s )\n"
	  "%s    {\n"
	  "%s        break;\n"
	  "%s    }\n"
	  "%s    \n"
	, indention( value )
	, expr->getLabel()
	, indention( value )
	, indention( value )
	, indention( value )
	, indention( value )
	);
}
void NovelToC11Pass::visit_epilog( LoopStatement& value )
{
	visit_epilog( *((TrivialStatement*)(&value)) );
}


//
// CallInstruction
//

void NovelToC11Pass::visit_prolog( CallInstruction& value )
{
	if( Value::isa< CastInstruction >( value.getValue(0) ) )
	{
		fprintf
		( stream
		, "%s%s( ); // call (indirect)\n"
		, indention( value )
		, value.getValue(0)->getLabel()
		);
		
		return;
	}
	
	fprintf
	( stream
	, "%s%s( "
	, indention( value )
	, value.getValue(0)->getName()
	);
	
	u8 cnt = 0;
	
	for( auto v : value.getValues() )
	{
		if( cnt == 0 )
		{
			cnt++;
			continue;
		}
		
		fprintf
		( stream
		, "%s(%s*)&%s"
		, ( cnt > 1 ? ", " : "" )
		, getTypeString( *v )
		, v->getLabel()
		);
		cnt++;
	}
	
	fprintf
	( stream
	, " ); // call %lu\n"
	, value.getValues().size()
	);
}
void NovelToC11Pass::visit_epilog( CallInstruction& value )
{}


//
// StreamInstruction
//

static void replace( std::string& str, const std::string& search, const std::string& replace )
{
	size_t pos = 0;
	while( ( pos = str.find(search, pos) ) != std::string::npos)
	{
		str.replace( pos, search.length(), replace );
		pos += replace.length();
	}
}

void NovelToC11Pass::visit_prolog( StreamInstruction& value )
{
	assert( value.getChannel() == StreamInstruction::OUTPUT );

	const char* channel = "stdout";

	string fmt = "\"";
	string arg = "";
	for( Value* i : value.getValues() )
	{
		if( Value::isa< Variable >( i ) )
		{
			fmt += "%p";
			arg += ", &" + arg += string( i->getLabel() );
		}
		else if( Value::isa< StringConstant >( i ) )
		{
			StringConstant* c = (StringConstant*)i;
			std::string tmp = std::string( c->getValue() );
			replace( tmp, "\n", "\\n" );
			fmt += tmp;
		}
		else
		{
			assert( !" unimplemented Value to 'stream'! " );
		}
		
		
	}
	fmt += "\"";
	
	fprintf
	( stream
	, "%sfprintf( %s, %s%s );\n"
    , indention( value )
	, channel
	, fmt.c_str()
	, arg.c_str()
	);
}
void NovelToC11Pass::visit_epilog( StreamInstruction& value )
{}


//
// NopInstruction
//

void NovelToC11Pass::visit_prolog( NopInstruction& value )
{
	fprintf
	( stream
	, "%s// nop\n"
    , indention( value )
	);
}
void NovelToC11Pass::visit_epilog( NopInstruction& value )
{}


//
// AllocInstruction
//

void NovelToC11Pass::visit_prolog( AllocInstruction& value )
{
	fprintf
	( stream
	, "%s%s %s;// alloc\n"
    , indention( value )
	, getTypeString( value )
	, value.getLabel()
	);
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
	, "%s%s %s = (%s)&%s;// id\n"
	, indention( value )
	, getTypeString( value )
	, value.getLabel()
	, getTypeString( value )
	, ( Value::isa< CallableUnit >( value.get() ) ? value.get()->getName() : value.get()->getLabel() )
	);
}
void NovelToC11Pass::visit_epilog( IdInstruction& value )
{}


//
// CastInstruction
//

void NovelToC11Pass::visit_prolog( CastInstruction& value )
{
	assert( value.getType()->getIDKind() == Type::ID::FUNCTION );
	
	fprintf
	( stream
	, "%svoid (*%s)() = (void (*)())%s;// cast Function\n"
	, indention( value )
	, value.getLabel()
	, value.get()->getLabel()
	);
}
void NovelToC11Pass::visit_epilog( CastInstruction& value )
{}


//
// ExtractInstruction
//

void NovelToC11Pass::visit_prolog( ExtractInstruction& value )
{}
void NovelToC11Pass::visit_epilog( ExtractInstruction& value )
{}


//
// LoadInstruction
//

void NovelToC11Pass::visit_prolog( LoadInstruction& value )
{
	assert( Value::isa< ExtractInstruction >( value.get() ) );
	ExtractInstruction* ext = (ExtractInstruction*)( value.get() );
	
	assert( Value::isa< Reference >( ext->getLHS() ) );
	Reference* ref = (Reference*)( ext->getLHS() );

	assert( Value::isa< Structure >( ext->getRHS() ) );
	Structure* str = (Structure*)( ext->getRHS() );

	assert( str->getParent() == ref->getStructure() );
	
	fprintf
	( stream
	, "%s%s %s = %s->%s;\n"
	, indention( value )
	, getTypeString( value )
	, value.getLabel()
	, ref->getIdentifier()->getName()
	, str->getName()
	);
	
}
void NovelToC11Pass::visit_epilog( LoadInstruction& value )
{}


//
// StoreInstruction
//

void NovelToC11Pass::visit_prolog( StoreInstruction& value )
{
	// fprintf
	// ( stream
	// , "%s*%s = %s;// store\n"
	// , "" // TODO: FIXME: indention!!!
	// , value.getValue(1)->getLabel()
	// , value.getValue(0)->getLabel()
	// );
	
	Value* dst = value.getRHS();
	Value* src = value.getLHS();
	
	if( Value::isa< ExtractInstruction >( dst ) )
	{
		ExtractInstruction* ext = (ExtractInstruction*)( dst );
		assert( ext );
		
		assert( Value::isa< Reference >( ext->getLHS() ) );
		Reference* ref = (Reference*)( ext->getLHS() );
		
		assert( Value::isa< Structure >( ext->getRHS() ) );
		Structure* str = (Structure*)( ext->getRHS() );
		
		assert( str->getParent() == ref->getStructure() );
		fprintf
		( stream
	    , "%s%s->%s = %s; // store\n"
	    , indention( value )
		, ref->getIdentifier()->getName()
	    , str->getName()
	    , src->getLabel()
	    );		
	}
	else if( Value::isa< Reference >( dst ) )
	{
		Reference* ref = (Reference*)( dst );
		assert( ref );
		
		fprintf
		( stream
		, "%s*%s = %s; // store\n"
		, indention( value )
	    , ref->getIdentifier()->getName()
	    , src->getLabel()
	    );
	}
	else
	{
		TODO;
		assert( !" unimplemented! " );
	}	
}
void NovelToC11Pass::visit_epilog( StoreInstruction& value )
{}


static void instr( BinaryInstruction& value, const char* op )
{
	fprintf
	( stream
	, "%s%s %s = %s %s %s;\n"
	, indention( value )
	, getTypeString( value )
	, value.getLabel()
	, value.getLHS()->getLabel()
	, op
	, value.getRHS()->getLabel()
	);	
}

//
// AndInstruction
//

void NovelToC11Pass::visit_prolog( AndInstruction& value )
{
	instr( value, "&" );
}
void NovelToC11Pass::visit_epilog( AndInstruction& value )
{}


//
// AddSignedInstruction
//

void NovelToC11Pass::visit_prolog( AddSignedInstruction& value )
{
	instr( value, "+" );
}
void NovelToC11Pass::visit_epilog( AddSignedInstruction& value )
{}


//
// DivSignedInstruction
//

void NovelToC11Pass::visit_prolog( DivSignedInstruction& value )
{
	instr( value, "/" );
}
void NovelToC11Pass::visit_epilog( DivSignedInstruction& value )
{}


//
// EquUnsignedInstruction
//

void NovelToC11Pass::visit_prolog( EquUnsignedInstruction& value )
{
	instr( value, "==" );
}
void NovelToC11Pass::visit_epilog( EquUnsignedInstruction& value )
{
}


//
// NeqUnsignedInstruction
//

void NovelToC11Pass::visit_prolog( NeqUnsignedInstruction& value )
{
	instr( value, "!=" );
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
		fprintf
		( stream
		, "const %s %s = %lu;\n"
		, getTypeString( value )
		, value.getLabel()
		, value.getValue()[0]
		);
	}
}
void NovelToC11Pass::visit_epilog( BitConstant& value )
{
	if( not value.isBound() )
	{
		Module* m = value.getRef<Module>();
	
		if( m->get< Constants >().back() == &value )
		{
			fprintf
			( stream
	        , "\n"
	        );
		}
	}
}


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
	, "const %s %s = { "
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

