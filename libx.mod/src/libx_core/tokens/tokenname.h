//-----------------------------------------------------------------------------------------------------------
/*
 *	LibX - A Library to export/import .x-models
 *
 *	This file is
 *		Copyright (C) 2008 Christian Oberholzer <coberholzer@gmx.ch>
 *
 *	It is part of LibX, which is
 *		Copyright (C) 2008 Christian Oberholzer <coberholzer@gmx.ch>
 *		Copyright (C) 2008 Basil Fierz <basil@gmx.ch>
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License version 2.1 as published by the Free Software Foundation.
 *	
 *	This library is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *	Lesser General Public License for more details.
 *	
 *	You should have received a copy of the GNU Lesser General Public
 *	License along with this library; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
/*!	\file tokens/tokenname.h
	\brief Definition von x::TokenName
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::TokenName
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef TOKENS_TOKENNAME_H
#define TOKENS_TOKENNAME_H

#include "../global/global.h"
#include "../temporalinfo.h"
#include "./token.h"

namespace x { namespace tokens {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Standalone-TokenName
	\version 0.1
	\date 2.4.2004
	\author Christian Oberholzer

	TokenName welches als einzigen Wert seine ID besitzt
*/
class TokenName : public Token
{
public:
	//-------------------------------------------------------------------------------------------------------
	TokenName();
	TokenName(const uint_t line);
	TokenName(const string_t& name);
	TokenName(const string_t& name, const uint_t line);
	virtual ~TokenName();

	//-------------------------------------------------------------------------------------------------------
	virtual const string_t& GetName() const;

protected:
	/*!
		\brief m_strName
		
		Name welcher in diesem Token gespeichert ist
	*/

	string_t name_;
};

}} // namespace x::tokens

#endif // TOKENS_TOKENNAME_H
