
WOSC_VERSION=00.11
WOSC_TARNAME=WOscLib
DISTDIR=$(WOSC_TARNAME)-$(WOSC_VERSION)

CFLAGS = -Wall -Os 
LDFLAGS = -Wall -Os

OBJS = \
    WOscString.o               \
    WOscInfo.o                 \
    WOscPacket.o               \
    WOscTimeTag.o              \
    WOscLib.o                  \
    WOscPatternMatch.o         \
    WOscBlob.o                 \
    WOscUtil.o                 \
    WOscPriorityQueue.o        \
    WOscBundle.o               \
    WOscMessage.o              \
    WOscReceiver.o             \
    WOscNetReturn.o            \
    WOscContainer.o            \
    WOscMethod.o               \
    WOscReceiverMethod.o       \
    WOscException.o            \
    WOscTcpHandler.o           \
    WOscTcpClient.o            \
    WOscTcpServer.o

EXAMPPATH=./examples
EXAMPOBJ = \
    $(EXAMPPATH)/WOscServer.o    \
	$(EXAMPPATH)/WOscClient.o    \
	$(EXAMPPATH)/WOscStreamingClient.o \
	$(EXAMPPATH)/WOscStreamingServer.o

TESTPATH=./tests
TESTOBJ = \
    $(TESTPATH)/patternmatch.o    \

TARGET = libwosclib.a

EXP_SERVER = $(EXAMPPATH)/woscserver
EXP_CLIENT = $(EXAMPPATH)/woscclient
EXP_TCPCLIENT = $(EXAMPPATH)/woscstreamingclient
EXP_TCPSERVER = $(EXAMPPATH)/woscstreamingserver
EXAMPLES=$(EXP_SERVER) $(EXP_CLIENT) $(EXP_TCPCLIENT) $(EXP_TCPSERVER)

TEST_PATTERNMATCH=$(TESTPATH)/patternmatch
TESTS=$(TEST_PATTERNMATCH)

all: $(OBJS) $(TARGET) $(EXAMPOBJ) $(EXAMPLES) $(TESTOBJ) $(TESTS)

new: clean all

clean:
	$(RM) -f $(OBJS) $(TARGET) $(EXAMPOBJ) $(EXAMPLES) $(TESTOBJ) $(TESTS)
	$(RM) -r doc/html doc/latex $(OBJS:.o=.d) $(EXAMPOBJ:.o=.d) $(TESTOBJ:.o=.d)

$(EXP_SERVER):$(TARGET) $(EXAMPPATH)/WOscServer.o
	$(CXX) -o $@ $(EXAMPPATH)/WOscServer.o -L./ -lwosclib -lpthread -framework CoreServices

$(EXP_CLIENT):$(TARGET) $(EXAMPPATH)/WOscClient.o
	$(CXX) -o $@ $(EXAMPPATH)/WOscClient.o -L./ -lwosclib -lpthread -framework CoreServices

$(EXP_TCPCLIENT):$(TARGET) $(EXAMPPATH)/WOscStreamingClient.o
	$(CXX) -o $@ $(EXAMPPATH)/WOscStreamingClient.o -L./ -lwosclib -lpthread -framework CoreServices

$(EXP_TCPSERVER):$(TARGET) $(EXAMPPATH)/WOscStreamingServer.o
	$(CXX) -o $@ $(EXAMPPATH)/WOscStreamingServer.o -L./ -lwosclib -lpthread -framework CoreServices

$(TEST_PATTERNMATCH):$(TARGET) $(TESTPATH)/patternmatch.o
	$(CXX) -o $@ $(TESTPATH)/patternmatch.o -L./ -lwosclib -lpthread -framework CoreServices
	
libwosclib.a: ${OBJS}
	$(AR) -r $@ $^

libwosclib.dll: ${OBJS}
	$(CXX)  -o $@ $^

libwosclib.so: ${OBJS}
	$(CXX)  -o $@ $^

libwosclib.dylib: ${OBJS}
	$(CXX)  -o $@ $^

$(OBJS): %.o: %.cpp
	$(CXX) -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" $(CFLAGS) -o$@ $<

$(EXAMPOBJ): %.o: %.cpp
	$(CXX) -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" $(CFLAGS) -o$@ -I. -I$(EXAMPPATH) $<

$(TESTOBJ): %.o: %.cpp
	$(CXX) -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" $(CFLAGS) -o$@ -I. -I$(TESTPATH) $<

dist: clean
	rm -rf $(DISTDIR)
	mkdir $(DISTDIR)
	mkdir $(DISTDIR)/cfg
	mkdir $(DISTDIR)/doc
	mkdir $(DISTDIR)/examples
	mkdir $(DISTDIR)/tests
	cp configure $(DISTDIR)/
	cp configure.ac $(DISTDIR)/
	cp *.cpp $(DISTDIR)/
	cp *.h $(DISTDIR)/
	cp *.in $(DISTDIR)/
	cp *.txt $(DISTDIR)/
	cp cfg/* $(DISTDIR)/cfg/
	cp doc/* $(DISTDIR)/doc/
	cp examples/* $(DISTDIR)/examples/
	cp tests/* $(DISTDIR)/tests/
	
dist-zip: dist
	zip -r $(DISTDIR).zip $(DISTDIR)

dist-clean: clean
	rm -rf $(DISTDIR)
	rm -f Makefile config.h config.log config.status $(DISTDIR).zip

doc:
	cd doc && doxygen wosclib.doxyfile

.PHONY : clean all dist dist-zip dist-clean doc new

-include $(OBJS:.o=.d)
-include $(EXAMPOBJ:.o=.d)
-include $(TESTOBJ:.o=.d)
