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

#include "Lx200Command.hpp"
#include "ServerLx200.hpp"

#include <math.h>

#include <iostream>
using namespace std;

Lx200Command::Lx200Command(Server &server)
             :server(*static_cast<ServerLx200*>(&server)),
              has_been_written_to_buffer(false) {
}

Lx200CommandGotoPosition::Lx200CommandGotoPosition(Server &server,
                                                   unsigned int ra_int,
                                                   int dec_int)
                         :Lx200Command(server) {
  dec = (int)floor(0.5 + dec_int * (360*3600/4294967296.0));
  if (dec < -90*3600) {
    dec = -180*3600 - dec;
    ra_int += 0x80000000;
  } else if (dec > 90*3600) {
    dec = 180*3600 - dec;
    ra_int += 0x80000000;
  }
  ra = (int)floor(0.5 + ra_int * (86400.0/4294967296.0));
  if (ra >= 86400) ra -= 86400;
}

bool Lx200CommandGotoPosition::writeCommandToBuffer(char *&p,char *end) {
  if (end-p < 34) return false;
    // stop slew:
  *p++ = '#';
  *p++ = ':';
  *p++ = 'Q';
  *p++ = '#';
    // set object ra:
  *p++ = ':';
  *p++ = 'S';
  *p++ = 'r';
  *p++ = ' ';
  int x = ra;
  p += 8;
  p[-1] = '0' + (x % 10); x /= 10;
  p[-2] = '0' + (x %  6); x /=  6;
  p[-3] = ':';
  p[-4] = '0' + (x % 10); x /= 10;
  p[-5] = '0' + (x %  6); x /=  6;
  p[-6] = ':';
  p[-7] = '0' + (x % 10); x /= 10;
  p[-8] = '0' + x;
  *p++ = '#';
    // set object dec:
  *p++ = ':';
  *p++ = 'S';
  *p++ = 'd';
  *p++ = ' ';
  x = dec;
  if (x < 0) {*p++ = '-';x = -x;}
  else {*p++ = '+';}
  p += 8;
  p[-1] = '0' + (x % 10); x /= 10;
  p[-2] = '0' + (x %  6); x /=  6;
  p[-3] = ':';
  p[-4] = '0' + (x % 10); x /= 10;
  p[-5] = '0' + (x %  6); x /=  6;
  p[-6] = 223; // degree symbol
  p[-7] = '0' + (x % 10); x /= 10;
  p[-8] = '0' + x;
  *p++ = '#';
    // slew to current object coordinates
  *p++ = ':';
  *p++ = 'M';
  *p++ = 'S';
  *p++ = '#';
  has_been_written_to_buffer = true;
  return true;
}

int Lx200CommandGotoPosition::readAnswerFromBuffer(const char *&buff,
                                                    const char *end) const {
  if (buff < end && *buff=='#') buff++; // ignore silly byte
  if (buff >= end) return 0;
  const char *p = buff;
  const int rc = (*p++) - '0';
  if (rc == 0) {
#ifdef DEBUG4
    cout << "Lx200CommandGotoPosition::readAnswerFromBuffer: slew ok" << endl;
#endif
  } else {
#ifdef DEBUG4
    cout << "Lx200CommandGotoPosition::readAnswerFromBuffer: slew failed ("
         << rc << ")" << endl;
#endif
  }
  buff = p;
  return 1;
}

void Lx200CommandGotoPosition::print(ostream &o) const {
  o << "Lx200CommandGotoPosition("
    << (ra/3600) << ':' << ((ra/60)%60) << ':' << (ra%60);
  int d = abs(dec);
  o << "/" << ((dec<0)?'-':'+')
    << (d/3600) << ':' << ((d/60)%60) << ':' << (d%60) << ')';
}


bool CommandToggleFormat::writeCommandToBuffer(char *&p,char *end) {
  if (end-p < 4) return false;
  *p++ = '#';
  *p++ = ':';
  *p++ = 'U';
  *p++ = '#';
  has_been_written_to_buffer = true;
  return true;
}

void CommandToggleFormat::print(ostream &o) const {
  o << "CommandToggleFormat";
}



bool Lx200CommandGetRa::writeCommandToBuffer(char *&p,char *end) {
  if (end-p < 5) return false;
    // get RA:
  *p++ = '#';
  *p++ = ':';
  *p++ = 'G';
  *p++ = 'R';
  *p++ = '#';
  has_been_written_to_buffer = true;
  return true;
}

