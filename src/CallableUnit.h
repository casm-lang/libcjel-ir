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

#include "User.h"

#include "Reference.h"

namespace libcsel_ir
{
    class BitConstant;
    class Scope;

    class CallableUnit : public User
    {
      public:
        using Ptr = std::shared_ptr< CallableUnit >;

        CallableUnit( const std::string& name, const RelationType::Ptr& type,
            Value::ID id = classid() );

        void setContext( const std::shared_ptr< Scope >& scope );

        std::shared_ptr< Scope > context( void ) const;

        std::shared_ptr< BitConstant > allocId( void ) const;

        void add( const Reference::Ptr& reference );

        Reference::Ptr in( const std::string& name, const Type::Ptr& type );

        Reference::Ptr out( const std::string& name, const Type::Ptr& type );

        Reference::Ptr link( const std::string& name, const Type::Ptr& type );

        const std::vector< Reference::Ptr >& inputs( void ) const;
        const std::vector< Reference::Ptr >& outputs( void ) const;
        const std::vector< Reference::Ptr >& linkage( void ) const;

        u16 indexOf( const Reference::Ptr& reference ) const;

        u1 isLast( const Reference::Ptr& reference ) const;

        u16 length( void ) const;

        Reference::Ptr reference( const std::string& name ) const;

        static inline Value::ID classid( void )
        {
            return Value::CALLABLE_UNIT;
        }

        static bool classof( Value const* obj );

      private:
        static u64 m_allocation_cnt;

        std::shared_ptr< Scope > m_context;

        std::shared_ptr< BitConstant > m_allocation_id;

        std::vector< Reference::Ptr > m_references[ Reference::Kind::_SIZE_ ];

        std::unordered_map< std::string, u16 > m_name2index;

        std::unordered_map< std::string, std::weak_ptr< Reference > >
            m_name2ref;
    };
}

#endif // _LIB_CSELIR_CALLABLE_UNIT_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
