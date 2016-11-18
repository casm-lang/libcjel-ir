//
//  Copyright (c) 2015-2016 Philipp Paulweber
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                https://github.com/casm-lang/libnovel
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

#include "NovelToLLPass.h"

using namespace libnovel;

char NovelToLLPass::id = 0;

static libpass::PassRegistration< NovelToLLPass > PASS(
    "NOVEL IR to LLVM IR", "generates LLVM IR code out of the NOVEL IR", 0, 0 );

static FILE* stream = stderr;

bool NovelToLLPass::run( libpass::PassResult& pr )
{
    Module* value = (Module*)pr.getResult< NovelDumpPass >();
    assert( value );

    string fn = "obj/" + string( value->getName() ) + ".vhd";
    stream = fopen( fn.c_str(), "w" );

    value->iterate( Traversal::PREORDER, this );

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
        string t = "i" + to_string( type->getBitsize() );
        return t.c_str();
    }
    else if( type->getIDKind() == Type::ID::STRUCTURE )
    {
        assert( Value::isa< Structure >( &value ) );
        return ( (Structure*)&value )->getIdentifier()->getName();
    }
    else
    {
        assert( !"unimplemented or unsupported type to convert!" );
    }
}

void NovelToLLPass::visit_prolog( Module& value )
{
    fprintf( stdout,
        "; ModuleID = '%s'\n"
        ";; begin of module: '%s'\n"
        "\n",
        value.getName(), value.getName() );
}
void NovelToLLPass::visit_epilog( Module& value )
{
    fprintf( stdout,
        ";; end of module: '%s'\n"
        "\n",
        value.getName() );
}

void NovelToLLPass::visit_prolog( Function& value )
{
    fprintf( stdout,
        "define void @%s ;; Function\n"
        "( ",
        value.getName() );
}
void NovelToLLPass::visit_interlog( Function& value )
{
    fprintf( stdout,
        "\n)\n"
        "{\n"
        "begin:\n" );
}
void NovelToLLPass::visit_epilog( Function& value )
{
    fprintf( stdout,
        "  ret void\n"
        "}\n"
        "\n" );
}

void NovelToLLPass::visit_prolog( Intrinsic& value )
{
    fprintf( stdout,
        "define void @%s ;; Intrinsic\n"
        "( ",
        value.getName() );
}
void NovelToLLPass::visit_interlog( Intrinsic& value )
{
    fprintf( stdout, "\n)\n{\nbegin:\n" );
}
void NovelToLLPass::visit_epilog( Intrinsic& value )
{
    fprintf( stdout,
        "  ret void\n"
        "}\n"
        "\n" );
}

void NovelToLLPass::visit_prolog( Reference& value )
{
    fprintf( stdout, "%s %%%s%s",
        "i32" // value.getType()->getName() // TODO: FIXME!!!
        ,
        value.getIdentifier()->getName(),
        ( value.getCallableUnit()->isLastParameter( &value ) ? "" : "\n, " ) );
}
void NovelToLLPass::visit_epilog( Reference& value )
{
}

void NovelToLLPass::visit_prolog( Structure& value )
{
    if( value.getElements().size() == 0 )
    {
        // all bit types can be represented in LLVM IR directly!
        return;
    }

    fprintf( stdout,
        ";; begin of structure: '%s'\n"
        "%%%s = type\n"
        "{ ",
        value.getIdentifier()->getName(), value.getIdentifier()->getName() );

    u16 cnt = 0;
    for( const Structure* s : value.getElements() )
    {
        cnt++;

        fprintf( stdout, "%s%s ;; %s\n%s", getTypeString( *( (Value*)s ) ),
            s->getElements().size() > 0 ? "*" : "",
            s->getIdentifier()->getName(),
            cnt < value.getElements().size() ? ", " : "" );
    }

    fprintf( stdout,
        "}\n"
        ";; end of structure: '%s'\n"
        "\n",
        value.getIdentifier()->getName() );
}
void NovelToLLPass::visit_epilog( Structure& value )
{
}

void NovelToLLPass::visit_prolog( Variable& value )
{
    TODO;
}
void NovelToLLPass::visit_epilog( Variable& value )
{
}

void NovelToLLPass::visit_prolog( Memory& value )
{
    TODO;
}
void NovelToLLPass::visit_epilog( Memory& value )
{
}

void NovelToLLPass::visit_prolog( ParallelScope& value )
{
    TODO;
}
void NovelToLLPass::visit_epilog( ParallelScope& value )
{
}

void NovelToLLPass::visit_prolog( SequentialScope& value )
{
    TODO;
}
void NovelToLLPass::visit_epilog( SequentialScope& value )
{
}

void NovelToLLPass::visit_prolog( TrivialStatement& value )
{
    TODO;
}
void NovelToLLPass::visit_epilog( TrivialStatement& value )
{
}

void NovelToLLPass::visit_prolog( BranchStatement& value )
{
    TODO;
}
void NovelToLLPass::visit_interlog( BranchStatement& value )
{
}
void NovelToLLPass::visit_epilog( BranchStatement& value )
{
}

void NovelToLLPass::visit_prolog( LoopStatement& value )
{
    TODO;
}
void NovelToLLPass::visit_interlog( LoopStatement& value )
{
}
void NovelToLLPass::visit_epilog( LoopStatement& value )
{
}

