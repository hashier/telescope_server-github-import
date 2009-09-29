/*
The stellarium telescope library helps building
telescope server programs, that can communicate with stellarium
by means of the stellarium TCP telescope protocol.
It also contains smaple server classes (dummy, Meade LX200).

Author and Copyright of this file and of the stellarium telescope library:
Johannes Gajdosik, 2006

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _SERIAL_PORT_HPP_
#define _SERIAL_PORT_HPP_

#include "Connection.hpp"

#ifdef WIN32
  #include <windows.h>
#else
  #include <termios.h>
#endif

class SerialPort : public Connection
{
public:
	SerialPort(Server &server, const char *serial_device);
	~SerialPort(void);
	virtual bool isClosed(void) const
	{
	#ifdef WIN32
		return (handle == INVALID_HANDLE_VALUE);
	#else
		return IS_INVALID_SOCKET(fd);
	#endif
	}
	
protected:
	void prepareSelectFds(fd_set&, fd_set&, int&);
	
private:
	bool isTcpConnection(void) const {return false;}
	bool isAsciiConnection(void) const {return true;}
	
private:
#ifdef WIN32
	int readNonblocking(char *buf, int count);
	int writeNonblocking(const char *buf, int count);
	void handleSelectFds(const fd_set&, const fd_set&) {}
	HANDLE handle;
	DCB dcb_original;
#else
	struct termios termios_original;
#endif
};

#endif
