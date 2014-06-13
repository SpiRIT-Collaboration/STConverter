MODULES = GETDecoder SPiRIT

make:
	@for i in $(MODULES); do (cd Source/$$i; make); done
	@ln -s Source/GETDecoder/libGETDecoder.so .
	@ln -s Source/SPiRIT/libSTData.so .
	@ln -s Source/SPiRIT/libSTCore.so .
	@ln -s Source/SPiRIT/libSTUtil.so .
	@ln -s Source/SPiRIT/mapping/UnitAsAd.map .
	@ln -s Source/SPiRIT/mapping/ChToPad.map .
	@g++ -fPIC --shared -o libSPiRIT.so libSTData.so libSTCore.so libSTUtil.so libGETDecoder.so
	@cp -rf Source/GETDecoder/examples GETDecoder_examples

clean:
	cd Source/GETDecoder; make clean
	cd Source/SPiRIT; make clean
	@rm -f *.so
	@rm -f *.map
