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
/*!	\file global/io/exception/filenotopenedexception.h
	\brief Definition von x::FileNotOpenedException
	\version 0.1
	\date 5.10.2002
	\author Christian Oberholzer

	Definition von x::FileNotOpenedException
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef GLOBAL_IO_EXCEPTION_FILENOTOPENEDEXCEPTION_H
#define GLOBAL_IO_EXCEPTION_FILENOTOPENEDEXCEPTION_H

#include "../../global.h"
#include "./fileexception.h"

namespace x {

/*!	\brief Wird aus einem File-Objekt geworfen wenn dieses nicht geöffnet worden ist
	\version 0.1
	\date 9.1.2003
	\author Christian Oberholzer

	Diese Exception wird aus File-Objekten immer dann geworfen wenn auf eine Datei zugegriffen
	werden sollte, aber noch keine Datei geöffnet worden ist.
*/
class FileNotOpenedException : public FileException
{
public:
	//-------------------------------------------------------------------------------------------------------
	/*!	\brief Konstruktor
		\param strDescription Beschreibung der Exception

		Konstruktor
	*/
	FileNotOpenedException(const string_t& strDescription = UNICODE_STRING(""))
	:	FileException(strDescription)
	{
	}




	//-------------------------------------------------------------------------------------------------------
	/*!	\brief Copy Konstruktor
		\param Rhs 

		Kopiert die übergebene Exception.
	*/
	FileNotOpenedException(const FileNotOpenedException& Rhs)
	:	FileException(Rhs)
	{
	}

private:
	//-----------------------------------------------------------------------------------------------------------
	/*!	\brief Versteckt

		Die Verwendung des Assign-Operators ist auf Kaos-Exceptions ungültig.
	*/
	FileNotOpenedException& operator = (const FileNotOpenedException&);
};

} // namespace x

#endif // GLOBAL_IO_EXCEPTION_FILENOTOPENEDEXCEPTION_H
