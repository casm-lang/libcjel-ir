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

#include "NovelToVHDLPass.h"

using namespace libnovel;


char NovelToVHDLPass::id = 0;

static libpass::PassRegistration< NovelToVHDLPass > PASS
( "NOVEL IR to VHDL"
, "generates VDHL code out of the NOVEL IR"
, 0
, 0
);

static FILE* stream = stderr;


bool NovelToVHDLPass::run( libpass::PassResult& pr )
{
    Module* value = (Module*)pr.getResult< NovelDumpPass >();
	assert( value );
    
	string fn = "obj/" + string( value->getName() ) + ".vhd"; 
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
	
	if( type->getIDKind() == Type::ID::BIT )
	{
		if( type->getBitsize() == 1 )
		{
			return "std_logic";
		}
		else if( type->getBitsize() > 1 )
		{
			string t
				= "std_logic_vector( "
				+ to_string( type->getBitsize() - 1 )
				+ " downto 0 )";
			return libstdhl::Allocator::string( t );
		}
		else
		{
			assert( !"invalid type bit size" );
		}
	}
	else if( type->getIDKind() == Type::ID::STRUCTURE )
	{
		Value* ty = type->getBound();
		assert(  Value::isa< Structure >( ty ) );
		string t = "struct_" + string( ((Structure*)ty)->getName() );
	    return libstdhl::Allocator::string( t );
	}
	else
	{
		assert( !"unimplemented or unsupported type to convert!" );
	}
}


static void emit_wire( Value& value )
{
	assert( Value::isa< CallableUnit >( &value ) );
	CallableUnit* cunit = ((CallableUnit*)(&value));

	for( auto link : cunit->getLinkage() )
	{
		Reference* linkage = (Reference*)link;
		
		fprintf
		( stream
		, "  signal %s : %s := %s;\n"
		, linkage->getIdentifier()->getName()
		, getTypeString( *linkage )
		, linkage->getRef< Variable >()->getExpression()->getLabel()
		);
	}
	
	Block* context = cunit->getContext();
	assert( context );
	
	if( Value::isa< Scope >( context ) )
	{
		for( auto block : ((Scope*)context)->getBlocks() )
		{
			fprintf
			( stream
			, "  signal req_%s : std_logic := '0';\n"
			  "  signal ack_%s : std_logic := '0';\n"
			, block->getLabel()
			, block->getLabel()
			);
		}
	}
}


//
// Module
//

void NovelToVHDLPass::visit_prolog( Module& value )
{
	std::chrono::time_point<std::chrono::system_clock> stamp;
	stamp = std::chrono::system_clock::now();
	std::time_t timestamp = std::chrono::system_clock::to_time_t( stamp );
	
	fprintf
	( stream
	, "-- Generated At: %s"
	  "-- Module: '%s'\n"
	  "\n"
	, std::ctime( &timestamp )
	, value.getName()
	);
}
void NovelToVHDLPass::visit_epilog( Module& value )
{
	fprintf( stream, "-- end of module: '%s'\n\n", value.getName() );		
}


//
// Function
//

void NovelToVHDLPass::visit_prolog( Function& value )
{
	fprintf
	( stream
	, "-- Function '%s'\n"
	  "library IEEE;\n"
	  "use IEEE.std_logic_1164.all;\n"
	  "use IEEE.numeric_std.all;\n"
	  "use work.Structure.all;\n"
	  "use work.Constants.all;\n"
	  "use work.Variables.all;\n"
	  "entity %s is port\n"
	  "( req : in  std_logic\n"
	  "; ack : out std_logic\n"
	, value.getLabel()
	, value.getName()
	);
}
void NovelToVHDLPass::visit_interlog( Function& value )
{
	string tmp( "\n;" );
	
	if( value.getInParameters().size()  == 0
	and value.getOutParameters().size() == 0 )
	{
		tmp = "";
	}
	
	fprintf
	( stream
	, ");\n"
	  "end %s;\n"
	  "architecture \\@%s@\\ of %s is\n"
	, value.getName()
	, value.getName()
	, value.getName()
	);
	
	emit_wire( value );
	
	fprintf
	( stream
	, "begin\n"
	);
}
void NovelToVHDLPass::visit_epilog( Function& value )
{
	fprintf
	( stream
	, "end \\@%s@\\;\n"
	  "\n"
	, value.getName()
	);
}


