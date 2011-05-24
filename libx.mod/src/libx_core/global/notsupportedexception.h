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
/*!	\file global/notsupportedexception.h
	\brief Definition von x::NotSupportedException
	\version 0.15.10.2002
	\author Christian Oberholzer

	Definition von x::NotSupportedException
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef GLOBAL_NOTSUPPORTEDEXCEPTION_H
#define GLOBAL_NOTSUPPORTEDEXCEPTION_H

namespace x { 

/*!	\brief Exception wenn etwas nicht unterstützt wird
	\version 0.1
	\date 30.10.2003 implementiert
	\author Christian Oberholzer

	Diese Exception wird geworfen wenn von der Engine aus ein Feature nicht unterstützt wird, welches als 
	Unterstützt angenommen werden könnte. Dies kann auch der Fall sein wenn ein Feature noch nicht 
	implementiert worden ist.
*/
class NotSupportedException : public Exception
{
public:
	//-------------------------------------------------------------------------------------------------------
	/*!	\brief Konstruktor
		\param strDescription Beschreibung

		Konstruktor
	*/
	NotSupportedException(const string_t& strDescription = UNICODE_STRING(""))
	:	Exception(strDescription)
	{
	}




	//-----------------------------------------------------------------------------------------------------------
	/*!	\brief Copy-Konstruktor

		Konstruktor
	*/
	NotSupportedException(const NotSupportedException& Rhs)
	:	Exception(Rhs)
	{
	}




private:
	NotSupportedException& operator = (const NotSupportedException&);
};

}; // namespace x

#endif // GLOBAL_NOTSUPPORTEDEXCEPTION_H
