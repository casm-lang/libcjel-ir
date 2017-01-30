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

using namespace libcsel_ir;

Instruction::Instruction( const char* name, Type* type,
    const std::vector< Value* >& values, Value::ID id )
: User( name, type, id )
, values( values )
, statement( 0 )
{
    ( *Value::getSymbols() )[ ".instruction" ].insert( this );
}

Instruction::~Instruction( void )
{
    ( *Value::getSymbols() )[ ".instruction" ].erase( this );
}

void Instruction::setStatement( Statement* stmt )
{
    statement = stmt;

    for( Value* value : values )
    {
        if( not isa< Instruction >( value ) )
        {
            continue;
        }

        Instruction* instr = (Instruction*)value;

        if( instr->getStatement() == 0 )
        {
            stmt->add( value );
        }

        if( instr->getStatement() != stmt )
        {
            fprintf( stderr,
                "warning: %s:%i: Instruction %p does belong to a different "
                "Statement block\n",
                __FUNCTION__, __LINE__, value );
            // assert(0);
        }
    }
}

Statement* Instruction::getStatement( void ) const
{
    return statement;
}

void Instruction::add( Value* value )
{
    assert( value );

    if( isa< UnaryInstruction >( this ) )
    {
        assert( values.size() < 1 );
    }
    else if( isa< BinaryInstruction >( this ) )
    {
        assert( values.size() < 2 );
    }

    values.push_back( value );
}

Value* Instruction::getValue( u8 index ) const
{
    // TODO: IDEA: assert( getType() ); // to force the type check!
    assert( index < values.size() );
    return values[ index ];
}

const std::vector< Value* >& Instruction::getValues( void ) const
{
    // TODO: IDEA: assert( getType() ); // to force the type check!
    return values;
}

void Instruction::dump( void ) const
{
    printf( "[Instr] %p %s ", this, &getName()[ 1 ] );
    u1 flag = 0;
    for( auto instr : values )
    {
        if( flag )
        {
            printf( ", " );
        }
        else
        {
            flag = 1;
        }
        printf( "%p", instr );
    }
    printf( "\n" );
}

bool Instruction::classof( Value const* obj )
{
    return obj->getValueID() == classid() or UnaryInstruction::classof( obj )
           or BinaryInstruction::classof( obj )
           or OperatorInstruction::classof( obj )
           or NopInstruction::classof( obj ) or AllocInstruction::classof( obj )
           or CallInstruction::classof( obj )
           or IdCallInstruction::classof( obj )
           or StreamInstruction::classof( obj );
}

UnaryInstruction::UnaryInstruction( Instruction* self )
: self( *self )
{
}

Value* UnaryInstruction::get( void ) const
{
    return self.getValue( 0 );
}

bool UnaryInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid() or IdInstruction::classof( obj )
           or LoadInstruction::classof( obj )
           or ZeroExtendInstruction::classof( obj )
           or TruncationInstruction::classof( obj )
           or NotInstruction::classof( obj );
}

BinaryInstruction::BinaryInstruction( Instruction* self )
: self( *self )
{
}

Value* BinaryInstruction::getLHS( void ) const
{
    return self.getValue( 0 );
}

Value* BinaryInstruction::getRHS( void ) const
{
    return self.getValue( 1 );
}

bool BinaryInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid() or StoreInstruction::classof( obj )
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
    Type* lhs_ty = getValue( 0 )->getType();
    Type* rhs_ty = getValue( 1 )->getType();

    assert( lhs_ty );
    assert( rhs_ty );
    assert( lhs_ty->getID() == rhs_ty->getID() );
}

bool OperatorInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid()
           or ArithmeticInstruction::classof( obj )
           or LogicalInstruction::classof( obj )
           or CompareInstruction::classof( obj );
}

ArithmeticInstruction::ArithmeticInstruction(
    const char* name, const std::vector< Value* >& values, Value::ID id )
: OperatorInstruction( name, 0, values, id )
{
    assert( !getType() );
    setType( getValue( 0 )->getType() );
}

bool ArithmeticInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid() or AndInstruction::classof( obj )
           or OrInstruction::classof( obj ) or XorInstruction::classof( obj )
           or AddSignedInstruction::classof( obj )
           or DivSignedInstruction::classof( obj )
           or ModUnsignedInstruction::classof( obj );
}

LogicalInstruction::LogicalInstruction(
    const char* name, const std::vector< Value* >& values, Value::ID id )
: OperatorInstruction( name, Type::getBit( 1 ), values, id )
{
}

bool LogicalInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid() or NotInstruction::classof( obj );
}

CompareInstruction::CompareInstruction(
    const char* name, const std::vector< Value* >& values, Value::ID id )
: OperatorInstruction( name, Type::getBit( 1 ), values, id )
{
}

bool CompareInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid() or EquInstruction::classof( obj )
           or NeqInstruction::classof( obj );
}

// -----------------------------------------------------------------------------
// INSTRUCTIONS
// -----------------------------------------------------------------------------

NopInstruction::NopInstruction( void )
: Instruction( "nop", Type::getVoid(), {}, classid() )
{
}
bool NopInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

AllocInstruction::AllocInstruction( Type* type )
: Instruction( "alloc", type, {}, classid() )
{
}
bool AllocInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

CallInstruction::CallInstruction( Value* symbol )
: Instruction( "call", Type::getVoid(), { symbol }, classid() )
{
    assert( symbol );

    assert( isa< CallableUnit >( symbol ) and symbol->getType()->isRelation() );

    auto& res_tys = symbol->getType()->getResults();

    if( res_tys.size() == 1 )
    {
        setType( res_tys[ 0 ] );
    }
    else
    {
        assert( not" unimplemented call instruction result type behavior! " );
    }

    // assert( isa< CallableUnit >( symbol ) or isa< CastInstruction >( symbol )
    // );
    // if( isa< CastInstruction >( symbol ) )
    // {
    //     assert( symbol->getType()->getID() == Type::RELATION );
    // }
}
bool CallInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

