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

#ifndef _LIB_CSELIR_SCOPE_H_
#define _LIB_CSELIR_SCOPE_H_

#include "Block.h"

namespace libcsel_ir
{
    class Scope : public Block
    {
      public:
        using Ptr = std::shared_ptr< Scope >;

        Scope( const std::string& name, const Type::Ptr& type, u1 parallel,
            Value::ID id = classid() );

        void add( const Block::Ptr& block );

        Blocks blocks( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::SCOPE;
        }

        static bool classof( Value const* obj );

      private:
        Blocks m_blocks;
    };

    using Scopes = libstdhl::List< Scope >;

    class SequentialScope : public Scope
    {
      public:
        using Ptr = std::shared_ptr< SequentialScope >;

        SequentialScope( void );

        static inline Value::ID classid( void )
        {
            return Value::SEQUENTIAL_SCOPE;
        }

        static bool classof( Value const* obj );
    };

    class ParallelScope : public Scope
    {
      public:
        using Ptr = std::shared_ptr< ParallelScope >;

        ParallelScope( void );

        static inline Value::ID classid( void )
        {
            return Value::PARALLEL_SCOPE;
        }

        static bool classof( Value const* obj );
    };
}

#endif // _LIB_CSELIR_SCOPE_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
