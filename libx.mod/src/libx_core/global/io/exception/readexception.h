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
/*!	\file global/io/exception/readexception.h
	\brief Definition von x::ReadException
	\version 0.1
	\date 5.10.2002
	\author Christian Oberholzer

	Definition von x::ReadException
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef GLOBAL_IO_EXCEPTION_READEXCEPTION
#define GLOBAL_IO_EXCEPTION_READEXCEPTION

#include "../../global.h"
#include "./bufferexception.h"

namespace x { 

/*!	\brief Fehler beim Lesen aus einem Buffer
	\version 0.1
	\date 9.1.2003
	\author Christian Oberholzer

	Diese Exception wird von Buffern geworfen wenn ein Fehler beim Lesen von 
	Daten aufgetreten ist.
*/
class ReadException : public BufferException
{
public:
	//-------------------------------------------------------------------------------------------------------
	/*!	\brief Konstruktor
		\param strDescription Beschreibung der BufferException

		Konstruktor
	*/
	ReadException(const string_t& strDescription = UNICODE_STRING(""))
	:	BufferException(strDescription)
	{
	}




	//-------------------------------------------------------------------------------------------------------
	/*!	\brief Copy-Konstruktor
		\param Rhs

		Copy-Konstruktor
	*/
	ReadException(const ReadException& Rhs)
	:	BufferException(Rhs)
	{
	}

private:
	//-----------------------------------------------------------------------------------------------------------
	/*!	\brief Versteckt

		Die Verwendung des Assign-Operators ist auf Kaos-Exceptions ung?ltig.
	*/
	ReadException& operator = (const ReadException&);
};

} // namespace x

#endif // GLOBAL_IO_EXCEPTION_READEXCEPTION
