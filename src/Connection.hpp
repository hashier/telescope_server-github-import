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

#ifndef _CONNECTION_HPP_
#define _CONNECTION_HPP_

#include "Socket.hpp"

class Connection : public Socket
{
public:
	Connection(Server &server, SOCKET fd);
	long long int getServerMinusClientTime(void) const
	{
		return server_minus_client_time;
	}
	
protected:
	void performReading(void);
	void performWriting(void);
	void prepareSelectFds(fd_set &read_fds,fd_set &write_fds,int &fd_max);
	
private:
	virtual bool isTcpConnection(void) const {return true;}
	virtual bool isAsciiConnection(void) const {return false;}
	void handleSelectFds(const fd_set &read_fds, const fd_set &write_fds);
	virtual void dataReceived(const char *&p, const char *read_buff_end);
	void sendPosition(unsigned int ra_int, int dec_int, int status);
	
protected:
	char read_buff[120];
	char *read_buff_end;
	char write_buff[120];
	char *write_buff_end;
	
private:
	long long int server_minus_client_time;
};

#endif //_CONNECTION_HPP_
