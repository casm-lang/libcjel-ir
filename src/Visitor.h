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

#ifndef _LIB_CSELIR_VISITOR_H_
#define _LIB_CSELIR_VISITOR_H_

//#include "CselIR.h"

#include "Constant.h"
#include "Function.h"
#include "Instruction.h"
#include "Interconnect.h"
#include "Intrinsic.h"
#include "Memory.h"
#include "Module.h"
#include "Reference.h"
#include "Scope.h"
#include "Variable.h"

#include "CselIR.h"

namespace libcsel_ir
{
    enum class Traversal
    {
        PREORDER,
        POSTORDER
    };

    class Context : public CselIR
    {
    };

    class Visitor : public CselIR
    {
      public:
        enum class Stage
        {
            PROLOG,
            INTERLOG,
            EPILOG
        };

        void dispatch( Stage stage, Value& value, Context& cxt );

#define LIB_CSELIR_VISITOR_INTERFACE_( PREFIX, POSTFIX )                       \
    PREFIX void visit_prolog(                                                  \
        libcsel_ir::Module& value, libcsel_ir::Context& cxt ) POSTFIX;         \
    PREFIX void visit_epilog(                                                  \
        libcsel_ir::Module& value, libcsel_ir::Context& cxt ) POSTFIX;         \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcsel_ir::Function& value, libcsel_ir::Context& cxt ) POSTFIX;       \
    PREFIX void visit_interlog(                                                \
        libcsel_ir::Function& value, libcsel_ir::Context& cxt ) POSTFIX;       \
    PREFIX void visit_epilog(                                                  \
        libcsel_ir::Function& value, libcsel_ir::Context& cxt ) POSTFIX;       \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcsel_ir::Intrinsic& value, libcsel_ir::Context& cxt ) POSTFIX;      \
    PREFIX void visit_interlog(                                                \
        libcsel_ir::Intrinsic& value, libcsel_ir::Context& cxt ) POSTFIX;      \
    PREFIX void visit_epilog(                                                  \
        libcsel_ir::Intrinsic& value, libcsel_ir::Context& cxt ) POSTFIX;      \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcsel_ir::Reference& value, libcsel_ir::Context& cxt ) POSTFIX;      \
    PREFIX void visit_epilog(                                                  \
        libcsel_ir::Reference& value, libcsel_ir::Context& cxt ) POSTFIX;      \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcsel_ir::Structure& value, libcsel_ir::Context& cxt ) POSTFIX;      \
    PREFIX void visit_epilog(                                                  \
        libcsel_ir::Structure& value, libcsel_ir::Context& cxt ) POSTFIX;      \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcsel_ir::Variable& value, libcsel_ir::Context& cxt ) POSTFIX;       \
    PREFIX void visit_epilog(                                                  \
        libcsel_ir::Variable& value, libcsel_ir::Context& cxt ) POSTFIX;       \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcsel_ir::Memory& value, libcsel_ir::Context& cxt ) POSTFIX;         \
    PREFIX void visit_epilog(                                                  \
        libcsel_ir::Memory& value, libcsel_ir::Context& cxt ) POSTFIX;         \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcsel_ir::ParallelScope& value, libcsel_ir::Context& cxt ) POSTFIX;  \
    PREFIX void visit_epilog(                                                  \
        libcsel_ir::ParallelScope& value, libcsel_ir::Context& cxt ) POSTFIX;  \
                                                                               \
    PREFIX void visit_prolog( libcsel_ir::SequentialScope& value,              \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcsel_ir::SequentialScope& value,              \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog( libcsel_ir::TrivialStatement& value,             \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcsel_ir::TrivialStatement& value,             \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog( libcsel_ir::BranchStatement& value,              \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_interlog( libcsel_ir::BranchStatement& value,            \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcsel_ir::BranchStatement& value,              \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcsel_ir::LoopStatement& value, libcsel_ir::Context& cxt ) POSTFIX;  \
    PREFIX void visit_interlog(                                                \
        libcsel_ir::LoopStatement& value, libcsel_ir::Context& cxt ) POSTFIX;  \
    PREFIX void visit_epilog(                                                  \
        libcsel_ir::LoopStatement& value, libcsel_ir::Context& cxt ) POSTFIX;  \
                                                                               \
    PREFIX void visit_prolog( libcsel_ir::CallInstruction& value,              \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcsel_ir::CallInstruction& value,              \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog( libcsel_ir::IdCallInstruction& value,            \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcsel_ir::IdCallInstruction& value,            \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog( libcsel_ir::StreamInstruction& value,            \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcsel_ir::StreamInstruction& value,            \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcsel_ir::NopInstruction& value, libcsel_ir::Context& cxt ) POSTFIX; \
    PREFIX void visit_epilog(                                                  \
        libcsel_ir::NopInstruction& value, libcsel_ir::Context& cxt ) POSTFIX; \
                                                                               \
    PREFIX void visit_prolog( libcsel_ir::AllocInstruction& value,             \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcsel_ir::AllocInstruction& value,             \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcsel_ir::IdInstruction& value, libcsel_ir::Context& cxt ) POSTFIX;  \
    PREFIX void visit_epilog(                                                  \
        libcsel_ir::IdInstruction& value, libcsel_ir::Context& cxt ) POSTFIX;  \
                                                                               \
    PREFIX void visit_prolog( libcsel_ir::CastInstruction& value,              \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcsel_ir::CastInstruction& value,              \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog( libcsel_ir::ExtractInstruction& value,           \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcsel_ir::ExtractInstruction& value,           \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog( libcsel_ir::LoadInstruction& value,              \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcsel_ir::LoadInstruction& value,              \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog( libcsel_ir::StoreInstruction& value,             \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcsel_ir::StoreInstruction& value,             \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcsel_ir::NotInstruction& value, libcsel_ir::Context& cxt ) POSTFIX; \
    PREFIX void visit_epilog(                                                  \
        libcsel_ir::NotInstruction& value, libcsel_ir::Context& cxt ) POSTFIX; \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcsel_ir::AndInstruction& value, libcsel_ir::Context& cxt ) POSTFIX; \
    PREFIX void visit_epilog(                                                  \
        libcsel_ir::AndInstruction& value, libcsel_ir::Context& cxt ) POSTFIX; \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcsel_ir::OrInstruction& value, libcsel_ir::Context& cxt ) POSTFIX;  \
    PREFIX void visit_epilog(                                                  \
        libcsel_ir::OrInstruction& value, libcsel_ir::Context& cxt ) POSTFIX;  \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcsel_ir::XorInstruction& value, libcsel_ir::Context& cxt ) POSTFIX; \
    PREFIX void visit_epilog(                                                  \
        libcsel_ir::XorInstruction& value, libcsel_ir::Context& cxt ) POSTFIX; \
                                                                               \
    PREFIX void visit_prolog( libcsel_ir::AddSignedInstruction& value,         \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcsel_ir::AddSignedInstruction& value,         \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog( libcsel_ir::DivSignedInstruction& value,         \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcsel_ir::DivSignedInstruction& value,         \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog( libcsel_ir::ModUnsignedInstruction& value,       \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcsel_ir::ModUnsignedInstruction& value,       \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcsel_ir::EquInstruction& value, libcsel_ir::Context& cxt ) POSTFIX; \
    PREFIX void visit_epilog(                                                  \
        libcsel_ir::EquInstruction& value, libcsel_ir::Context& cxt ) POSTFIX; \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcsel_ir::NeqInstruction& value, libcsel_ir::Context& cxt ) POSTFIX; \
    PREFIX void visit_epilog(                                                  \
        libcsel_ir::NeqInstruction& value, libcsel_ir::Context& cxt ) POSTFIX; \
                                                                               \
    PREFIX void visit_prolog( libcsel_ir::ZeroExtendInstruction& value,        \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcsel_ir::ZeroExtendInstruction& value,        \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog( libcsel_ir::TruncationInstruction& value,        \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcsel_ir::TruncationInstruction& value,        \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcsel_ir::BitConstant& value, libcsel_ir::Context& cxt ) POSTFIX;    \
    PREFIX void visit_epilog(                                                  \
        libcsel_ir::BitConstant& value, libcsel_ir::Context& cxt ) POSTFIX;    \
                                                                               \
    PREFIX void visit_prolog( libcsel_ir::StructureConstant& value,            \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
    PREFIX void visit_epilog( libcsel_ir::StructureConstant& value,            \
        libcsel_ir::Context& cxt ) POSTFIX;                                    \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcsel_ir::StringConstant& value, libcsel_ir::Context& cxt ) POSTFIX; \
    PREFIX void visit_epilog(                                                  \
        libcsel_ir::StringConstant& value, libcsel_ir::Context& cxt ) POSTFIX; \
                                                                               \
    PREFIX void visit_prolog(                                                  \
        libcsel_ir::Interconnect& value, libcsel_ir::Context& cxt ) POSTFIX;   \
    PREFIX void visit_epilog(                                                  \
        libcsel_ir::Interconnect& value, libcsel_ir::Context& cxt ) POSTFIX

#define LIB_CSELIR_VISITOR_INTERFACE LIB_CSELIR_VISITOR_INTERFACE_(, override )

        LIB_CSELIR_VISITOR_INTERFACE_( virtual, = 0 );
    };
}

#endif /* _LIB_CSELIR_VISITOR_H_ */

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
