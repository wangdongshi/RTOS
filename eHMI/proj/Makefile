# ---- paramter ----
SRCS 		:= $(wildcard src/*.cpp)
OBJS		:= $(patsubst %.cpp,%.o,$(SRCS))
OBJS		:= $(patsubst src/%,obj/%,$(OBJS))
TARGET		:= ehmi

GPP			= g++
GCCFLAGS 	= -g -Wall -pthread -lX11 -DDEBUG -I inc/
#GCCFLAGS 	= -g -Wall -pthread -lX11 -I inc/
GPPFLAGS	= -std=c++11 $(GCCFLAGS)

#---- rule ----
all: $(TARGET) $(OBJS)
	@echo finished of making Extutable file.

clean:
	rm -f $(TARGET) $(OBJS)

$(TARGET) : $(OBJS)
	$(GPP) -o $@ $(OBJS) $(GPPFLAGS) $(LIBS)

obj/%.o : src/%.cpp
	$(GPP) -c $(GPPFLAGS) -o $@ $<

