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


static const char* toString( Type* type )
{
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
	else
	{
		assert( !"unimplemented type to emit" );
	}
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
		assert( Value::isa< Structure >( &value ) );
		return ((Structure*)&value)->getIdentifier()->getName();
	}
	else
	{
		assert( !"unimplemented or unsupported type to convert!" );
	}
}


void NovelToVHDLPass::visit_prolog( Module& value )
{
	fprintf
	( stdout
	, "-- begin of module: '%s'\n"
	  "library IEEE;\n"
	  "use IEEE.std_logic_1164.all;\n"
	  "\n"
	, value.getName()
	);
}
void NovelToVHDLPass::visit_epilog( Module& value )
{
	fprintf( stdout, "-- end of module: '%s'\n\n", value.getName() );		
}


void NovelToVHDLPass::visit_prolog( Component& value )
{
	fprintf
	( stdout
	, "entity %s is port -- Component\n"
	  "( "
	, value.getName()
	);
}
void NovelToVHDLPass::visit_interlog( Component& value )
{
	fprintf
	( stdout
	, "\n"
	  ");\n"
	  "end %s;\n"
	  "architecture \\@%s@\\ of %s is begin\n"
	, value.getName()
	, value.getName()
	, value.getName()
	);
}
void NovelToVHDLPass::visit_epilog( Component& value )
{
	fprintf
	( stdout
	, "end \\@%s@\\;\n\n"
	, value.getName()
	);
}


void NovelToVHDLPass::visit_prolog( Function& value )
{
	fprintf
	( stdout
	, "procedure %s -- Function\n( "
	, value.getName()
	);
}
void NovelToVHDLPass::visit_interlog( Function& value )
{
	fprintf
	( stdout
	, "\n"
	  ") is begin\n"
	);
}
void NovelToVHDLPass::visit_epilog( Function& value )
{
	fprintf
	( stdout
	, "end procedure %s;\n\n"
	, value.getName()
	);
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
	( stdout
	, "%s : %-5s %s%s"
	, value.getIdentifier()->getName()
	, kind
	, toString( value.getType() )
	, ( value.getCallableUnit()->isLastParameter( &value ) ? "" : "\n; " )
	);
}
void NovelToVHDLPass::visit_epilog( Reference& value )
{

}


void NovelToVHDLPass::visit_prolog( Structure& value )
{

}
void NovelToVHDLPass::visit_epilog( Structure& value )
{

}


void NovelToVHDLPass::visit_prolog( Memory& value )
{

}
void NovelToVHDLPass::visit_epilog( Memory& value )
{

}

void NovelToVHDLPass::visit_prolog( ParallelScope& value )
{

}
void NovelToVHDLPass::visit_epilog( ParallelScope& value )		
{

}

void NovelToVHDLPass::visit_prolog( SequentialScope& value )
{

}
void NovelToVHDLPass::visit_epilog( SequentialScope& value )
{

}

void NovelToVHDLPass::visit_prolog( TrivialStatement& value )
{

}
void NovelToVHDLPass::visit_epilog( TrivialStatement& value )
{

}

void NovelToVHDLPass::visit_prolog( CallInstruction& value )
{

}
void NovelToVHDLPass::visit_epilog( CallInstruction& value )	
{

}

void NovelToVHDLPass::visit_prolog( LoadInstruction& value )
{

}
void NovelToVHDLPass::visit_epilog( LoadInstruction& value )		
{

}

void NovelToVHDLPass::visit_prolog( StoreInstruction& value )
{

}
void NovelToVHDLPass::visit_epilog( StoreInstruction& value )		
{

}

void NovelToVHDLPass::visit_prolog( AndInstruction& value )
{

}
void NovelToVHDLPass::visit_epilog( AndInstruction& value )
{

}

void NovelToVHDLPass::visit_prolog( AddSignedInstruction& value )
{

}
void NovelToVHDLPass::visit_epilog( AddSignedInstruction& value )
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

