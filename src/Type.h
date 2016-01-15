//  
//  Copyright (c) 2015 Philipp Paulweber
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/ppaulweber/libnovel
//  
//  Permission is hereby granted, free of charge, to any person obtaining a 
//  copy of this software and associated documentation files (the "Software"), 
//  to deal with the Software without restriction, including without limitation 
//  the rights to use, copy, modify, merge, publish, distribute, sublicense, 
//  and/or sell copies of the Software, and to permit persons to whom the 
//  Software is furnished to do so, subject to the following conditions:
//  
//  * Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimers.
//  
//  * Redistributions in binary form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimers in the 
//    documentation and/or other materials provided with the distribution.
//  
//  * Neither the names of the copyright holders, nor the names of its 
//    contributors may be used to endorse or promote products derived from 
//    this Software without specific prior written permission.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
//  CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
//  WITH THE SOFTWARE.
//  

/**
   @brief    TODO
   
   TODO
*/

#ifndef _LIB_NOVEL_TYPE_H_
#define _LIB_NOVEL_TYPE_H_

#include "stdhl/cpp/Type.h"

#include "Novel.h"

namespace libnovel
{
	class Rule;
	
	class Type : public Novel
	{
	public:		
		typedef u64*   Bit;
		typedef i64    Integer;
		//typedef char*  String;
		
		enum ID
		{ BIT
		, INTEGER
	  //, STRING
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
	
    static Type IntegerType     = Type( Type::INTEGER,      Type::STATE::LOCKED );
	//static Type StringType      = Type( Type::STRING,       Type::STATE::LOCKED );	
	
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
