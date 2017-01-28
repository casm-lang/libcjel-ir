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

#ifndef _LIB_CSELIR_INSTRUCTION_H_
#define _LIB_CSELIR_INSTRUCTION_H_

#include "CselIR.h"

#include "Function.h"
#include "Interconnect.h"
#include "Intrinsic.h"
#include "Statement.h"
#include "Structure.h"
#include "User.h"
#include "Value.h"
#include "Variable.h"

namespace libcsel_ir
{
    class Statement;

    class Instruction : public User
    {
      private:
        std::vector< Value* > values;
        Statement* statement;

      public:
        Instruction( const char* name, Type* type,
            const std::vector< Value* >& values,
            Value::ID id = Value::INSTRUCTION );

        ~Instruction( void );

        void setStatement( Statement* stmt );
        Statement* getStatement( void ) const;

        void add( Value* value );
        Value* getValue( u8 index ) const;
        const std::vector< Value* >& getValues( void ) const;

        void dump( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::INSTRUCTION;
        };
        static bool classof( Value const* obj );

        virtual const char* getLabelName( void ) override final
        {
            return "%r";
        }

        virtual u64 getLabelID( void ) override final
        {
            static u64 cnt = 0;
            return cnt++;
        }
    };

    class UnaryInstruction
    {
      private:
        Instruction& self;

      public:
        UnaryInstruction( Instruction* self );

