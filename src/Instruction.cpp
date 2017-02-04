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

#include "Instruction.h"

#include "Reference.h"
#include "Type.h"

#include "../stdhl/cpp/Default.h"
#include "../stdhl/cpp/Log.h"

using namespace libcsel_ir;

Instruction::Instruction( const char* name, Type* type,
    const std::vector< Value* >& values, Value::ID id )
: User( name, type, id )
, m_values( values )
, m_statement( 0 )
{
}

Instruction::~Instruction( void )
{
}

void Instruction::setStatement( Statement* stmt )
{
    m_statement = stmt;

    for( Value* value : m_values )
    {
        if( not isa< Instruction >( value ) )
        {
            continue;
        }

        Instruction* instr = (Instruction*)value;

        if( instr->statement() == 0 )
        {
            stmt->add( value );
        }

        if( instr->statement() != stmt )
        {
            libstdhl::Log::warning(
                "warning: %s:%i: Instruction %p does belong to a different "
                "Statement block\n",
                __FILE__, __LINE__, value );
            // assert(0);
        }
    }
}

Statement* Instruction::statement( void ) const
{
    return m_statement;
}

void Instruction::add( Value* value )
{
    assert( value );

    if( isa< UnaryInstruction >( this ) )
    {
        assert( m_values.size() < 1 );
    }
    else if( isa< BinaryInstruction >( this ) )
    {
        assert( m_values.size() < 2 );
    }

    m_values.push_back( value );
}

Value* Instruction::value( u8 index ) const
{
    // TODO: IDEA: assert( type() ); // to force the type check!
    assert( index < m_values.size() );
    return m_values[ index ];
}

const std::vector< Value* >& Instruction::values( void ) const
{
    // TODO: IDEA: assert( type() ); // to force the type check!
    return m_values;
}

bool Instruction::classof( Value const* obj )
{
    return obj->id() == classid() or UnaryInstruction::classof( obj )
           or BinaryInstruction::classof( obj )
           or OperatorInstruction::classof( obj )
           or NopInstruction::classof( obj ) or AllocInstruction::classof( obj )
           or CallInstruction::classof( obj )
           or IdCallInstruction::classof( obj )
           or StreamInstruction::classof( obj );
}

UnaryInstruction::UnaryInstruction( Instruction* self )
: m_self( *self )
{
}

Value* UnaryInstruction::get( void ) const
{
    assert( m_self.values().size() == 1 );
    return m_self.value( 0 );
}

bool UnaryInstruction::classof( Value const* obj )
{
    return obj->id() == classid() or IdInstruction::classof( obj )
           or LoadInstruction::classof( obj )
           or ZeroExtendInstruction::classof( obj )
           or TruncationInstruction::classof( obj )
           or NotInstruction::classof( obj );
}

BinaryInstruction::BinaryInstruction( Instruction* self )
: m_self( *self )
{
}

Value* BinaryInstruction::lhs( void ) const
{
    assert( m_self.values().size() == 2 );
    return m_self.value( 0 );
}

Value* BinaryInstruction::rhs( void ) const
{
    assert( m_self.values().size() == 2 );
    return m_self.value( 1 );
}

bool BinaryInstruction::classof( Value const* obj )
{
    return obj->id() == classid() or StoreInstruction::classof( obj )
           or ExtractInstruction::classof( obj )
           or CastInstruction::classof( obj ) or AndInstruction::classof( obj )
           or OrInstruction::classof( obj ) or XorInstruction::classof( obj )
           or AddSignedInstruction::classof( obj )
           or DivSignedInstruction::classof( obj )
           or ModUnsignedInstruction::classof( obj )
           or EquInstruction::classof( obj ) or NeqInstruction::classof( obj );
}

OperatorInstruction::OperatorInstruction( const char* name, Type* type,
    const std::vector< Value* >& values, Value::ID id )
: Instruction( name, type, values, id )
{
    Type& lhs_ty = value( 0 )->type();
    Type& rhs_ty = value( 1 )->type();

    assert( lhs_ty == rhs_ty );
}

