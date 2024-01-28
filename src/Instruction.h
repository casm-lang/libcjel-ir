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

#ifndef _LIBCJEL_IR_INSTRUCTION_H_
#define _LIBCJEL_IR_INSTRUCTION_H_

#include <libcjel-ir/User>

namespace libcjel_ir
{
    class Statement;

    class Instruction : public User
    {
      public:
        using Ptr = std::shared_ptr< Instruction >;

        Instruction(
            const std::string& name,
            const Type::Ptr& type,
            const std::vector< Value::Ptr >& operands,
            Value::ID id = classid() );

        void add( const Value::Ptr& operand );

        Value::Ptr operand( u8 position ) const;

        Values operands( void ) const;

        // void replace(...) // TODO: PPA:

        void setStatement( const std::shared_ptr< Statement >& statement );

        std::shared_ptr< Statement > statement( void ) const;

        std::size_t hash( void ) const override;

        static inline Value::ID classid( void )
        {
            return Value::INSTRUCTION;
        }

        static bool classof( Value const* obj );

      private:
        Values m_operands;

        std::weak_ptr< Statement > m_statement;
    };

    using Instructions = libstdhl::List< Instruction >;

    class UnaryInstruction
    {
      public:
        UnaryInstruction( Instruction* self );

        Value::Ptr get( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::UNARY_INSTRUCTION;
        }

        static bool classof( Value const* obj );

      private:
        Instruction& m_self;
    };

    class BinaryInstruction
    {
      public:
        BinaryInstruction( Instruction* self );

        Value::Ptr lhs( void ) const;
        Value::Ptr rhs( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::BINARY_INSTRUCTION;
        }

        static bool classof( Value const* obj );

      private:
        Instruction& m_self;
    };

    class OperatorInstruction : public Instruction
    {
      public:
        using Ptr = std::shared_ptr< OperatorInstruction >;

        OperatorInstruction(
            const std::string& name,
            const Type::Ptr& type,
            const std::vector< Value::Ptr >& values,
            Value::ID id = classid() );

        static inline Value::ID classid( void )
        {
            return Value::OPERATOR_INSTRUCTION;
        }

        static bool classof( Value const* obj );
    };

    class ArithmeticInstruction : public OperatorInstruction
    {
      public:
        using Ptr = std::shared_ptr< ArithmeticInstruction >;

        ArithmeticInstruction(
            const std::string& name,
            const std::vector< Value::Ptr >& values,
            Value::ID id = classid() );

        static inline Value::ID classid( void )
        {
            return Value::ARITHMETIC_INSTRUCTION;
        }

        static bool classof( Value const* obj );
    };

    class CompareInstruction : public OperatorInstruction
    {
      public:
        using Ptr = std::shared_ptr< CompareInstruction >;

        CompareInstruction(
            const std::string& name,
            const std::vector< Value::Ptr >& values,
            Value::ID id = classid() );

        static inline Value::ID classid( void )
        {
            return Value::COMPARE_INSTRUCTION;
        }

        static bool classof( Value const* obj );
    };

    class LogicalInstruction : public OperatorInstruction
    {
      public:
        using Ptr = std::shared_ptr< LogicalInstruction >;

        LogicalInstruction(
            const std::string& name,
            const std::vector< Value::Ptr >& values,
            Value::ID id = classid() );

        static inline Value::ID classid( void )
        {
            return Value::LOGICAL_INSTRUCTION;
        }

        static bool classof( Value const* obj );
    };

    class NopInstruction : public Instruction
    {
      public:
        using Ptr = std::shared_ptr< NopInstruction >;

        NopInstruction( void );

        static inline Value::ID classid( void )
        {
            return Value::NOP_INSTRUCTION;
        }

        static bool classof( Value const* obj );
    };

    class AllocInstruction : public Instruction
    {
      public:
        using Ptr = std::shared_ptr< AllocInstruction >;

        AllocInstruction( const Type::Ptr& type );

        static inline Value::ID classid( void )
        {
            return Value::ALLOC_INSTRUCTION;
        }

        static bool classof( Value const* obj );
    };

    class IdInstruction
    : public Instruction
    , public UnaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< IdInstruction >;

        IdInstruction( const Value::Ptr& src );

        static inline Value::ID classid( void )
        {
            return Value::ID_INSTRUCTION;
        }

