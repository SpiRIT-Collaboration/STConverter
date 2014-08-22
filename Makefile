MODULES = GETDecoder STConverter

make:
	@for i in $(MODULES); do (cd Source/$$i; make); done
	@ln -sf Source/GETDecoder/libGETDecoder.so .
	@ln -sf Source/STConverter/libSTData.so .
	@ln -sf Source/STConverter/libSTCore.so .
	@ln -sf Source/STConverter/libSTUtil.so .
	@ln -sf Source/STConverter/mapping/UnitAsAd.map .
	@ln -sf Source/STConverter/mapping/AGET.map .
	@g++ -fPIC --shared -o libSTConverter.so libSTData.so libSTCore.so libSTUtil.so libGETDecoder.so
	@cp -rf Source/GETDecoder/examples GETDecoder_examples

clean:
	cd Source/GETDecoder; make clean
	cd Source/STConverter; make clean
	@rm -f *.so
	@rm -f *.map
