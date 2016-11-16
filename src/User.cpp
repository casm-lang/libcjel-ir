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

#include "User.h"
#include "Module.h"
#include "Memory.h"
#include "CallableUnit.h"
#include "Reference.h"
#include "Structure.h"
#include "Constant.h"
#include "Instruction.h"
#include "Variable.h"
#include "Interconnect.h"


using namespace libnovel;

bool User::classof( Value const* obj )
{
	return obj->getValueID() == classid()
		or Module::classof( obj )
		or Memory::classof( obj )
		or CallableUnit::classof( obj )
		or Reference::classof( obj )
		or Structure::classof( obj )
		or Constants::classof( obj )
		or Instruction::classof( obj )
		or Variable::classof( obj )
		or Interconnect::classof( obj )
		;
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
