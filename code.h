/**
 * Copyright (c) 2011 Johannes Schickel (LordHoto)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef CODE_H
#define CODE_H

#include "macresfork.h"
#include "code0.h"

#include <string>

/**
 * Any code segement different to CODE 0
 */
class CodeSegment {
public:
	/**
	 * Load a Code segment.
	 *
	 * @param code0 The code 0 segement.
	 * @param id The id of the code segment.
	 * @param name The name of the code segment.
	 * @param pair The resource data to load from.
	 * @throws std::exception Errors on loading.
	 */
	CodeSegment(const Code0Segment &code0, const uint id, const std::string &name, const DataPair &pair) throw(std::exception);

	/**
	 * Output information about the segment header.
	 *
	 * @param out The stream to output to.
	 */
	void outputHeader(std::ostream &out) const throw();

	/**
	 * Query the segment name.
	 */
	const std::string &getName() const { return _name; }

	/**
	 * Query the size of the whole segment.
	 */
	uint32 getSegmentSize() const { return _data.length; }

	/**
	 * Write the segment into memory.
	 *
	 * @param code0 CODE0 Segement containing the jump table.
	 * @param memory Where to write to.
	 * @param offset The offset into the memory.
	 * @param size Size of the memory.
	 */
	void loadIntoMemory(Code0Segment &code0, uint8 *memory, uint32 offset, uint32 size) const throw(std::exception);
private:
	/**
	 * The id of the segment.
	 */
	const uint _id;

	/**
	 * The name of the segment.
	 */
	const std::string _name;

	/**
	 * Offset into the jump table.
	 */
	uint16 _jumpTableOffset;

	/**
	 * Number of exported functions in the jump table.
	 */
	uint16 _jumpTableEntries;

	/**
	 * The segment data.
	 */
	DataPair _data;
};


#endif
