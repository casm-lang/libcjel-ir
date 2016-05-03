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

	if( fclose( stream ) )
	{
		fprintf( stderr, "error: unable to close file stream\n" );
	}
 	
	return false;
}


static const char* getTypeString( Value& value )
{
	Type* type = value.getType();
	assert( type );
	
	if( type->getIDKind() == Type::BIT )
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
	else if( type->getIDKind() == Type::STRUCTURE )
	{
		Value* ty = type->getBound();
		assert(  Value::isa< Structure >( ty ) );
		//string t = "struct_" + string( ((Structure*)ty)->getName() );
		string t = string( ((Structure*)ty)->getName() );
	    return libstdhl::Allocator::string( t );
	}
	else if( type->getIDKind() == Type::INTERCONNECT )
	{
	    return libstdhl::Allocator::string( "uint64_t**" );
	}
	else if( type->getIDKind() == Type::STRING )
	{
	    return libstdhl::Allocator::string( "char*" );
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
			    , "%s  %s_var = { 0 }; // linkage '%s'\n"
				  "%s* %s = &%s_var; // \n"
			    , getTypeString( *ref )
			    , ref->getLabel()
			    , ref->getIdentifier()->getName()
			    , getTypeString( *ref )
			    , ref->getLabel()
			    , ref->getLabel()
				//, origin->getLabel() 
			    );
			    // fprintf
			    // ( stream
			    // , "%s* %s = &%s; // linkage '%s'\n"
			    // , getTypeString( *ref )
			    // , ref->getLabel()
			    // , origin->getLabel() 
			    // , ref->getIdentifier()->getName()
			    // );
			}
			origin = ref->getRef< Memory >();
			if( origin )
			{
				Memory* mem = (Memory*)origin;				
			    fprintf
			    ( stream
				, "%s* %s = malloc( sizeof( %s ) * %u ); // linkage '%s'\n"
				  "assert( %s );\n"
				, getTypeString( *mem )
				, ref->getLabel()
				, getTypeString( *mem )
				, mem->getSize()
				, ref->getIdentifier()->getName()
				, ref->getLabel()
				);
			    // fprintf
			    // ( stream
				// , "%s = malloc( sizeof( %s ) * %u ); // linkage '%s'\n"
				//   "assert( %s );\n"
				//   "%s* %s = %s;\n"
				// , mem->getLabel()
				// , getTypeString( *mem )
				// , mem->getSize()
				// , ref->getIdentifier()->getName()
				// , mem->getLabel()
				// , getTypeString( *mem )
				// , ref->getLabel()
				// , mem->getLabel()
			    // );
			}
			origin = ref->getRef< Interconnect >();
			if( origin )
			{
				Interconnect* ic = (Interconnect*)origin;

				// TODO: FIXME: PPA: HACK: needs better implemented directly in the NOVEL model
				Module* m = value.getRef< Module >();
				assert( m and m->has< Variable >() );
				
				fprintf
				( stream
				, "uint64_t* %s[] = // interconnect '%s'\n"
				  "{ "
				, ref->getLabel()
				, ic->getLabel()
				);
				for( Value* v : m->get< Variable >() )
				{
					fprintf
					( stream
					, "(uint64_t*)%s // '%s'\n%s"
					, v->getRef< Reference >()->getLabel()
					, v->getLabel()
					, m->get< Variable >().back() == v ? "" : ", "
					);
				}
				fprintf
				( stream
				, "};\n"
				  //"uint64_t*** %s = (uint64_t***)&%s_var;\n"
				// , ref->getLabel()
				// , ref->getLabel()
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
				fprintf
			    ( stream
				, "free( %s ); // linkage '%s'\n"
				, ref->getLabel()
				, ref->getIdentifier()->getName()
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
	const char* kind = "link";
	if( value.isInput() )
	{
		kind = "in";
	}
	if( value.isOutput() )
	{
		kind = "out";
	}
	
	fprintf
	( stream
	, "%s%s %s // %s %s%s"
	, getTypeString( value )
	, ( ( value.getType()->getIDKind() == Type::STRUCTURE or value.isOutput() ) ? "*" : "" )
	, value.getLabel()
	, value.getIdentifier()->getName()
	, kind
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
	static Value n( "", &TypeId, libnovel::Value::VALUE );

    Module* m = value.getRef< Module >();
	
	if( m->get< Variable >().front() == &value )
	{
		fprintf
		( stream
	    , "// Variables\n"
	    );
	}

	static u64 var_allocation = 0;
	
	fprintf
	( stream
	, "const %s %s = %lu; // '%s'\n"
	, getTypeString( n )
	, value.getLabel()
	, var_allocation
	, value.getIdent()
	);
	
	var_allocation++;
	
    // fprintf
	// ( stream
	// , "%s %s = { 0 }; // '%s'\n"
	// , getTypeString( *value.getType()->getBound() )
	// , value.getLabel()
	// , value.getIdent()
	// );
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
	fprintf
	( stream
	, "%s%s( "
	, indention( value )
	, ( Value::isa< CastInstruction >( value.getValue(0) ) ) ? value.getValue(0)->getLabel() : value.getValue(0)->getName()
	);
	
	CallableUnit* cu = 0;
	if( Value::isa< CallableUnit >( value.getValue(0) ) )
	{
		cu = (CallableUnit*)value.getValue(0);
	}
	else if( Value::isa< CastInstruction >( value.getValue(0) ) )
	{
		CastInstruction* ci = (CastInstruction*)value.getValue(0);
		assert( Value::isa< CallableUnit >( ci->getLHS() ) );
		cu = (CallableUnit*)ci->getLHS();
	}
    
	u8 cnt = 0;
	
	for( auto v : value.getValues() )
	{
		if( cnt == 0 )
		{
			cnt++;
			continue;
		}
		
		const char* kind = ")";
		if( Value::isa< Instruction >( v ) and cnt > cu->getInParameters().size() )
		{
			kind = "*)&";
		}
		else if( Value::isa< Instruction >( v ) and v->getType()->getIDKind() == Type::STRUCTURE )
		{
			kind = "*)&";
		}
		else if( Value::isa< Reference >( v ) and v->getType()->getIDKind() == Type::STRUCTURE )
		{
			kind = "*)";
		}
		else if( Value::isa< Constants >( v ) and v->getType()->getIDKind() == Type::STRUCTURE )
		{
			kind = "*)&";
		}
		else
		{
			printf( "\33[07mwarning:\33[0m unhandled 'kind' of a argument for call instr!\n" );
		}
		
		// if( Value::isa< Reference >( v ) )
		// {
		// 	kind = "*)";
		// }
		// else if( Value::isa< Instruction >( v ) or Value::isa< Constants >( v ) )
		// {
		// 	if( v->getType()->getIDKind() == Type::STRUCTURE ) // cnt <= cu->getInParameters().size() ) 
		// 	{
		// 		kind = "*)&";
		// 	}
		// 	else
		// 	{
		// 		assert(0);
		// 	}
		// }
		// else
		// {
		// 	assert(0);
		// }
		
		fprintf
		( stream
		, "%s(%s%s%s"
		, ( cnt > 1 ? ", " : "" )
		, getTypeString( *v )
		, kind
		, v->getLabel()
		);
		cnt++;
	}
	
	fprintf
	( stream
	, " ); // call %lu\n"
	, value.getValues().size() - 1
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
	, "%s%s %s = (%s)%s;// id\n"
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
	Value* kind = value.getLHS();
	Value* src  = value.getRHS();
	
	if( Value::isa< CallableUnit >( kind ) )
	{
		// TODO: FIXME: use 'kind' func signature to create better casts!
	    fprintf
	    ( stream
	    , "%svoid (*%s)() = (void (*)())%s;// cast Function\n"
	    , indention( value )
	    , value.getLabel()
	    , src->getLabel()
	    );
	}
	else if( Value::isa< Structure >( kind ) )
	{
	    fprintf
	    ( stream
		  //, "%s%s %s = *((%s*)%s%s);\n"
		, "%s%s* %s = (%s*)(%s%s); // cast Structure\n"
	    , indention( value )
		, getTypeString( value )
	    , value.getLabel()
		, getTypeString( value )
		, ( Value::isa< Reference >( src ) and src->getType()->getIDKind() == Type::STRUCTURE ) ? "*" : ""
	    , src->getLabel()
	    );
	}
	else
	{
		assert( !" unsupported feature !!! " );
	}
}
void NovelToC11Pass::visit_epilog( CastInstruction& value )
{}


//
// ExtractInstruction
//

void NovelToC11Pass::visit_prolog( ExtractInstruction& value )
{
	Value* base_   = value.getLHS();
	Value* offset_ = value.getRHS();
	
	assert( Value::isa< Reference >( base_ ) or Value::isa< CastInstruction >( base_ ) );
	Reference* base = (Reference*)base_;

	if( Value::isa< Structure >( offset_ ) )
	{
		Structure* offset = (Structure*)offset_;
		if(  Value::isa< Reference >( base_ ) )
		{
			assert( offset->getParent() == base->getStructure() and " offset is not a element of base structure! " );
		}
		
	    fprintf
	    ( stream
	    , "%s%s* %s = &(%s->%s); // extract '%s'\n"
	    , indention( value )
	    , getTypeString( value )
	    , value.getLabel()
	    , base->getLabel()
		, offset->getName()
		, base->getIdentifier()->getName()
        );
	}
    else if( Value::isa< Reference >( offset_ ) )
	{
		Reference* offset = (Reference*)offset_;
	    assert( base->getType()->getIDKind() == Type::INTERCONNECT );
		
		fprintf
	    ( stream
	    , "%s%s* %s = (%s*)(%s[%s]); // extract '%s'\n"
	    , indention( value )
	    , getTypeString( value )
	    , value.getLabel()
	    , getTypeString( value )
	    , base->getLabel()
		, offset->getLabel()
		, base->getIdentifier()->getName()
        );
	}
	else if( Value::isa< Instruction >( offset_ ) )
	{
		fprintf
	    ( stream
	    , "%s%s* %s = (%s*)(&%s[%s]); // extract '%s'\n"
	    , indention( value )
	    , getTypeString( value )
	    , value.getLabel()
	    , getTypeString( value )
	    , base->getLabel()
		, offset_->getLabel()
		, base->getIdentifier()->getName()
        );
	}
	else
	{
		assert( !" unsupported/unimplemented feature! " );
	}
}
void NovelToC11Pass::visit_epilog( ExtractInstruction& value )
{}


//
// LoadInstruction
//

void NovelToC11Pass::visit_prolog( LoadInstruction& value )
{
	Value* addr_ = value.get();
	
	if(  Value::isa< ExtractInstruction >( addr_ )
	  or Value::isa< CastInstruction >( addr_ )
	  )
	{
		fprintf
	    ( stream
	    , "%s%s %s = *%s; // load\n"
	    , indention( value )
	    , getTypeString( value )
	    , value.getLabel()
	    , addr_->getLabel()
	    );		
	}
	else
	{
		assert( !"  unimplemented feature! " );
	}
	
	// assert( Value::isa< ExtractInstruction >( value.get() ) );
	// ExtractInstruction* ext = (ExtractInstruction*)( value.get() );
	
	// assert( Value::isa< Reference >( ext->getLHS() ) );
	// Reference* ref = (Reference*)( ext->getLHS() );

	// if( Value::isa< Structure >( ext->getRHS() ) )
	// {
	// 	Structure* str = (Structure*)( ext->getRHS() );
	// 	assert( str->getParent() == ref->getStructure() );
	
	// 	fprintf
	//     ( stream
	//     , "%s%s %s = %s->%s; // load '%s'\n"
	//     , indention( value )
	//     , getTypeString( value )
	//     , value.getLabel()
	//     , ref->getLabel()
	//     , str->getName()
	//     , ref->getIdentifier()->getName()
	//     );
	// }
	// else if( Value::isa< Reference >( ext->getRHS() ) )
	// {
	// 	Reference* off = (Reference*)( ext->getRHS() );
		
	// 	fprintf
	//     ( stream
	// 	, "%s(uint64_t*) %s = %s[%s]; // load '%s'\n" // TODO: FIXME: PPA: HACK: !!!
	//     , indention( value )
	//     , value.getLabel()
	//     , ref->getLabel()
	//     , off->getLabel()
	//     , ref->getIdentifier()->getName()
	//     );
	// }
}
void NovelToC11Pass::visit_epilog( LoadInstruction& value )
{}


//
// StoreInstruction
//

void NovelToC11Pass::visit_prolog( StoreInstruction& value )
{
	Value* dst = value.getRHS();
	Value* src = value.getLHS();
	
	if( Value::isa< ExtractInstruction >( dst ) )
	{
		fprintf
		( stream
		, "%s*%s = %s%s; // store\n"
		, indention( value )
	    , dst->getLabel()
		, ( Value::isa< ExtractInstruction >( src ) ? "*" : "" )
	    , src->getLabel()
	    );
	}
	else if( Value::isa< Reference >( dst ) )
	{
		Reference* ref = (Reference*)( dst );
		assert( ref );
		
		fprintf
		( stream
		, "%s*%s = %s%s; // store '%s'\n"
		, indention( value )
	    , ref->getLabel()
		, ( Value::isa< CastInstruction >( src ) ? "*" : "" )
	    , src->getLabel()
	    , ref->getIdentifier()->getName()
	    );
	}
	else
	{
		TODO;
		assert( !" unimplemented feature! " );
	}
}
void NovelToC11Pass::visit_epilog( StoreInstruction& value )
{}


static void instr( BinaryInstruction& value, const char* op )
{
	fprintf
	( stream
	, "%s%s %s = (%s %s %s);\n"
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
// ModUnsignedInstruction
//

void NovelToC11Pass::visit_prolog( ModUnsignedInstruction& value )
{
    instr( value, "%" );
}
void NovelToC11Pass::visit_epilog( ModUnsignedInstruction& value )
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


static void cast( UnaryInstruction& value, const char* comment )
{
	fprintf
	( stream
	, "%s%s %s = (%s)%s; // %s\n"
	, indention( value )
	, getTypeString( value )
	, value.getLabel()
	, getTypeString( value )
	, value.get()->getLabel()
	, comment
	);	
}

//
// ZeroExtendInstruction
//

void NovelToC11Pass::visit_prolog( ZeroExtendInstruction& value )
{
	cast( value, "zext" );
}
void NovelToC11Pass::visit_epilog( ZeroExtendInstruction& value )
{}


//
// TruncationInstruction
//

void NovelToC11Pass::visit_prolog( TruncationInstruction& value )
{
	cast( value, "trunc" );	
}
void NovelToC11Pass::visit_epilog( TruncationInstruction& value )
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
// StringConstant
//

void NovelToC11Pass::visit_prolog( StringConstant& value )
{
	StructureConstant* sc = 0;
	if( value.isBound() )
	{
		sc = value.getBound();
		u1 last = sc->getElements().back() == &value;
	    
		fprintf
		( stream
		, "\"%s\"%s"
		, value.getValue()
		, last ? "" : ", "
		);
	}
	else
	{
		fprintf
		( stream
		, "const %s %s = \"%s\";\n"
		, getTypeString( value )
		, value.getLabel()
		, value.getValue()
		);
	}
}
void NovelToC11Pass::visit_epilog( StringConstant& value )
{}


//
// Interconnect
//

void NovelToC11Pass::visit_prolog( Interconnect& value )
{
    TODO;
}
void NovelToC11Pass::visit_epilog( Interconnect& value )
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