//
// Intrinsic
//

void NovelToVHDLPass::visit_prolog( Intrinsic& value )
{
	fprintf
	( stream
	, "-- Intrinsic '%s'\n"
	  "library IEEE;\n"
	  "use IEEE.std_logic_1164.all;\n"
	  "use IEEE.numeric_std.all;\n"
	  "use work.Structure.all;\n"
	  "use work.Constants.all;\n"
	  "use work.Variables.all;\n"
	  "entity %s is port\n"
	  "( req : in  std_logic\n"
	  "; ack : out std_logic\n"
	, value.getLabel()
	, value.getName()
	);
}
void NovelToVHDLPass::visit_interlog( Intrinsic& value )
{
	visit_interlog( *((Function*)(&value)) );
}
void NovelToVHDLPass::visit_epilog( Intrinsic& value )
{
	visit_epilog( *((Function*)(&value)) );
}


//
// Reference
//

void NovelToVHDLPass::visit_prolog( Reference& value )
{
	const char* kind = 0;
	
	if( value.isInput() )
	{
		kind = "in";
	}
	else
	{
		kind = "out";
	}
	
	fprintf
	( stream
	, "; %s : %s %s\n"
	, value.getIdentifier()->getName()
	, kind
	, getTypeString( value )
	);
}
void NovelToVHDLPass::visit_epilog( Reference& value )
{}


//
// Structure
//

void NovelToVHDLPass::visit_prolog( Structure& value )
{
	Module* m = value.getRef<Module>();
	
	if( m->get< Structure >().front() == &value )
	{
		fprintf
		( stream
	    , "-- Structures\n"
	      "library IEEE;\n"
	      "use IEEE.std_logic_1164.all;\n"
	      "use IEEE.numeric_std.all;\n"
	      "package Structure is\n"
	    );
	}
	
    fprintf
	( stream
	, "type %s is\n"
	  "record\n"
    , getTypeString( value )
	);
	
	for( const Structure* s : value.getElements() )
	{
        fprintf
	    ( stream
		, "  %s : %s;\n"
		, s->getIdentifier()->getName()
		, getTypeString( *((Value*)s) )
		);
	}
	
	fprintf
	( stream
	, "end record;\n"
	);
	
	if( m->get< Structure >().back() == &value )
	{
		fprintf
		( stream
	    , "end;\n"
	    );
	}
	
	fprintf
	( stream
	, "\n"
	);
}
void NovelToVHDLPass::visit_epilog( Structure& value )
{
}


//
// Variable
//

void NovelToVHDLPass::visit_prolog( Variable& value )
{
	static Value n( "", &TypeId, libnovel::Value::VALUE );
	
	Module* m = value.getRef< Module >();
	
	if( m->get< Variable >().front() == &value )
	{
		fprintf
		( stream
	    , "-- Variables\n"
	      "library IEEE;\n"
	      "use IEEE.std_logic_1164.all;\n"
	      "use IEEE.numeric_std.all;\n"
		  "use work.Structure.all;\n"
		  "use work.Constants.all;\n"
		  "package Variables is\n"
	    );
	}

	static u64 var_allocation = 0;
	std::bitset< 48 > v( var_allocation );
	var_allocation++;
	
	fprintf
	( stream
	, "  constant %s_id : %s := \"%s\";\n"
	, value.getLabel()
	, getTypeString( n )
	, v.to_string().c_str()
	);
}
void NovelToVHDLPass::visit_epilog( Variable& value )
{
	Module* m = value.getRef< Module >();
	
	if( m->get< Variable >().back() == &value )
	{
		fprintf
		( stream
	    , "end;\n"
		  "\n"
	    );
	}
}


//
// Memory
//

void NovelToVHDLPass::visit_prolog( Memory& value )
{
	TODO;
}
void NovelToVHDLPass::visit_epilog( Memory& value )
{
}


//
// ParallelScope
//

