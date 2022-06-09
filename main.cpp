/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.cpp
 * Author: avdmit
 *
 * Created on December 23, 2019, 03:56 PM
 */

#include "TApplication.h"
#include "TCanvas.h"
#include "TROOT.h"

#include <TBenchmark.h>
#include <TBranch.h>
#include <TChain.h>
#include <TClonesArray.h>
#include <TCanvas.h>
#include <TDatime.h>
#include <TF1.h>
#include <TFile.h>
#include <TGraph.h>
#include <TH1.h>
#include <TH2.h>
#include <THStack.h>
#include <TLeaf.h>
#include <TList.h>
#include <TMath.h>
#include <TMultiGraph.h>
#include <TObject.h>
#include <TPad.h>
#include <TProfile.h>
#include "TRandom2.h"
#include <TSpline.h>
#include <TTree.h>

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

#include "BmnTOF1Conteiner.h"
#include "Track.h"

using namespace std;

Double_t gaus(Double_t x, Double_t mean, Double_t sigma){
  Double_t toReturn = x * TMath::Exp(-0.5 * TMath::Power((x - mean) / sigma, 2));
  return toReturn;
}

void Analysis(Int_t run_number, Short_t nBoards = 1)
{

  TString name;

  printf("Start analysis\n");
  TString FileName = "/home/facecerep/Downloads/Merge_RPC_DRS_allTracks (1)"; // NameInFile
  FileName += ".root";                                                        // NameInFile

  cout << "\nOpen file \n " << FileName << endl
       << endl;

  TClonesArray *TOF_digi = nullptr;
  TClonesArray *Tracks_digi = nullptr;
  Int_t EventID, RunID;
  TDatime *EventTime = nullptr;

  Int_t ResultSetBranch = -1;
  Int_t ResultAdd;
  Long64_t nEvForRead;

  TChain *eveTree = new TChain("TOF400");
  ResultAdd = eveTree->Add(FileName.Data());
  cout << "total number of files connected = " << ResultAdd << endl;
  if (ResultAdd != 1)
    cout << Form("Error opening files %s*\n", FileName.Data()) << endl;

  nEvForRead = eveTree->GetEntries();
  cout << "In the file " << nEvForRead << " events \n"
       << endl;

  ResultSetBranch = eveTree->SetBranchAddress("TOF400Conteiner", &TOF_digi);
  printf("TOF400Conteiner Set Branch = %i\n", ResultSetBranch);
  if (TOF_digi == nullptr)
    cout << "error" << endl;
  ResultSetBranch = -1;
  ResultSetBranch = eveTree->SetBranchAddress("Tracks", &Tracks_digi);
  printf("Tracks Set Branch = %i\n", ResultSetBranch);
  ResultSetBranch = -1;
  ResultSetBranch = eveTree->SetBranchAddress("EventID", &EventID);
  printf("EventID Set Branch = %i\n", ResultSetBranch);
  ResultSetBranch = -1;
  ResultSetBranch = eveTree->SetBranchAddress("RunID", &RunID);
  printf("RunID Set Branch = %i\n", ResultSetBranch);
  ResultSetBranch = -1;
  ResultSetBranch = eveTree->SetBranchAddress("EventTime", &EventTime);
  printf("EventTime Set Branch = %i\n", ResultSetBranch);
  ResultSetBranch = -1;

  Int_t ReadBytes = -1;
  Int_t entriesReadTOF = 0;
  Int_t entriesReadTrack = 0;


  Double_t xTOFTemp[100], yTOFTemp[100];
  Double_t xTrackTemp[100], yTrackTemp[100];

  Double_t xSub[6624], ySub[6624];
  Double_t xTOF[6624], yTOF[6624];
  Double_t xTrack[6624], yTrack[6624];

  Int_t xMinimum = 0.99;
  Int_t xMaximum = 0.99;
  Int_t yMinimum = 0.99;
  Int_t yMaximum = 0.99;

  Int_t pointsTotal = 0;

  for (Int_t iEv = 0; iEv < nEvForRead; iEv++)
  {
    ReadBytes = eveTree->GetEntry(iEv);

    entriesReadTOF = 0;
    entriesReadTrack = 0;

    for (Int_t iDig = 0; iDig < TOF_digi->GetEntriesFast(); iDig++)
    {
      BmnTOF1Conteiner *digi = (BmnTOF1Conteiner *)TOF_digi->At(iDig);
      xTOFTemp[iDig] = digi->GetX();
      yTOFTemp[iDig] = digi->GetY();
      entriesReadTOF++;
    }

    for (Int_t iDig = 0; iDig < Tracks_digi->GetEntriesFast(); iDig++)
    {
      Track *digi = (Track *)Tracks_digi->At(iDig);
      xTrackTemp[iDig] = digi->GetfStopPointX();
      yTrackTemp[iDig] = digi->GetfStopPointY();
      entriesReadTrack++;
    }

    if (entriesReadTOF != 0 && entriesReadTrack != 0)
    {
      Int_t minimumEntriesRead = 0;

      if (entriesReadTOF < entriesReadTrack)
        minimumEntriesRead = entriesReadTOF;
      else
        minimumEntriesRead = entriesReadTrack;

      for (Int_t i = 0; i < minimumEntriesRead; i++)
      {
        //if(xTrack[i] - xTOF[i] < 100 && xTrack[i] - xTOF[i] > -350){
          Double_t x = xTrackTemp[i] - xTOFTemp[i];
          Double_t y = yTrackTemp[i] - yTOFTemp[i];
          xSub[pointsTotal] = x;
          ySub[pointsTotal] = y;

          xTOF[pointsTotal] = xTOFTemp[i];
          yTOF[pointsTotal] = yTOFTemp[i];
          xTrack[pointsTotal] = xTrackTemp[i];
          yTrack[pointsTotal] = yTrackTemp[i];

          if(x < xMinimum)
            xMinimum = x;
          if(x > xMaximum)
            xMaximum = x;
          if(y < yMinimum)
            yMinimum = y;
          if(y > yMaximum)
            yMaximum = y;

          pointsTotal++;
        //}

        //if(!xSub[pointsTotal] > 100)
      }
    }

    // cout << "Enrties read track: " << entriesReadTrack << endl;
    // cout << "Entries read TOF: " << entriesReadTOF << endl;
    // cout << "Entries read: " << minimumEntriesRead << endl;
  }
  cout << "xMinimum: " << xMinimum << endl;
  cout << "xMaximum: " << xMaximum << endl;
  cout << "yMinimum: " << yMinimum << endl;
  cout << "yMaximum: " << yMaximum << endl;
  cout << "Points Total: " << pointsTotal << endl;

  auto* hSubX = new TH1D("hx", "Sub X Histo", 100, xMinimum, xMaximum);
  //hSubX -> SetFillStyle(4050);
  hSubX -> SetFillColor(3);
  auto* hSubXSimulated = new TH1D("hxsim", "Sub X Histo Simulated", 100, xMinimum, xMaximum);
  //hSubXSimulated -> SetFillStyle(4050);
  hSubXSimulated -> SetFillColor(2);

  auto* hSubY = new TH1D("hy", "Sub Y Histo", 100, yMinimum, yMaximum);
  //hSubY -> SetFillStyle(4050);
  hSubY -> SetFillColor(3);
  auto* hSubYSimulated = new TH1D("hysim", "Sub Y Histo Simulated", 100, yMinimum, yMaximum);
  //hSubYSimulated -> SetFillStyle(4050);
  hSubYSimulated -> SetFillColor(2);

  for (Int_t i = 0; i < pointsTotal; i++)
  {
    if(xSub[i] < xMinimum)
     cout << "Overriden xMinimum" << endl;

    hSubX->Fill(xSub[i]);
    hSubY->Fill(ySub[i]);
  }

  TF1 *fx = new TF1("fx", "gaus", xMinimum, xMaximum);
  TF1 *fy = new TF1("fy", "gaus", yMinimum, yMaximum);

  TCanvas *c = new TCanvas("c", "c", 1000, 1000);
  c -> Draw();
  c -> cd();

  TPad *xPad = new TPad("xPad", "xPad", 0, 0, 0.5, 1);
  TPad *yPad = new TPad("yPad", "yPad", 0.5, 0, 1, 1);
  
  xPad -> Draw();
  yPad -> Draw();

  xPad -> cd();
  hSubX -> Fit(fx);

  yPad -> cd();
  hSubY -> Fit(fy);

  c -> Modified();
  c -> Update();

  cout << "fy mean: " << fy -> GetParameter(1) << endl;
  cout << "actual y: " << ySub[1] << endl;
  cout << "result: " << gaus(ySub[1], fy -> GetParameter(1), fy -> GetParameter(2)) << endl;

  // for(Int_t i = 0; i < pointsTotal; i++){
  //   hSubXSimulated -> Fill(xSub[i], gaus(xSub[i], fx -> GetParameter(1), fx -> GetParameter(2)));
  //   hSubYSimulated -> Fill(ySub[i], gaus(ySub[i], fy -> GetParameter(1), fy -> GetParameter(2)));
  // }

  hSubXSimulated -> FillRandom("fx");
  hSubYSimulated -> FillRandom("fy");

  xPad -> cd();
  hSubX -> Draw();
  hSubXSimulated -> Draw("SAME");

  yPad -> cd();
  //hSubX->GetXaxis()->SetRangeUser(fy -> GetParameter(1) - fy -> GetParameter(2), fy -> GetParameter(1) + fy -> GetParameter(2));
  //hSubXSimulated->GetXaxis()->SetRangeUser(fy -> GetParameter(1) - fy -> GetParameter(2), fy -> GetParameter(1) + fy -> GetParameter(2));
  hSubY -> Draw();
  hSubYSimulated -> Draw("SAME");

  c -> Modified();
  c -> Update();

  // TString FileToWriteName = "/home/facecerep/diplom/histo.root";
  // TFile f(FileToWriteName, "RECREATE");
  // cout << "file created at: " << FileToWriteName << endl;
  // auto T = new TTree("T", "test");
  // T->Branch("graph", &hSub, 10000, 0);

  getchar();
}

 
int main(int argc, char **argv)
{

  Int_t run_number;

  if (argc < 2)
  {
    std::cout << "Usage: " << argv[0] << " <run number> " << std::endl;
    return 1;
  }
  else if (argc >= 2)
    run_number = atoi(argv[1]);
  std::cout << "Doing analysis for run " << run_number << "\n";
  TApplication theApp("App", &argc, argv);
  Analysis(run_number);
  std::cout << "Analysis complete!" << std::endl;
  return 0;
}