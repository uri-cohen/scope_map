
.PHONY: all clean

include ../Makefile.base


TESTS = gtest_basic

all: $(foreach t, $(TESTS), $t)
	for t in $(TESTS); do ./$$t; done
	echo Done

clean:
	rm -rf *~ *# *.o $(foreach t, $(TESTS), $t)

%: %.cpp
	$(CPP) -o $@ $(TEST_INCLUDE) $(CPPFLAGS) $< $(TEST_LIBS)
