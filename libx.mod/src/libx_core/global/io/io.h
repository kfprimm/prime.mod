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
/*!	\file global/io/io.h
	\brief Definition des I/O-Systems
	\version 0.1
	\date 12.9.2001
	\author Christian Oberholzer

	Definition des I/O-Systems
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef GLOBAL_IO_IO_H
#define GLOBAL_IO_IO_H

/*
 *	Definierte Exceptions
 */
#include "./exception/ioexception.h"
#include "./exception/bufferexception.h"
#include "./exception/readexception.h"
#include "./exception/fileexception.h"
#include "./exception/endoffileexception.h"
#include "./exception/filenotopenedexception.h"
#include "./exception/streamexception.h"
#include "./exception/nobufferexception.h"
#include "./exception/endofbufferexception.h"

/*
 *	Buffer
 */
#include "./buffer/buffer.h"
#include "./buffer/inputbuffer.h"
#include "./buffer/fileinputbuffer.h"

/*
 *	Streams
 */
#include "./stream/inputstream.h"
#include "./stream/bufferinputstream.h"

#endif // GLOBAL_IO_IO_H
