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
/*!	\file guid.h
	\brief Definition von x::Guid
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::Guid
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef GUID_H
#define GUID_H

#include "./global/global.h"
#include "./temporalinfo.h"

namespace x {

/*!
	\brief GUID-Struktur
	\version 0.1
	\date 15.4.2004
	\author Christian Oberholzer

	GUID-Klasse fürs einfache Handling von Templates
*/
class Guid
{
public:
	//-------------------------------------------------------------------------------------------------------
	/*!
		\brief standardkonstruktor

		Konstruktor
	*/
	inline Guid()
	{
		field1_ = 0;
		field2_ = 0;
		field3_ = 0;
		field4_[0] = 0;
		field4_[1] = 0;
		field4_[2] = 0;
		field4_[3] = 0;
		field4_[4] = 0;
		field4_[5] = 0;
		field4_[6] = 0;
		field4_[7] = 0;
	}




	//-------------------------------------------------------------------------------------------------------
	/*!
		\brief Konstruktor
		\param field1
		\param field2
		\param field3
		\param field4_1
		\param field4_2
		\param field4_3
		\param field4_4
		\param field4_5
		\param field4_6
		\param field4_7
		\param field4_8

		Konstruktor
	*/
	inline Guid(
		udword_t field1, 
		uword_t field2, 
		uword_t field3,
		ubyte_t field4_1, ubyte_t field4_2, ubyte_t field4_3, ubyte_t field4_4,
		ubyte_t field4_5, ubyte_t field4_6, ubyte_t field4_7, ubyte_t field4_8)
	{
		field1_ = field1;
		field2_ = field2;
		field3_ = field3;
		field4_[0] = field4_1;
		field4_[1] = field4_2;
		field4_[2] = field4_3;
		field4_[3] = field4_4;
		field4_[4] = field4_5;
		field4_[5] = field4_6;
		field4_[6] = field4_7;
		field4_[7] = field4_8;
	}




	//-------------------------------------------------------------------------------------------------------
	/*!
		\brief Copy Konstruktor

		Copy Konstrukotr
	*/
	inline Guid(
		const Guid& rhs
	)
	{
		field1_ = rhs.field1_;
		field2_ = rhs.field2_;
		field3_ = rhs.field3_;
		field4_[0] = rhs.field4_[0];
		field4_[1] = rhs.field4_[1];
		field4_[2] = rhs.field4_[2];
		field4_[3] = rhs.field4_[3];
		field4_[4] = rhs.field4_[4];
		field4_[5] = rhs.field4_[5];
		field4_[6] = rhs.field4_[6];
		field4_[7] = rhs.field4_[7];
	}




	//-------------------------------------------------------------------------------------------------------
	/*!
		\brief Assign-Operator
		\param rhs
		\return Referenz auf diese GUID

		Kopiert die übergebene Guid
	*/
	inline const Guid& operator = (
		const Guid& rhs)
	{
		field1_ = rhs.field1_;
		field2_ = rhs.field2_;
		field3_ = rhs.field3_;
		field4_[0] = rhs.field4_[0];
		field4_[1] = rhs.field4_[1];
		field4_[2] = rhs.field4_[2];
		field4_[3] = rhs.field4_[3];
		field4_[4] = rhs.field4_[4];
		field4_[5] = rhs.field4_[5];
		field4_[6] = rhs.field4_[6];
		field4_[7] = rhs.field4_[7];
		return (*this);
	}




	//-------------------------------------------------------------------------------------------------------
	/*!
		\brief Vergleichsoperator
		\param rhs
		\return Flag

		Vergleicht diese und die übergebene Guid
	*/
	inline const bool_t operator == (
		const Guid& rhs
	) const
	{
		return (
			field1_ == rhs.field1_ &&
			field2_ == rhs.field2_ &&
			field3_ == rhs.field3_ &&
			field4_[0] == rhs.field4_[0] && 
			field4_[1] == rhs.field4_[1] && 
			field4_[2] == rhs.field4_[2] && 
			field4_[3] == rhs.field4_[3] && 
			field4_[4] == rhs.field4_[4] && 
			field4_[5] == rhs.field4_[5] && 
			field4_[6] == rhs.field4_[6] && 
			field4_[7] == rhs.field4_[7]
		);
	}



protected:
	udword_t field1_;
	uword_t field2_;
	uword_t field3_;
	ubyte_t field4_[8];
};

} // namespace x

#endif // GUID_H
