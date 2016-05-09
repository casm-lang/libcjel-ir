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

static Module* module = 0;

static u1 instruction_implementation = false;

bool NovelToVHDLPass::run( libpass::PassResult& pr )
{
    Module* value = (Module*)pr.getResult< NovelDumpPass >();
	assert( value );
	module = value;
	
	string fn = "obj/" + string( value->getName() ) + ".vhd"; 
	stream = fopen( fn.c_str(), "w" );
	
	value->iterate
	( Traversal::PREORDER
	, this
	);

	instruction_implementation = true;
	
	value->iterate
	( Traversal::PREORDER
	, [ this ]( Value* value )
	  {
		  if( Value::isa< Instruction >( value ) )
		  {
			  this->dispatch( Visitor::Stage::PROLOG, value );
		  }
	  }
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
	else if( type->getIDKind() == Type::INTERCONNECT )
	{
	    return libstdhl::Allocator::string( "Interconnect" );
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
	    , "      -- %s %s -- linkage\n"
		, linkage->getIdentifier()->getName()
	    , linkage->getLabel()
	    );	
	    continue;
    
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
	  "; "
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
	, "\n"
	  ");\n"
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
	  "; "
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
 	const char* kind = "link";
	if( value.isInput() )
	{
		kind = "in";
	}
	else if( value.isOutput() )
	{
		kind = "out";
	}
	else
	{
		assert(0);
	}
	
	fprintf
	( stream
	, "%s : %s %s -- %s %s%s"
	, value.getLabel()
	, kind
	, getTypeString( value )
	, value.getIdentifier()->getName()
	, kind
	, ( value.getCallableUnit()->isLastParameter( &value ) ? "" : "\n; " )
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

	std::bitset< 48 > v( value.getAllocationID()->getValue()[0] );
	
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
	fprintf
	( stream
	, "  stmt_%s: block -- stmt\n"
	  "    signal sig_%s : std_logic := '0';\n"
	, value.getLabel()
	, value.getLabel()
	);
	
	for( Value* instr : value.getInstructions() )
	{
		if( Value::isa< AllocInstruction >( instr ) )
		{
		    fprintf
		    ( stream
	        , "    signal     %s : %s; -- %s\n"
		    , instr->getLabel()
		    , getTypeString( *instr )
		    , instr->getName()
		    );
		}
		
		if
		(   not Value::isa< AllocInstruction  >( instr )
		and not Value::isa< StoreInstruction  >( instr )
		and not Value::isa< CallInstruction   >( instr )
		and not Value::isa< IdCallInstruction >( instr )
		)
		{
		    fprintf
		    ( stream
	        , "    signal sig_%s : std_logic := '0'; -- %s\n"
		    , instr->getLabel()
		    , instr->getName()
		    );
			
			fprintf
		    ( stream
	        , "    signal     %s : %s; -- %s\n"
		    , instr->getLabel()
		    , getTypeString( *instr )
		    , instr->getName()
		    );
		}
	}
	
	Value* v = value.getInstructions().front();
	while( Value::isa< AllocInstruction >( v ) )
	{
		v = v->getNext();
	}
	
	fprintf
	( stream
	, "  begin\n"
	  "    process( req_%s ) is\n"
	  "    begin\n"
	  "      if rising_edge( req_%s ) then\n"
	  "        sig_%s <= transport '1' after 5 ns;\n"
	  "      end if;\n"
	  "    end process;\n"
	, value.getLabel()
	, value.getLabel()
	, v->getLabel()
	);
}
void NovelToVHDLPass::visit_epilog( TrivialStatement& value )
{
	fprintf
	( stream
	, "    process( sig_%s ) is\n"
	  "    begin\n"
	  "      if rising_edge( sig_%s ) then\n"
	  "        ack_%s <= transport '1' after 5 ns;\n"
	  "      end if;\n"
	  "    end process;\n"
	  "  end block;\n"
	  "\n"
	, value.getLabel()
	, value.getLabel()
	, value.getLabel()
	);
}


//
// BranchStatement
//

void NovelToVHDLPass::visit_prolog( BranchStatement& value )
{
	fprintf
	( stream
	, "  \n"
	  "  -- BRANCH STATEMENT BEGIN;\n"
	  "\n"
	);
	
 	TODO;
}
void NovelToVHDLPass::visit_interlog( BranchStatement& value )
{}
void NovelToVHDLPass::visit_epilog( BranchStatement& value )
{
	fprintf
	( stream
	, "  \n"
	  "  -- BRANCH STATEMENT END;\n"
	  "\n"
	);
}


//
// LoopStatement
//

void NovelToVHDLPass::visit_prolog( LoopStatement& value )
{
	fprintf
	( stream
	, "  \n"
	  "  -- LOOP STATEMENT BEGIN;\n"
	  "\n"
	);

	TODO;
}
void NovelToVHDLPass::visit_interlog( LoopStatement& value )
{}
void NovelToVHDLPass::visit_epilog( LoopStatement& value )
{
	fprintf
	( stream
	, "  \n"
	  "  -- LOOP STATEMENT END;\n"
	  "\n"
	);
}


//
// CallInstruction
//

void NovelToVHDLPass::visit_prolog( CallInstruction& value )
{
	fprintf
	( stream
	, "    call_%s : entity work.%s port map( sig_%s, sig_%s"
	, value.getLabel()
	, value.getValue(0)->getName()
	, value.getLabel()
	, value.getNext() != 0 ? value.getNext()->getLabel() : value.getStatement()->getLabel()
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
	, value.getValues().size() - 1
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
    fprintf
	( stream
	, "      -- %s = %s %s() -- id call\n"
	, value.getLabel()
	, value.getValue(0)->getLabel()
	, value.getValue(1)->getLabel()
	);	
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
	, "      null; -- %s\n"
	, value.getName()
	);
}
void NovelToVHDLPass::visit_epilog( NopInstruction& value )
{}


//
// AllocInstruction
//

void NovelToVHDLPass::visit_prolog( AllocInstruction& value )
{}
void NovelToVHDLPass::visit_epilog( AllocInstruction& value )
{}






static void instr_unary( UnaryInstruction& value )
{
	fprintf
	( stream
	, "    iuna_%s : entity work.%s port map( sig_%s, sig_%s, %s, %s ); -- %s\n"
	, value.getLabel()
	, &value.getName()[1]
	, value.getLabel()
	, value.getNext() != 0 ? value.getNext()->getLabel() : value.getStatement()->getLabel()
	, value.getLabel()
	, value.get()->getLabel()
	, &value.getName()[1]
	);
}

static void instr_binary( BinaryInstruction& value )
{
	fprintf
	( stream
	, "    ibin_%s : entity work.%s port map( sig_%s, sig_%s, %s, %s, %s ); -- %s\n"
	, value.getLabel()
	, &value.getName()[1]
	, value.getLabel()
	, value.getNext() != 0 ? value.getNext()->getLabel() : value.getStatement()->getLabel()
	, value.getLabel()
	, value.getLHS()->getLabel()
	, value.getRHS()->getLabel()
	, &value.getName()[1]
	);
}


//
// IdInstruction
//

void NovelToVHDLPass::visit_prolog( IdInstruction& value )
{
	// if( Value::isa< Variable >( value.get() ) )
	// {
	// 	fprintf
	// 	( stream
	// 	, "      %s := %s_id;\n "
	// 	, value.getLabel()
	// 	, value.get()->getLabel()
	// 	);
	// }
	// else
	// {
	    fprintf
		( stream
		, "    -- %s = %s -- id\n"
		, value.getLabel()
		, value.get()->getLabel()
		);	
		//}
}
void NovelToVHDLPass::visit_epilog( IdInstruction& value )		
{}


//
// CastInstruction
//

void NovelToVHDLPass::visit_prolog( CastInstruction& value )
{
	instr_binary( value );	
}
void NovelToVHDLPass::visit_epilog( CastInstruction& value )
{}




//
// ExtractInstruction
//

void NovelToVHDLPass::visit_prolog( ExtractInstruction& value )
{
	instr_binary( value );
}
void NovelToVHDLPass::visit_epilog( ExtractInstruction& value )
{
}


//
// LoadInstruction
//

void NovelToVHDLPass::visit_prolog( LoadInstruction& value )
{
	fprintf
	( stream
	, "    load_%s : entity work.%s port map( sig_%s, sig_%s, %s, %s ); -- %s\n"
	, value.getLabel()
	, &value.getName()[1]
	, value.getLabel()
	, value.getNext() != 0 ? value.getNext()->getLabel() : value.getStatement()->getLabel()
	, value.getLabel()
	, value.get()->getLabel()
	, &value.getName()[1]
	);

	return;
	
	// assert( Value::isa< ExtractInstruction >( value.get() ) );
	// ExtractInstruction* ext = (ExtractInstruction*)( value.get() );
	
	// assert( Value::isa< Reference >( ext->getLHS() ) );
	// Reference* ref = (Reference*)( ext->getLHS() );

	// assert( Value::isa< Structure >( ext->getRHS() ) );
	// Structure* str = (Structure*)( ext->getRHS() );

	// assert( str->getParent() == ref->getStructure() );
	
	// fprintf
	// ( stream
	// , "      %s := %s.%s;\n"
	// , value.getLabel()
	// , ref->getIdentifier()->getName()
	// , str->getName()
	// );
	
	// // fprintf
	// // ( stream
	// // , "      %s := %s;\n"
	// // , value.getLabel()
	// //   , value.get()->getLabel()
	// // );
}
void NovelToVHDLPass::visit_epilog( LoadInstruction& value )		
{
}


//
// StoreInstruction
//

void NovelToVHDLPass::visit_prolog( StoreInstruction& value )
{
	// Value* dst = value.getRHS();
	// Value* src = value.getLHS();
	
	fprintf
	( stream
	, "    stor_%s : entity work.%s port map( sig_%s, sig_%s, %s, %s ); -- %s\n"
	, value.getLabel()
	, &value.getName()[1]
	, value.getLabel()
	, value.getNext() != 0 ? value.getNext()->getLabel() : value.getStatement()->getLabel()
	, value.getLHS()->getLabel()
	, value.getRHS()->getLabel()
	, &value.getName()[1]
	);
	
	return;
	
	// if( Value::isa< ExtractInstruction >( dst ) )
	// {
	// 	ExtractInstruction* ext = (ExtractInstruction*)( dst );
	// 	assert( ext );
		
	// 	assert( Value::isa< Reference >( ext->getLHS() ) );
	// 	Reference* ref = (Reference*)( ext->getLHS() );
		
	// 	assert( Value::isa< Structure >( ext->getRHS() ) );
	// 	Structure* str = (Structure*)( ext->getRHS() );
		
	// 	assert( str->getParent() == ref->getStructure() );
		
	// 	fprintf
	// 	( stream
	//     , "      %s.%s <= transport %s after 20 ns; -- store '%s'\n"
	//     , ref->getIdentifier()->getName()
	//     , str->getName()
	//     , src->getLabel()
	// 	, ref->getIdentifier()->getName()
	//     );
	// }
	// else if( Value::isa< Reference >( dst ) )
	// {
	// 	Reference* ref = (Reference*)( dst );
	// 	assert( ref );
		
	// 	fprintf
	// 	( stream
	//     , "     %s <= transport %s after 20 ns; -- store '%s'\n"
	//     , ref->getLabel()
	//     , src->getLabel()
	//     , ref->getIdentifier()->getName()
	//     );
	// }
	// else
	// {
	// 	TODO;
	// 	assert( !" unimplemented! " );
	// }
}
void NovelToVHDLPass::visit_epilog( StoreInstruction& value )		
{
}




//
// NotInstruction
//

void NovelToVHDLPass::visit_prolog( NotInstruction& value )
{
	instr_unary( value );

	// fprintf
	// ( stream
	// , "      %s <= not %s;\n"
	// , value.getLabel()
	// , value.get()->getLabel()
	// );	
}
void NovelToVHDLPass::visit_epilog( NotInstruction& value )
{}


//
// AndInstruction
//

void NovelToVHDLPass::visit_prolog( AndInstruction& value )
{
	instr_binary( value );

	// fprintf
	// ( stream
	// , "      %s := %s and %s;\n"
	// , value.getLabel()
	// , value.getLHS()->getLabel()
	// , value.getRHS()->getLabel()
	// );
}
void NovelToVHDLPass::visit_epilog( AndInstruction& value )
{
}


//
// OrInstruction
//

void NovelToVHDLPass::visit_prolog( OrInstruction& value )
{
	instr_binary( value );
	
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
	instr_binary( value );

	// fprintf
	// ( stream
	// , "      %s <= %s xor %s; -- xor\n"
	// , value.getLabel()
	// , value.getLHS()->getLabel()
	// , value.getRHS()->getLabel()
	// );
}
void NovelToVHDLPass::visit_epilog( XorInstruction& value )
{}





//
// AddSignedInstruction
//

void NovelToVHDLPass::visit_prolog( AddSignedInstruction& value )
{
	instr_binary( value );
	
	// fprintf
	// ( stream
	// , "      %s := std_logic_vector( signed( %s ) + signed( %s ) ); -- adds\n"
	// , value.getLabel()
	// , value.getLHS()->getLabel()
	// , value.getRHS()->getLabel()
	// );
}
void NovelToVHDLPass::visit_epilog( AddSignedInstruction& value )
{
}


//
// DivSignedInstruction
//

void NovelToVHDLPass::visit_prolog( DivSignedInstruction& value )
{
	instr_binary( value );

	// fprintf
	// ( stream
	// , "      %s := std_logic_vector( signed( %s ) / signed( %s ) ); -- divu\n"
	// , value.getLabel()
	// , value.getLHS()->getLabel()
	// , value.getRHS()->getLabel()
	// );
}
void NovelToVHDLPass::visit_epilog( DivSignedInstruction& value )
{}


//
// ModUnsignedInstruction
//

void NovelToVHDLPass::visit_prolog( ModUnsignedInstruction& value )
{
	instr_binary( value );

	// fprintf
	// ( stream
	// , "      %s := std_logic_vector( unsigned( %s ) mod unsigned( %s ) ); -- modu\n"
	// , value.getLabel()
	// , value.getLHS()->getLabel()
	// , value.getRHS()->getLabel()
	// );
}
void NovelToVHDLPass::visit_epilog( ModUnsignedInstruction& value )
{}


//
// EquUnsignedInstruction
//

void NovelToVHDLPass::visit_prolog( EquUnsignedInstruction& value )
{
	instr_binary( value );
	TODO;
}
void NovelToVHDLPass::visit_epilog( EquUnsignedInstruction& value )
{}


//
// NeqUnsignedInstruction
//

void NovelToVHDLPass::visit_prolog( NeqUnsignedInstruction& value )
{
	instr_binary( value );
	TODO;
}
void NovelToVHDLPass::visit_epilog( NeqUnsignedInstruction& value )
{}



//
// ZeroExtendInstruction
//

void NovelToVHDLPass::visit_prolog( ZeroExtendInstruction& value )
{
	static u1 emitted = false;
	if( instruction_implementation )
	{
		if( not emitted )
		{
			// const char* name = &value.getName()[1];
			
			// fprintf
	        // ( stream
	        // , "-- Instruction '%s'\n"
	        //   "library IEEE;\n"
	        //   "use IEEE.std_logic_1164.all;\n"
	        //   "use IEEE.numeric_std.all;\n"
	        //   "use work.Structure.all;\n"
	        //   "use work.Constants.all;\n"
	        //   "use work.Variables.all;\n"
	        //   "entity %s is port\n"
	        //   "( req : in  std_logic\n"
	        //   "; ack : out std_logic\n"
	        //   "; res : out %s\n"
			//   "\n"
			//   ");\n"
			//   "end %s;\n"
			//   "architecture \\@%s@\\ of %s is\n"
			//   "begin\n"
			//   "end \\@%s@\\;\n"
			//   "\n"
			// , name
			// , name
			//   , 
			// );
			
		}
		return;
	}
	
	instr_unary( value );
	
	// u16 bs = value.getType()->getBitsize() - value.get()->getType()->getBitsize();
    // std::bitset< 256 > v( 0 );
	// const char* bits = &(v.to_string().c_str()[ 256 - bs ]);
	// const char* fmt = ( bs > 1 ? "\"" : "'" );
	
	// fprintf
	// ( stream
	// , "      %s := %s%s%s & %s; -- zext\n"
	// , value.getLabel()
	// , fmt
	// , bits
	// , fmt
	// , value.get()->getLabel()
	// );
}
void NovelToVHDLPass::visit_epilog( ZeroExtendInstruction& value )
{}


//
// TruncationInstruction
//

void NovelToVHDLPass::visit_prolog( TruncationInstruction& value )
{
	instr_unary( value );
	
	// fprintf
	// ( stream
	// , "      %s := %s( %lu downto 0 ); -- trunc\n"
	// , value.getLabel()
	// , value.get()->getLabel()
	// , (u64)(value.getType()->getBitsize() - 1)
	// );
}
void NovelToVHDLPass::visit_epilog( TruncationInstruction& value )
{}



//
// BitConstant
//

void NovelToVHDLPass::visit_prolog( BitConstant& value )
{
	if( module->get< Constants >().front() == &value )
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

	u16 bs = value.getType()->getBitsize();
	std::bitset< 256 > v( value.getValue()[0] );
	const char* bits = &(v.to_string().c_str()[ 256 - bs ]);	
	const char* fmt = ( bs > 1 ? "\"" : "'" );
	
	StructureConstant* sc = 0;
	if( value.isBound() )
	{
		sc = value.getBound();
		u1 last = sc->getElements().back() == &value;
		
		fprintf
		( stream
		, "%s%s%s%s"
		, fmt
		, bits
		, fmt
		, last ? "" : ", "
		);
	}
	else
	{
		fprintf
	    ( stream
	    , "  constant %s : %s := %s%s%s;\n"
	    , value.getLabel()
	    , getTypeString( value )
		, fmt
		, bits
		, fmt
	    );
	}
}
void NovelToVHDLPass::visit_epilog( BitConstant& value )
{
	if( module->get< Constants >().back() == &value )
	{
		fprintf
		( stream
	    , "end;\n"
		  "\n"
	    );
	}
}


//
// StructureConstant
//

void NovelToVHDLPass::visit_prolog( StructureConstant& value )
{
	if( module->get< Constants >().front() == &value )
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
	fprintf
	( stream
	, ");\n"
	);
	
	if( module->get< Constants >().back() == &value )
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
	static Value n( "", &TypeId, libnovel::Value::VALUE );
	
    fprintf
	( stream
	, "-- Interconnect '%s'\n"
	  "library IEEE;\n"
	  "use IEEE.std_logic_1164.all;\n"
	  "use IEEE.numeric_std.all;\n"
	  "use work.Structure.all;\n"
	  "use work.Constants.all;\n"
	  "use work.Variables.all;\n"
	  "entity %s is port\n"
	  "( req  : in  std_logic\n"
	  "; ack  : out std_logic\n"
	  "; addr : in  %s\n"
	  "; data : out std_logic_vector( 64 downto 0 ) -- TBD\n"
	, value.getLabel()
	, value.getLabel()
	, getTypeString( n )
	);
	
	for( auto v : value.getObjects() )
	{
	    fprintf
	    ( stream
	    , "; %s : in %s\n"
	    , v->getLabel()
		, getTypeString( *v )
	    );
	}
	
	fprintf
	( stream
	, ");\n"
	  "end %s;\n"
	  "architecture \\@%s@\\ of %s is\n"
	  "begin\n"
	  "  process( req ) is\n"
	  "  begin\n"
	  "    if rising_edge( req ) then\n"
	  "      case addr is\n"
	, value.getLabel()
	, value.getLabel()
	, value.getLabel()
	);
	
	for( auto v : value.getObjects() )
	{
		assert( Value::isa< Variable >( v ) );
		Variable* var = (Variable*)v;
		
		Value* ty = var->getType()->getBound();
		assert(  Value::isa< Structure >( ty ) );
		Structure* sty = (Structure*)ty;
		
		std::bitset< 48 > bits( var->getAllocationID()->getValue()[0] );
		
		// TODO: FIXME: PPA: HACK: this needs to be generic in the future!!! 
	    fprintf
	    ( stream
	    , "        when \"%s\" => data <= "
		, bits.to_string().c_str()
	    );

		u64 padding = 0;
		for( auto e : sty->getElements() )
		{
			padding += e->getType()->getBitsize();
	    }
		
		u1 first = true;
		
		if( padding < 65 )
		{
			std::bitset< 256 > bits( 0 );
			
	        fprintf
	        ( stream
	        , "\"%s\""
			, &bits.to_string().c_str()[ 256 - (65-padding) ]
	        );
			
			first = false;
		}
		
		for( auto e : sty->getElements() )
		{
	        fprintf
	        ( stream
	        , "%s%s.%s"
			, ( first ? "" : " & " )
			, var->getLabel()
			, e->getName()
	        );
			
			if( first )
			{
				first = false;
			}
		}

		fprintf
	    ( stream
	    , ";\n"
	    );
	}
	
	fprintf
	( stream
	, "        when others => data <= ( others => 'U' );\n"
	  "      end case;\n"
	  "      ack <= transport '1' after 5 ns;\n"
	  "    end if;\n"
	  "  end process;\n"
	  "end \\@%s@\\;\n"
	  "\n"
	, value.getLabel()
	);
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

