/*
 * Author and Copyright of this file and of the stellarium telescope feature:
 * Johannes Gajdosik, 2006
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "LogFile.hpp"

#include <iostream>

ostream *log_file = &cout;

static ofstream log_stream;

void SetLogFile(const char *name) {
  if (log_file == &log_stream) {
    log_stream.close();
  }
  if (name && name[0]) {
    log_file = &log_stream;
    log_stream.open(name,ios::out|ios::trunc);
  } else {
    log_file = &cout;
  }
}
