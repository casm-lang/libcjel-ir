//  
//  Copyright (c) 2016 Philipp Paulweber
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

static FILE* stream = stdout;


bool NovelToVHDLPass::run( libpass::PassResult& pr )
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
		if( type->getBitsize() == 1 )
		{
			return "std_logic";
		}
		else if( type->getBitsize() > 1 )
		{
			string x
				= "std_logic_vector( "
				+ to_string( type->getBitsize() - 1 )
				+ " downto 0 )";
			return x.c_str();
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
		return ((Structure*)ty)->getName();
	}
	else
	{
		assert( !"unimplemented or unsupported type to convert!" );
	}
}


void NovelToVHDLPass::visit_prolog( Module& value )
{
	fprintf
	( stream
	, "-- begin of module: '%s'\n"
	  // "library IEEE;\n"
	  // "use IEEE.std_logic_1164.all;\n"
	  "\n"
	, value.getName()
	);
}
void NovelToVHDLPass::visit_epilog( Module& value )
{
	fprintf( stream, "-- end of module: '%s'\n\n", value.getName() );		
}

static void emit_wire( Value& value )
{
	assert( Value::isa< CallableUnit >( &value ) );

	Block* context = ((CallableUnit*)(&value))->getContext();
	assert( context );
	
	if( Value::isa< Scope >( context ) )
	{
		for( auto block : ((Scope*)context)->getBlocks() )
		{
			fprintf
			( stream
			, "  signal %s : std_logic;\n"
			, block->getLabel()
			);
		}
	}
}


static void emit_req( Value& value )
{
	fprintf
	( stream
	, "  -- req %s\n"
	  "  process( req_%s ) is begin\n"
	  "    if rising_edge( req_%s ) then\n"
	, value.getLabel()
	, value.getLabel()
	, value.getLabel()
	);
	
	// Block* context = 0;
	
	// if( Value::isa< CallableUnit >( &value ) )
	// {
	// 	context = ((CallableUnit*)(&value))->getContext();
	// }
	// assert( context );
	
	if( Value::isa< Scope >( &value ) )
	{
		for( auto block : ((Scope*)&value)->getBlocks() )
		{
			fprintf
			( stream
			, "      " // TODO: FIXME: dynamic indention calculation 
			  "%s <= transport '1' after 5ns\n"
			, block->getLabel()
			);
		}
		if( ((Scope*)&value)->getBlocks().size() == 0 )
		{
			fprintf
			( stream
			, "      " // TODO: FIXME: dynamic indention calculation 
			  "null; -- EMPTY SCOPE!\n"
			);
		}
	}
	
	fprintf
	( stream
	, "    end if;\n"
	  "  end process;\n"
	  "\n"
	);	
}

static void emit_ack( Value& value )
{
	fprintf
	( stream
	, "  -- ack %s\n"
	  "  ack_%s <= transport ( "
	, value.getLabel()
	, value.getLabel()
	);
	
	// Block* context = 0;
	
	// if( Value::isa< CallableUnit >( &value ) )
	// {
	// 	context = ((CallableUnit*)(&value))->getContext();
	// }
	// assert( context );
	
	if( Value::isa< Scope >( &value ) )
	{
		u1 first = true;
		for( auto block : ((Scope*)&value)->getBlocks() )
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
		if( ((Scope*)&value)->getBlocks().size() == 0 )
		{
			fprintf
			( stream
			, "'1'"
			);
		}
	}
	
	fprintf
	( stream
	, " ) after 5ns;\n"
	);
}


