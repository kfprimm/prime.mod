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
/*!	\file tokens/tokenguid.h
	\brief Definition von x::TokenGuid
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::TokenGuid
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef TOKENS_TOKENGUID_H
#define TOKENS_TOKENGUID_H

#include "../global/global.h"
#include "../temporalinfo.h"
#include "../guid.h"
#include "./token.h"

namespace x { namespace tokens {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief TokenGuid
	\version 0.1
	\date 2.4.2004
	\author Christian Oberholzer

	TokenGuid welches als einzigen Wert seine ID besitzt
*/
class TokenGuid : public Token
{
public:
	//-------------------------------------------------------------------------------------------------------
	TokenGuid();
	TokenGuid(const uint_t line);
	TokenGuid(const Guid& guid);
	TokenGuid(const Guid& guid, const uint_t line);
	virtual ~TokenGuid();

	//-------------------------------------------------------------------------------------------------------
	const Guid& GetGuid() const;

protected:
	Guid guid_;
};

}} // namespace x::tokens

#endif // TOKENS_TOKENGUID_H
