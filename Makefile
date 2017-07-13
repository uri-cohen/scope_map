
.PHONY: all test clean

all:
	$(MAKE) -C src
	$(MAKE) -C test

test:
	$(MAKE) -C test

clean:
	rm -f *~ *# *.o
	$(MAKE) -C src clean
	$(MAKE) -C test clean
