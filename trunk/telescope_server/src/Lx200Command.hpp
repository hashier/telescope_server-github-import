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

#ifndef _LX200_COMMAND_HPP_
#define _LX200_COMMAND_HPP_

#include <iostream>
using namespace std;

class Server;
class ServerLx200;

class Lx200Command {
public:
  virtual ~Lx200Command(void) {}
  virtual bool writeCommandToBuffer(char *&buff,char *end) = 0;
  bool hasBeenWrittenToBuffer(void) const {return has_been_written_to_buffer;}
  virtual int readAnswerFromBuffer(const char *&buff,
                                   const char *end) const = 0;
  virtual bool needsNoAnswer(void) const {return false;}
  virtual void print(ostream &o) const = 0;
    // returns true when reading is finished
protected:
  Lx200Command(Server &server);
  ServerLx200 &server;
  bool has_been_written_to_buffer;
};

inline ostream &operator<<(ostream &o,const Lx200Command &c) {
  c.print(o);return o;
}

class CommandToggleFormat : public Lx200Command {
public:
  CommandToggleFormat(Server &server) : Lx200Command(server) {}
private:
  bool writeCommandToBuffer(char *&buff,char *end);
  int readAnswerFromBuffer(const char*&,const char*) const {return 1;}
  bool needsNoAnswer(void) const {return true;}
  void print(ostream &o) const;
};

class Lx200CommandGotoPosition : public Lx200Command {
public:
  Lx200CommandGotoPosition(Server &server,unsigned int ra_int,int dec_int);
  bool writeCommandToBuffer(char *&buff,char *end);
  int readAnswerFromBuffer(const char *&buff,const char *end) const;
  void print(ostream &o) const;
private:
  int ra,dec;
};

class Lx200CommandGetRa : public Lx200Command {
public:
  Lx200CommandGetRa(Server &server) : Lx200Command(server) {}
  bool writeCommandToBuffer(char *&buff,char *end);
  int readAnswerFromBuffer(const char *&buff,const char *end) const;
  void print(ostream &o) const;
};

class Lx200CommandGetDec : public Lx200Command {
public:
  Lx200CommandGetDec(Server &server) : Lx200Command(server) {}
  bool writeCommandToBuffer(char *&buff,char *end);
  int readAnswerFromBuffer(const char *&buff,const char *end) const;
  void print(ostream &o) const;
};

#endif