void NovelToVHDLPass::visit_prolog( ParallelScope& value )
{
	string tmp("req");
		
	if( not Value::isa< CallableUnit >( value.getParent() ) )
	{
		tmp += "_" + std::string( value.getLabel() );
	}
	
	fprintf
	( stream
	, "  -- req %s -- par begin\n"
	  "  process( %s ) is\n"
	  "  begin\n"
	  "    if rising_edge( %s ) then\n"
	, value.getLabel()
	, tmp.c_str()
	, tmp.c_str()
	);
	
	for( auto block : value.getBlocks() )
	{
		fprintf
		( stream
		, "      " // TODO: FIXME: dynamic indention calculation 
		  "req_%s <= transport '1' after 5 ns;\n"
		, block->getLabel()
		);
	}
	if( value.getBlocks().size() == 0 )
	{
		fprintf
		( stream
		, "      " // TODO: FIXME: dynamic indention calculation 
		  "null; -- EMPTY SCOPE!\n"
	    );
	}
	
	fprintf
	( stream
	, "    end if;\n"
	  "  end process;\n"
	  "\n"
	);	
}
void NovelToVHDLPass::visit_epilog( ParallelScope& value )		
{
	string tmp("ack");
		
	if( not Value::isa< CallableUnit >( value.getParent() ) )
	{
		tmp += "_" + std::string( value.getLabel() );
	}
	
	fprintf
	( stream
	, "  -- ack %s -- par end\n"
	  "  %s <= transport ( "
	, value.getLabel()
	, tmp.c_str()
	);
	
	u1 first = true;
	for( auto block : value.getBlocks() )
	{
		fprintf
		( stream
		, "%sack_%s"
		, first ? "" : " and "
		, block->getLabel()
		);
			
		if( first )
		{
			first = false;
		}
	}
	if( value.getBlocks().size() == 0 )
	{
		fprintf
		( stream
		, "'1'"
		);
	}
	
	fprintf
	( stream
	, " ) after 5 ns;\n"
	);
}


//
// SequentialScope
//

void NovelToVHDLPass::visit_prolog( SequentialScope& value )
{
	fprintf
	( stream
	, "  -- req %s -- seq begin\n"
	, value.getLabel()
	);	

	u1 first = true;
	Value* last = &value;
	
	for( Value* block : value.getBlocks() )
	{
		string tmp("");
		
		if( first )
		{
			if( not Value::isa< CallableUnit >( value.getParent() ) )
			{
				tmp += "req_" + std::string(last->getLabel());
			}
			else
			{
				tmp = "req";
			}
		}
		else
		{
			tmp += "ack_" + std::string(last->getLabel());
		}
		
		fprintf
		( stream
		, "  process( %s ) is\n"
		  "  begin\n"
		  "    if rising_edge( %s ) then\n"
		  "      req_%s <= transport '1' after 5 ns;\n" // TODO: FIXME: dynamic indention calculation
		  "    end if;\n"
		  "  end process;\n"
		  "\n"
		, tmp.c_str()
		, tmp.c_str()
		, block->getLabel()
		);
		
		first = false;
		last = block;
	}
	
	if( value.getBlocks().size() == 0 )
	{
		fprintf
		( stream
		, "  -- EMPTY SCOPE!\n"
	    );
	}	
}
void NovelToVHDLPass::visit_epilog( SequentialScope& value )
{
	string tmp("ack");

	if( not Value::isa< CallableUnit >( value.getParent() ) )
	{
		tmp += "_" + std::string(value.getLabel());
	}
	
	fprintf
	( stream
	, "  -- ack %s -- seq end\n"
	  "  %s <= transport ( %s%s ) after 5 ns;\n"
	, value.getLabel()
	, tmp.c_str()
	, value.getBlocks().size() > 0 ? "ack_" : ""
	, value.getBlocks().size() > 0 ? value.getBlocks().back()->getLabel() : "'1'"
	);
}


//
// TrivialStatement
//

