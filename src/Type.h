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

#ifndef _LIB_NOVEL_TYPE_H_
#define _LIB_NOVEL_TYPE_H_

#include "stdhl/cpp/Type.h"
#include "stdhl/cpp/Binding.h"

#include "Novel.h"

namespace libnovel
{
	class Value;
	
    class Type : public libstdhl::Binding< Value >  //: public Novel
	{
	public:		
		typedef u64*       Bit;
		
		enum ID
		{ BIT
		, STRUCTURE
		, FUNCTION
		, _TOP_
		};
		
		enum STATE
		{ UNCHANGED
		, CHANGED
		, LOCKED
		};
		
	private:		
		ID type_id;
		u64 type_uid_hash;
		STATE type_state;
		i16 bitsize;
		std::string description;
		std::vector< Type* > parameters;
		std::vector< Type* > subtypes;
		
		static const char* ID2str[ ID::_TOP_ ];
		
	public:
		Type( ID id, i16 bitsize = -1, STATE state = STATE::UNCHANGED );
		const ID getIDKind( void ) const;
		const u64 getID( void ) const;
		const char* getName( void );
		const i16 getBitsize( void );

		const std::vector< Type* >& getParameters( void ) const;
	    const std::vector< Type* >& getSubTypes( void ) const;
	    
		void addParameter( Type* parameter );
		void addSubType( Type* subtype );
		
		Type* getResultType( void );
		
	private:
		void setID( ID id );		
	};

	static Type TypeB1        = Type( Type::BIT,        1, Type::STATE::LOCKED );
	static Type TypeB32       = Type( Type::BIT,       32, Type::STATE::LOCKED );
	static Type TypeB48       = Type( Type::BIT,       48, Type::STATE::LOCKED );
	static Type TypeB64       = Type( Type::BIT,       64, Type::STATE::LOCKED );
	static Type TypeFunction  = Type( Type::FUNCTION,  -1, Type::STATE::LOCKED );
	static Type TypeStructure = Type( Type::STRUCTURE, -1, Type::STATE::LOCKED );
	static Type TypeId        = TypeB48;
}

#endif /* _LIB_NOVEL_TYPE_H_ */


//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  