Value& CallInstruction::getCallee( void ) const
{
    return *getValue( 0 );
}

IdCallInstruction::IdCallInstruction( Value* kind, Value* symbol )
: Instruction( "icall", 0, { kind, symbol }, classid() )
{
    assert( kind );
    assert( symbol );

    assert( strcmp( symbol->getType()->getName(), Type::getTypeID()->getName() )
            == 0 );
}
bool IdCallInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

StreamInstruction::StreamInstruction( Channel channel )
: Instruction( "stream", 0, {}, classid() )
, channel( channel )
{
}
const StreamInstruction::Channel StreamInstruction::getChannel( void ) const
{
    return channel;
}
bool StreamInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

// -----------------------------------------------------------------------------
// UNARY INSTRUCTIONS
// -----------------------------------------------------------------------------

IdInstruction::IdInstruction( Value* src )
: Instruction( "id", Type::getTypeID(), { src }, classid() )
, UnaryInstruction( this )
{
    assert( src );
    assert( isa< Variable >( src ) or isa< Function >( src ) );
}
bool IdInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

LoadInstruction::LoadInstruction( Value* src )
: Instruction( "load", 0, { src }, classid() )
, UnaryInstruction( this )
{
    setType( src->getType() );
}
bool LoadInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

ZeroExtendInstruction::ZeroExtendInstruction( Value* src, Type* type )
: Instruction( "zext", type, { src }, classid() )
, UnaryInstruction( this )
{
}
bool ZeroExtendInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

TruncationInstruction::TruncationInstruction( Value* src, Type* type )
: Instruction( "trunc", type, { src }, classid() )
, UnaryInstruction( this )
{
}
bool TruncationInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

// -----------------------------------------------------------------------------
// BINARY INSTRUCTIONS
// -----------------------------------------------------------------------------

StoreInstruction::StoreInstruction( Value* src, Value* dst )
: Instruction( "store", Type::getVoid(), { src, dst }, classid() )
, BinaryInstruction( this )
{
    assert( src->getType()->getID() == Type::BIT );
    assert(
        strcmp( src->getType()->getName(), dst->getType()->getName() ) == 0 );
}
bool StoreInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

ExtractInstruction::ExtractInstruction( Value* src, Value* dst )
: Instruction( "extract", 0, { src, dst }, classid() )
, BinaryInstruction( this )
{
    assert( src->getType() );
    assert( dst->getType() );

    // TODO: IDEA: FIXME: PPA: possible check to implement if 'dst' is inside
    // 'src'

    if( isa< Reference >( src ) )
    {
        if( src->getType()->isStructure() )
        {
            StructureType* ty = static_cast< StructureType* >( src->getType() );
            BitConstant* c = cast< BitConstant >( dst );

            setType( ty->getResults()[ c->getValue() ] );
        }
        else
        {
            assert( 0 );
        }
    }
    else
    {
        assert( 0 );
        setType( dst->getType() );
    }
}
bool ExtractInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

CastInstruction::CastInstruction( Value* kind, Value* src )
: Instruction( "cast", Type::getTypeID(), { kind, src }, classid() )
, BinaryInstruction( this )
{
    if( ( not isa< CallableUnit >( kind ) )
        and ( not isa< Structure >( kind ) ) )
    {
        assert( !" unimplemented/unsupported CastInstr type! " );
    }

    setType( kind->getType() );
}
bool CastInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid();
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
    return obj->getValueID() == classid();
}

OrInstruction::OrInstruction( Value* lhs, Value* rhs )
: ArithmeticInstruction( "or", { lhs, rhs }, classid() )
, BinaryInstruction( this )
{
}
bool OrInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

XorInstruction::XorInstruction( Value* lhs, Value* rhs )
: ArithmeticInstruction( "xor", { lhs, rhs }, classid() )
, BinaryInstruction( this )
{
}
bool XorInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

AddSignedInstruction::AddSignedInstruction( Value* lhs, Value* rhs )
: ArithmeticInstruction( "adds", { lhs, rhs }, classid() )
, BinaryInstruction( this )
{
}
bool AddSignedInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

DivSignedInstruction::DivSignedInstruction( Value* lhs, Value* rhs )
: ArithmeticInstruction( "divs", { lhs, rhs }, classid() )
, BinaryInstruction( this )
{
}
bool DivSignedInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

ModUnsignedInstruction::ModUnsignedInstruction( Value* lhs, Value* rhs )
: ArithmeticInstruction( "modu", { lhs, rhs }, classid() )
, BinaryInstruction( this )
{
}
bool ModUnsignedInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

// -----------------------------------------------------------------------------
// LOGICAL INSTRUCTIONS
// -----------------------------------------------------------------------------

NotInstruction::NotInstruction( Value* lhs )
: LogicalInstruction( "lnot", { lhs }, classid() )
, UnaryInstruction( this )
{
    assert( getType() == get()->getType() );
}
bool NotInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid();
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
    return obj->getValueID() == classid();
}

NeqInstruction::NeqInstruction( Value* lhs, Value* rhs )
: CompareInstruction( "neq", { lhs, rhs }, classid() )
, BinaryInstruction( this )
{
}
bool NeqInstruction::classof( Value const* obj )
{
    return obj->getValueID() == classid();
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
