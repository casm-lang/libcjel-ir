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

#ifndef _LIB_CJELIR_MEMORY_H_
#define _LIB_CJELIR_MEMORY_H_

#include "User.h"

namespace libcjel_ir
{
    class Memory : public User
    {
      public:
        Memory( const std::string& name, const Type::Ptr& type, u32 length );

        u32 length( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::MEMORY;
        }

        static bool classof( Value const* obj );

      private:
        u32 m_length;
    };
}

#endif // _LIB_CJELIR_MEMORY_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
