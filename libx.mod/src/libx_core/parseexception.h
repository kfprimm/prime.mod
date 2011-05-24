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
/*!	\file parseexception.h
	\brief Definition von x::ParseException
	\version 0.1
	\date 5.10.2002
	\author Christian Oberholzer

	Definition von x::ParseException
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef PARSEEXCEPTION_H
#define PARSEEXCEPTION_H

#include "./global/global.h"

namespace x { 

/*!	\brief thrown if there's an error while parsing
	\version 0.1
	\date 07.05.2003
	\author Christian Oberholzer

	this exception will be thrown by the serializer if there's an error during the parsing 
	process.
*/
class ParseException : public Exception
{
public:
	//-------------------------------------------------------------------------------------------------------
	/*!	\brief Konstruktor
		\param description Beschreibung der Exception

		Konstruktor
	*/
	ParseException(const string_t& description = UNICODE_STRING(""))
	:	Exception(description)
	{
	}




	//-------------------------------------------------------------------------------------------------------
	/*!	\brief Copy Konstruktor
		\param rhs 

		Kopiert die ?bergebene Exception.
	*/
	ParseException(const ParseException& rhs)
	:	Exception(rhs)
	{
	}

private:
	//-----------------------------------------------------------------------------------------------------------
	/*!	\brief Versteckt

		Die Verwendung des Assign-Operators ist auf Kaos-Exceptions ung?ltig.
	*/
	ParseException& operator = (const ParseException&);
};

} // namespace x

#endif // PARSEEXCEPTION_H
