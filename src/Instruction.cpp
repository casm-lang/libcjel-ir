//
//  Copyright (C) 2015-2019 CASM Organization <https://casm-lang.org>
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

#include "Instruction.h"

#include <libcjel-ir/Constant>
#include <libcjel-ir/Function>
#include <libcjel-ir/Reference>
#include <libcjel-ir/Statement>
#include <libcjel-ir/Structure>
#include <libcjel-ir/Type>
#include <libcjel-ir/Variable>

#include <libstdhl/Memory>

#include <cassert>

using namespace libcjel_ir;

Instruction::Instruction(
    const std::string& name,
    const Type::Ptr& type,
    const std::vector< Value::Ptr >& operands,
    Value::ID id )
: User( name, type, id )
, m_operands( operands )
{
}

void Instruction::setStatement( const Statement::Ptr& statement )
{
    m_statement = statement;

    for( auto operand : operands() )
    {
        if( not isa< Instruction >( operand ) )
        {
            continue;
        }

        auto instr = std::static_pointer_cast< Instruction >( operand );

        if( instr->statement() == 0 )
        {
            statement->add( instr );
        }

        if( *instr->statement() != *statement )
        {
            throw std::domain_error(
                "Instruction '" + operand->description() +
                "' does belong to a different Statement block" );
        }
    }
}

Statement::Ptr Instruction::statement( void ) const
{
    return m_statement.lock();
}

void Instruction::add( const Value::Ptr& value )
{
    assert( value );

    if( isa< UnaryInstruction >( this ) )
    {
        assert( m_operands.size() < 1 );
    }
    else if( isa< BinaryInstruction >( this ) )
    {
        assert( m_operands.size() < 2 );
    }

    m_operands.add( value );
}

Value::Ptr Instruction::operand( u8 position ) const
{
    if( position >= m_operands.size() )
    {
        throw std::domain_error(
            "operand position '" + std::to_string( position ) + "' does not exist!" );
    }

    return m_operands[ position ];
}

Values Instruction::operands( void ) const
{
    return m_operands;
}

u1 Instruction::classof( Value const* obj )
{
    return obj->id() == classid() or UnaryInstruction::classof( obj ) or
           BinaryInstruction::classof( obj ) or OperatorInstruction::classof( obj ) or
           NopInstruction::classof( obj ) or AllocInstruction::classof( obj ) or
           CallInstruction::classof( obj ) or IdCallInstruction::classof( obj ) or
           StreamInstruction::classof( obj );
}

UnaryInstruction::UnaryInstruction( Instruction* self )
: m_self( *self )
{
}

Value::Ptr UnaryInstruction::get( void ) const
{
    assert( m_self.operands().size() == 1 );
    return m_self.operand( 0 );
}

u1 UnaryInstruction::classof( Value const* obj )
{
    return obj->id() == classid() or IdInstruction::classof( obj ) or
           LoadInstruction::classof( obj ) or ZeroExtendInstruction::classof( obj ) or
           TruncationInstruction::classof( obj ) or NotInstruction::classof( obj );
}

BinaryInstruction::BinaryInstruction( Instruction* self )
: m_self( *self )
{
}

Value::Ptr BinaryInstruction::lhs( void ) const
{
    assert( m_self.operands().size() == 2 );
    return m_self.operand( 0 );
}

Value::Ptr BinaryInstruction::rhs( void ) const
{
    assert( m_self.operands().size() == 2 );
    return m_self.operand( 1 );
}

u1 BinaryInstruction::classof( Value const* obj )
{
    return obj->id() == classid() or StoreInstruction::classof( obj ) or
           ExtractInstruction::classof( obj ) or CastInstruction::classof( obj ) or
           AndInstruction::classof( obj ) or OrInstruction::classof( obj ) or
           XorInstruction::classof( obj ) or AddUnsignedInstruction::classof( obj ) or
           AddSignedInstruction::classof( obj ) or DivSignedInstruction::classof( obj ) or
           ModUnsignedInstruction::classof( obj ) or EquInstruction::classof( obj ) or
           NeqInstruction::classof( obj );
}

OperatorInstruction::OperatorInstruction(
    const std::string& name,
    const Type::Ptr& type,
    const std::vector< Value::Ptr >& values,
    Value::ID id )
: Instruction( name, type, values, id )
{
    if( isa< BinaryInstruction >( this ) )
    {
        assert( operand( 0 )->type() == operand( 1 )->type() );
    }
}

