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

#define XLIB_CORE
#include "./interface.h"

namespace x {

using namespace std;
using namespace x::templates;

//-----------------------------------------------------------------------------------------------------------
bool_t Interface::Import(
	const string_t& path,
	instances_t& instances
)
{
	try {
		FileInputBuffer* input_buffer(new FileInputBuffer);
		input_buffer->Open(path);
		BufferInputStream input;
		input.SetBuffer(input_buffer);
		return Import(input, instances);
	}
	catch (Exception& e) {
		cerr << "Exception occured! (" << e.GetName() << ", " << e.GetDescription() << ")" << endl;
		return false;
	}
}




//-----------------------------------------------------------------------------------------------------------
bool_t Interface::Import(
	BufferInputStream& input,
	instances_t& instances
)
{
	try {
		Serializer serializer;
		serializer.Import(input, instances);
	}
	catch (Exception& e) {
		cout << "Exception occured! (" << e.GetName() << ", " << e.GetDescription() << ")" << endl;
		return false;
	}

	return true;
}




//-----------------------------------------------------------------------------------------------------------
void Interface::ReleaseInstances(
	instances_t& instances
)
{
	for (
		instances_t::iterator begin = instances.begin();
		begin != instances.end();
		++begin
	)
	{
		Template* current_template = (*begin);
		current_template->RemRef();
	}

	instances.clear();
}

} // namespace x
