/**
 * Copyright (c) 2011 Johannes Schickel (LordHoto)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "macexe.h"

#include <cassert>
#include <cstring>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

const uint32 kCodeTag = 0x434F4445;

Executable::Executable(const std::string &filename) throw(std::exception)
    : _resFork(), _code0() {
	if (!_resFork.load(filename.c_str()))
		throw std::runtime_error("Could not load file " + filename);

	DataPair *data = _resFork.getResource(kCodeTag, 0);
	if (data == nullptr)
		throw std::runtime_error("File " + filename + " does not contain any CODE 0 segment");

	try {
		_code0 = std::auto_ptr<Code0Segment>(new Code0Segment(*data));
	} catch (std::exception &e) {
		destroy(data);
		throw;
	}

	destroy(data);
}

void Executable::outputInfo(std::ostream &out) throw() {
	assert(_code0.get() != nullptr);
	_code0->outputHeader(out);
	_code0->outputJumptable(out);
}

Code0Segment::Code0Segment(const DataPair &data) throw(std::exception)
    : _jumpTable(), _sizeAboveA5(0), _applicationGlobalsSize(0), _jumpTableSize(0), _jumpTableOffset(0) {
	if (data.length < 24)
		throw std::runtime_error("CODE 0 segment contains only " + boost::lexical_cast<std::string>(data.length) + " bytes");

	_sizeAboveA5 = READ_UINT32_BE(data.data + 0);
	_applicationGlobalsSize = READ_UINT32_BE(data.data + 4);
	_jumpTableSize = READ_UINT32_BE(data.data + 8);
	_jumpTableOffset = READ_UINT32_BE(data.data + 12);

	if (_jumpTableSize % 8 != 0)
		throw std::runtime_error("CODE 0 segment has invalid jump table size " + boost::lexical_cast<std::string>(_jumpTableSize));
	if (_sizeAboveA5 != _jumpTableSize + _jumpTableOffset)
		throw std::runtime_error("CODE 0 segment has invalid above a5 size " + boost::lexical_cast<std::string>(_sizeAboveA5));

	_jumpTable.resize(_jumpTableSize / 8);

	for (uint offset = 0, i = 0; offset < _jumpTableSize; offset += 8, ++i) {
		JumpTableEntry entry;
		std::memcpy(entry.rawData, data.data + 16 + offset, 8);
		_jumpTable[i] = entry;
	}
}

void Code0Segment::outputHeader(std::ostream &out) throw() {
	out << "CODE0 header\n"
	    << "============\n"
	    << "Size above A5: " << _sizeAboveA5 << "\n"
	    << "Global data size: " << _applicationGlobalsSize << "\n"
	    << "Jump table size: " << _jumpTableSize << "\n"
	    << "Jump table offset: " << _jumpTableOffset << "\n" << std::endl;
}

void Code0Segment::outputJumptable(std::ostream &out) throw() {
	out << "Jump table information\n"
	    << "======================\n"
	    << "Entries: " << _jumpTable.size() << "\n";

	for (uint i = 0, offset = 16, size = _jumpTable.size(); i < size; ++i, offset += 8) {
		const JumpTableEntry &entry = _jumpTable[i];

		out << "Entry " << i << ": Raw: " << boost::format("%1$02X%2$02X%3$02X%4$02X%5$02X%6$02X%7$02X%8$02X") % (uint)entry.rawData[0] % (uint)entry.rawData[1] %
		    (uint)entry.rawData[2] % (uint)entry.rawData[3] % (uint)entry.rawData[4] % (uint)entry.rawData[5] % (uint)entry.rawData[6] % (uint)entry.rawData[7] << "\n";
	}

	out << "\n" << std::flush;
}

