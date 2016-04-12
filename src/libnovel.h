//  
//  Copyright (c) 2016 Philipp Paulweber
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/ppaulweber/libnovel
//  
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program. If not, see <http://www.gnu.org/licenses/>.
//  

#ifndef _LIB_NOVEL_H_
#define _LIB_NOVEL_H_

#include "Novel.h"
#include "Type.h"
#include "Value.h"
#include "Visitor.h"

#include "Module.h"
#include "Structure.h"
#include "Variable.h"
#include "Memory.h"
#include "CallableUnit.h"
#include "Function.h"
#include "Intrinsic.h"
#include "Reference.h"

#include "Block.h"
#include "Scope.h"

#include "Statement.h"
#include "Instruction.h"

#include "stdhl/cpp/Allocator.h"

namespace libnovel
{
}

#define TODO fprintf( stdout, "%s:%i: '%s' implementation TODO\n", __FILE__, __LINE__, __FUNCTION__ )

#endif /* _LIB_NOVEL_H_ */

//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  
