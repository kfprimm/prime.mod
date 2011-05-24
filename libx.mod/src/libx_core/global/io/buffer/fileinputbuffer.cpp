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
/*!	\file global/io/buffer/fileinputbuffer.cpp
	\brief Implementation von x::FileInputBuffer
	\version 0.1
	\date 30.8.2003
	\author Christian Oberholzer

	Implementation von x::FileInputBuffer
*/
//-----------------------------------------------------------------------------------------------------------
#define K_KERNEL
#include <stdlib.h>
#include "../io.h"

namespace x {

using namespace std;

//-----------------------------------------------------------------------------------------------------------
/*!	\brief Konstruktor

	Konstruktor
*/
FileInputBuffer::FileInputBuffer()
:	InputBuffer(),
	m_File()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Destruktor
	
	Destruktor
*/
FileInputBuffer::~FileInputBuffer()
{
	if (IsOpened())
		Close();
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Datei öffnen
	\param strPath Dateiname

	Öffnet die über den übergebenen Pfad angegebene Datei. Wenn dabei ein Fehler auftritt wird eine
	Exception geworfen.
*/
void FileInputBuffer::Open(const string_t& strPath)
{
	m_File.open(strPath.c_str(), ios_base::in | ios_base::binary);
	if (m_File.fail())
	{
		throw FileException(
			UNICODE_STRING("Datei konnte nicht geöffnet werden! Dateiname: '") +
			strPath + 
			UNICODE_STRING("'.")
		);
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Datei schliessen
	
	Schliesst die Datei.
*/
void FileInputBuffer::Close()
{
	_TestFileOpened();
	m_File.close();
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Position setzen
	\param uiNewPosition Neue Position

	Setzt die Position von welcher aus gelesen wird neu. Falls die Position hinter das Ende des Streams
	gesetzt werden soll, so wird eine io::OutOfBoundsException geworfen.
*/
void FileInputBuffer::SetPosition(const uint_t uiNewPosition)
{
	m_File.seekg(uiNewPosition);
	if (!m_File.good())
		throw FileException(UNICODE_STRING("fseek() failed!"));
	m_uiPosition = uiNewPosition;
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Elemente lesen
	\param pBuffer Buffer in welchen die Elemente gelesen werden
	\param uiNum Anzahl der zu lesenden Elemente
	
	Diese Methode liest die gewünschte Anzahl Elemente und schreibt sie in den übergebenen Buffer. Wenn
	hinter das Ende der Datei gelesen wurde, so wird eine EndOfFileException geworfen. Als Parameter wird
	der EndOfFileException die Anzahl der Bytes übergeben welche doch noch in pBuffer gelesen werden 
	konnten.
*/
void FileInputBuffer::Read(ubyte_t* const pBuffer, const uint_t uiNum)
{
	_TestFileOpened();
	m_File.read((char*)pBuffer, uiNum);
	if (m_File.fail())
	{
		throw EndOfFileException(
			UNICODE_STRING("read failed, eof"),
			pBuffer,
			m_File.gcount()
		);
	}

	m_uiPosition += (uint_t)uiNum;
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Pointer auf den gesamten Inhalt zurückgeben
	\return Pointer

	Gibt einen Pointer auf den gesamten Inhalt zurück. 
*/
const ubyte_t* FileInputBuffer::GetContent() const
{
	throw NotSupportedException(
		UNICODE_STRING("FileInputBuffer unterstützt die Methode InputBuffer::GetContent nicht!")
	);
	return NULL;
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Am Ende angelangt?
	\return Flag

	Gibt ein Flag zurück, welches anzeigt ob wir mit Lesen am Ende des Buffers angelangt sind und keine
	weiteren Daten zum Lesen zur Verfügung stehen.
*/
const bool FileInputBuffer::Eof() const
{
	bool_t bEof = m_File.eof();
	if (bEof) return true;

	// HACK damit EOF() funktioniert wie gewünscht!

	ubyte_t buffer[2];
	uint_t pos = const_cast<ifstream&>(m_File).tellg();
	const_cast<ifstream&>(m_File).read((char*)buffer, 1);
	bEof = m_File.eof();
	const_cast<ifstream&>(m_File).seekg(pos);

	return bEof;
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Beschreibung 
	\return Beschreibung des Buffers

	Gibt eine Beschreibung für den Benutzer sinnvolle Beschreibung (und wenns nur der Klassenname) des
	abgeleiteten Buffers zurück.
*/
const string_t& FileInputBuffer::GetDescription() const
{
	static string_t strDescription(UNICODE_STRING("FileInputBuffer"));
	return strDescription;
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Datei noch geöffnet?
	\return Flag

	Gibt zurück ob die Datei geöffnet ist.
*/
const bool FileInputBuffer::IsOpened() const
{
	return m_File.is_open();
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Prüft ob die Datei geöffnet ist

	Wirft eine io::FileNotOpenedException falls die Inputdatei nicht geöffnet ist.
*/
void FileInputBuffer::_TestFileOpened() 
{
	if (!m_File.is_open())
		throw FileNotOpenedException();
}

} // namespace x