void NovelToVHDLPass::visit_prolog( TrivialStatement& value )
{
	u1 plain = true;
	for( Value* i : value.getInstructions() )
	{
		if( not Value::isa< CallInstruction >( i )
		and not Value::isa< AllocInstruction >( i ) )
		{
			plain = false;
			break;
		}
	}
	
	//if( value.consistsOnlyOf< CallInstruction >() )
	if( plain ) // TODO: FIXME: PPA: this can be much more generic!!!
	{
		fprintf
		( stream
		, "  -- stmt %s\n"
		  "  %s : block\n"
		, value.getLabel()
		, value.getLabel()
		);
		
		for( Value* instr : value.getInstructions() )
		{
			if( Value::isa< AllocInstruction >( instr ) )
			{
				fprintf
				( stream
				, "    signal %s : %s;\n"
				, instr->getLabel()
				, getTypeString( *instr )
				);
				//continue;
			}
			
			fprintf
			( stream
			, "    signal sig_%s : std_logic;\n"
			, instr->getLabel()
			);
		}
		fprintf
		( stream
		, "    signal sig_%s : std_logic;\n"
		, value.getLabel()
		);

		// Value* n = value.getNext();
		// while( n != 0 and Value::isa< AllocInstruction >( n ) )
		// {
		// 	n = n->getNext();
		// }
		
		fprintf
		( stream
		, "  begin\n"
		  "    sig_%s <= req_%s;\n"
		  "    ack_%s <= sig_%s;\n"
		  //, n != 0 ? n->getLabel() : ((Instruction*)&value)->getStatement()->getLabel()
		, value.getInstructions().front()->getLabel()
		, value.getLabel()
		, value.getLabel()
		, value.getLabel()
		);
		
		return;
	}
	
	string tmp("req_" + std::string(value.getLabel()));
	
	fprintf
	( stream
	, "  -- stmt %s\n"
	  "  process( %s ) is\n"
	, value.getLabel()
	, tmp.c_str()
	);
	
	for( Value* instr : value.getInstructions() )
	{
		if( Value::isa< ExtractInstruction >( instr )
		or  Value::isa< StoreInstruction >( instr )
		or  Value::isa< NopInstruction >( instr ) )
		{
			continue;
		}
		
		fprintf
		( stream
	    , "    variable %s : %s;\n"
		, instr->getLabel()
		, getTypeString( *instr )
		);
	}
	
	fprintf
	( stream
	, "  begin\n"
	  "    if rising_edge( %s ) then\n"
	, tmp.c_str()
	);
}
void NovelToVHDLPass::visit_epilog( TrivialStatement& value )
{
	u1 plain = true;
	for( Value* i : value.getInstructions() )
	{
		if( not Value::isa< CallInstruction >( i )
		and not Value::isa< AllocInstruction >( i ) )
		{
			plain = false;
			break;
		}
	}
	
	//if( value.consistsOnlyOf< CallInstruction >() )
	if( plain ) // TODO: FIXME: PPA: this can be much more generic!!!
	{
		fprintf
		( stream
		, "  end block;\n"
		  "\n"
		);
		
		return;
	}
	
	string tmp("ack_" + std::string(value.getLabel()));
	
	fprintf
	( stream
	, "      ack_%s <= transport ( '1' ) after 25 ns;\n"
	  "    end if;\n"
	  "  end process;\n"
	  "\n"
	, value.getLabel()
	);
}


//
// BranchStatement
//

void NovelToVHDLPass::visit_prolog( BranchStatement& value )
{
	TODO;
}
void NovelToVHDLPass::visit_interlog( BranchStatement& value )
{}
void NovelToVHDLPass::visit_epilog( BranchStatement& value )
{}


//
// LoopStatement
//

void NovelToVHDLPass::visit_prolog( LoopStatement& value )
{
	TODO;
}
void NovelToVHDLPass::visit_interlog( LoopStatement& value )
{}
void NovelToVHDLPass::visit_epilog( LoopStatement& value )
{}


//
// CallInstruction
//

void NovelToVHDLPass::visit_prolog( CallInstruction& value )
{
	fprintf
	( stream
	, "    call_%s : entity work.%-15s port map( "
	, value.getLabel()
	, value.getValue(0)->getName()
	);

	// Value* n = value.getNext();
	// while( n != 0 and Value::isa< AllocInstruction >( n ) )
	// {
	// 	n = n->getNext();
	// }
	
	fprintf
	( stream
	, "sig_%s, sig_%s"
	, value.getLabel()
	  //, n != 0 ? n->getLabel() : ((Instruction*)&value)->getStatement()->getLabel()
	, value.getNext() != 0 ? value.getNext()->getLabel() : ((Instruction*)&value)->getStatement()->getLabel()
	);
	
	u1 first = true;
	for( auto v : value.getValues() )
	{
		if( first )
		{
			first = false;
			continue;
		}

		fprintf( stream, ", %s", v->getLabel() );
	}
	
	fprintf
	( stream
	, " ); -- call %lu\n"
	, value.getValues().size()
	);
}
void NovelToVHDLPass::visit_epilog( CallInstruction& value )	
{
}


