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

#include "User.h"
#include "CallableUnit.h"
#include "Constant.h"
#include "Instruction.h"
#include "Interconnect.h"
#include "Memory.h"
#include "Module.h"
#include "Reference.h"
#include "Structure.h"
#include "Variable.h"

using namespace libcsel_ir;

bool User::classof( Value const* obj )
{
    return obj->id() == classid() or Module::classof( obj )
           or Memory::classof( obj ) or CallableUnit::classof( obj )
           or Reference::classof( obj ) or Structure::classof( obj )
           or Constant::classof( obj ) or Instruction::classof( obj )
           or Variable::classof( obj ) or Interconnect::classof( obj );
}

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
