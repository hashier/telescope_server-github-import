TelescopeServerDummy_SOURCES = Connection.cpp Listener.cpp Socket.cpp \
  Server.cpp ServerDummy.cpp MainDummy.cpp

TelescopeServerDummy_OBJECTS = $(TelescopeServerDummy_SOURCES:.cpp=.o)

TelescopeServerLx200_SOURCES = Connection.cpp Listener.cpp Socket.cpp \
  SerialPort.cpp Lx200Connection.cpp Lx200Command.cpp \
  Server.cpp ServerLx200.cpp MainLx200.cpp

TelescopeServerLx200_OBJECTS = $(TelescopeServerLx200_SOURCES:.cpp=.o)

CCFLAGS = -O2 -Wall -Werror
LDFLAGS = -s

.SUFFIX: .cpp .o

.cpp.o:
	g++ $(CCFLAGS) $< -c

all: TelescopeServerDummy TelescopeServerLx200

TelescopeServerDummy: $(TelescopeServerDummy_OBJECTS)
	g++ $(LDFLAGS) $(TelescopeServerDummy_OBJECTS) -o TelescopeServerDummy

TelescopeServerLx200: $(TelescopeServerLx200_OBJECTS)
	g++ $(LDFLAGS) $(TelescopeServerLx200_OBJECTS) -o TelescopeServerLx200

depend:
	makedepend -Y -- $(CCFLAGS) -- -f- >make.dep 2>/dev/null \
        $(TelescopeServerDummy_SOURCES) $(TelescopeServerLx200_SOURCES)

clean:
	rm *.o make.dep.bak

make.dep:
	touch make.dep

include make.dep

