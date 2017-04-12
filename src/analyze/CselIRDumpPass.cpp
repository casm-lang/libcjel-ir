//
//  Copyright (c) 2015-2017 Philipp Paulweber
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                https://github.com/casm-lang/libcsel-ir
//
//  This file is part of libcsel-ir.
//
//  libcsel-ir is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  libcsel-ir is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with libcsel-ir. If not, see <http://www.gnu.org/licenses/>.
//

#include "CselIRDumpPass.h"

#include "libcsel-ir.h"

using namespace libcsel_ir;

char CselIRDumpPass::id = 0;

static libpass::PassRegistration< CselIRDumpPass > PASS( "CSEL IR Dumping Pass",
    "generates ASCII representation of the CSEL IR", "el-dump-debug", 0 );

bool CselIRDumpPass::run( libpass::PassResult& pr )
{
    auto data = pr.result< CselIRDumpPass >();
    assert( data );

    try
    {
        // data->module()->iterate( Traversal::PREORDER, this );
        data->module()->iterate( Traversal::PREORDER, this );
    }
    catch( ... )
    {
        fprintf( stderr, "unsuccessful EL dump\n" );
        return false;
    }

    // libstdhl::Log::info( "%p: %s", tmp.c_str() );

    return true;
}

std::string CselIRDumpPass::indention( Value& value )
{
    std::string ind = "";
    u8 cnt = 0;
    Value* p = ( &value );
    while( p != 0 )
    {
        if( isa< Block >( p ) )
        {
            p = static_cast< Block* >( p )->parent().get();
        }
        else if( isa< Instruction >( p ) )
        {
            p = static_cast< Instruction* >( p )->statement().get();
        }
        else
        {
            break;
            // assert(0);
        }

        if( not p or isa< CallableUnit >( p ) )
        {
            break;
        }

        cnt++;
        ind += "  ";
    }

    return ind;
}

#define DUMP_PREFIX                                                            \
    printf( "%-14s: %p, %s, %s%s ", __FUNCTION__, &value,                      \
        value.label().c_str(), indention( value ).c_str(),                     \
        value.name().c_str() )
#define DUMP_POSTFIX printf( "\n" );

#define DUMP_INSTR                                                             \
    for( auto operand : value.operands() )                                     \
    {                                                                          \
        printf( ", %s (%s)", operand->label().c_str(),                         \
            operand->type().name().c_str() );                                  \
    }

void CselIRDumpPass::visit_prolog( Module& value, Context& )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( Module& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( Function& value, Context& )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_interlog( Function& value, Context& )
{
}
void CselIRDumpPass::visit_epilog( Function& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( Intrinsic& value, Context& )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_interlog( Intrinsic& value, Context& )
{
}
void CselIRDumpPass::visit_epilog( Intrinsic& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( Reference& value, Context& )
{
    DUMP_PREFIX;
    printf(
        "%s %s", value.type().name().c_str(), value.isInput() ? "in" : "out" );
    // printf(
    //     "%s, %s", value.identifier()->name(), value.isInput() ? "in" : "out"
    //     );
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( Reference& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( Structure& value, Context& )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( Structure& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( Variable& value, Context& )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( Variable& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( Memory& value, Context& )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( Memory& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( ParallelScope& value, Context& )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( ParallelScope& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( SequentialScope& value, Context& )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( SequentialScope& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( TrivialStatement& value, Context& )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( TrivialStatement& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( BranchStatement& value, Context& )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_interlog( BranchStatement& value, Context& )
{
}
void CselIRDumpPass::visit_epilog( BranchStatement& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( LoopStatement& value, Context& )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_interlog( LoopStatement& value, Context& )
{
}
void CselIRDumpPass::visit_epilog( LoopStatement& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( NopInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( NopInstruction& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( AllocInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( AllocInstruction& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( IdInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( IdInstruction& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( CastInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( CastInstruction& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( CallInstruction& value, Context& )
{
    DUMP_PREFIX;
    // DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( CallInstruction& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( IdCallInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( IdCallInstruction& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( StreamInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( StreamInstruction& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( ExtractInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( ExtractInstruction& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( LoadInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( LoadInstruction& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( StoreInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( StoreInstruction& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( NotInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( NotInstruction& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( LnotInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( LnotInstruction& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( AndInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( AndInstruction& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( OrInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( OrInstruction& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( XorInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( XorInstruction& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( AddUnsignedInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( AddUnsignedInstruction& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( AddSignedInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( AddSignedInstruction& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( DivSignedInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( DivSignedInstruction& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( EquInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( EquInstruction& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( NeqInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( NeqInstruction& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( ZeroExtendInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( ZeroExtendInstruction& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( TruncationInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( TruncationInstruction& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( ModUnsignedInstruction& value, Context& )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( ModUnsignedInstruction& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( BitConstant& value, Context& )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( BitConstant& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( StructureConstant& value, Context& )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( StructureConstant& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( StringConstant& value, Context& )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( StringConstant& value, Context& )
{
}

void CselIRDumpPass::visit_prolog( Interconnect& value, Context& )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( Interconnect& value, Context& )
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
