# Makefile for socket examples

CXX=			g++ $(CCFLAGS)
MSGD=			msgd.o inet-server.o server.o Worker.o QueueThreaded.o
MSG=			msg.o inet-client.o client.o
OBJS =			$(ECHO-SERVER-SIMPLE) $(ECHO-CLIENT-SIMPLE) $(ECHO-SERVER-SLOW) $(MSGD) $(MSG) $(UNIX-ECHO-SERVER) $(UNIX-ECHO-CLIENT) 

LIBS=			-pthread

CCFLAGS= -g -std=c++0x

all:	msgd msg


msgd:$(MSGD)
	$(CXX) -o msgd $(MSGD) $(LIBS)

msg:$(MSG)
	$(CXX) -o msg $(MSG) $(LIBS)

clean:
	rm -f $(OBJS) $(OBJS:.o=.d)

realclean:
	rm -f $(OBJS) $(OBJS:.o=.d) msgd msg


# These lines ensure that dependencies are handled automatically.
%.d:	%.cc
	$(SHELL) -ec '$(CC) -M $(CPPFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'

include	$(OBJS:.o=.d)
