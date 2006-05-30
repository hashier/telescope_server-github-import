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

#ifndef _SOCKET_HPP_
#define _SOCKET_HPP_

#ifdef WIN32
  #include <winsock2.h>
  #define ERRNO WSAGetLastError()
  #undef EAGAIN
  #define EAGAIN WSAEWOULDBLOCK
  #undef EINTR
  #define EINTR WSAEINTR
  static u_long ioctlsocket_arg = 1;
  #define SETNONBLOCK(s) ioctlsocket(s,FIONBIO,&ioctlsocket_arg)
  #define SOCKLEN_T int
  #define close closesocket
  #define IS_INVALID_SOCKET(fd) (fd==INVALID_SOCKET)
  #define WRITE(fd,buf,count) send(fd,buf,count,0)
  #define READ(fd,buf,count) recv(fd,buf,count,0)
  #define STRERROR(x) x
#else
  #include <netdb.h>
  #include <netinet/in.h>
  #include <sys/socket.h>
  #include <unistd.h>
  #include <fcntl.h>
  #include <errno.h>
  #include <string.h> // strerror
  #define ERRNO errno
  #define SETNONBLOCK(s) fcntl(s,F_SETFL,O_NONBLOCK)
  #define SOCKLEN_T socklen_t
  #define SOCKET int
  #define IS_INVALID_SOCKET(fd) (fd<0)
  #define INVALID_SOCKET (-1)
  #define WRITE(fd,buf,count) write(fd,buf,count)
  #define READ(fd,buf,count) read(fd,buf,count)
  #define STRERROR(x) strerror(x)
#endif

long long int GetNow(void);

class Server;

class Socket {
public:
  virtual ~Socket(void) {hangup();}
  void hangup(void);
  virtual void prepareSelectFds(fd_set &read_fds,fd_set &write_fds,
                                int &fd_max) = 0;
  virtual void handleSelectFds(const fd_set &read_fds,
                               const fd_set &write_fds) = 0;
  virtual bool isClosed(void) const {return (fd<0);}
  virtual void sendPosition(unsigned int ra_int,int dec_int,int status) {}
protected:
  Socket(Server &server,SOCKET fd) : server(server),fd(fd) {}
  Server &server;
  SOCKET fd;
private:
    // no copying
  Socket(const Socket&);
  const Socket &operator=(const Socket&);
};

#endif
