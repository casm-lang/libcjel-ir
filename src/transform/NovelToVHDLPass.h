//  
//  Copyright (c) 2015-2016 Philipp Paulweber
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/ppaulweber/libnovel
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
//  along with this program. If not, see <http://www.gnu.org/licenses/>.
//  

#ifndef _LIB_NOVEL_TO_VDHL_PASS_H_
#define _LIB_NOVEL_TO_VDHL_PASS_H_

#include "Pass.h"

#include "libnovel.h"
#include "NovelDumpPass.h"

namespace libnovel
{
	class NovelToVHDLPass : public libpass::Pass, public Visitor
	{
	public:
		static char id;
			
		bool run( libpass::PassResult& pr );
		
		LIB_NOVEL_VISITOR_INTERFACE;
	};
}


#endif /* _LIB_NOVEL_TO_VDHL_PASS_H_ */


//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  