u1 OperatorInstruction::classof( Value const* obj )
{
    return obj->id() == classid() or ArithmeticInstruction::classof( obj ) or
           LogicalInstruction::classof( obj ) or CompareInstruction::classof( obj );
}

ArithmeticInstruction::ArithmeticInstruction(
    const std::string& name, const std::vector< Value::Ptr >& values, Value::ID id )
: OperatorInstruction(
      name, ( values.size() > 0 and values[ 0 ] ) ? values[ 0 ]->ptr_type() : nullptr, values, id )
{
}

u1 ArithmeticInstruction::classof( Value const* obj )
{
    return obj->id() == classid() or NotInstruction::classof( obj ) or
           AndInstruction::classof( obj ) or OrInstruction::classof( obj ) or
           XorInstruction::classof( obj ) or AddUnsignedInstruction::classof( obj ) or
           AddSignedInstruction::classof( obj ) or DivSignedInstruction::classof( obj ) or
           ModUnsignedInstruction::classof( obj );
}

LogicalInstruction::LogicalInstruction(
    const std::string& name, const std::vector< Value::Ptr >& values, Value::ID id )
: OperatorInstruction( name, libstdhl::Memory::get< BitType >( 1 ), values, id )
{
}

u1 LogicalInstruction::classof( Value const* obj )
{
    return obj->id() == classid() or LnotInstruction::classof( obj );
}

CompareInstruction::CompareInstruction(
    const std::string& name, const std::vector< Value::Ptr >& values, Value::ID id )
: OperatorInstruction( name, libstdhl::Memory::get< BitType >( 1 ), values, id )
{
}

u1 CompareInstruction::classof( Value const* obj )
{
    return obj->id() == classid() or EquInstruction::classof( obj ) or
           NeqInstruction::classof( obj );
}

// -----------------------------------------------------------------------------
// INSTRUCTIONS
// -----------------------------------------------------------------------------

NopInstruction::NopInstruction( void )
: Instruction( "nop", libstdhl::Memory::get< VoidType >(), {}, classid() )
{
}

u1 NopInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

AllocInstruction::AllocInstruction( const Type::Ptr& type )
: Instruction( "alloc", type, {}, classid() )
{
}

u1 AllocInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

CallInstruction::CallInstruction(
    const Value::Ptr& symbol, const std::vector< Value::Ptr >& operands )
: Instruction( "call", symbol->type().ptr_results()[ 0 ], { symbol }, classid() )
{
    assert( isa< CallableUnit >( symbol ) and symbol->type().isRelation() );

    if( symbol->type().results().size() != 1 )
    {
        assert( !" unimplemented call instruction result type behavior! " );
    }

    for( auto operand : operands )
    {
        add( operand );
    }
}

Value::Ptr CallInstruction::callee( void ) const
{
    return operand( 0 );
}

u1 CallInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

IdCallInstruction::IdCallInstruction( const Value::Ptr& kind, const Value::Ptr& symbol )
: Instruction( "icall", 0, { kind, symbol }, classid() )
{
    assert( kind );
    assert( symbol );

    assert( symbol->type() == *libstdhl::Memory::get< BitType >( 64 ) );
}

u1 IdCallInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

StreamInstruction::StreamInstruction( Channel channel )
: Instruction( "stream", 0, {}, classid() )
, m_channel( channel )
{
}

StreamInstruction::Channel StreamInstruction::channel( void ) const
{
    return m_channel;
}

u1 StreamInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

// -----------------------------------------------------------------------------
// UNARY INSTRUCTIONS
// -----------------------------------------------------------------------------

IdInstruction::IdInstruction( const Value::Ptr& src )
: Instruction( "id", libstdhl::Memory::get< BitType >( 64 ), { src }, classid() )
, UnaryInstruction( this )
{
    assert( src );
    assert( isa< Variable >( src ) or isa< Function >( src ) );
}

u1 IdInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

LoadInstruction::LoadInstruction( const Value::Ptr& src )
: Instruction( "load", src ? src->ptr_type() : nullptr, { src }, classid() )
, UnaryInstruction( this )
{
}

u1 LoadInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

ZeroExtendInstruction::ZeroExtendInstruction( const Value::Ptr& src, const Type::Ptr& type )
: Instruction( "zext", type, { src }, classid() )
, UnaryInstruction( this )
{
}

