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

#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include <list>
using namespace std;

class Socket;

class Server {
public:
  Server(int port);
  virtual ~Server(void) {}
  virtual void step(long long int timeout_micros);
protected:
  void sendPosition(unsigned int ra_int,int dec_int,int status);
    // called by Listener
  void addConnection(Socket *s) {if (s) socket_list.push_back(s);}
  friend class Listener;
private:
    // called by Connection:
  virtual void gotoReceived(unsigned int ra_int,int dec_int) = 0;
  friend class Connection;

  class SocketList : public list<Socket*> {
  public:
    ~SocketList(void) {clear();}
    void clear(void);
  };
  SocketList socket_list;
};

#endif
