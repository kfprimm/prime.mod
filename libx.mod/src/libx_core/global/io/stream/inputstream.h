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
/*!	\file global/io/stream/inputstream.h
	\brief Definition von x::InputStream
	\version 0.1
	\date 14.11.2003
	\author Christian Oberholzer

	Definition von x::InputStream
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef GLOBAL_IO_STREAM_INPUTSTREAM_H
#define GLOBAL_IO_STREAM_INPUTSTREAM_H

#include "../../global.h"

namespace x {

class InputStream
{
public:
	InputStream() {}
	virtual ~InputStream() {}

	virtual void Read(bool_t&) = 0;
	virtual void Read(schar_t&) = 0;
	virtual void Read(swchar_t&) = 0;
	virtual void Read(signed_char_t&) = 0;
	virtual void Read(unsigned_char_t&) = 0;
	virtual void Read(signed_short_t&) = 0;
	virtual void Read(unsigned_short_t&) = 0;
	virtual void Read(signed_int_t&) = 0;
	virtual void Read(unsigned_int_t&) = 0;
	virtual void Read(signed_long_t&) = 0;
	virtual void Read(unsigned_long_t&) = 0;
	virtual void Read(sfloat_t&) = 0;
	virtual void Read(sdouble_t&) = 0;
	virtual void Read(ubyte_t* const pBuffer, const uint_t uiLength) = 0;

	virtual const string_t& GetDescription() const = 0;
};




//-----------------------------------------------------------------------------------------------------------
/*!
*/
template <
	typename TypeT
>
inline InputStream& operator >> (InputStream& Stream, TypeT& Item)
{
	Stream.Read(Item);
	return Stream;
}

} // namespace x

#endif // GLOBAL_IO_STREAM_INPUTSTREAM_H
