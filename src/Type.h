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

#ifndef _LIB_CSELIR_TYPE_H_
#define _LIB_CSELIR_TYPE_H_

#include "cpp/Binding.h"
#include "cpp/Type.h"

#include "CselIR.h"

namespace libcsel_ir
{
    class Value;

    class Type : public libstdhl::Binding< Value > //: public CselIR
    {
      public:
        typedef u64* Bit;
        typedef const char* String;

        enum ID
        {
            BIT,
            STRUCTURE,
            FUNCTION,
            MEMORY,
            INTERCONNECT,
            STRING,
            _TOP_
        };

        enum STATE
        {
            UNCHANGED,
            CHANGED,
            LOCKED
        };

      private:
        ID type_id;
        u64 type_uid_hash;
        STATE type_state;
        i16 bitsize;
        std::string description;
        std::vector< Type* > parameters;
        std::vector< Type* > subtypes;

        static const char* ID2str[ ID::_TOP_ ];

      public:
        Type( ID id, i16 bitsize = -1, STATE state = STATE::UNCHANGED );
        const ID getIDKind( void ) const;
        const u64 getID( void ) const;
        const char* getName( void );
        const i16 getBitsize( void );

        const std::vector< Type* >& getParameters( void ) const;
        const std::vector< Type* >& getSubTypes( void ) const;

        void addParameter( Type* parameter );
        void addSubType( Type* subtype );

        Type* getResultType( void );

      private:
        void setID( ID id );
    };

    static Type TypeB1 = Type( Type::BIT, 1, Type::STATE::LOCKED );
    static Type TypeB8 = Type( Type::BIT, 8, Type::STATE::LOCKED );
    static Type TypeB16 = Type( Type::BIT, 16, Type::STATE::LOCKED );
    static Type TypeB32 = Type( Type::BIT, 32, Type::STATE::LOCKED );
    static Type TypeB48 = Type( Type::BIT, 48, Type::STATE::LOCKED );
    static Type TypeB64 = Type( Type::BIT, 64, Type::STATE::LOCKED );
    static Type TypeId = TypeB48;

    static Type TypeMemory = Type( Type::MEMORY, -1, Type::STATE::LOCKED );
    static Type TypeInterconnect
        = Type( Type::INTERCONNECT, -1, Type::STATE::LOCKED );

    static Type TypeStructure
        = Type( Type::STRUCTURE, -1, Type::STATE::LOCKED );

    static Type TypeFunction = Type( Type::FUNCTION, -1, Type::STATE::LOCKED );

    static Type TypeCharacter = Type( Type::STRING, 1, Type::STATE::LOCKED );
}

#endif /* _LIB_CSELIR_TYPE_H_ */

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
