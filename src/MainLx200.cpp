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

#include "ServerLx200.hpp"

#ifdef WIN32
  #include "Socket.hpp" // winsock2
#endif

static volatile bool continue_looping = true;

#ifndef WIN32
#include <signal.h>
static
void signal_handler(int signum) {
  switch (signum) {
    case SIGINT:
    case SIGQUIT:
    case SIGTERM:
      continue_looping = false;
      break;
    default:
        // just ignore
      break;
  }
}
#endif


#include <iostream>
using namespace std;


int main(int argc,char *argv[]) {
#ifdef WIN32
  WSADATA wsaData;
  if (WSAStartup(0x202,&wsaData) != 0) {
    cerr << "WSAStartup failed" << endl;
    return 127;
  }
#else
    // SIGPIPE is normal operation when we send while the other side
    // has already closed the socket. We must ignore it:
  signal(SIGPIPE,SIG_IGN);
  signal(SIGINT,signal_handler);
  signal(SIGTERM,signal_handler);
  signal(SIGQUIT,signal_handler);
    // maybe the user wants to continue after SIGHUP ?
  //signal(SIGHUP,signal_handler);
#endif

  int port;
  if (argc != 3 ||
      1 != sscanf(argv[1],"%d",&port) ||
      port < 0 || port > 0xFFFF) {
    cerr << "Usage: " << argv[0] << " port device("
#ifdef WIN32
            "COM1:"
#else
            "/dev/ttyS0"
#endif
            " or similar)"
         << endl;
    return 126;
  }
  ServerLx200 server(port,argv[2]);
  while (continue_looping) {
    server.step(10000);
  }

#ifdef WIN32
  WSACleanup();
#endif
  cout << "bye." << endl;
}
