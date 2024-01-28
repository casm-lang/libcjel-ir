//
//  Copyright (C) 2015-2024 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber et al.
//  <https://github.com/casm-lang/libcjel-ir/graphs/contributors>
//
//  This file is part of libcjel-ir.
//
//  libcjel-ir is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  libcjel-ir is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with libcjel-ir. If not, see <http://www.gnu.org/licenses/>.
//
//  Additional permission under GNU GPL version 3 section 7
//
//  libcjel-ir is distributed under the terms of the GNU General Public License
//  with the following clarification and special exception: Linking libcjel-ir
//  statically or dynamically with other modules is making a combined work
//  based on libcjel-ir. Thus, the terms and conditions of the GNU General
//  Public License cover the whole combination. As a special exception,
//  the copyright holders of libcjel-ir give you permission to link libcjel-ir
//  with independent modules to produce an executable, regardless of the
//  license terms of these independent modules, and to copy and distribute
//  the resulting executable under terms of your choice, provided that you
//  also meet, for each linked independent module, the terms and conditions
//  of the license of that module. An independent module is a module which
//  is not derived from or based on libcjel-ir. If you modify libcjel-ir, you
//  may extend this exception to your version of the library, but you are
//  not obliged to do so. If you do not wish to do so, delete this exception
//  statement from your version.
//

#include "Visitor.h"

#include <libcjel-ir/Value>

#include <cassert>

using namespace libcjel_ir;

#define CASE_VALUE( VID, CLASS )                       \
    case Value::ID::VID:                               \
        if( stage == Stage::PROLOG )                   \
            visit_prolog( (CLASS&)value, cxt );        \
        else if( stage == Stage::EPILOG )              \
            visit_epilog( (CLASS&)value, cxt );        \
        else                                           \
            assert( !"invalid visitor stage value!" ); \
        break

#define CASE_VALUE_INTER( VID, CLASS )                 \
    case Value::ID::VID:                               \
        if( stage == Stage::PROLOG )                   \
            visit_prolog( (CLASS&)value, cxt );        \
        else if( stage == Stage::INTERLOG )            \
            visit_interlog( (CLASS&)value, cxt );      \
        else if( stage == Stage::EPILOG )              \
            visit_epilog( (CLASS&)value, cxt );        \
        else                                           \
            assert( !"invalid visitor stage value!" ); \
        break

void Visitor::dispatch( Stage stage, Value& value, Context& cxt )
{
    switch( value.id() )
    {
        CASE_VALUE( MODULE, Module );

        CASE_VALUE_INTER( FUNCTION, Function );
        CASE_VALUE_INTER( INTRINSIC, Intrinsic );

        CASE_VALUE( REFERENCE, Reference );

        CASE_VALUE( VARIABLE, Variable );
        CASE_VALUE( MEMORY, Memory );
        CASE_VALUE( STRUCTURE, Structure );

        CASE_VALUE( BIT_CONSTANT, BitConstant );
        CASE_VALUE( STRING_CONSTANT, StringConstant );
        CASE_VALUE( STRUCTURE_CONSTANT, StructureConstant );

        CASE_VALUE( PARALLEL_SCOPE, ParallelScope );
        CASE_VALUE( SEQUENTIAL_SCOPE, SequentialScope );

        CASE_VALUE( TRIVIAL_STATEMENT, TrivialStatement );
        CASE_VALUE_INTER( BRANCH_STATEMENT, BranchStatement );
        CASE_VALUE_INTER( LOOP_STATEMENT, LoopStatement );

        CASE_VALUE( CALL_INSTRUCTION, CallInstruction );
        CASE_VALUE( ID_CALL_INSTRUCTION, IdCallInstruction );

        CASE_VALUE( STREAM_INSTRUCTION, StreamInstruction );

        CASE_VALUE( NOP_INSTRUCTION, NopInstruction );
        CASE_VALUE( ALLOC_INSTRUCTION, AllocInstruction );

        CASE_VALUE( ID_INSTRUCTION, IdInstruction );
        CASE_VALUE( CAST_INSTRUCTION, CastInstruction );
        CASE_VALUE( EXTRACT_INSTRUCTION, ExtractInstruction );

        CASE_VALUE( LOAD_INSTRUCTION, LoadInstruction );
        CASE_VALUE( STORE_INSTRUCTION, StoreInstruction );

        CASE_VALUE( NOT_INSTRUCTION, NotInstruction );
        CASE_VALUE( LNOT_INSTRUCTION, LnotInstruction );

        CASE_VALUE( AND_INSTRUCTION, AndInstruction );
        CASE_VALUE( OR_INSTRUCTION, OrInstruction );
        CASE_VALUE( XOR_INSTRUCTION, XorInstruction );

        CASE_VALUE( ADDU_INSTRUCTION, AddUnsignedInstruction );
        CASE_VALUE( ADDS_INSTRUCTION, AddSignedInstruction );
        CASE_VALUE( DIVS_INSTRUCTION, DivSignedInstruction );
        CASE_VALUE( MODU_INSTRUCTION, ModUnsignedInstruction );

        CASE_VALUE( EQU_INSTRUCTION, EquInstruction );
        CASE_VALUE( NEQ_INSTRUCTION, NeqInstruction );

        CASE_VALUE( ZEXT_INSTRUCTION, ZeroExtendInstruction );
        CASE_VALUE( TRUNC_INSTRUCTION, TruncationInstruction );

        CASE_VALUE( INTERCONNECT, Interconnect );

        default:
            fprintf(
                stderr,
                "%s:%i: warning: unimplemented value ID '%s' to dispatch for "
                "stage '%i' and context '%p'",
                __FILE__,
                __LINE__,
                value.name().c_str(),
                stage,
                &cxt );
            assert( 0 );
            break;
    }
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
