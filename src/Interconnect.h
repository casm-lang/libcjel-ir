//
//  Copyright (c) 2015-2017 Philipp Paulweber
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                https://github.com/casm-lang/libcjel-ir
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

#ifndef _LIB_CJELIR_INTERCONNECT_H_
#define _LIB_CJELIR_INTERCONNECT_H_

#include "User.h"

namespace libcjel_ir
{
    class Interconnect : public User
    {
      public:
        Interconnect( const std::string& name );

        void add( const Value::Ptr& object );

        Values objects( void ) const;

        u64 bitsizeMax( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::INTERCONNECT;
        }

        static bool classof( Value const* obj );

      private:
        Values m_objects;

        u64 m_bs_max;
    };
}

#endif // _LIB_CJELIR_INTERCONNECT_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
