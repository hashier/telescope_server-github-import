# for cross compilation with mingw export the path to your cross compiler:
# export CROSS_GPP=/usr/local/mingw/i386-mingw32/bin/g++

TelescopeServerDummy_CPP = Connection.cpp Listener.cpp Socket.cpp \
  Server.cpp ServerDummy.cpp LogFile.cpp MainDummy.cpp

TelescopeServerDummy_O = $(TelescopeServerDummy_CPP:.cpp=.o)
TelescopeServerDummy_OBJ = $(TelescopeServerDummy_CPP:.cpp=.obj)

TelescopeServerLx200_CPP = Connection.cpp Listener.cpp Socket.cpp \
  SerialPort.cpp Lx200Connection.cpp Lx200Command.cpp \
  Server.cpp ServerLx200.cpp LogFile.cpp MainLx200.cpp

TelescopeServerLx200_O = $(TelescopeServerLx200_CPP:.cpp=.o)
TelescopeServerLx200_OBJ = $(TelescopeServerLx200_CPP:.cpp=.obj)

TelescopeServerNexStar_CPP = Connection.cpp Listener.cpp Socket.cpp \
  SerialPort.cpp NexStarConnection.cpp NexStarCommand.cpp \
  Server.cpp ServerNexStar.cpp LogFile.cpp MainNexStar.cpp

TelescopeServerNexStar_O = $(TelescopeServerNexStar_CPP:.cpp=.o)
TelescopeServerNexStar_OBJ = $(TelescopeServerNexStar_CPP:.cpp=.obj)

CCFLAGS = -O4 -Wall -Wextra -Wno-unused-parameter -Werror -DDEBUG3 -DDEBUG4 -DDEBUG5
LDFLAGS = -O4 -s

.SUFFIXES: .cpp .o .obj

.cpp.o:
	g++ $(CCFLAGS) $< -c -o $@

.cpp.obj:
	$(CROSS_GPP) $(CCFLAGS) $< -c -o $@

all: TelescopeServerDummy TelescopeServerLx200 TelescopeServerNexStar

exe: TelescopeServerDummy.exe TelescopeServerLx200.exe TelescopeServerNexStar.exe

TelescopeServerDummy: $(TelescopeServerDummy_O)
	g++ $(LDFLAGS) $(TelescopeServerDummy_O) -o $@

TelescopeServerDummy.exe: $(TelescopeServerDummy_OBJ)
	$(CROSS_GPP) $(LDFLAGS) $(TelescopeServerDummy_OBJ) -o $@ \
	-lws2_32

TelescopeServerLx200: $(TelescopeServerLx200_O)
	g++ $(LDFLAGS) $(TelescopeServerLx200_O) -o $@

TelescopeServerLx200.exe: $(TelescopeServerLx200_OBJ)
	$(CROSS_GPP) $(LDFLAGS) $(TelescopeServerLx200_OBJ) -o $@ \
	-lws2_32

TelescopeServerNexStar: $(TelescopeServerNexStar_O)
	g++ $(LDFLAGS) $(TelescopeServerNexStar_O) -o $@

TelescopeServerNexStar.exe: $(TelescopeServerNexStar_OBJ)
	$(CROSS_GPP) $(LDFLAGS) $(TelescopeServerNexStar_OBJ) -o $@ \
	-lws2_32

depend:
	makedepend -Y -- $(CCFLAGS) -- -f- >make.dep 2>/dev/null \
	$(TelescopeServerDummy_CPP) $(TelescopeServerLx200_CPP)
	sed -e 's/\.o/\.obj/g' make.dep > make.cross_dep

clean:
	rm *.o *.obj

make.dep:
	touch make.dep

make.cross_dep:
	touch make.cross_dep

include make.dep
include make.cross_dep

