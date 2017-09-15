all: src

.PHONY: clean src

src:
	cd src && $(MAKE)
clean:
	cd src && $(MAKE) clean
