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

//
// Constants
//

BitConstant::BitConstant( Type* result, u64 value )
: ConstantOf< Type::Bit >(
      libstdhl::Allocator::string( std::to_string( value ) ), result,
      this->value, Value::BIT_CONSTANT )
{
    this->value[ 0 ] = value;
}

void BitConstant::dump( void ) const
{
    printf( "[Const] %p = bit %li\n", this, getValue()[ 0 ] );
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

void StringConstant::dump( void ) const
{
    printf( "[Const] %p = string %s\n", this, getValue() );
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

    assert( !" PPA: TODO: " );

    // libcsel_ir::Value* b = type->getBound();
    // assert( b and libcsel_ir::isa< libcsel_ir::Structure >( b ) );
    // libcsel_ir::Structure* s = (libcsel_ir::Structure*)b;

    // if( value.size() == 0 )
    // {
    //     if( s->getElements().size() > 0 )
    //     {
    //         for( Value* e : s->getElements() )
    //         {
    //             if( e->getType()->getIDKind() == Type::BIT )
    //             {
    //                 this->value.push_back(
    //                     BitConstant::create( 0, e->getType()->getBitsize() )
    //                     );
    //             }
    //             else
    //             {
    //                 assert( !"struct in struct in ... not implemented yet!"
    //                 );
    //             }
    //         }
    //     }
    //     else
    //     {
    //         assert( !"empty structure found, should not be possible!" );
    //     }
    // }

    // assert( this->value.size() != 0
    //         && "empty structure found, should not be possible!" );

    // for( Value* p : this->value )
    // {
    //     assert( p );
    //     if( isa< BitConstant >( p ) )
    //     {
    //         ( (BitConstant*)p )->bind( this );
    //     }
    //     else if( isa< StructureConstant >( p ) )
    //     {
    //         ( (StructureConstant*)p )->bind( this );
    //     }
    //     else if( isa< StringConstant >( p ) )
    //     {
    //         ( (StringConstant*)p )->bind( this );
    //     }
    //     else
    //     {
    //         assert( !" unimplemented case found! " );
    //     }
    // }
}

// const std::vector< Value* >& StructureConstant::getElements( void ) const
// {
//     return value;
// }

void StructureConstant::dump( void ) const
{
    printf( "[Const] %p = structure\n", this );
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

void Identifier::dump( void ) const
{
    printf( "[Ident] " );
    debug();
}

bool Identifier::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

// template < typename V >
// Constant< V >::Constant( const char* name, Type* type, V value, Value::ID id
// )
// : User( name, type, id )
// , value( value )
// {
//     ( *Value::getSymbols() )[ ".constant" ].insert( this );
// }

// template < typename V >
// Constant< V >::~Constant( void )
// {
//     ( *Value::getSymbols() )[ ".constant" ].erase( this );
// }

// template < typename T >
// bool Constant< T >::classof( Value const* obj )
// {
//     assert( 0 && "invalid to check 'classof' Constant< V > template class,
//     use Constants" );
//     return false;
// }

// template < typename V >
// const V Constant< V >::getValue( void ) const
// {
//     return value;
// }

// template < typename V >
// void Constant< V >::setValue( V val )
// {
//     value = val;
// }

// bool Constants::classof( Value const* obj )
// {
//     return obj->getValueID() == classid() or BitConstant::classof( obj )
//            or StructureConstant::classof( obj )
//            or StringConstant::classof( obj );
// }

// BitConstant::BitConstant( Type* type, u64 value )
// : Constant< Type::Bit >( ".const_bit", type, this->value, Value::BIT_CONSTANT
// )
// {
//     this->value[ 0 ] = value;
// }

// BitConstant* BitConstant::create( u64 value, u16 bitsize )
// {
//     // static std::unordered_map< u64, BitConstant* > cache[256];

//     assert(
//         bitsize > 0 and bitsize <= 256 and "invalid 'Bit' constant bit size"
//         );

//     // TODO: FIXME: improve this to save memory and gain more compilation
//     // performance
//     // auto result = cache[bitsize].find( value );
//     // if( result != cache[bitsize].end() )
//     // {
//     // 	assert( result->second );
//     // 	printf( "[Const] found %p\n", result->second );
//     // 	return result->second;
//     // }

//     BitConstant* obj
//         = new BitConstant( new Type( Type::ID::BIT, bitsize ), value );
//     // cache[bitsize][ value ] = obj;
//     return obj;
// }

// void BitConstant::dump( void ) const
// {
//     printf( "[Const] %p = bit %li\n", this, getValue()[ 0 ] );
// }

// bool BitConstant::classof( Value const* obj )
// {
//     return obj->getValueID() == classid();
// }

// StructureConstant::StructureConstant( Type* type, std::vector< Value* > value
// )
// : Constant< std::vector< Value* > >(
//       ".const_struct", type, value, Value::STRUCTURE_CONSTANT )
// {
//     assert( type );
//     libcsel_ir::Value* b = type->getBound();
//     assert( b and libcsel_ir::isa< libcsel_ir::Structure >( b ) );
//     libcsel_ir::Structure* s = (libcsel_ir::Structure*)b;

//     if( value.size() == 0 )
//     {
//         if( s->getElements().size() > 0 )
//         {
//             for( Value* e : s->getElements() )
//             {
//                 if( e->getType()->getIDKind() == Type::BIT )
//                 {
//                     this->value.push_back(
//                         BitConstant::create( 0, e->getType()->getBitsize() )
//                         );
//                 }
//                 else
//                 {
//                     assert( !"struct in struct in ... not implemented yet!"
//                     );
//                 }
//             }
//         }
//         else
//         {
//             assert( !"empty structure found, should not be possible!" );
//         }
//     }

//     assert( this->value.size() != 0
//             && "empty structure found, should not be possible!" );

//     for( Value* p : this->value )
//     {
//         assert( p );
//         if( isa< BitConstant >( p ) )
//         {
//             ( (BitConstant*)p )->bind( this );
//         }
//         else if( isa< StructureConstant >( p ) )
//         {
//             ( (StructureConstant*)p )->bind( this );
//         }
//         else if( isa< StringConstant >( p ) )
//         {
//             ( (StringConstant*)p )->bind( this );
//         }
//         else
//         {
//             assert( !" unimplemented case found! " );
//         }
//     }
// }

// StructureConstant* StructureConstant::create(
//     Structure* kind, const std::vector< Value* >& value )
// {
//     // TODO: FIXME: PPA: silly implmeentation for now!!! must be improved to
//     // gain performance and use less memory!!!

//     // TODO: FIXME: PPA: check if KIND structure subtypes match the provided
//     // value subtypes in the vector!!!

//     assert( kind );
//     assert( kind->getType() );

//     StructureConstant* obj = new StructureConstant( kind->getType(), value );
//     return obj;
// }

// const std::vector< Value* >& StructureConstant::getElements( void ) const
// {
//     return value;
// }

// void StructureConstant::dump( void ) const
// {
//     printf( "[Const] %p = structure %p\n", this, &getElements() );
// }

// bool StructureConstant::classof( Value const* obj )
// {
//     return obj->getValueID() == classid();
// }

// Identifier::Identifier( Type* type, const char* value )
// : Constant< const char* >( value, type, value, Value::IDENTIFIER )
// {
//     ( *Value::getSymbols() )[ ".identifier" ].insert( this );
// }

// Identifier::~Identifier( void )
// {
//     ( *Value::getSymbols() )[ ".identifier" ].erase( this );
// }

// Identifier* Identifier::create( Type* type, std::string value, Value* scope )
// {
//     return create( type, value.c_str(), scope );
// }

// Identifier* Identifier::create( Type* type, const char* value, Value* scope )
// {
//     SymbolTable& symbols = *getSymbols();

//     const char* tmp = value;

//     if( scope )
//     {
//         tmp = libstdhl::Allocator::string(
//             std::string( scope->getName() ) + "::" + std::string( value ) );
//     }
//     else
//     {
//         if( type && type->getIDKind() == Type::STRUCTURE )
//         {
//             tmp = libstdhl::Allocator::string(
//                 "@@Structure::" + std::string( value ) );
//         }
//     }

//     auto result = symbols.find( tmp );
//     if( result != symbols.end() )
//     {
//         assert( result->second.size() == 1 );
//         Value* x = *result->second.begin();

//         // assert( x->getType()->getID() == type->getID() );
//         if( x->getType()->getID() == type->getID() )
//         {
//             printf( "[Ident] found '%s' of type %lu @ %p\n", value,
//                 type->getID(), x );
//             return (Identifier*)x;
//         }
//     }

//     // //const char* tmp_scope = value;

//     // 	if( scope )
//     // 	{
//     // 		// std::string tmp;
//     // 		// tmp.append( scope->getName() );
//     // 		// tmp.append( "::" );
//     // 		// tmp.append( value );
//     // 		// tmp_scope = tmp.c_str();
//     // 	}

//     // 	auto result = symbols.find( value );
//     // 	if( result != symbols.end() )
//     // 	{
//     // 		assert( result->second.size() == 1 );
//     // 		Value* x = *result->second.begin();
//     // 	 	assert( x->getType()->getID() == type->getID() );
//     // 		printf( "[Ident] found '%s' of type %lu @ %p\n", value,
//     // type->getID(),
//     // x );
//     // 		return (Identifier*)x;
//     // 	}

//     printf( "[Ident] creating '%s' of type %lu\n", value, type->getID() );
//     return new Identifier( type, value );
// }

// void Identifier::forgetSymbol( const char* value )
// {
//     printf( "[Ident] forgetting '%s'\n", value );
//     getSymbols()->erase( value );
// }

// void Identifier::dump( void ) const
// {
//     printf( "[Ident] " );
//     debug();
// }

// bool Identifier::classof( Value const* obj )
// {
//     return obj->getValueID() == classid();
// }

// StringConstant::StringConstant( Type* type, Type::String value )
// : Constant< Type::String >(
//       ".const_string", type, value, Value::STRING_CONSTANT )
// {
// }

// StringConstant* StringConstant::create( Type::String value )
// {
//     u32 length = 0;
//     if( value )
//     {
//         length = strlen( value );
//     }

//     assert( length <= 256 and "invalid 'String' constant size" ); // TODO:
//                                                                   // FIXME:
//                                                                   // internal
//                                                                   //
//                                                                   limitation
//                                                                   // for now!

//     StringConstant* obj
//         = new StringConstant( new Type( Type::ID::STRING, length ), value );
//     assert( obj );
//     return obj;
// }

// void StringConstant::dump( void ) const
// {
//     printf( "[Const] %p = string '%s'\n", this, getValue() );
// }

// bool StringConstant::classof( Value const* obj )
// {
//     return obj->getValueID() == classid();
// }

// BitConstant BitConstant::FALSE = BitConstant( &TypeB1, 0 );
// BitConstant BitConstant::TRUE = BitConstant( &TypeB1, 1 );
// BitConstant BitConstant::NIL = BitConstant( &TypeId, 0 );
// StringConstant StringConstant::LF = StringConstant( &TypeCharacter, "\n" );

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
