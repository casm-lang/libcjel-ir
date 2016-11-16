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

#ifndef _LIB_NOVEL_VISITOR_H_
#define _LIB_NOVEL_VISITOR_H_

//#include "Novel.h"

#include "Instruction.h"
#include "Scope.h"
#include "Module.h"
#include "Function.h"
#include "Intrinsic.h"
#include "Reference.h"
#include "Variable.h"
#include "Memory.h"
#include "Constant.h"
#include "Interconnect.h"


namespace libnovel
{
	enum class Traversal
	{ PREORDER
	, POSTORDER
    };
	
	class Visitor //: public Novel
	{
    public:
		enum class Stage
		{ PROLOG
		, INTERLOG
		, EPILOG
		};
		
		virtual void dispatch( Stage stage, Value* value ) final;
	    
#define LIB_NOVEL_VISITOR_INTERFACE_( PREFIX, POSTFIX )					\
		PREFIX void visit_prolog( Module& value ) POSTFIX;				\
		PREFIX void visit_epilog( Module& value ) POSTFIX;				\
																		\
		PREFIX void visit_prolog( Function& value ) POSTFIX;			\
		PREFIX void visit_interlog( Function& value ) POSTFIX;			\
		PREFIX void visit_epilog( Function& value ) POSTFIX;			\
																		\
		PREFIX void visit_prolog( Intrinsic& value ) POSTFIX;			\
		PREFIX void visit_interlog( Intrinsic& value ) POSTFIX;			\
		PREFIX void visit_epilog( Intrinsic& value ) POSTFIX;			\
																		\
		PREFIX void visit_prolog( Reference& value ) POSTFIX;			\
		PREFIX void visit_epilog( Reference& value ) POSTFIX;			\
																		\
		PREFIX void visit_prolog( Structure& value ) POSTFIX;			\
		PREFIX void visit_epilog( Structure& value ) POSTFIX;			\
																		\
		PREFIX void visit_prolog( Variable& value ) POSTFIX;			\
		PREFIX void visit_epilog( Variable& value ) POSTFIX;			\
																		\
		PREFIX void visit_prolog( Memory& value ) POSTFIX;				\
		PREFIX void visit_epilog( Memory& value ) POSTFIX;				\
																		\
		PREFIX void visit_prolog( ParallelScope& value ) POSTFIX;		\
		PREFIX void visit_epilog( ParallelScope& value ) POSTFIX;		\
																		\
		PREFIX void visit_prolog( SequentialScope& value ) POSTFIX;		\
		PREFIX void visit_epilog( SequentialScope& value ) POSTFIX;		\
																		\
		PREFIX void visit_prolog( TrivialStatement& value ) POSTFIX;	\
		PREFIX void visit_epilog( TrivialStatement& value ) POSTFIX;	\
																		\
		PREFIX void visit_prolog( BranchStatement& value ) POSTFIX;		\
		PREFIX void visit_interlog( BranchStatement& value ) POSTFIX;	\
		PREFIX void visit_epilog( BranchStatement& value ) POSTFIX;		\
																		\
		PREFIX void visit_prolog( LoopStatement& value ) POSTFIX;		\
		PREFIX void visit_interlog( LoopStatement& value ) POSTFIX;		\
		PREFIX void visit_epilog( LoopStatement& value ) POSTFIX;		\
																		\
		PREFIX void visit_prolog( CallInstruction& value ) POSTFIX;		\
		PREFIX void visit_epilog( CallInstruction& value ) POSTFIX;		\
																		\
		PREFIX void visit_prolog( IdCallInstruction& value ) POSTFIX;	\
		PREFIX void visit_epilog( IdCallInstruction& value ) POSTFIX;	\
																		\
		PREFIX void visit_prolog( StreamInstruction& value ) POSTFIX;	\
		PREFIX void visit_epilog( StreamInstruction& value ) POSTFIX;	\
																		\
		PREFIX void visit_prolog( NopInstruction& value ) POSTFIX;		\
		PREFIX void visit_epilog( NopInstruction& value ) POSTFIX;		\
																		\
		PREFIX void visit_prolog( AllocInstruction& value ) POSTFIX;	\
		PREFIX void visit_epilog( AllocInstruction& value ) POSTFIX;	\
																		\
		PREFIX void visit_prolog( IdInstruction& value ) POSTFIX;		\
		PREFIX void visit_epilog( IdInstruction& value ) POSTFIX;		\
																		\
		PREFIX void visit_prolog( CastInstruction& value ) POSTFIX;		\
		PREFIX void visit_epilog( CastInstruction& value ) POSTFIX;		\
																		\
		PREFIX void visit_prolog( ExtractInstruction& value ) POSTFIX;	\
		PREFIX void visit_epilog( ExtractInstruction& value ) POSTFIX;	\
																		\
		PREFIX void visit_prolog( LoadInstruction& value ) POSTFIX;		\
		PREFIX void visit_epilog( LoadInstruction& value ) POSTFIX;		\
																		\
		PREFIX void visit_prolog( StoreInstruction& value ) POSTFIX;	\
		PREFIX void visit_epilog( StoreInstruction& value ) POSTFIX;	\
																		\
		PREFIX void visit_prolog( NotInstruction& value ) POSTFIX;		\
		PREFIX void visit_epilog( NotInstruction& value ) POSTFIX;		\
																		\
		PREFIX void visit_prolog( AndInstruction& value ) POSTFIX;		\
		PREFIX void visit_epilog( AndInstruction& value ) POSTFIX;		\
																		\
		PREFIX void visit_prolog( OrInstruction& value ) POSTFIX;		\
		PREFIX void visit_epilog( OrInstruction& value ) POSTFIX;		\
																		\
		PREFIX void visit_prolog( XorInstruction& value ) POSTFIX;		\
		PREFIX void visit_epilog( XorInstruction& value ) POSTFIX;		\
																		\
		PREFIX void visit_prolog( AddSignedInstruction& value ) POSTFIX; \
		PREFIX void visit_epilog( AddSignedInstruction& value ) POSTFIX; \
																		\
		PREFIX void visit_prolog( DivSignedInstruction& value ) POSTFIX; \
		PREFIX void visit_epilog( DivSignedInstruction& value ) POSTFIX; \
																		\
		PREFIX void visit_prolog( ModUnsignedInstruction& value ) POSTFIX; \
		PREFIX void visit_epilog( ModUnsignedInstruction& value ) POSTFIX; \
																		\
		PREFIX void visit_prolog( EquUnsignedInstruction& value ) POSTFIX; \
		PREFIX void visit_epilog( EquUnsignedInstruction& value ) POSTFIX; \
																		\
		PREFIX void visit_prolog( NeqUnsignedInstruction& value ) POSTFIX; \
		PREFIX void visit_epilog( NeqUnsignedInstruction& value ) POSTFIX; \
																		\
		PREFIX void visit_prolog( ZeroExtendInstruction& value ) POSTFIX; \
		PREFIX void visit_epilog( ZeroExtendInstruction& value ) POSTFIX; \
																		\
		PREFIX void visit_prolog( TruncationInstruction& value ) POSTFIX; \
		PREFIX void visit_epilog( TruncationInstruction& value ) POSTFIX; \
																		\
		PREFIX void visit_prolog( BitConstant& value ) POSTFIX;			\
		PREFIX void visit_epilog( BitConstant& value ) POSTFIX;			\
																		\
		PREFIX void visit_prolog( StructureConstant& value ) POSTFIX;	\
		PREFIX void visit_epilog( StructureConstant& value ) POSTFIX;	\
																		\
		PREFIX void visit_prolog( StringConstant& value ) POSTFIX;		\
		PREFIX void visit_epilog( StringConstant& value ) POSTFIX;		\
																		\
		PREFIX void visit_prolog( Interconnect& value ) POSTFIX;		\
		PREFIX void visit_epilog( Interconnect& value ) POSTFIX
		
		
		LIB_NOVEL_VISITOR_INTERFACE_( virtual, = 0 );
	};
}

#define LIB_NOVEL_VISITOR_INTERFACE LIB_NOVEL_VISITOR_INTERFACE_(, override final )





#endif /* _LIB_NOVEL_VISITOR_H_ */

//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  
