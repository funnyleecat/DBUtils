ifeq	($(release),1)

FLG := -O3 -fPIC

else

FLG := -O3 -g -fPIC

endif

PRG := bin/DBUtilsTest

DIR_BIN := ./bin

SRC := $(wildcard *.cpp)

OBJ := $(patsubst %.cpp,%.o,$(SRC))

CC := g++

INC := -I../interface \
       -I../../../../lib/cross/boost/include/


LIB := -L../bin/ -lDBUtils \
	   -L../../../../lib_libSelf/cross/utils/bin -lutils \
	   -L../../../../lib/cross/ocilib-4.3.0/lib/linux/so/ -locilib \
	   -L../../../../lib/cross/instantclient_12_1/sdk/lib/linux -lclntsh  -lclntshcore -lipc1 -lmql1 -lnnz12 -locci -lociei -lons \
	   -L../../../../lib/cross/boost/lib/linux/a -lboost_filesystem -lboost_system -lboost_thread \
	   -L../../../../lib/cross/tinyxml/bin -ltinyxml \
	   -L../../../../lib/linux/lib/ -liconv -lcconv

	   
#LIB := -L./bin -lcrypto \
#       -L./bin -lssl \
#	   -L../../../../lib/linux/lib/ -liconv -lcconv  \
#       -L../../../../lib/cross/thrift-0.10.0/lib/cpp/.libs -lthrift
	  
		
	   
.PHONY: clean

$(PRG): $(OBJ)
	$(CC) -o $@ $^ $(LIB)

%.o: %.cpp
	$(CC) $(FLG) -o $@ -c $< $(INC)

clean:
	rm -f $(OBJ) $(PRG)

copy:
	cp -f ../bin/libDBUtils.so $(DIR_BIN)/
	cp -f ../../../../lib_libSelf/cross/utils/bin/libutils.so $(DIR_BIN)/   
	cp -f ../../../../lib/linux/lib/libiconv.so.2 $(DIR_BIN)/   
	cp -f ../../../../lib/linux/lib/libcconv.so.0 $(DIR_BIN)/   
	cp -f ../../../../lib/cross/tinyxml/bin/libtinyxml.so $(DIR_BIN)/
	cp -f ../../../../lib/cross/ocilib/bin/libocilib.so $(DIR_BIN)/
	cp -f ../../../../lib/cross/ocilib/bin/libocilib.so.4 $(DIR_BIN)/
	cp -f ../../../../lib/cross/instantclient_12_1/sdk/lib/linux/* $(DIR_BIN)/ 