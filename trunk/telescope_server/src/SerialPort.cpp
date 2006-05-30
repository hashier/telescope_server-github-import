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

#include "SerialPort.hpp"

#include <unistd.h>
#include <string.h> // memset

#include <iostream>
using namespace std;

SerialPort::SerialPort(Server &server,const char *serial_device)
           :Connection(server,-1) {
  fd = open(serial_device,O_RDWR|O_NOCTTY);
  struct termios termios_new;
  memset(&termios_new,0,sizeof(termios_new));
  termios_new.c_cflag = CS8    |  // 8 data bits
                        CREAD  |  // Enable receiver
                                  // no parity because PARENB is not given
                        CLOCAL |  // Ignore modem control lines
                        CREAD;    // Enable receiver
  cfsetospeed(&termios_new,B9600);

//  termios_new.c_iflag = IGNPAR;
//  termios_new.c_oflag = 0;

    // set input mode (non-canonical, no echo, ...
  termios_new.c_lflag = 0;

    // do not return until 1 character is received:
  termios_new.c_cc[VTIME] = 0;
  termios_new.c_cc[VMIN] = 1;
  if (fd >= 0) {
    if (fcntl(fd,F_SETFL,O_NONBLOCK) >= 0) {
      if (tcgetattr(fd,&termios_original) >= 0) {
        if (tcsetattr(fd,TCSAFLUSH,&termios_new) >= 0) {
          return;
        } else {
          cerr << "SerialPort::SerialPort(" << serial_device << "): "
                  "tcsetattr failed: " << strerror(errno) << endl;
        }
      } else {
        cerr << "SerialPort::SerialPort(" << serial_device << "): "
                "tcgetattr failed: " << strerror(errno) << endl;
      }
    } else {
      cerr << "SerialPort::SerialPort(" << serial_device << "): "
              "fcntl(O_NONBLOCK) failed: " << strerror(errno) << endl;
    }
    ::close(fd);
    fd = -1;
  } else {
    cerr << "SerialPort::SerialPort(" << serial_device << "): "
            "open() failed: " << strerror(errno) << endl;
  }
}

SerialPort::~SerialPort(void) {
  if (fd >= 0) {
    tcsetattr(fd,TCSANOW,&termios_original); // restore original settings
    ::close(fd);
    fd = -1;
  }
}