//
// IdCallInstruction
//

void NovelToVHDLPass::visit_prolog( IdCallInstruction& value )
{
	TODO;
	assert(0);
}
void NovelToVHDLPass::visit_epilog( IdCallInstruction& value )
{}


//
// StreamInstruction
//

void NovelToVHDLPass::visit_prolog( StreamInstruction& value )
{
	TODO;
}
void NovelToVHDLPass::visit_epilog( StreamInstruction& value )
{}



//
// NopInstruction
//

void NovelToVHDLPass::visit_prolog( NopInstruction& value )
{
	fprintf
	( stream
	, "      null; -- NO OPERATION\n"
	);
}
void NovelToVHDLPass::visit_epilog( NopInstruction& value )
{
}


//
// AllocInstruction
//

void NovelToVHDLPass::visit_prolog( AllocInstruction& value )
{
	fprintf
	( stream
	, "    sig_%s <= sig_%s;\n"
	, value.getNext() != 0 ? value.getNext()->getLabel() : ((Instruction*)&value)->getStatement()->getLabel()
	, value.getLabel()
	);
}
void NovelToVHDLPass::visit_epilog( AllocInstruction& value )
{
}


//
// IdInstruction
//

void NovelToVHDLPass::visit_prolog( IdInstruction& value )
{
	if( Value::isa< Variable >( value.get() ) )
	{
		fprintf
		( stream
		, "      %s := %s_id;\n "
		, value.getLabel()
		, value.get()->getLabel()
		);
	}
	else
	{
		TODO;
		assert( !"unimplemented!" );
	}
}
void NovelToVHDLPass::visit_epilog( IdInstruction& value )		
{}


//
// CastInstruction
//

void NovelToVHDLPass::visit_prolog( CastInstruction& value )
{
	TODO;
}
void NovelToVHDLPass::visit_epilog( CastInstruction& value )
{}


//
// ExtractInstruction
//

void NovelToVHDLPass::visit_prolog( ExtractInstruction& value )
{
}
void NovelToVHDLPass::visit_epilog( ExtractInstruction& value )
{
}


//
// LoadInstruction
//

void NovelToVHDLPass::visit_prolog( LoadInstruction& value )
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
	, "      %s := %s.%s;\n"
	, value.getLabel()
	, ref->getIdentifier()->getName()
	, str->getName()
	);
	
	// fprintf
	// ( stream
	// , "      %s := %s;\n"
	// , value.getLabel()
	//   , value.get()->getLabel()
	// );
}
void NovelToVHDLPass::visit_epilog( LoadInstruction& value )		
{
}


//
// StoreInstruction
//

