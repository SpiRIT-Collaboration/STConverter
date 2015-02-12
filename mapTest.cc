void mapTest() {
  gSystem -> Load("libSTConverter.so");
  STMapTest *test = new STMapTest();
  test -> SetAGETMap("AGET.map");
  test -> SetUAMap("UnitAsAd.map");
  test -> ShowAGETMap();
  test -> ShowUAMap();
}
