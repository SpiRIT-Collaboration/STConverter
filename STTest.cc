// =================================================
//  STTest Class
// 
//  Description:
//    Test AGET & UnitAsAd map with plot
// 
//  Genie Jhang ( geniejhang@majimak.com )
//  2013. 08. 30
// =================================================

#include "STTest.hh"
#include "STMap.hh"

#include "TStyle.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TLatex.h"
#include "TList.h"
#include "TLine.h"

ClassImp(STTest);

STTest::STTest()
{
  map = new STMap();

  agetCvs = NULL;
  agetHist = NULL;
  agetChList = NULL;
  agetLineList = NULL;

  uaCvs = NULL;
  uaHist = NULL;
  uaList = NULL;
  uaLineList = NULL;
}

STTest::~STTest()
{
  if (agetHist != NULL)
    delete agetHist;
}

void STTest::ShowAGETMap()
{
  if (agetHist == NULL) {
    gStyle -> SetOptStat(0);
    agetCvs = new TCanvas("agetCvs", "", 600, 500);
    agetHist = new TH2D("agetHist", "AGET Map Test (Top View)", 7, -0.5, 6.5, 9, -0.5, 8.5);
    agetHist -> GetXaxis() -> SetTitle("Local Layer Number");
    agetHist -> GetXaxis() -> CenterTitle();
    agetHist -> GetYaxis() -> SetTitle("Local Row Number");
    agetHist -> GetYaxis() -> CenterTitle();
    agetHist -> Draw("colz");

    agetChList = new TList();
    for (Int_t iCh = 0; iCh < 68; iCh++) {
      Int_t row, layer;
      map -> GetRowNLayer(0, 0, 0, iCh, row, layer);

      TLatex *textCh = NULL;
      if (iCh < 10)
        textCh = new TLatex(layer - 0.07, row - 0.16, Form("%d", iCh));
      else 
        textCh = new TLatex(layer - 0.19, row - 0.16, Form("%d", iCh));

      textCh -> Draw("same");
      agetChList -> Add(textCh);
      textCh = NULL;
    }

    agetLineList = new TList();
    for (Int_t iLine = 0; iLine < 6; iLine++) {
      TLine *line = new TLine(iLine + 0.5, -0.5, iLine + 0.5, 8.5);
      line -> Draw("same");
      agetLineList -> Add(line);
    }
    for (Int_t iLine = 0; iLine < 8; iLine++) {
      TLine *line = new TLine(-0.5, iLine + 0.5, 6.5, iLine + 0.5);
      line -> Draw("same");
      agetLineList -> Add(line);
    }
    
  } else {
    gStyle -> SetOptStat(0);
    agetCvs = new TCanvas("agetCvs", "", 600, 500);
    agetHist -> Draw("colz");
 
    TIter nextCh(agetChList);
    TLatex *text = NULL;
    while ((text = (TLatex *) nextCh()))
      text -> Draw("same");

    TIter nextLine(agetLineList);
    TLine *line = NULL;
    while ((line = (TLine *) nextLine()))
      line -> Draw("same");
  }
}

void STTest::ShowUAMap()
{
  if (uaHist == NULL) {
    gStyle -> SetOptStat(0);
    uaCvs = new TCanvas("uaCvs", "", 800, 530);
    uaHist = new TH2D("uaHist", "UnitAsAd Map Test (Top View)", 4, -0.5, 3.5, 12, -0.5, 11.5);
    uaHist -> GetXaxis() -> SetTitle("Beam Right");
    uaHist -> GetXaxis() -> CenterTitle();
    uaHist -> GetYaxis() -> SetTitle("Upstream");
    uaHist -> GetYaxis() -> CenterTitle();
    uaHist -> SetTickLength(0, "x");
    uaHist -> SetLabelColor(0, "x");
    uaHist -> SetTickLength(0, "y");
    uaHist -> SetLabelColor(0, "y");
    uaHist -> Draw("colz");

    uaList = new TList();
    for (Int_t iUA = 0; iUA < 48; iUA++) {
      Int_t coboIdx = map -> GetCoboIdx(iUA);
      Int_t asadIdx = map -> GetAsadIdx(iUA);

      Int_t row = iUA/4;
      Int_t layer = iUA%4;
      TLatex *textUA = NULL;
      if (coboIdx < 10)
        textUA = new TLatex(layer - 0.20, row - 0.23, Form("C%dA%d", coboIdx, asadIdx));
      else
        textUA = new TLatex(layer - 0.23, row - 0.23, Form("C%dA%d", coboIdx, asadIdx));

      textUA -> Draw("same");
      uaList -> Add(textUA);
      textUA = NULL;
    }

    uaLineList = new TList();
    for (Int_t iLine = 0; iLine < 3; iLine++) {
      TLine *line = new TLine(iLine + 0.5, -0.5, iLine + 0.5, 11.5);
      line -> Draw("same");
      uaLineList -> Add(line);
    }
    for (Int_t iLine = 0; iLine < 11; iLine++) {
      TLine *line = new TLine(-0.5, iLine + 0.5, 3.5, iLine + 0.5);
      line -> Draw("same");
      uaLineList -> Add(line);
    }
    
  } else {
    gStyle -> SetOptStat(0);
    uaCvs = new TCanvas("uaCvs", "", 800, 530);
    uaHist -> Draw("colz");
 
    TIter nextUA(uaList);
    TLatex *text = NULL;
    while ((text = (TLatex *) nextUA()))
      text -> Draw("same");

    TIter nextLine(uaLineList);
    TLine *line = NULL;
    while ((line = (TLine *) nextLine()))
      line -> Draw("same");
  }
}
