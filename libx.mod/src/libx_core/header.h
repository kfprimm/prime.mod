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
/*!	\file header.h
	\brief Definition von x::Header
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::Header
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef HEADER_H
#define HEADER_H

#include "./global/global.h"

namespace x {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief .x-File-Header
	\version 0.1
	\date 2.4.2004
	\author Christian Oberholzer

	Diese Klasse implementiert den Header eines .x-files.
*/
class Header
{
public:
	//-------------------------------------------------------------------------------------------------------
	// Konstruktur & Destruktor
	Header();
	~Header();

	//-------------------------------------------------------------------------------------------------------
	void Read(BufferInputStream& input);
	void CheckMagic() const;
	const bool_t IsBinary() const;
	const bool_t IsText() const;
	const bool_t IsBinaryCompressed() const;
	const bool_t IsTextCompressed() const;
	const bool_t UseFloat32() const;
	const bool_t UseFloat64() const;

protected:
	/*!
		\var magic_
		\brief Magic-Folge

		X-File-Magic-Folge. Muss in jedem File am Anfang stehen
	*//*!
		\var version_
		\brief Versionsinfo

		X-File-Version
	*//*!
		\var filetype_
		\brief Dateityp?

		Speichert ob es sich um ein Text-, Binary- oder komprimiertes
		File handelt
	*//*!
		\var floattype_
		\brief Floatgrösse

		Speichert die grösse eines einzelnen Floats
	*/

	ubyte_t magic_[4];
	ubyte_t version_[4];
	ubyte_t filetype_[4];
	ubyte_t floattype_[4];
};

} // namespace x

#endif // HEADER_H
