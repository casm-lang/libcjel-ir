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

#ifndef _LIB_CSELIR_STRUCTURE_H_
#define _LIB_CSELIR_STRUCTURE_H_

#include "User.h"

#include "Constant.h"
#include "Intrinsic.h"
#include "Module.h"
#include "Value.h"

namespace libcsel_ir
{
    class Structure;
    using Structures = libstdhl::List< Structure >;

    struct StructureElement
    {
        Type::Ptr type;
        std::string name;
    };

    class Structure : public User
    {
      public:
        using Ptr = std::shared_ptr< Structure >;

        Structure( const std::string& name, const StructureType::Ptr& type,
            const std::vector< StructureElement >& elements );

        StructureElement element( std::size_t index ) const;

        std::vector< StructureElement > elements( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::STRUCTURE;
        }

        static bool classof( Value const* obj );

      private:
        std::vector< StructureElement > m_elements;
    };
}

#endif // _LIB_CSELIR_STRUCTURE_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
