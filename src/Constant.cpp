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

#include "Constant.h"
#include "Structure.h"

#include "../stdhl/cpp/Log.h"

using namespace libcsel_ir;

bool Constant::classof( Value const* obj )
{
    return obj->getValueID() == classid() or BitConstant::classof( obj )
           or StringConstant::classof( obj )
           or StructureConstant::classof( obj ) or Identifier::classof( obj );
}

Value* Constant::getBit( Type* result, u64 value )
{
    BitConstant tmp = BitConstant( result, value );

    auto cache = str2obj().find( tmp.getDescription() );
    if( cache != str2obj().end() )
    {
        return cache->second;
    }

    Value* ptr = new BitConstant( tmp );
    return str2obj().emplace( tmp.getDescription(), ptr ).first->second;
}

Value* Constant::getString( const char* value )
{
    StringConstant tmp = StringConstant( value );

    auto cache = str2obj().find( tmp.getDescription() );
    if( cache != str2obj().end() )
    {
        return cache->second;
    }

    Value* ptr = new StringConstant( tmp );
    return str2obj().emplace( tmp.getDescription(), ptr ).first->second;
}

Value* Constant::getStructure( Type* result, std::vector< Value* > values )
{
    StructureConstant tmp = StructureConstant( result, values );

    auto cache = str2obj().find( tmp.getDescription() );
    if( cache != str2obj().end() )
    {
        return cache->second;
    }

    Value* ptr = new StructureConstant( tmp );
    return str2obj().emplace( tmp.getDescription(), ptr ).first->second;
}

Value* Constant::getStructureZero( Type& result )
{
    std::vector< Value* > values
        = { libcsel_ir::Constant::getBit( result.getResults()[ 0 ], 0 ),
            libcsel_ir::Constant::getBit( result.getResults()[ 1 ], 0 ) };

    return getStructure( &result, values );
}

//
// Constants
//

BitConstant::BitConstant( Type* result, u64 value )
: ConstantOf< Type::Bit >(
      libstdhl::Allocator::string( std::to_string( value ) ), result, value,
      Value::BIT_CONSTANT )
{
}

bool BitConstant::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

StringConstant::StringConstant( Type::String value )
: ConstantOf< Type::String >( libstdhl::Allocator::string( value ),
      Type::getString(), value, Value::STRING_CONSTANT )
{
}

StringConstant::StringConstant( const char* value )
: StringConstant( ( Type::String )( value ) )
{
}

bool StringConstant::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

StructureConstant::StructureConstant( Type* type, Type::Struct value )
: ConstantOf< Type::Struct >(
      ".const_struct", type, value, Value::STRUCTURE_CONSTANT )
{
    assert( type );
    assert( type->isStructure() );
    assert( type->getArguments().size() == 0 );
    assert( type->getResults().size() == value.size() );

    for( u32 i = 0; i < value.size(); i++ )
    {
        assert( strcmp( type->getResults()[ i ]->getName(),
                    value[ i ]->getType()->getName() )
                == 0 );
    }
}

bool StructureConstant::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

Identifier::Identifier( Type* type, const char* value )
: ConstantOf< const char* >( value, type, value, Value::IDENTIFIER )
{
    // auto result = ident2obj().find( value );
    // if( result != ident2obj().end() )
    // {
    //     assert( !" identifier already used! " );
    // }
}

Identifier::~Identifier( void )
{
}

Identifier* Identifier::create( Type* type, const char* value, Value* scope )
{
    assert( type );
    assert( value );

    Identifier tmp = Identifier( type, value );

    auto cache = ident2obj().find( value );
    if( cache != ident2obj().end() )
    {
        Value* v = cache->second;
        assert( strcmp( v->getType()->getName(), type->getName() ) == 0 );
        return cache->second;
    }

    Identifier* ptr = new Identifier( tmp );
    ident2obj()[ std::string( value ) ] = ptr;
    return ptr;
}

void Identifier::forgetSymbol( const char* value )
{
    ident2obj().erase( std::string( value ) );
}

bool Identifier::classof( Value const* obj )
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
