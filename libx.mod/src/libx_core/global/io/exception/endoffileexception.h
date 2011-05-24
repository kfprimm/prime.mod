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
/*!	\file global/io/exception/endoffileexception.h
	\brief Definition von x::EndOfFileException
	\version 0.1
	\date 5.10.2002
	\author Christian Oberholzer

	Definition von x::EndOfFileException
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef GLOBAL_IO_EXCEPTION_ENDOFFILEEXCEPTION_H
#define GLOBAL_IO_EXCEPTION_ENDOFFILEEXCEPTION_H

#include "../../global.h"
#include "./fileexception.h"

namespace x {

/*!	\brief Das Ende der Datei wurde erreicht
	\version 0.1
	\date 28.10.2003
	\author Christian Oberholzer

	Diese Exception wird geworfen, wenn bei einer Leseaktion hinter das Ende der Datei gelesen
	werden soll, oder der Filepointer hinter dem Ende oder vor dem Anfang der Datei
	positioniert werden soll.
*/
class EndOfFileException : public FileException
{
public:
	//-------------------------------------------------------------------------------------------------------
	/*!	\brief Konstruktor
		\param strDescription Beschreibung der EndOfFileException
		\param pBuffer
		\param uiNumBytesRead

		Konstruktor
	*/
	EndOfFileException(const string_t& strDescription = UNICODE_STRING(""),
					   ubyte_t* pBuffer = NULL,
					   uint_t uiNumBytesRead = 0)
	:	FileException(strDescription),
		m_pBuffer(pBuffer),
		m_uiNumBytesRead(uiNumBytesRead)
	{
	}




	//-------------------------------------------------------------------------------------------------------
	/*!	\brief Copy Konstruktor
		\param Rhs 

		Kopiert die übergebene EndOfFileException.
	*/
	EndOfFileException(const EndOfFileException& Rhs)
	:	FileException(Rhs),
		m_pBuffer(Rhs.m_pBuffer),
		m_uiNumBytesRead(Rhs.m_uiNumBytesRead)
	{
	}




	//-------------------------------------------------------------------------------------------------------
	/*!	\brief Speicher für die gelesenen Bytes zurückgeben
		\return Pointer auf den Buffer

		Gibt einen Pointer auf den Speicher in welchen die doch noch gelesenen Bytes
		gespeichert worden sind zurück.
	*/
	ubyte_t* const GetData() const
	{
		return m_pBuffer;
	}




	//-------------------------------------------------------------------------------------------------------
	/*!	\brief Anzahl der doch noch gelesenen Bytes zurückgeben
		\return Anzahl Bytes

		Gibt die Anzahl der Bytes zurück welche doch noch gelesen werden konnten.
	*/
	const uint_t GetNumBytesRead() const
	{
		return m_uiNumBytesRead;
	}

private:
	//-----------------------------------------------------------------------------------------------------------
	/*!	\brief Versteckt
		
		Die Verwendung des Assign-Operators ist auf Kaos-Exceptions ungültig.
	*/
	EndOfFileException& operator = (const EndOfFileException&);

protected:
	/*!
		\var m_pBuffer
		\brief Pointer auf den Buffer in welchen die Bytes gespeichert worden sind

		Dieser Pointer zeigt auf den Buffer in welchen die doch noch gelesenen Bytes
		geschrieben worden sind.
	*//*!
		\var m_uiNumBytesRead
		\brief Anzahl der doch noch gelesenen Bytes
		
		Speichert die Anzahl der Bytes welche doch noch gelesen worden sind.
	*/

	ubyte_t* m_pBuffer;
	uint_t m_uiNumBytesRead;
};

} // namespace x

#endif // GLOBAL_IO_EXCEPTION_ENDOFFILEEXCEPTION_H
