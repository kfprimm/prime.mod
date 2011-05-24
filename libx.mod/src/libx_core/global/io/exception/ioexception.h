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
/*!	\file global/io/exception/ioexception.h
	\brief Definition von x::IoException
	\version 0.1
	\date 5.10.2002
	\author Christian Oberholzer

	Definition von x::IoException
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef GLOBAL_IO_EXCEPTION_IOEXCEPTION_H
#define GLOBAL_IO_EXCEPTION_IOEXCEPTION_H

#include "../../global.h"

namespace x { 

/*!	\brief Input/Output-Exception
	\version 0.1
	\date 28.10.2003
	\author Christian Oberholzer

	Diese Klasse ist die Basisklasse s?mtlicher Input/Output-Exceptions der Kaos-Engine.
*/
class IoException : public Exception
{
public:
	//-------------------------------------------------------------------------------------------------------
	/*!	\brief Konstruktor
		\param strDescription Beschreibung der Exception

		Konstruktor
	*/
	IoException(const string_t& strDescription = UNICODE_STRING(""))
	:	Exception(strDescription)
	{
	}




	//-------------------------------------------------------------------------------------------------------
	/*!	\brief Copy Konstruktor
		\param Rhs 

		Kopiert die ?bergebene Exception.
	*/
	IoException(const IoException& Rhs)
	:	Exception(Rhs)
	{
	}




private:
	//-----------------------------------------------------------------------------------------------------------
	/*!	\brief Versteckt
		
		Die Verwendung des Assign-Operators ist auf Kaos-Exceptions ung?ltig.
	*/
	IoException& operator = (const IoException&);
};

} // namespace x

#endif // GLOBAL_IO_EXCEPTION_IOEXCEPTION_H
