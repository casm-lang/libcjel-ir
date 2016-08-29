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

#ifndef _LIB_NOVEL_H_
#define _LIB_NOVEL_H_

#include "src/Novel.h"
#include "src/Type.h"
#include "src/Value.h"
#include "src/Module.h"
#include "src/Structure.h"
#include "src/Variable.h"
#include "src/Memory.h"
#include "src/CallableUnit.h"
#include "src/Function.h"
#include "src/Interconnect.h"
#include "src/Intrinsic.h"
#include "src/Reference.h"
#include "src/Block.h"
#include "src/Scope.h"
#include "src/Statement.h"
#include "src/Instruction.h"
#include "src/Visitor.h"


namespace libnovel
{
	class Novel
	{
	};
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
