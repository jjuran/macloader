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

#ifndef IDC_H
#define IDC_H

#include "macexe.h"

#include <string>
#include <stdexcept>

namespace IDC {

/**
 * Create the memory dump initialization script for IDA Pro.
 *
 * @param exe The executable to write the script for.
 * @param baseFilename The base filename.
 */
void writeMemDumpInitScript(const Executable &exe, const std::string &baseFilename) throw(std::exception);

} // End of namespace IDC

#endif

