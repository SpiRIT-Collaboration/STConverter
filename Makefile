MODULES = GETDecoder STConverter

make:
	@for i in $(MODULES); do (cd Source/$$i; make); done
	@ln -sf Source/GETDecoder/libGETDecoder.so .
	@ln -sf Source/STConverter/libSTCore.so .
	@ln -sf Source/STConverter/mapping/UnitAsAd.map .
	@ln -sf Source/STConverter/mapping/AGET.map .
	@g++ -fPIC --shared -o libSTConverter.so libSTCore.so libGETDecoder.so
	@cp -rf Source/GETDecoder/examples GETDecoder_examples
	@if [ ! -d "headers" ]; then mkdir headers; fi
	@cp -f Source/STConverter/include/*.hh headers
	@if [ -e "Source/STConverter/STCoreDict_rdict.pcm" ]; then ln -sf Source/STConverter/STCoreDict_rdict.pcm .; fi
	@cp -f Source/GETDecoder/include/*.hh headers
	@if [ -e "Source/GETDecoder/libGETDecoderDict_rdict.pcm" ]; then ln -sf Source/GETDecoder/libGETDecoderDict_rdict.pcm .; fi

clean:
	cd Source/GETDecoder; make clean
	cd Source/STConverter; make clean
	@rm -f makeSummary
	@rm -f *.pcm
	@rm -f *.so
	@rm -f *.map.*
	@if [ -d "headers" ]; then (rm -f headers/*; rmdir headers;); fi

summary:
	@g++ -o makeSummary makeSummary.cc `root-config --cflags --glibs` -I./headers -L./ -lGETDecoder