        Value* get( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::UNARY_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class BinaryInstruction
    {
      private:
        Instruction& self;

      public:
        BinaryInstruction( Instruction* self );

        Value* getLHS( void ) const;
        Value* getRHS( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::BINARY_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class OperatorInstruction : public Instruction
    {
      public:
        OperatorInstruction( const char* name, Type* type,
            const std::vector< Value* >& values,
            Value::ID id = Value::OPERATOR_INSTRUCTION );

        static inline Value::ID classid( void )
        {
            return Value::OPERATOR_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class ArithmeticInstruction : public OperatorInstruction
    {
      public:
        ArithmeticInstruction( const char* name,
            const std::vector< Value* >& values,
            Value::ID id = Value::ARITHMETIC_INSTRUCTION );

        static inline Value::ID classid( void )
        {
            return Value::ARITHMETIC_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class CompareInstruction : public OperatorInstruction
    {
      public:
        CompareInstruction( const char* name,
            const std::vector< Value* >& values,
            Value::ID id = Value::COMPARE_INSTRUCTION );

        static inline Value::ID classid( void )
        {
            return Value::COMPARE_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class LogicalInstruction : public OperatorInstruction
    {
      public:
        LogicalInstruction( const char* name,
            const std::vector< Value* >& values,
            Value::ID id = Value::LOGICAL_INSTRUCTION );

        static inline Value::ID classid( void )
        {
            return Value::LOGICAL_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class NopInstruction : public Instruction
    {
      public:
        NopInstruction( void );
        static inline Value::ID classid( void )
        {
            return Value::NOP_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class AllocInstruction : public Instruction
    {
      public:
        AllocInstruction( Type* type );
        static inline Value::ID classid( void )
        {
            return Value::ALLOC_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class IdInstruction : public Instruction, public UnaryInstruction
    {
      public:
        IdInstruction( Value* src );
        static inline Value::ID classid( void )
        {
            return Value::ID_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class LoadInstruction : public Instruction, public UnaryInstruction
    {
      public:
        LoadInstruction( Value* src );
        static inline Value::ID classid( void )
        {
            return Value::LOAD_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class ZeroExtendInstruction : public Instruction, public UnaryInstruction
    {
      public:
        ZeroExtendInstruction( Value* src, Type* type );
        static inline Value::ID classid( void )
        {
            return Value::ZEXT_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class TruncationInstruction : public Instruction, public UnaryInstruction
    {
      public:
        TruncationInstruction( Value* src, Type* type );
        static inline Value::ID classid( void )
        {
            return Value::TRUNC_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class StoreInstruction : public Instruction, public BinaryInstruction
    {
      public:
        StoreInstruction( Value* src, Value* dst );
        static inline Value::ID classid( void )
        {
            return Value::STORE_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class ExtractInstruction : public Instruction, public BinaryInstruction
    {
      public:
        ExtractInstruction( Value* ref, Value* element );
        static inline Value::ID classid( void )
        {
            return Value::EXTRACT_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class CastInstruction : public Instruction, public BinaryInstruction
    {
      public:
        CastInstruction( Value* kind, Value* src );
        static inline Value::ID classid( void )
        {
            return Value::CAST_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class CallInstruction : public Instruction
    {
      public:
        CallInstruction( Value* symbol );
        static inline Value::ID classid( void )
        {
            return Value::CALL_INSTRUCTION;
        };
        static bool classof( Value const* obj );

        Value& getCallee( void ) const;
    };

    class IdCallInstruction : public Instruction
    {
      public:
        IdCallInstruction( Value* kind, Value* symbol );
        static inline Value::ID classid( void )
        {
            return Value::ID_CALL_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class StreamInstruction : public Instruction
    {
      public:
        enum Channel
        {
            INPUT,
            OUTPUT,
            ERROR,
            WARNING
        };

      private:
        Channel channel;

      public:
        StreamInstruction( Channel channel );
        const Channel getChannel( void ) const;
        static inline Value::ID classid( void )
        {
            return Value::STREAM_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class AndInstruction : public ArithmeticInstruction,
                           public BinaryInstruction
    {
      public:
        AndInstruction( Value* lhs, Value* rhs );
        static inline Value::ID classid( void )
        {
            return Value::AND_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class OrInstruction : public ArithmeticInstruction, public BinaryInstruction
    {
      public:
        OrInstruction( Value* lhs, Value* rhs );
        static inline Value::ID classid( void )
        {
            return Value::OR_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class XorInstruction : public ArithmeticInstruction,
                           public BinaryInstruction
    {
      public:
        XorInstruction( Value* lhs, Value* rhs );
        static inline Value::ID classid( void )
        {
            return Value::XOR_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class AddSignedInstruction : public ArithmeticInstruction,
                                 public BinaryInstruction
    {
      public:
        AddSignedInstruction( Value* lhs, Value* rhs );
        static inline Value::ID classid( void )
        {
            return Value::ADDS_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class DivSignedInstruction : public ArithmeticInstruction,
                                 public BinaryInstruction
    {
      public:
        DivSignedInstruction( Value* lhs, Value* rhs );
        static inline Value::ID classid( void )
        {
            return Value::DIVS_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    // Class SubInstruction : public OperatorInstruction
    // {
    // public:
    // 	SubInstruction( Value* lhs, Value* rhs );
    // 	static bool classof( Value const* obj );
    // };

    // class MulInstruction : public OperatorInstruction
    // {
    // public:
    // 	MulInstruction( Value* lhs, Value* rhs );
    // 	static bool classof( Value const* obj );
    // };

    // class DivInstruction : public OperatorInstruction
    // {
    // public:
    // 	DivInstruction( Value* lhs, Value* rhs );
    // 	static bool classof( Value const* obj );
    // };

    // class RivInstruction : public OperatorInstruction
    // {
    // public:
    // 	RivInstruction( Value* lhs, Value* rhs );
    // 	static bool classof( Value const* obj );
    // };

    class ModUnsignedInstruction : public ArithmeticInstruction,
                                   public BinaryInstruction
    {
      public:
        ModUnsignedInstruction( Value* lhs, Value* rhs );
        static inline Value::ID classid( void )
        {
            return Value::MODU_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class EquInstruction : public CompareInstruction, public BinaryInstruction
    {
      public:
        EquInstruction( Value* lhs, Value* rhs );
        static inline Value::ID classid( void )
        {
            return Value::EQU_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    class NeqInstruction : public CompareInstruction, public BinaryInstruction
    {
      public:
        NeqInstruction( Value* lhs, Value* rhs );
        static inline Value::ID classid( void )
        {
            return Value::NEQ_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };

    // class LthInstruction : public OperatorInstruction
    // {
    // public:
    // 	LthInstruction( Value* lhs, Value* rhs );
    // 	static bool classof( Value const* obj );
    // };

    // class LeqInstruction : public OperatorInstruction
    // {
    // public:
    // 	LeqInstruction( Value* lhs, Value* rhs );
    // 	static bool classof( Value const* obj );
    // };

    // class GthInstruction : public OperatorInstruction
    // {
    // public:
    // 	GthInstruction( Value* lhs, Value* rhs );
    // 	static bool classof( Value const* obj );
    // };

    // class GeqInstruction : public OperatorInstruction
    // {
    // public:
    // 	GeqInstruction( Value* lhs, Value* rhs );
    // 	static bool classof( Value const* obj );
    // };

    // class MovInstruction : public UnaryInstruction
    // {
    // public:
    // 	MovInstruction( Value* lhs );
    // 	static bool classof( Value const* obj );
    // };

    class NotInstruction : public LogicalInstruction, public UnaryInstruction
    {
      public:
        NotInstruction( Value* lhs );
        static inline Value::ID classid( void )
        {
            return Value::NOT_INSTRUCTION;
        };
        static bool classof( Value const* obj );
    };
}

#endif /* _LIB_CSELIR_INSTRUCTION_H_ */

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
