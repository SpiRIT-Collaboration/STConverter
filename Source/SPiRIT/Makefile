MODULES = STData STCore STUtil

make:
	@for i in $(MODULES); do (cd Source/$$i; make); done

clean:
	@for i in $(MODULES); do (cd Source/$$i; make clean); done
