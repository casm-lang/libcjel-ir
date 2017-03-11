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

#ifndef _LIB_CSELIR_MODULE_H_
#define _LIB_CSELIR_MODULE_H_

#include "User.h"

namespace libcsel_ir
{
    class Module : public User
    {
      public:
        using Ptr = std::shared_ptr< Module >;

        Module( const std::string& name );

        void add( const Value::Ptr& value );

        template < class C >
        bool has( void ) const
        {
            return m_content.count( C::classid() ) > 0;
        }

        template < class C >
        Values get( void ) const
        {
            auto result = m_content.find( C::classid() );
            assert( result != m_content.end() );
            return result->second;
        }

        static inline Value::ID classid( void )
        {
            return Value::MODULE;
        }

        static bool classof( Value const* obj );

      private:
        std::unordered_map< u32, Values > m_content;
    };
}

#endif // _LIB_CSELIR_MODULE_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