void NovelToVHDLPass::visit_prolog( Component& value )
{
	fprintf
	( stream
	, "-- Component '%s'\n"
	  "library IEEE;\n"
	  "use IEEE.std_logic_1164.all;\n"
	  "entity %s is port\n"
	  "( "
	, value.getLabel()
	, value.getName()
	);
}
void NovelToVHDLPass::visit_interlog( Component& value )
{
	fprintf
	( stream
	, "\n"
	  "; req : in std_logic\n"
	  "; ack : in std_logic\n"
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
void NovelToVHDLPass::visit_epilog( Component& value )
{
	fprintf
	( stream
	, "end \\@%s@\\;\n\n"
	, value.getName()
	);
}


void NovelToVHDLPass::visit_prolog( Function& value )
{
	visit_prolog( *((Component*)(&value)) );
	// fprintf
	// ( stream
	// , "procedure %s -- Function\n( "
	// , value.getName()
	// );
}
void NovelToVHDLPass::visit_interlog( Function& value )
{
	visit_interlog( *((Component*)(&value)) );
	// fprintf
	// ( stream
	// , "\n"
	//   ") is begin\n"
	// );
}
void NovelToVHDLPass::visit_epilog( Function& value )
{
	visit_epilog( *((Component*)(&value)) );
	// fprintf
	// ( stream
	// , "end procedure %s;\n\n"
	// , value.getName()
	// );
}


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
	, "%s : %-5s %s%s"
	, value.getIdentifier()->getName()
	, kind
	, getTypeString( value )
	, ( value.getCallableUnit()->isLastParameter( &value ) ? "" : "\n; " )
	);
}
void NovelToVHDLPass::visit_epilog( Reference& value )
{
	
}


void NovelToVHDLPass::visit_prolog( Structure& value )
{
	TODO;
}
void NovelToVHDLPass::visit_epilog( Structure& value )
{

}


void NovelToVHDLPass::visit_prolog( Variable& value )
{
	TODO;
}
void NovelToVHDLPass::visit_epilog( Variable& value )
{
}

void NovelToVHDLPass::visit_prolog( Memory& value )
{
	TODO;
}
void NovelToVHDLPass::visit_epilog( Memory& value )
{
}

void NovelToVHDLPass::visit_prolog( ParallelScope& value )
{
	emit_req( value );
}
void NovelToVHDLPass::visit_epilog( ParallelScope& value )		
{
	emit_ack( value );
}

void NovelToVHDLPass::visit_prolog( SequentialScope& value )
{
	emit_req( value );
}
void NovelToVHDLPass::visit_epilog( SequentialScope& value )
{
	emit_ack( value );
}

void NovelToVHDLPass::visit_prolog( TrivialStatement& value )
{
	TODO;
}
void NovelToVHDLPass::visit_epilog( TrivialStatement& value )
{
}

void NovelToVHDLPass::visit_prolog( CallInstruction& value )
{
	TODO;
}
void NovelToVHDLPass::visit_epilog( CallInstruction& value )	
{
}

void NovelToVHDLPass::visit_prolog( IdInstruction& value )
{
	TODO;
}
void NovelToVHDLPass::visit_epilog( IdInstruction& value )		
{}

void NovelToVHDLPass::visit_prolog( ExtractInstruction& value )
{
	TODO;
}
void NovelToVHDLPass::visit_epilog( ExtractInstruction& value )
{}

void NovelToVHDLPass::visit_prolog( LoadInstruction& value )
{
	TODO;
}
void NovelToVHDLPass::visit_epilog( LoadInstruction& value )		
{
}

void NovelToVHDLPass::visit_prolog( StoreInstruction& value )
{
	TODO;
}
void NovelToVHDLPass::visit_epilog( StoreInstruction& value )		
{
}

void NovelToVHDLPass::visit_prolog( AndInstruction& value )
{
	TODO;
}
void NovelToVHDLPass::visit_epilog( AndInstruction& value )
{
}

void NovelToVHDLPass::visit_prolog( AddSignedInstruction& value )
{
	TODO;
}
void NovelToVHDLPass::visit_epilog( AddSignedInstruction& value )
{
}

void NovelToVHDLPass::visit_prolog( DivSignedInstruction& value )
{
	TODO;	
}
void NovelToVHDLPass::visit_epilog( DivSignedInstruction& value )
{}


void NovelToVHDLPass::visit_prolog( BitConstant& value )
{
	TODO;
}
void NovelToVHDLPass::visit_epilog( BitConstant& value )
{
}

void NovelToVHDLPass::visit_prolog( StructureConstant& value )
{
	TODO;
}
void NovelToVHDLPass::visit_epilog( StructureConstant& value )
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

