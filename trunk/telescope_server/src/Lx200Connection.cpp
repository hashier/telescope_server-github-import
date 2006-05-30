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

#include "Lx200Connection.hpp"
#include "Lx200Command.hpp"
#include "ServerLx200.hpp"

#include <iostream>
using namespace std;

Lx200Connection::Lx200Connection(Server &server,const char *serial_device)
                :SerialPort(server,serial_device) {
}

void Lx200Connection::resetCommunication(void) {
  while (!command_list.empty()) {
    delete command_list.front();
    command_list.pop_front();
  }
  read_buff_end = read_buff;
  write_buff_end = write_buff;
#ifdef DEBUG4
  cout << "Lx200Connection::resetCommunication" << endl;
#endif
}

void Lx200Connection::sendGoto(unsigned int ra_int,int dec_int) {
  sendCommand(new Lx200CommandGotoPosition(server,ra_int,dec_int));
}

void Lx200Connection::dataReceived(const char *&p,const char *read_buff_end) {
  if (fd < 0) {
    cerr << "Lx200Connection::dataReceived: "
            "strange: fd is closed" << endl;
  } else if (command_list.empty()) {
#ifdef DEBUG4
    cerr << "Lx200Connection::dataReceived: "
            "error: command_list is empty" << endl;
#endif
    resetCommunication();
    static_cast<ServerLx200*>(&server)->communicationResetReceived();
  } else if (command_list.front()->needsNoAnswer()) {
    cerr << "Lx200Connection::dataReceived: "
            "strange: command(" << *command_list.front()
         << ") needs no answer" << endl;
  } else {
    for (;;) {
      const int rc=command_list.front()->readAnswerFromBuffer(p,read_buff_end);
//      cout << "Lx200Connection::dataReceived: "
//           << *command_list.front() << "->readAnswerFromBuffer returned "
//           << rc << endl;
      if (rc <= 0) {
        if (rc < 0) {
          resetCommunication();
          static_cast<ServerLx200*>(&server)->communicationResetReceived();
        }
        break;
      }
      delete command_list.front();
      command_list.pop_front();
      if (command_list.empty()) break;
      if (!command_list.front()->writeCommandToBuffer(
                                   write_buff_end,
                                   write_buff+sizeof(write_buff))) break;
    }
  }
}

void Lx200Connection::sendCommand(Lx200Command *command) {
  if (command) {
#ifdef DEBUG4
    cout << "Lx200Connection::sendCommand(" << *command << ")" << endl;
#endif
    command_list.push_back(command);
    while (!command_list.front()->hasBeenWrittenToBuffer()) {
      if (command_list.front()->writeCommandToBuffer(
                                  write_buff_end,
                                  write_buff+sizeof(write_buff))) {
//        cout << "Lx200Connection::sendCommand: " << (*command_list.front())
//             << "::writeCommandToBuffer ok" << endl;
        if (command_list.front()->needsNoAnswer()) {
          delete command_list.front();
          command_list.pop_front();
          if (command_list.empty()) break;
        } else {
          break;
        }
      } else {
//        cout << "Lx200Connection::sendCommand: " << (*command_list.front())
//             << "::writeCommandToBuffer failed" << endl;
        break;
      }
    }
//    cout << "Lx200Connection::sendCommand(" << *command << ") end" << endl;
  }
}

