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

#ifndef _LIB_CSELIR_CALLABLE_UNIT_H_
#define _LIB_CSELIR_CALLABLE_UNIT_H_

#include "Block.h"
#include "Constant.h"
#include "User.h"
#include "Value.h"

namespace libcsel_ir
{
    class Reference;

    class CallableUnit : public User
    {
      private:
        static u64 m_allocation_cnt;

        Block* m_context;
        Identifier* m_identifier;

        BitConstant m_allocation_id;

        std::vector< Value* > m_parameter_in;
        std::vector< Value* > m_parameter_out;
        std::vector< Value* > m_linkage;

        std::unordered_map< Value*, u16 > m_parameter2index;
        std::unordered_map< const char*, Value* > m_name2ref;

      public:
        CallableUnit(
            const char* name, Type* type, Value::ID id = CALLABLE_UNIT );

        ~CallableUnit( void );

        BitConstant& allocId( void );

        Block* context( void ) const;

        void setContext( Block* scope );

        const Identifier* identifier( void ) const;

      private:
        Reference* add( const char* ref_name, Type* ref_type, u8 ref_kind );

      public:
        Reference* in( const char* ref_name, Type* ref_type );
        Reference* out( const char* ref_name, Type* ref_type );
        Reference* link( const char* ref_name, Type* ref_type );

        void addParameter( Value* value, u1 input = true );
        void addLinkage( Value* value );

        const std::vector< Value* >& inParameters( void ) const;
        const std::vector< Value* >& outParameters( void ) const;

        const i16 indexOfParameter( Value* value ) const;
        const u1 isLastParameter( Value* value ) const;
        const i16 parameterLength( void ) const;

        const std::vector< Value* >& linkage( void ) const;

        const Reference* reference( const char* name ) const;

        static inline Value::ID classid( void )
        {
            return Value::CALLABLE_UNIT;
        }

        static bool classof( Value const* obj );
    };
}

#endif /* _LIB_CSELIR_CALLABLE_UNIT_H_ */

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
