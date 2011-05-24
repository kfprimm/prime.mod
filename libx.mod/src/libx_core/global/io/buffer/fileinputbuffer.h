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
/*!	\file global/io/buffer/fileinputbuffer.h
	\brief Definition von x::FileInputBuffer
	\version 0.1
	\date 23.4.2003
	\author Christian Oberholzer

	Definition von x::FileInputBuffer
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef GLOBAL_IO_BUFFER_FILEINPUTBUFFER_H
#define GLOBAL_IO_BUFFER_FILEINPUTBUFFER_H

#include <fstream>
#include <iostream>
#include "../../global.h"
#include "./inputbuffer.h"

namespace x {

using std::ifstream;

/*!	\brief File-Bufferklasse
	\version 0.1
	\date 23.4.2003
	\author Christian Oberholzer

	Der FileInputBuffer liest Elemente aus einem File von Elementen.

	Changes:
		- 29.10.2003 Christian Oberholzer Der FileInputBuffer löst den kMemByteStream
		- 23.04.2003 Christian Oberholzer implementiert und löst die Klasse kDataStream ab
*/
class FileInputBuffer : public InputBuffer
{
public:
	FileInputBuffer();
	virtual ~FileInputBuffer();

	virtual void Open(const string_t& strPath);
	virtual void Close();

	virtual void SetPosition(const uint_t uiNewPosition);
	virtual void Read(ubyte_t* const pBuffer, const uint_t uiNum);
	virtual const ubyte_t* GetContent() const;
	virtual const bool Eof() const;
	virtual const string_t& GetDescription() const;
	virtual const bool IsOpened() const;

protected:
	void _TestFileOpened();

protected:
	/*! 
		\var m_File;
		\brief Dateipointer
		
		InputStream
	*/

	ifstream m_File;
};

} // namespace x

#endif // GLOBAL_IO_BUFFER_FILEINPUTBUFFER_H