bool OperatorInstruction::classof( Value const* obj )
{
    return obj->id() == classid() or ArithmeticInstruction::classof( obj )
           or LogicalInstruction::classof( obj )
           or CompareInstruction::classof( obj );
}

ArithmeticInstruction::ArithmeticInstruction(
    const char* name, const std::vector< Value* >& values, Value::ID id )
: OperatorInstruction( name, 0, values, id )
{
    setType( value( 0 )->type() );
}

bool ArithmeticInstruction::classof( Value const* obj )
{
    return obj->id() == classid() or AndInstruction::classof( obj )
           or OrInstruction::classof( obj ) or XorInstruction::classof( obj )
           or AddSignedInstruction::classof( obj )
           or DivSignedInstruction::classof( obj )
           or ModUnsignedInstruction::classof( obj );
}

LogicalInstruction::LogicalInstruction(
    const char* name, const std::vector< Value* >& values, Value::ID id )
: OperatorInstruction( name, Type::Bit( 1 ), values, id )
{
}

bool LogicalInstruction::classof( Value const* obj )
{
    return obj->id() == classid() or NotInstruction::classof( obj );
}

CompareInstruction::CompareInstruction(
    const char* name, const std::vector< Value* >& values, Value::ID id )
: OperatorInstruction( name, Type::Bit( 1 ), values, id )
{
}

bool CompareInstruction::classof( Value const* obj )
{
    return obj->id() == classid() or EquInstruction::classof( obj )
           or NeqInstruction::classof( obj );
}

// -----------------------------------------------------------------------------
// INSTRUCTIONS
// -----------------------------------------------------------------------------

NopInstruction::NopInstruction( void )
: Instruction( "nop", Type::Void(), {}, classid() )
{
}
bool NopInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

AllocInstruction::AllocInstruction( Type* type )
: Instruction( "alloc", type, {}, classid() )
{
}
bool AllocInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

CallInstruction::CallInstruction( Value* symbol )
: Instruction( "call", Type::Void(), { symbol }, classid() )
{
    assert( symbol );

    assert( isa< CallableUnit >( symbol ) and symbol->type().isRelation() );

    auto& res_tys = symbol->type().results();

    if( res_tys.size() == 1 )
    {
        setType( *res_tys[ 0 ] );
    }
    else
    {
        assert( not" unimplemented call instruction result type behavior! " );
    }

    // assert( isa< CallableUnit >( symbol ) or isa< CastInstruction >( symbol )
    // );
    // if( isa< CastInstruction >( symbol ) )
    // {
    //     assert( symbol->type().id() == Type::RELATION );
    // }
}

Value& CallInstruction::callee( void ) const
{
    return *value( 0 );
}

bool CallInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

IdCallInstruction::IdCallInstruction( Value* kind, Value* symbol )
: Instruction( "icall", 0, { kind, symbol }, classid() )
{
    assert( kind );
    assert( symbol );

    assert( strcmp( symbol->type().name(), Type::TypeID()->name() ) == 0 );
}
bool IdCallInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

StreamInstruction::StreamInstruction( Channel channel )
: Instruction( "stream", 0, {}, classid() )
, m_channel( channel )
{
}
const StreamInstruction::Channel StreamInstruction::channel( void ) const
{
    return m_channel;
}
bool StreamInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

// -----------------------------------------------------------------------------
// UNARY INSTRUCTIONS
// -----------------------------------------------------------------------------

IdInstruction::IdInstruction( Value* src )
: Instruction( "id", Type::TypeID(), { src }, classid() )
, UnaryInstruction( this )
{
    assert( src );
    assert( isa< Variable >( src ) or isa< Function >( src ) );
}
bool IdInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

LoadInstruction::LoadInstruction( Value* src )
: Instruction( "load", 0, { src }, classid() )
, UnaryInstruction( this )
{
    setType( src->type() );
}
bool LoadInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

ZeroExtendInstruction::ZeroExtendInstruction( Value* src, Type* type )
: Instruction( "zext", type, { src }, classid() )
, UnaryInstruction( this )
{
}
bool ZeroExtendInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

TruncationInstruction::TruncationInstruction( Value* src, Type* type )
: Instruction( "trunc", type, { src }, classid() )
, UnaryInstruction( this )
{
}
bool TruncationInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

