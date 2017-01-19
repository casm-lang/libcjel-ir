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

using namespace libcsel_ir;

char CselIRDumpPass::id = 0;

static libpass::PassRegistration< CselIRDumpPass > PASS( "CSELIR Dumping Pass",
    "generates ASCII representation of the CSEL IR", "el-dump", 0 );

bool CselIRDumpPass::run( libpass::PassResult& pr )
{
    Module* value = (Module*)pr.getResult< CselIRDumpPass >();
    assert( value );

    value->iterate( Traversal::PREORDER, this );

    return true;
}

static const char* indention( Value& value )
{
    std::string ind = "";
    u8 cnt = 0;
    Value* p = ( &value );
    while( p != 0 )
    {
        if( isa< Block >( p ) )
        {
            p = (Value*)( (Block*)p )->getParent();
        }
        else if( isa< Instruction >( p ) )
        {
            p = (Value*)( (Instruction*)p )->getStatement();
        }
        else
        {
            break;
            // assert(0);
        }

        if( isa< CallableUnit >( p ) )
        {
            break;
        }

        cnt++;
        ind += "  ";
    }

    return libstdhl::Allocator::string( ind );
}

#define DUMP_PREFIX                                                            \
    printf( "%-14s: %p, %s, %s%s ", __FUNCTION__, &value, value.getLabel(),    \
        indention( value ), value.getName() )
#define DUMP_POSTFIX printf( "\n" );

#define DUMP_INSTR                                                             \
    for( auto v : value.getValues() )                                          \
    {                                                                          \
        printf( ", %s (%s)", v->getLabel(), v->getType()->getName() );         \
    }

void CselIRDumpPass::visit_prolog( Module& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( Module& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( Function& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_interlog( Function& value, void* cxt )
{
}
void CselIRDumpPass::visit_epilog( Function& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( Intrinsic& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_interlog( Intrinsic& value, void* cxt )
{
}
void CselIRDumpPass::visit_epilog( Intrinsic& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( Reference& value, void* cxt )
{
    DUMP_PREFIX;
    printf( "%s, %s", value.getIdentifier()->getName(),
        value.isInput() ? "in" : "out" );
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( Reference& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( Structure& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( Structure& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( Variable& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( Variable& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( Memory& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( Memory& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( ParallelScope& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( ParallelScope& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( SequentialScope& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( SequentialScope& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( TrivialStatement& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( TrivialStatement& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( BranchStatement& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_interlog( BranchStatement& value, void* cxt )
{
}
void CselIRDumpPass::visit_epilog( BranchStatement& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( LoopStatement& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_interlog( LoopStatement& value, void* cxt )
{
}
void CselIRDumpPass::visit_epilog( LoopStatement& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( NopInstruction& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( NopInstruction& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( AllocInstruction& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( AllocInstruction& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( IdInstruction& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( IdInstruction& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( CastInstruction& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( CastInstruction& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( CallInstruction& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( CallInstruction& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( IdCallInstruction& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( IdCallInstruction& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( StreamInstruction& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( StreamInstruction& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( ExtractInstruction& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( ExtractInstruction& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( LoadInstruction& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( LoadInstruction& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( StoreInstruction& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( StoreInstruction& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( NotInstruction& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( NotInstruction& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( AndInstruction& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( AndInstruction& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( OrInstruction& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( OrInstruction& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( XorInstruction& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( XorInstruction& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( AddSignedInstruction& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( AddSignedInstruction& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( DivSignedInstruction& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( DivSignedInstruction& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( EquUnsignedInstruction& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( EquUnsignedInstruction& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( NeqUnsignedInstruction& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( NeqUnsignedInstruction& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( ZeroExtendInstruction& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( ZeroExtendInstruction& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( TruncationInstruction& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( TruncationInstruction& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( ModUnsignedInstruction& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( ModUnsignedInstruction& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( BitConstant& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( BitConstant& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( StructureConstant& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( StructureConstant& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( StringConstant& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( StringConstant& value, void* cxt )
{
}

void CselIRDumpPass::visit_prolog( Interconnect& value, void* cxt )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( Interconnect& value, void* cxt )
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
