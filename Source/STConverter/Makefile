MODULES = STData STCore STUtil

make:
	@for i in $(MODULES); do (cd Source/$$i; make); done

clean:
	@for i in $(MODULES); do (cd Source/$$i; make clean); done
	@rm -f headers/*
	@rmdir headers

install:
	@if [ ! -d "headers" ]; then mkdir headers; fi
	@for i in $(MODULES); do (cp -f Source/$$i/*.hh headers); done