// -----------------------------------------------------------------------------
// BINARY INSTRUCTIONS
// -----------------------------------------------------------------------------

StoreInstruction::StoreInstruction( Value* src, Value* dst )
: Instruction( "store", Type::Void(), { src, dst }, classid() )
, BinaryInstruction( this )
{
    assert( src->type() == dst->type() );
    
    assert( src->type().isBit() ); // PPA: only bit type for now!
}
bool StoreInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

ExtractInstruction::ExtractInstruction( Value* src, Value* dst )
: Instruction( "extract", 0, { src, dst }, classid() )
, BinaryInstruction( this )
{
    // TODO: IDEA: FIXME: PPA: possible check to implement if 'dst' is inside
    // 'src'

    if( isa< Reference >( src ) )
    {
        if( src->type().isStructure() )
        {
            StructureType& ty = static_cast< StructureType& >( src->type() );
            BitConstant* c = cast< BitConstant >( dst );

            setType( *ty.results()[ c->value() ] );
        }
        else
        {
            assert( 0 );
        }
    }
    else
    {
        assert( 0 );
        setType( dst->type() );
    }
}
bool ExtractInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

CastInstruction::CastInstruction( Value* kind, Value* src )
: Instruction( "cast", Type::TypeID(), { kind, src }, classid() )
, BinaryInstruction( this )
{
    if( ( not isa< CallableUnit >( kind ) )
        and ( not isa< Structure >( kind ) ) )
    {
        assert( !" unimplemented/unsupported CastInstr type! " );
    }

    setType( kind->type() );
}
bool CastInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

// -----------------------------------------------------------------------------
// ARITHMETIC INSTRUCTIONS
// -----------------------------------------------------------------------------

AndInstruction::AndInstruction( Value* lhs, Value* rhs )
: ArithmeticInstruction( "and", { lhs, rhs }, classid() )
, BinaryInstruction( this )
{
}
bool AndInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

OrInstruction::OrInstruction( Value* lhs, Value* rhs )
: ArithmeticInstruction( "or", { lhs, rhs }, classid() )
, BinaryInstruction( this )
{
}
bool OrInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

XorInstruction::XorInstruction( Value* lhs, Value* rhs )
: ArithmeticInstruction( "xor", { lhs, rhs }, classid() )
, BinaryInstruction( this )
{
}
bool XorInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

AddSignedInstruction::AddSignedInstruction( Value* lhs, Value* rhs )
: ArithmeticInstruction( "adds", { lhs, rhs }, classid() )
, BinaryInstruction( this )
{
}
bool AddSignedInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

DivSignedInstruction::DivSignedInstruction( Value* lhs, Value* rhs )
: ArithmeticInstruction( "divs", { lhs, rhs }, classid() )
, BinaryInstruction( this )
{
}
bool DivSignedInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

ModUnsignedInstruction::ModUnsignedInstruction( Value* lhs, Value* rhs )
: ArithmeticInstruction( "modu", { lhs, rhs }, classid() )
, BinaryInstruction( this )
{
}
bool ModUnsignedInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

// -----------------------------------------------------------------------------
// LOGICAL INSTRUCTIONS
// -----------------------------------------------------------------------------

NotInstruction::NotInstruction( Value* lhs )
: LogicalInstruction( "lnot", { lhs }, classid() )
, UnaryInstruction( this )
{
    assert( type() == get()->type() );
}
bool NotInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

// -----------------------------------------------------------------------------
// COMPARE INSTRUCTIONS
// -----------------------------------------------------------------------------

EquInstruction::EquInstruction( Value* lhs, Value* rhs )
: CompareInstruction( "equ", { lhs, rhs }, classid() )
, BinaryInstruction( this )
{
}
bool EquInstruction::classof( Value const* obj )
{
    return obj->id() == classid();
}

NeqInstruction::NeqInstruction( Value* lhs, Value* rhs )
: CompareInstruction( "neq", { lhs, rhs }, classid() )
, BinaryInstruction( this )
{
}
bool NeqInstruction::classof( Value const* obj )
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