void NovelToVHDLPass::visit_prolog( StoreInstruction& value )
{
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
	    , "      %s.%s <= transport %s after 20 ns;\n"
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
	    , "     %s <= transport %s after 20 ns;\n"
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
void NovelToVHDLPass::visit_epilog( StoreInstruction& value )		
{
}




//
// NotInstruction
//

void NovelToVHDLPass::visit_prolog( NotInstruction& value )
{
	TODO;
	assert(0);
}
void NovelToVHDLPass::visit_epilog( NotInstruction& value )
{}


//
// AndInstruction
//

void NovelToVHDLPass::visit_prolog( AndInstruction& value )
{
	fprintf
	( stream
	, "      %s := %s and %s;\n"
	, value.getLabel()
	, value.getLHS()->getLabel()
	, value.getRHS()->getLabel()
	);	
}
void NovelToVHDLPass::visit_epilog( AndInstruction& value )
{
}


//
// OrInstruction
//

void NovelToVHDLPass::visit_prolog( OrInstruction& value )
{
	TODO;
	assert(0);
}
void NovelToVHDLPass::visit_epilog( OrInstruction& value )
{}


//
// XorInstruction
//

void NovelToVHDLPass::visit_prolog( XorInstruction& value )
{
	TODO;
	assert(0);
}
void NovelToVHDLPass::visit_epilog( XorInstruction& value )
{}





//
// AddSignedInstruction
//

void NovelToVHDLPass::visit_prolog( AddSignedInstruction& value )
{
	fprintf
	( stream
	, "      %s := std_logic_vector( signed( %s ) + signed( %s ) );\n"
	, value.getLabel()
	, value.getLHS()->getLabel()
	, value.getRHS()->getLabel()
	);
}
void NovelToVHDLPass::visit_epilog( AddSignedInstruction& value )
{
}


//
// DivSignedInstruction
//

void NovelToVHDLPass::visit_prolog( DivSignedInstruction& value )
{
	fprintf
	( stream
	, "      %s := std_logic_vector( signed( %s ) / signed( %s ) );\n"
	, value.getLabel()
	, value.getLHS()->getLabel()
	, value.getRHS()->getLabel()
	);	
}
void NovelToVHDLPass::visit_epilog( DivSignedInstruction& value )
{}


//
// ModUnsignedInstruction
//

void NovelToVHDLPass::visit_prolog( ModUnsignedInstruction& value )
{
	TODO;
}
void NovelToVHDLPass::visit_epilog( ModUnsignedInstruction& value )
{}


//
// EquUnsignedInstruction
//

void NovelToVHDLPass::visit_prolog( EquUnsignedInstruction& value )
{
	TODO;
}
void NovelToVHDLPass::visit_epilog( EquUnsignedInstruction& value )
{}


//
// NeqUnsignedInstruction
//

void NovelToVHDLPass::visit_prolog( NeqUnsignedInstruction& value )
{
	TODO;
}
void NovelToVHDLPass::visit_epilog( NeqUnsignedInstruction& value )
{}



//
// ZeroExtendInstruction
//

void NovelToVHDLPass::visit_prolog( ZeroExtendInstruction& value )
{
	TODO;
}
void NovelToVHDLPass::visit_epilog( ZeroExtendInstruction& value )
{}


//
// TruncationInstruction
//

void NovelToVHDLPass::visit_prolog( TruncationInstruction& value )
{
	TODO;
}
void NovelToVHDLPass::visit_epilog( TruncationInstruction& value )
{}



//
// BitConstant
//

void NovelToVHDLPass::visit_prolog( BitConstant& value )
{
	StructureConstant* sc = 0;
	if( value.isBound() )
	{
		sc = value.getBound();
		u1 last = sc->getElements().back() == &value;
		u16 bs = value.getType()->getBitsize();
		std::bitset< 128 > v( value.getValue()[0] );
		const char* fmt = ( bs > 1 ? "\"" : "'" );
		
		fprintf
		( stream
		, "%s%s%s%s"
		, fmt
		, &(v.to_string().c_str()[ 128 - bs ])
		, fmt
		, last ? "" : ", "
		);
	}
	else
	{
		//assert( !" unimplemented !!! " );
		TODO;
	}
}
void NovelToVHDLPass::visit_epilog( BitConstant& value )
{}


//
// StructureConstant
//

void NovelToVHDLPass::visit_prolog( StructureConstant& value )
{
	Module* m = value.getRef<Module>();
	
	if( m->get< Constants >().front() == &value )
	{
		fprintf
		( stream
	    , "-- Constants\n"
	      "library IEEE;\n"
	      "use IEEE.std_logic_1164.all;\n"
	      "use IEEE.numeric_std.all;\n"
		  "use work.Structure.all;\n"
		  "package Constants is\n"
	    );
	}
	
	fprintf
	( stream
	, "  constant %s : %s := ( "
	, value.getLabel()
	, getTypeString( value )
	);
}
void NovelToVHDLPass::visit_epilog( StructureConstant& value )
{
	Module* m = value.getRef<Module>();
	
	fprintf
	( stream
	, ");\n"
	);
	
	if( m->get< Constants >().back() == &value )
	{
		fprintf
		( stream
	    , "end;\n"
		  "\n"
	    );
	}
}


//
// StringConstant
//

void NovelToVHDLPass::visit_prolog( StringConstant& value )
{
	TODO;
}
void NovelToVHDLPass::visit_epilog( StringConstant& value )
{}


//
// Interconnect
//

void NovelToVHDLPass::visit_prolog( Interconnect& value )
{
    TODO;
}
void NovelToVHDLPass::visit_epilog( Interconnect& value )
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

