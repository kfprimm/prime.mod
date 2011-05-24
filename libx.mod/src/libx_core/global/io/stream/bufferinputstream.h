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
/*!	\file global/io/stream/bufferinputstream.h
	\brief Definition von x::BufferInputStream
	\version 0.1
	\date 14.11.2003
	\author Christian Oberholzer

	Definition von x::BufferInputStream
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef GLOBAL_IO_STREAM_BUFFERINPUTSTREAM_H
#define GLOBAL_IO_STREAM_BUFFERINPUTSTREAM_H

#include "../../global.h"
#include "../buffer/inputbuffer.h"
#include "./inputstream.h"

namespace x {

template <
	typename ItemT
>
class InputStreamIterator;

class BufferInputStream : public InputStream
{
public:
	BufferInputStream();
	virtual ~BufferInputStream();

	virtual void SetBuffer(InputBuffer* pBuffer); // übernimmt besitzt!
	virtual InputBuffer* GetBuffer();
	virtual InputBuffer* ResetBuffer(); // Buffer zurückgeben, gibt Besitz ab!

	/*
	 *	Lese-Methoden
	 */
	virtual void Read(bool_t&);
	virtual void Read(schar_t&);
	virtual void Read(swchar_t&);
	virtual void Read(signed_char_t&);
	virtual void Read(unsigned_char_t&);
	virtual void Read(signed_short_t&);
	virtual void Read(unsigned_short_t&);
	virtual void Read(signed_int_t&);
	virtual void Read(unsigned_int_t&);
	virtual void Read(signed_long_t&);
	virtual void Read(unsigned_long_t&);
	virtual void Read(sfloat_t&);
	virtual void Read(sdouble_t&);
	virtual void Read(ubyte_t* const pBuffer, const uint_t uiLength);
	
	/*
	 *	Positionierungen
	 */
	virtual void SetPosition(const uint_t uiNewPosition);
	virtual const uint_t GetPosition() const;
	virtual void PushPosition();
	virtual void PopPosition();
	virtual void DiscardPosition();

	/*
	 *	Abfragen
	 */
	virtual const bool Eof() const;
	virtual const string_t& GetDescription() const;

protected:
	void _TestBufferAvailable() const;

protected:
	InputBuffer* m_pInput;
};



//-----------------------------------------------------------------------------------------------------------
/*!
*/
template <
	typename TypeT
>
inline BufferInputStream& operator >> (BufferInputStream& Stream, TypeT& Item)
{
	Stream.Read(Item);
	return Stream;
}


} // namespace x

#endif // GLOBAL_IO_STREAM_BUFFERINPUTSTREAM_H
