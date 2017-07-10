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

#ifndef _LIB_CJELIR_VARIABLE_H_
#define _LIB_CJELIR_VARIABLE_H_

#include "User.h"

#include "Constant.h"
#include "Instruction.h"
#include "Value.h"

namespace libcjel_ir
{
    class Variable : public User
    {
      public:
        using Ptr = std::shared_ptr< Variable >;

        Variable( const Type::Ptr& type, const Value::Ptr& expression,
            const std::string& ident = 0 );

        ~Variable( void );

        BitConstant::Ptr allocId( void );

        Value::Ptr expression( void ) const;

        static inline ID classid( void )
        {
            return Value::VARIABLE;
        }

        static bool classof( Value const* obj );

      private:
        static u64 m_allocation_cnt;

        Value::Ptr m_expression;
        BitConstant::Ptr m_allocation_id;
    };
}

#endif // _LIB_CJELIR_VARIABLE_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
