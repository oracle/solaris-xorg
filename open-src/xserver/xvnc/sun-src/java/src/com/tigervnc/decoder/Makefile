
JDK_VERSION = 1.8.0
JC = /usr/jdk/instances/jdk$(JDK_VERSION)/bin/javac
JCFLAGS = -target 1.8 -classpath ../../../

SOURCES = \
	common/Repaintable.java \
	CoRREDecoder.java \
	CopyRectDecoder.java \
	HextileDecoder.java \
	TightDecoder.java \
	RawDecoder.java \
	ZlibDecoder.java \
	RREDecoder.java \
	ZRLEDecoder.java

CLASSES = $(SOURCES:.java=.class)

all: $(CLASSES)

$(CLASSES:%=% + ): $(SOURCES)
	$(JC) $(JCFLAGS) -O $(SOURCES)

clean::
	$(RM) *.class common/*.class
