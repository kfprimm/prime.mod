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
/*!	\file tokens/tokenfloatlist.h
	\brief Definition von x::TokenFloatList
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::TokenFloatList
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef TOKENS_TOKENFLOATLIST_H
#define TOKENS_TOKENFLOATLIST_H

#include "../global/global.h"
#include "../temporalinfo.h"
#include "./token.h"

namespace x { namespace tokens {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Standalone-TokenFloatList
	\version 0.1
	\date 2.4.2004
	\author Christian Oberholzer

	TokenFloatList welches als einzigen Wert seine ID besitzt
*/
class TokenFloatList : public Token
{
public:
	//-------------------------------------------------------------------------------------------------------
	// Konstruktur & Destruktor
	TokenFloatList();
	TokenFloatList(const uint_t line);
	TokenFloatList(const float_t* values, const uint_t value_count);
	TokenFloatList(const float_t* values, const uint_t value_count, const uint_t line);
	TokenFloatList(const double_t* values, const uint_t value_count);
	TokenFloatList(const double_t* values, const uint_t value_count, const uint_t line);
	virtual ~TokenFloatList();

	virtual void SetValue(TemporalInfo& info, const float_t value);
	virtual void SetValue(TemporalInfo& info, const double_t value);
	const udword_t GetCount() const;
	const float_t* const GetFloat32Array() const;
	const double_t* const GetFloat64Array() const;

protected:
	udword_t count_;
	float_t* floatarray_;
	double_t* doublearray_;

	union
	{
		float_t floatvalue_;
		double_t doublevalue_;
	} singlevalue_;
};

}} // namespace x::tokens

#endif // TOKENS_TOKENFLOATLIST_H