void NovelToLLPass::visit_prolog( CallInstruction& value )
{
    TODO;
}
void NovelToLLPass::visit_epilog( CallInstruction& value )
{
}

//
// IdCallInstruction
//

void NovelToLLPass::visit_prolog( IdCallInstruction& value )
{
    TODO;
    assert( 0 );
}
void NovelToLLPass::visit_epilog( IdCallInstruction& value )
{
}

//
// StreamInstruction
//

void NovelToLLPass::visit_prolog( StreamInstruction& value )
{
    TODO;
}
void NovelToLLPass::visit_epilog( StreamInstruction& value )
{
}

void NovelToLLPass::visit_prolog( NopInstruction& value )
{
    TODO;
    fprintf( stream, "    ;; nop\n" );
}
void NovelToLLPass::visit_epilog( NopInstruction& value )
{
}

void NovelToLLPass::visit_prolog( AllocInstruction& value )
{
    TODO;
}
void NovelToLLPass::visit_epilog( AllocInstruction& value )
{
}

void NovelToLLPass::visit_prolog( IdInstruction& value )
{
    TODO;
}
void NovelToLLPass::visit_epilog( IdInstruction& value )
{
}

//
// CastInstruction
//

void NovelToLLPass::visit_prolog( CastInstruction& value )
{
    TODO;
}
void NovelToLLPass::visit_epilog( CastInstruction& value )
{
}

void NovelToLLPass::visit_prolog( ExtractInstruction& value )
{
    TODO;
}
void NovelToLLPass::visit_epilog( ExtractInstruction& value )
{
}

void NovelToLLPass::visit_prolog( LoadInstruction& value )
{
    TODO;
}
void NovelToLLPass::visit_epilog( LoadInstruction& value )
{
}

void NovelToLLPass::visit_prolog( StoreInstruction& value )
{
    TODO;
}
void NovelToLLPass::visit_epilog( StoreInstruction& value )
{
}

//
// NotInstruction
//

void NovelToLLPass::visit_prolog( NotInstruction& value )
{
    TODO;
    assert( 0 );
}
void NovelToLLPass::visit_epilog( NotInstruction& value )
{
}

//
// AndInstruction
//

void NovelToLLPass::visit_prolog( AndInstruction& value )
{
    TODO;
    assert( 0 );
}
void NovelToLLPass::visit_epilog( AndInstruction& value )
{
}

//
// OrInstruction
//

void NovelToLLPass::visit_prolog( OrInstruction& value )
{
    TODO;
    assert( 0 );
}
void NovelToLLPass::visit_epilog( OrInstruction& value )
{
}

//
// XorInstruction
//

void NovelToLLPass::visit_prolog( XorInstruction& value )
{
    TODO;
    assert( 0 );
}
void NovelToLLPass::visit_epilog( XorInstruction& value )
{
}

void NovelToLLPass::visit_prolog( AddSignedInstruction& value )
{
    TODO;
}
void NovelToLLPass::visit_epilog( AddSignedInstruction& value )
{
}

void NovelToLLPass::visit_prolog( DivSignedInstruction& value )
{
    TODO;
}
void NovelToLLPass::visit_epilog( DivSignedInstruction& value )
{
}

//
// ModUnsignedInstruction
//

void NovelToLLPass::visit_prolog( ModUnsignedInstruction& value )
{
    TODO;
}
void NovelToLLPass::visit_epilog( ModUnsignedInstruction& value )
{
}

//
// EquUnsignedInstruction
//

void NovelToLLPass::visit_prolog( EquUnsignedInstruction& value )
{
    TODO;
}
void NovelToLLPass::visit_epilog( EquUnsignedInstruction& value )
{
}

//
// NeqUnsignedInstruction
//

void NovelToLLPass::visit_prolog( NeqUnsignedInstruction& value )
{
    TODO;
}
void NovelToLLPass::visit_epilog( NeqUnsignedInstruction& value )
{
}

//
// ZeroExtendInstruction
//

void NovelToLLPass::visit_prolog( ZeroExtendInstruction& value )
{
    TODO;
}
void NovelToLLPass::visit_epilog( ZeroExtendInstruction& value )
{
}

//
// TruncationInstruction
//

void NovelToLLPass::visit_prolog( TruncationInstruction& value )
{
    TODO;
}
void NovelToLLPass::visit_epilog( TruncationInstruction& value )
{
}

void NovelToLLPass::visit_prolog( BitConstant& value )
{
    TODO;
}
void NovelToLLPass::visit_epilog( BitConstant& value )
{
}

void NovelToLLPass::visit_prolog( StructureConstant& value )
{
    TODO;
}
void NovelToLLPass::visit_epilog( StructureConstant& value )
{
}

//
// StringConstant
//

void NovelToLLPass::visit_prolog( StringConstant& value )
{
    TODO;
}
void NovelToLLPass::visit_epilog( StringConstant& value )
{
}

//
// Interconnect
//

void NovelToLLPass::visit_prolog( Interconnect& value )
{
    TODO;
}
void NovelToLLPass::visit_epilog( Interconnect& value )
{
}

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
