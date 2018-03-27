//
//  Copyright (C) 2015-2018 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                <https://github.com/casm-lang/libcjel-ir>
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

#ifndef _LIBCJEL_IR_VISITOR_H_
#define _LIBCJEL_IR_VISITOR_H_

#include <libcjel-ir/CjelIR>

namespace libcjel_ir
{
    class Module;

    class Variable;
    class Memory;
    class Interconnect;

    class Function;
    class Intrinsic;
    class Reference;
    class Structure;

    class ParallelScope;
    class SequentialScope;

    class TrivialStatement;
    class BranchStatement;
    class LoopStatement;

    class CallInstruction;
    class IdCallInstruction;
    class NopInstruction;
    class StreamInstruction;
    class AllocInstruction;
    class IdInstruction;
    class CastInstruction;
    class LoadInstruction;
    class StoreInstruction;
    class ExtractInstruction;
    class NotInstruction;
    class LnotInstruction;
    class AndInstruction;
    class OrInstruction;
    class XorInstruction;
    class EquInstruction;
    class NeqInstruction;
    class AddSignedInstruction;
    class AddUnsignedInstruction;
    class DivSignedInstruction;
    class ModUnsignedInstruction;
    class ZeroExtendInstruction;
    class TruncationInstruction;

    class BitConstant;
    class StringConstant;
    class StructureConstant;

    enum Traversal : u8
    {
        PREORDER = 0,
        POSTORDER
    };

    class Context : public CjelIR
    {
    };

    class Visitor : public CjelIR
    {
      public:
        enum class Stage
        {
            PROLOG,
            INTERLOG,
            EPILOG
        };

