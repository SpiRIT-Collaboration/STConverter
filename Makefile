MODULES = GETDecoder SPiRIT

make:
	@for i in $(MODULES); do (cd Source/$$i; make); done
	@ln -s Source/GETDecoder/libGETDecoder.so .
	@cp -rf Source/GETDecoder/examples GETDecoder_examples

clean:
	cd Source/GETDecoder; make clean
	@rm libGETDecoder.so
	cd Source/SPiRIT; make clean