int Lx200CommandGetRa::readAnswerFromBuffer(const char *&buff,
                                            const char *end) const {
  if (buff < end && *buff=='#') buff++; // ignore silly byte
  if (end-buff < 8) return 0;
  bool long_format = true;
  int ra;
  const char *p = buff;
  ra = ((*p++) - '0');
  ra *= 10;ra += ((*p++) - '0');
  if (*p++ != ':') {
#ifdef DEBUG4
    cerr << "Lx200CommandGetRa::readAnswerFromBuffer: "
            "error: ':' expected" << endl;
#endif
    return -1;
  }
  ra *=  6;ra += ((*p++) - '0');
  ra *= 10;ra += ((*p++) - '0');
  switch (*p++) {
    case ':':
      ra *=  6;ra += ((*p++) - '0');
      ra *= 10;ra += ((*p++) - '0');
      if (end-buff < 9) return 0;
      break;
    case '.':
      ra *= 10;ra += ((*p++) - '0');
      ra *= 6;
      long_format = false;
      break;
    default:
      cerr << "Lx200CommandGetRa::readAnswerFromBuffer: "
              "error: '.' or ':' expected" << endl;
      return -1;
  }
  if (*p++ != '#') {
    cerr << "Lx200CommandGetRa::readAnswerFromBuffer: "
            "error: '#' expected" << endl;
    return -1;
  }
#ifdef DEBUG4
  cout << "Lx200CommandGetRa::readAnswerFromBuffer: "
          "ra = " << (ra/3600) << ':' << ((ra/60)%60) << ':' << (ra%60)
       << endl;
#endif
  buff = p;
  server.longFormatUsedReceived(long_format);
  server.raReceived((unsigned int)floor(ra * (4294967296.0/86400.0)));
  return 1;
}

void Lx200CommandGetRa::print(ostream &o) const {
  o << "Lx200CommandGetRa";
}




bool Lx200CommandGetDec::writeCommandToBuffer(char *&p,char *end) {
  if (end-p < 5) return false;
    // get Dec:
  *p++ = '#';
  *p++ = ':';
  *p++ = 'G';
  *p++ = 'D';
  *p++ = '#';
  has_been_written_to_buffer = true;
  return true;
}

int Lx200CommandGetDec::readAnswerFromBuffer(const char *&buff,
                                             const char *end) const {
  if (buff < end && *buff=='#') buff++; // ignore silly byte
  if (end-buff < 7) return 0;
  bool long_format = true;
  int dec;
  const char *p = buff;
  bool sign_dec = false;
  switch (*p++) {
    case '+':
      break;
    case '-':
      sign_dec = true;
      break;
    default:
#ifdef DEBUG4
      cerr << "Lx200CommandGetDec::readAnswerFromBuffer: "
              "error: '+' or '-' expected" << endl;
#endif
      return -1;
  }
  dec = ((*p++) - '0');
  dec *= 10;dec += ((*p++) - '0');
  if (*p++ != ((char)223)) {
    cerr << "Lx200CommandGetDec::readAnswerFromBuffer: "
            "error: degree sign expected" << endl;
  }
  dec *=  6;dec += ((*p++) - '0');
  dec *= 10;dec += ((*p++) - '0');
  switch (*p++) {
    case '#':
      long_format = false;
      dec *= 60;
      break;
    case ':':
      if (end-buff < 10) return 0;
      dec *=  6;dec += ((*p++) - '0');
      dec *= 10;dec += ((*p++) - '0');
      if (*p++ != '#') {
        cerr << "Lx200CommandGetDec::readAnswerFromBuffer: "
                "error: '#' expected" << endl;
        return -1;
      }
      break;
    default:
      cerr << "Lx200CommandGetDec::readAnswerFromBuffer: "
              "error: '#' or ':' expected" << endl;
      return -1;
  }
#ifdef DEBUG4
  cout << "Lx200CommandGetDec::readAnswerFromBuffer: "
       << "dec = " << (sign_dec?'-':'+')
       << (dec/3600) << ':' << ((dec/60)%60) << ':' << (dec%60) << endl;
#endif
  if (sign_dec) dec = -dec;
  buff = p;
  server.longFormatUsedReceived(long_format);
  server.decReceived((int)floor(dec* (4294967296.0/(360*3600.0))));
  return 1;
}

void Lx200CommandGetDec::print(ostream &o) const {
  o << "Lx200CommandGetDec";
}


