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
/*!	\file tokens/tokenfloatlist.cpp
	\brief Implementation von x::TokenFloatList
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::TokenFloatList
*/
//-----------------------------------------------------------------------------------------------------------
#define XLIB_CORE
#include "../parseexception.h"
#include "./tokenfloatlist.h"

namespace x { namespace tokens {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
TokenFloatList::TokenFloatList()
:	Token(Token::TypeFloatList),
	count_(0),
	floatarray_(NULL),
	doublearray_(NULL)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
TokenFloatList::TokenFloatList(
	const uint_t line
)
:	Token(Token::TypeFloatList, line),
	count_(0),
	floatarray_(NULL),
	doublearray_(NULL)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
TokenFloatList::TokenFloatList(
	const float_t* values,
	const uint_t value_count
)
:	Token(Token::TypeFloatList),
	count_(0),
	floatarray_(NULL),
	doublearray_(NULL)
{
	count_ = value_count;
	floatarray_ = new float_t[count_];
	memcpy(floatarray_, values, sizeof(float_t)*count_);
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
TokenFloatList::TokenFloatList(
	const float_t* values,
	const uint_t value_count,
	const uint_t line
)
:	Token(Token::TypeFloatList, line),
	count_(0),
	floatarray_(NULL),
	doublearray_(NULL)
{
	count_ = value_count;
	floatarray_ = new float_t[count_];
	memcpy(floatarray_, values, sizeof(float_t)*count_);
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
TokenFloatList::TokenFloatList(
	const double_t* values,
	const uint_t value_count
)
:	Token(Token::TypeFloatList),
	count_(0),
	floatarray_(NULL),
	doublearray_(NULL)
{
	count_ = value_count;
	doublearray_ = new double_t[count_];
	memcpy(doublearray_, values, sizeof(double_t)*count_);
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
TokenFloatList::TokenFloatList(
	const double_t* values,
	const uint_t value_count,
	const uint_t line
)
:	Token(Token::TypeFloatList, line),
	count_(0),
	floatarray_(NULL),
	doublearray_(NULL)
{
	count_ = value_count;
	doublearray_ = new double_t[count_];
	memcpy(doublearray_, values, sizeof(double_t)*count_);
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
TokenFloatList::~TokenFloatList()
{
	if (floatarray_ != NULL && floatarray_ != &singlevalue_.floatvalue_)
	{
		delete[] floatarray_;
		floatarray_ = NULL;
	}
	if (doublearray_ != NULL && doublearray_ != &singlevalue_.doublevalue_)
	{
		delete[] doublearray_;
		doublearray_ = NULL;
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief set a single token value
	\param info
	\param value
	\note 
		this could be optimized if we would parse all floats and make one token out of them
		instead of creating a token per float

	Sets a single float as value. 
*/
void TokenFloatList::SetValue(TemporalInfo& info, const float_t value)
{
	count_ = 1;
	if (info.header.UseFloat32())
	{
		singlevalue_.floatvalue_ = value;
		floatarray_ = &singlevalue_.floatvalue_;
	}
	else
	{
		throw ParseException("32bit-function called while parsing 64bit-file!");
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief set a single token value
	\param info
	\param value
	\note 
		this could be optimized if we would parse all floats and make one token out of them
		instead of creating a token per float

	Sets a single float as value. 
*/
void TokenFloatList::SetValue(
	TemporalInfo& info,
	const double_t value
)
{
	count_ = 1;
	if (info.header.UseFloat32())
	{
		throw ParseException("64bit-function called while parsing 32bit-file!");
	}
	else
	{
		singlevalue_.doublevalue_ = value;
		doublearray_ = &singlevalue_.doublevalue_;
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Anzahl Elemente im Array zurückgeben
	\return Grösse

	Gibt die Anzahl Elemente zurück welche sich in diesem Array befinden
*/
const udword_t TokenFloatList::GetCount() const
{
	return count_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief 32-Bit Floats zurückgeben
	\return Pointer

	Gibt einen Pointer auf das Float-Array zurück.
*/
const float_t* const TokenFloatList::GetFloat32Array() const
{
	return floatarray_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief 64-Bit Floats zurückgeben
	\return Pointer

	Gibt einen Pointer auf das Float-Array zurück.
*/
const double_t* const TokenFloatList::GetFloat64Array() const
{
	return doublearray_;
}

}} // namespace x::tokens

