MODULES = GETDecoder STConverter

make:
	@for i in $(MODULES); do (cd Source/$$i; make); done
	@ln -s Source/GETDecoder/libGETDecoder.so .
	@ln -s Source/STConverter/libSTData.so .
	@ln -s Source/STConverter/libSTCore.so .
	@ln -s Source/STConverter/libSTUtil.so .
	@ln -s Source/STConverter/mapping/AGET.map .
	@ln -s Source/STConverter/mapping/ChToPad.map .
	@g++ -fPIC --shared -o libSTConverter.so libSTData.so libSTCore.so libSTUtil.so libGETDecoder.so
	@cp -rf Source/GETDecoder/examples GETDecoder_examples

clean:
	cd Source/GETDecoder; make clean
	cd Source/STConverter; make clean
	@rm -f *.so
	@rm -f *.map
