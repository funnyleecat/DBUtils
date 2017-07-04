ifeq	($(release),1)

FLG := -O3 -fPIC

else

FLG := -O3 -g -fPIC

endif

PRG := bin/libDBUtils.so

SRC := $(wildcard src/*.cpp) $(wildcard oracle/*.cpp)

OBJ := $(patsubst %.cpp,%.o,$(SRC))

CC := g++

INC := -I./inc         \
       -I./interface         \
       -I../../../lib/linux/inc/ \
       -I../../../lib/cross/boost/include/ \
       -I../../../lib/cross/tinyxml/inc/     \
       -I../../../lib/cross/ocilib-4.3.0/include/          \
       -I../../../lib_libSelf/cross/utils/inc/
	   
LIB := -L../../../lib/linux/lib/ -lcconv  \
       -L../../../lib_libSelf/cross/utils/bin/  -lutils  \
       -L../../../lib/cross/ocilib-4.3.0/lib/linux/so  -locilib
	   
.PHONY: clean

$(PRG): $(OBJ)
	$(CC) -shared -o $@ $^ $(LIB)

%.o: %.cpp
	$(CC) $(FLG) -o $@ -c $< $(INC)

clean:
	rm -f $(OBJ) $(PRG)
