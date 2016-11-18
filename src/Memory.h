//
//  Copyright (c) 2015-2016 Philipp Paulweber
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                https://github.com/casm-lang/libnovel
//
//  This file is part of libnovel.
//
//  libnovel is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  libnovel is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with libnovel. If not, see <http://www.gnu.org/licenses/>.
//

#ifndef _LIB_NOVEL_MEMORY_H_
#define _LIB_NOVEL_MEMORY_H_

#include "Structure.h"
#include "User.h"
#include "Value.h"

namespace libnovel
{
    class Memory : public User
    {
        Structure* structure;
        u32 size;

      public:
        Memory( Structure* value, u32 size );

        ~Memory( void );

        Structure* getStructure( void ) const;

        const u32 getSize( void ) const;

        void dump( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::MEMORY;
        };
        static bool classof( Value const* obj );
    };
}

#endif /* _LIB_NOVEL_MEMORY_H_ */

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
