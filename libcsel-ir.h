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

#ifndef _LIB_CSELIR_H_
#define _LIB_CSELIR_H_

#include "src/Block.h"
#include "src/CallableUnit.h"
#include "src/CselIR.h"
#include "src/Function.h"
#include "src/Instruction.h"
#include "src/Interconnect.h"
#include "src/Intrinsic.h"
#include "src/Memory.h"
#include "src/Module.h"
#include "src/Reference.h"
#include "src/Scope.h"
#include "src/Statement.h"
#include "src/Structure.h"
#include "src/Type.h"
#include "src/Value.h"
#include "src/Variable.h"
#include "src/Visitor.h"

namespace libcsel_ir
{
    class CselIR
    {
    };
}

#define TODO                                                                   \
    fprintf( stdout, "%s:%i: '%s' implementation TODO\n", __FILE__, __LINE__,  \
        __FUNCTION__ )

#endif /* _LIB_CSELIR_H_ */

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