        static bool classof( Value const* obj );
    };

    class LoadInstruction
    : public Instruction
    , public UnaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< LoadInstruction >;

        LoadInstruction( const Value::Ptr& src );

        static inline Value::ID classid( void )
        {
            return Value::LOAD_INSTRUCTION;
        }

        static bool classof( Value const* obj );
    };

    class ZeroExtendInstruction
    : public Instruction
    , public UnaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< ZeroExtendInstruction >;

        ZeroExtendInstruction( const Value::Ptr& src, const Type::Ptr& type );

        static inline Value::ID classid( void )
        {
            return Value::ZEXT_INSTRUCTION;
        }

        static bool classof( Value const* obj );
    };

    class TruncationInstruction
    : public Instruction
    , public UnaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< TruncationInstruction >;

        TruncationInstruction( const Value::Ptr& src, const Type::Ptr& type );

        static inline Value::ID classid( void )
        {
            return Value::TRUNC_INSTRUCTION;
        }

        static bool classof( Value const* obj );
    };

    class StoreInstruction
    : public Instruction
    , public BinaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< StoreInstruction >;

        StoreInstruction( const Value::Ptr& src, const Value::Ptr& dst );

        static inline Value::ID classid( void )
        {
            return Value::STORE_INSTRUCTION;
        }

        static bool classof( Value const* obj );
    };

    class ExtractInstruction
    : public Instruction
    , public BinaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< ExtractInstruction >;

        ExtractInstruction( const Value::Ptr& src, const Value::Ptr& element );

        static inline Value::ID classid( void )
        {
            return Value::EXTRACT_INSTRUCTION;
        }

        static bool classof( Value const* obj );
    };

    class CastInstruction
    : public Instruction
    , public BinaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< CastInstruction >;

        CastInstruction( const Value::Ptr& kind, const Value::Ptr& src );

        static inline Value::ID classid( void )
        {
            return Value::CAST_INSTRUCTION;
        }

        static bool classof( Value const* obj );
    };

    class CallInstruction : public Instruction
    {
      public:
        using Ptr = std::shared_ptr< CallInstruction >;

        CallInstruction( const Value::Ptr& symbol, const std::vector< Value::Ptr >& operands = {} );

        Value::Ptr callee( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::CALL_INSTRUCTION;
        }

        static bool classof( Value const* obj );
    };

    class IdCallInstruction : public Instruction
    {
      public:
        using Ptr = std::shared_ptr< IdCallInstruction >;

        IdCallInstruction( const Value::Ptr& kind, const Value::Ptr& symbol );

        static inline Value::ID classid( void )
        {
            return Value::ID_CALL_INSTRUCTION;
        }

        static bool classof( Value const* obj );
    };

    class StreamInstruction : public Instruction
    {
      public:
        using Ptr = std::shared_ptr< StreamInstruction >;

        enum Channel
        {
            INPUT,
            OUTPUT,
            ERROR,
            WARNING
        };

        StreamInstruction( Channel channel );

        Channel channel( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::STREAM_INSTRUCTION;
        }

        static bool classof( Value const* obj );

      private:
        Channel m_channel;
    };

    class NotInstruction
    : public ArithmeticInstruction
    , public UnaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< NotInstruction >;

        NotInstruction( const Value::Ptr& lhs );

        static inline Value::ID classid( void )
        {
            return Value::NOT_INSTRUCTION;
        }

        static bool classof( Value const* obj );
    };

    class AndInstruction
    : public ArithmeticInstruction
    , public BinaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< AndInstruction >;

        AndInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        static inline Value::ID classid( void )
        {
            return Value::AND_INSTRUCTION;
        }

        static bool classof( Value const* obj );
    };

    class OrInstruction
    : public ArithmeticInstruction
    , public BinaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< OrInstruction >;

        OrInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        static inline Value::ID classid( void )
        {
            return Value::OR_INSTRUCTION;
        }

        static bool classof( Value const* obj );
    };

    class XorInstruction
    : public ArithmeticInstruction
    , public BinaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< XorInstruction >;

        XorInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        static inline Value::ID classid( void )
        {
            return Value::XOR_INSTRUCTION;
        }

        static bool classof( Value const* obj );
    };

    class AddUnsignedInstruction
    : public ArithmeticInstruction
    , public BinaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< AddUnsignedInstruction >;

        AddUnsignedInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        static inline Value::ID classid( void )
        {
            return Value::ADDU_INSTRUCTION;
        }

        static bool classof( Value const* obj );
    };

    class AddSignedInstruction
    : public ArithmeticInstruction
    , public BinaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< AddSignedInstruction >;

        AddSignedInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        static inline Value::ID classid( void )
        {
            return Value::ADDS_INSTRUCTION;
        }

        static bool classof( Value const* obj );
    };

    class DivSignedInstruction
    : public ArithmeticInstruction
    , public BinaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< DivSignedInstruction >;

        DivSignedInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        static inline Value::ID classid( void )
        {
            return Value::DIVS_INSTRUCTION;
        }

        static bool classof( Value const* obj );
    };

    class ModUnsignedInstruction
    : public ArithmeticInstruction
    , public BinaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< ModUnsignedInstruction >;

        ModUnsignedInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        static inline Value::ID classid( void )
        {
            return Value::MODU_INSTRUCTION;
        }

        static bool classof( Value const* obj );
    };

    //
    //
    //

    class EquInstruction
    : public CompareInstruction
    , public BinaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< EquInstruction >;

        EquInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        static inline Value::ID classid( void )
        {
            return Value::EQU_INSTRUCTION;
        }

        static bool classof( Value const* obj );
    };

    class NeqInstruction
    : public CompareInstruction
    , public BinaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< NeqInstruction >;

        NeqInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs );

        static inline Value::ID classid( void )
        {
            return Value::NEQ_INSTRUCTION;
        }

        static bool classof( Value const* obj );
    };

    //
    //
    //

    class LnotInstruction
    : public LogicalInstruction
    , public UnaryInstruction
    {
      public:
        using Ptr = std::shared_ptr< LnotInstruction >;

        LnotInstruction( const Value::Ptr& lhs );

        static inline Value::ID classid( void )
        {
            return Value::LNOT_INSTRUCTION;
        }

        static bool classof( Value const* obj );
    };
}

#endif  // _LIBCJEL_IR_INSTRUCTION_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