        void dispatch( Stage stage, Value& value, Context& cxt );

#define LIBCJEL_IR_VISITOR_INTERFACE_( PREFIX, POSTFIX )                                           \
    PREFIX void visit_prolog( libcjel_ir::Module& value, libcjel_ir::Context& cxt ) POSTFIX;       \
    PREFIX void visit_epilog( libcjel_ir::Module& value, libcjel_ir::Context& cxt ) POSTFIX;       \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::Function& value, libcjel_ir::Context& cxt ) POSTFIX;     \
    PREFIX void visit_interlog( libcjel_ir::Function& value, libcjel_ir::Context& cxt ) POSTFIX;   \
    PREFIX void visit_epilog( libcjel_ir::Function& value, libcjel_ir::Context& cxt ) POSTFIX;     \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::Intrinsic& value, libcjel_ir::Context& cxt ) POSTFIX;    \
    PREFIX void visit_interlog( libcjel_ir::Intrinsic& value, libcjel_ir::Context& cxt ) POSTFIX;  \
    PREFIX void visit_epilog( libcjel_ir::Intrinsic& value, libcjel_ir::Context& cxt ) POSTFIX;    \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::Reference& value, libcjel_ir::Context& cxt ) POSTFIX;    \
    PREFIX void visit_epilog( libcjel_ir::Reference& value, libcjel_ir::Context& cxt ) POSTFIX;    \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::Structure& value, libcjel_ir::Context& cxt ) POSTFIX;    \
    PREFIX void visit_epilog( libcjel_ir::Structure& value, libcjel_ir::Context& cxt ) POSTFIX;    \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::Variable& value, libcjel_ir::Context& cxt ) POSTFIX;     \
    PREFIX void visit_epilog( libcjel_ir::Variable& value, libcjel_ir::Context& cxt ) POSTFIX;     \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::Memory& value, libcjel_ir::Context& cxt ) POSTFIX;       \
    PREFIX void visit_epilog( libcjel_ir::Memory& value, libcjel_ir::Context& cxt ) POSTFIX;       \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::ParallelScope& value, libcjel_ir::Context& cxt )         \
        POSTFIX;                                                                                   \
    PREFIX void visit_epilog( libcjel_ir::ParallelScope& value, libcjel_ir::Context& cxt )         \
        POSTFIX;                                                                                   \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::SequentialScope& value, libcjel_ir::Context& cxt )       \
        POSTFIX;                                                                                   \
    PREFIX void visit_epilog( libcjel_ir::SequentialScope& value, libcjel_ir::Context& cxt )       \
        POSTFIX;                                                                                   \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::TrivialStatement& value, libcjel_ir::Context& cxt )      \
        POSTFIX;                                                                                   \
    PREFIX void visit_epilog( libcjel_ir::TrivialStatement& value, libcjel_ir::Context& cxt )      \
        POSTFIX;                                                                                   \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::BranchStatement& value, libcjel_ir::Context& cxt )       \
        POSTFIX;                                                                                   \
    PREFIX void visit_interlog( libcjel_ir::BranchStatement& value, libcjel_ir::Context& cxt )     \
        POSTFIX;                                                                                   \
    PREFIX void visit_epilog( libcjel_ir::BranchStatement& value, libcjel_ir::Context& cxt )       \
        POSTFIX;                                                                                   \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::LoopStatement& value, libcjel_ir::Context& cxt )         \
        POSTFIX;                                                                                   \
    PREFIX void visit_interlog( libcjel_ir::LoopStatement& value, libcjel_ir::Context& cxt )       \
        POSTFIX;                                                                                   \
    PREFIX void visit_epilog( libcjel_ir::LoopStatement& value, libcjel_ir::Context& cxt )         \
        POSTFIX;                                                                                   \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::CallInstruction& value, libcjel_ir::Context& cxt )       \
        POSTFIX;                                                                                   \
    PREFIX void visit_epilog( libcjel_ir::CallInstruction& value, libcjel_ir::Context& cxt )       \
        POSTFIX;                                                                                   \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::IdCallInstruction& value, libcjel_ir::Context& cxt )     \
        POSTFIX;                                                                                   \
    PREFIX void visit_epilog( libcjel_ir::IdCallInstruction& value, libcjel_ir::Context& cxt )     \
        POSTFIX;                                                                                   \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::StreamInstruction& value, libcjel_ir::Context& cxt )     \
        POSTFIX;                                                                                   \
    PREFIX void visit_epilog( libcjel_ir::StreamInstruction& value, libcjel_ir::Context& cxt )     \
        POSTFIX;                                                                                   \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::NopInstruction& value, libcjel_ir::Context& cxt )        \
        POSTFIX;                                                                                   \
    PREFIX void visit_epilog( libcjel_ir::NopInstruction& value, libcjel_ir::Context& cxt )        \
        POSTFIX;                                                                                   \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::AllocInstruction& value, libcjel_ir::Context& cxt )      \
        POSTFIX;                                                                                   \
    PREFIX void visit_epilog( libcjel_ir::AllocInstruction& value, libcjel_ir::Context& cxt )      \
        POSTFIX;                                                                                   \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::IdInstruction& value, libcjel_ir::Context& cxt )         \
        POSTFIX;                                                                                   \
    PREFIX void visit_epilog( libcjel_ir::IdInstruction& value, libcjel_ir::Context& cxt )         \
        POSTFIX;                                                                                   \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::CastInstruction& value, libcjel_ir::Context& cxt )       \
        POSTFIX;                                                                                   \
    PREFIX void visit_epilog( libcjel_ir::CastInstruction& value, libcjel_ir::Context& cxt )       \
        POSTFIX;                                                                                   \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::ExtractInstruction& value, libcjel_ir::Context& cxt )    \
        POSTFIX;                                                                                   \
    PREFIX void visit_epilog( libcjel_ir::ExtractInstruction& value, libcjel_ir::Context& cxt )    \
        POSTFIX;                                                                                   \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::LoadInstruction& value, libcjel_ir::Context& cxt )       \
        POSTFIX;                                                                                   \
    PREFIX void visit_epilog( libcjel_ir::LoadInstruction& value, libcjel_ir::Context& cxt )       \
        POSTFIX;                                                                                   \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::StoreInstruction& value, libcjel_ir::Context& cxt )      \
        POSTFIX;                                                                                   \
    PREFIX void visit_epilog( libcjel_ir::StoreInstruction& value, libcjel_ir::Context& cxt )      \
        POSTFIX;                                                                                   \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::NotInstruction& value, libcjel_ir::Context& cxt )        \
        POSTFIX;                                                                                   \
    PREFIX void visit_epilog( libcjel_ir::NotInstruction& value, libcjel_ir::Context& cxt )        \
        POSTFIX;                                                                                   \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::LnotInstruction& value, libcjel_ir::Context& cxt )       \
        POSTFIX;                                                                                   \
    PREFIX void visit_epilog( libcjel_ir::LnotInstruction& value, libcjel_ir::Context& cxt )       \
        POSTFIX;                                                                                   \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::AndInstruction& value, libcjel_ir::Context& cxt )        \
        POSTFIX;                                                                                   \
    PREFIX void visit_epilog( libcjel_ir::AndInstruction& value, libcjel_ir::Context& cxt )        \
        POSTFIX;                                                                                   \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::OrInstruction& value, libcjel_ir::Context& cxt )         \
        POSTFIX;                                                                                   \
    PREFIX void visit_epilog( libcjel_ir::OrInstruction& value, libcjel_ir::Context& cxt )         \
        POSTFIX;                                                                                   \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::XorInstruction& value, libcjel_ir::Context& cxt )        \
        POSTFIX;                                                                                   \
    PREFIX void visit_epilog( libcjel_ir::XorInstruction& value, libcjel_ir::Context& cxt )        \
        POSTFIX;                                                                                   \
                                                                                                   \
    PREFIX void visit_prolog(                                                                      \
        libcjel_ir::AddUnsignedInstruction& value, libcjel_ir::Context& cxt ) POSTFIX;             \
    PREFIX void visit_epilog(                                                                      \
        libcjel_ir::AddUnsignedInstruction& value, libcjel_ir::Context& cxt ) POSTFIX;             \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::AddSignedInstruction& value, libcjel_ir::Context& cxt )  \
        POSTFIX;                                                                                   \
    PREFIX void visit_epilog( libcjel_ir::AddSignedInstruction& value, libcjel_ir::Context& cxt )  \
        POSTFIX;                                                                                   \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::DivSignedInstruction& value, libcjel_ir::Context& cxt )  \
        POSTFIX;                                                                                   \
    PREFIX void visit_epilog( libcjel_ir::DivSignedInstruction& value, libcjel_ir::Context& cxt )  \
        POSTFIX;                                                                                   \
                                                                                                   \
    PREFIX void visit_prolog(                                                                      \
        libcjel_ir::ModUnsignedInstruction& value, libcjel_ir::Context& cxt ) POSTFIX;             \
    PREFIX void visit_epilog(                                                                      \
        libcjel_ir::ModUnsignedInstruction& value, libcjel_ir::Context& cxt ) POSTFIX;             \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::EquInstruction& value, libcjel_ir::Context& cxt )        \
        POSTFIX;                                                                                   \
    PREFIX void visit_epilog( libcjel_ir::EquInstruction& value, libcjel_ir::Context& cxt )        \
        POSTFIX;                                                                                   \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::NeqInstruction& value, libcjel_ir::Context& cxt )        \
        POSTFIX;                                                                                   \
    PREFIX void visit_epilog( libcjel_ir::NeqInstruction& value, libcjel_ir::Context& cxt )        \
        POSTFIX;                                                                                   \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::ZeroExtendInstruction& value, libcjel_ir::Context& cxt ) \
        POSTFIX;                                                                                   \
    PREFIX void visit_epilog( libcjel_ir::ZeroExtendInstruction& value, libcjel_ir::Context& cxt ) \
        POSTFIX;                                                                                   \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::TruncationInstruction& value, libcjel_ir::Context& cxt ) \
        POSTFIX;                                                                                   \
    PREFIX void visit_epilog( libcjel_ir::TruncationInstruction& value, libcjel_ir::Context& cxt ) \
        POSTFIX;                                                                                   \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::BitConstant& value, libcjel_ir::Context& cxt ) POSTFIX;  \
    PREFIX void visit_epilog( libcjel_ir::BitConstant& value, libcjel_ir::Context& cxt ) POSTFIX;  \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::StructureConstant& value, libcjel_ir::Context& cxt )     \
        POSTFIX;                                                                                   \
    PREFIX void visit_epilog( libcjel_ir::StructureConstant& value, libcjel_ir::Context& cxt )     \
        POSTFIX;                                                                                   \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::StringConstant& value, libcjel_ir::Context& cxt )        \
        POSTFIX;                                                                                   \
    PREFIX void visit_epilog( libcjel_ir::StringConstant& value, libcjel_ir::Context& cxt )        \
        POSTFIX;                                                                                   \
                                                                                                   \
    PREFIX void visit_prolog( libcjel_ir::Interconnect& value, libcjel_ir::Context& cxt ) POSTFIX; \
    PREFIX void visit_epilog( libcjel_ir::Interconnect& value, libcjel_ir::Context& cxt ) POSTFIX

#define LIBCJEL_IR_VISITOR_INTERFACE LIBCJEL_IR_VISITOR_INTERFACE_(, override )

        LIBCJEL_IR_VISITOR_INTERFACE_( virtual, = 0 );
    };
}

#endif  // _LIBCJEL_IR_VISITOR_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
