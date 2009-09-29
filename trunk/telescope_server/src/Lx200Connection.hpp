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

#ifndef _LX200_CONNECTION_HPP_
#define _LX200_CONNECTION_HPP_

#include "SerialPort.hpp"

#include <list>
using namespace std;

class Lx200Command;

class Lx200Connection : public SerialPort
{
public:
	Lx200Connection(Server &server, const char *serial_device);
	void sendGoto(unsigned int ra_int, int dec_int);
	void sendCommand(Lx200Command * command);
	void setTimeBetweenCommands(long long int micro_seconds)
	{
		time_between_commands = micro_seconds;
	}
	
private:
	void dataReceived(const char *&p, const char *read_buff_end);
	void sendPosition(unsigned int ra_int, int dec_int, int status) {}
	void resetCommunication(void);
	void prepareSelectFds(fd_set &read_fds, fd_set &write_fds, int &fd_max);
	bool writeFrontCommandToBuffer(void);
	void flushCommandList(void);
	
private:
	list<Lx200Command*> command_list;
	long long int time_between_commands;
	long long int next_send_time;
	long long int read_timeout_endtime;
	int goto_commands_queued;
};

#endif //_LX200_CONNECTION_HPP_
