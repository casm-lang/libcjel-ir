//
//  Copyright (c) 2015-2016 Philipp Paulweber
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
    string ind = "";
    u8 cnt = 0;
    Value* p = ( &value );
    while( p != 0 )
    {
        if( Value::isa< Block >( p ) )
        {
            p = (Value*)( (Block*)p )->getParent();
        }
        else if( Value::isa< Instruction >( p ) )
        {
            p = (Value*)( (Instruction*)p )->getStatement();
        }
        else
        {
            break;
            // assert(0);
        }

        if( Value::isa< CallableUnit >( p ) )
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

void CselIRDumpPass::visit_prolog( Module& value )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( Module& value )
{
}

void CselIRDumpPass::visit_prolog( Function& value )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_interlog( Function& value )
{
}
void CselIRDumpPass::visit_epilog( Function& value )
{
}

void CselIRDumpPass::visit_prolog( Intrinsic& value )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_interlog( Intrinsic& value )
{
}
void CselIRDumpPass::visit_epilog( Intrinsic& value )
{
}

void CselIRDumpPass::visit_prolog( Reference& value )
{
    DUMP_PREFIX;
    printf( "%s, %s", value.getIdentifier()->getName(),
        value.isInput() ? "in" : "out" );
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( Reference& value )
{
}

void CselIRDumpPass::visit_prolog( Structure& value )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( Structure& value )
{
}

void CselIRDumpPass::visit_prolog( Variable& value )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( Variable& value )
{
}

void CselIRDumpPass::visit_prolog( Memory& value )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( Memory& value )
{
}

void CselIRDumpPass::visit_prolog( ParallelScope& value )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( ParallelScope& value )
{
}

void CselIRDumpPass::visit_prolog( SequentialScope& value )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( SequentialScope& value )
{
}

void CselIRDumpPass::visit_prolog( TrivialStatement& value )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( TrivialStatement& value )
{
}

void CselIRDumpPass::visit_prolog( BranchStatement& value )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_interlog( BranchStatement& value )
{
}
void CselIRDumpPass::visit_epilog( BranchStatement& value )
{
}

void CselIRDumpPass::visit_prolog( LoopStatement& value )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_interlog( LoopStatement& value )
{
}
void CselIRDumpPass::visit_epilog( LoopStatement& value )
{
}

void CselIRDumpPass::visit_prolog( NopInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( NopInstruction& value )
{
}

void CselIRDumpPass::visit_prolog( AllocInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( AllocInstruction& value )
{
}

void CselIRDumpPass::visit_prolog( IdInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( IdInstruction& value )
{
}

void CselIRDumpPass::visit_prolog( CastInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( CastInstruction& value )
{
}

void CselIRDumpPass::visit_prolog( CallInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( CallInstruction& value )
{
}

void CselIRDumpPass::visit_prolog( IdCallInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( IdCallInstruction& value )
{
}

void CselIRDumpPass::visit_prolog( StreamInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( StreamInstruction& value )
{
}

void CselIRDumpPass::visit_prolog( ExtractInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( ExtractInstruction& value )
{
}

void CselIRDumpPass::visit_prolog( LoadInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( LoadInstruction& value )
{
}

void CselIRDumpPass::visit_prolog( StoreInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( StoreInstruction& value )
{
}

void CselIRDumpPass::visit_prolog( NotInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( NotInstruction& value )
{
}

void CselIRDumpPass::visit_prolog( AndInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( AndInstruction& value )
{
}

void CselIRDumpPass::visit_prolog( OrInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( OrInstruction& value )
{
}

void CselIRDumpPass::visit_prolog( XorInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( XorInstruction& value )
{
}

void CselIRDumpPass::visit_prolog( AddSignedInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( AddSignedInstruction& value )
{
}

void CselIRDumpPass::visit_prolog( DivSignedInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( DivSignedInstruction& value )
{
}

void CselIRDumpPass::visit_prolog( EquUnsignedInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( EquUnsignedInstruction& value )
{
}

void CselIRDumpPass::visit_prolog( NeqUnsignedInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( NeqUnsignedInstruction& value )
{
}

void CselIRDumpPass::visit_prolog( ZeroExtendInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( ZeroExtendInstruction& value )
{
}

void CselIRDumpPass::visit_prolog( TruncationInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( TruncationInstruction& value )
{
}

void CselIRDumpPass::visit_prolog( ModUnsignedInstruction& value )
{
    DUMP_PREFIX;
    DUMP_INSTR;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( ModUnsignedInstruction& value )
{
}

void CselIRDumpPass::visit_prolog( BitConstant& value )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( BitConstant& value )
{
}

void CselIRDumpPass::visit_prolog( StructureConstant& value )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( StructureConstant& value )
{
}

void CselIRDumpPass::visit_prolog( StringConstant& value )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( StringConstant& value )
{
}

void CselIRDumpPass::visit_prolog( Interconnect& value )
{
    DUMP_PREFIX;
    DUMP_POSTFIX;
}
void CselIRDumpPass::visit_epilog( Interconnect& value )
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