u1 ZeroExtendInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

TruncationInstruction::TruncationInstruction( const Value::Ptr& src, const Type::Ptr& type )
: Instruction( "trunc", type, { src }, classid() )
, UnaryInstruction( this )
{
}

u1 TruncationInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

// -----------------------------------------------------------------------------
// BINARY INSTRUCTIONS
// -----------------------------------------------------------------------------

StoreInstruction::StoreInstruction( const Value::Ptr& src, const Value::Ptr& dst )
: Instruction( "store", libstdhl::Memory::get< VoidType >(), { src, dst }, classid() )
, BinaryInstruction( this )
{
    assert( src->type() == dst->type() );
    assert( src->type().isBit() );  // PPA: only bit type for now!
}

u1 StoreInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

ExtractInstruction::ExtractInstruction( const Value::Ptr& src, const Value::Ptr& dst )
: Instruction(
      "extract",
      ( src and dst
            ? ( isa< Reference >( src ) and src->type().isStructure() and isa< BitConstant >( dst )
                    ? src->type().ptr_results()
                          [ std::static_pointer_cast< BitConstant >( dst )->value().value() ]
                    : nullptr )
            : nullptr ),
      { src, dst },
      classid() )
, BinaryInstruction( this )
{
    // TODO: IDEA: FIXME: PPA: possible check to implement if 'dst' is inside
    // 'src'
}

u1 ExtractInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

CastInstruction::CastInstruction( const Value::Ptr& kind, const Value::Ptr& src )
: Instruction( "cast", ( kind ? kind->ptr_type() : nullptr ), { kind, src }, classid() )
, BinaryInstruction( this )
{
    if( ( not isa< CallableUnit >( kind ) ) and ( not isa< Structure >( kind ) ) )
    {
        assert( !" unimplemented/unsupported CastInstr type! " );
    }
}

u1 CastInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

// -----------------------------------------------------------------------------
// ARITHMETIC INSTRUCTIONS
// -----------------------------------------------------------------------------

NotInstruction::NotInstruction( const Value::Ptr& lhs )
: ArithmeticInstruction( "not", { lhs }, classid() )
, UnaryInstruction( this )
{
}

u1 NotInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

AndInstruction::AndInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: ArithmeticInstruction( "and", { lhs, rhs }, classid() )
, BinaryInstruction( this )
{
}

u1 AndInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

OrInstruction::OrInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: ArithmeticInstruction( "or", { lhs, rhs }, classid() )
, BinaryInstruction( this )
{
}

u1 OrInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

XorInstruction::XorInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: ArithmeticInstruction( "xor", { lhs, rhs }, classid() )
, BinaryInstruction( this )
{
}

u1 XorInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

AddUnsignedInstruction::AddUnsignedInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: ArithmeticInstruction( "addu", { lhs, rhs }, classid() )
, BinaryInstruction( this )
{
}

u1 AddUnsignedInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

AddSignedInstruction::AddSignedInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: ArithmeticInstruction( "adds", { lhs, rhs }, classid() )
, BinaryInstruction( this )
{
}

u1 AddSignedInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

DivSignedInstruction::DivSignedInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: ArithmeticInstruction( "divs", { lhs, rhs }, classid() )
, BinaryInstruction( this )
{
}

u1 DivSignedInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

ModUnsignedInstruction::ModUnsignedInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: ArithmeticInstruction( "modu", { lhs, rhs }, classid() )
, BinaryInstruction( this )
{
}

u1 ModUnsignedInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

// -----------------------------------------------------------------------------
// LOGICAL INSTRUCTIONS
// -----------------------------------------------------------------------------

LnotInstruction::LnotInstruction( const Value::Ptr& lhs )
: LogicalInstruction( "lnot", { lhs }, classid() )
, UnaryInstruction( this )
{
}

u1 LnotInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

// -----------------------------------------------------------------------------
// COMPARE INSTRUCTIONS
// -----------------------------------------------------------------------------

EquInstruction::EquInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: CompareInstruction( "equ", { lhs, rhs }, classid() )
, BinaryInstruction( this )
{
}

u1 EquInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

NeqInstruction::NeqInstruction( const Value::Ptr& lhs, const Value::Ptr& rhs )
: CompareInstruction( "neq", { lhs, rhs }, classid() )
, BinaryInstruction( this )
{
}

u1 NeqInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
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
