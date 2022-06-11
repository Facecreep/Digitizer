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

Double_t FindMinimum(Int_t n, Double_t *array, Int_t start = 0, Int_t finish = 0){
  Double_t toReturn = 0;
  
  if(finish == 0)
    finish = n;

  for(Int_t i = start; i < finish; i++){
    cout << "read " << array[i] << endl;
    if(array[i] < toReturn)
      toReturn = array[i];
  }

  cout << "Minimum: " << toReturn << endl;

  return toReturn;
}

Double_t FindMaximum(Int_t n, Double_t *array, Int_t start = 0, Int_t finish = 0){
  Double_t toReturn = 0;
  
  if(finish == 0)
    finish = n;

  for(Int_t i = start; i < finish; i++){
    if(array[i] > toReturn)
      toReturn = array[i];
  }

  cout << "Maximum: " << toReturn << endl;

  return toReturn;
}

void Analysis(Int_t run_number, Short_t nBoards = 1){
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
          Double_t x = xTOFTemp[i] - xTrackTemp[i];
          Double_t y = yTOFTemp[i] - yTrackTemp[i];
          xSub[pointsTotal] = x;
          ySub[pointsTotal] = y;

          xTOF[pointsTotal] = xTOFTemp[i];
          yTOF[pointsTotal] = yTOFTemp[i];
          xTrack[pointsTotal] = xTrackTemp[i];
          yTrack[pointsTotal] = yTrackTemp[i];

          pointsTotal++;
        //}

        //if(!xSub[pointsTotal] > 100)
      }
    }

    // cout << "Enrties read track: " << entriesReadTrack << endl;
    // cout << "Entries read TOF: " << entriesReadTOF << endl;
    // cout << "Entries read: " << minimumEntriesRead << endl;
  }
  Double_t xMinimum = FindMinimum(pointsTotal, xSub);
  Double_t xMaximum = FindMaximum(pointsTotal, xSub);
  Double_t yMinimum = FindMinimum(pointsTotal, ySub);
  Double_t yMaximum = FindMaximum(pointsTotal, ySub);
  cout << "xMinimum: " << xMinimum << endl;
  //cout << "Points Total: " << pointsTotal << endl;

  auto* hSubX = new TH1D("hsubx", "Sub X Histo", 100, xMinimum, xMaximum);
  hSubX -> SetFillColor(30);
  //hSubX -> SetFillStyle(4050);
  auto* hSubXSimulated = new TH1D("hsubxsim", "Sub X Histo Simulated", 100, xMinimum, xMaximum);
  hSubXSimulated -> SetFillColor(46);
  hSubXSimulated -> SetFillStyle(3001);

  auto* hSubY = new TH1D("hsuby", "Sub Y Histo", 100, yMinimum, yMaximum);
  hSubY -> SetFillColor(30);
  //hSubY -> SetFillStyle(4050);
  auto* hSubYSimulated = new TH1D("hsubysim", "Sub Y Histo Simulated", 100, yMinimum, yMaximum);
  hSubYSimulated -> SetFillColor(46);
  hSubYSimulated -> SetFillStyle(3001);

  // auto* hTrackX = new TH1D("htrackx", "Track X Histo", 100, FindMinimum(pointsTotal, xTrack, pointsTotal / 2, pointsTotal), FindMaximum(pointsTotal, xTrack, pointsTotal / 2, pointsTotal));
  // auto* hTrackY = new TH1D("htracky", "Track Y Histo", 100, FindMinimum(pointsTotal, yTrack, pointsTotal / 2, pointsTotal), FindMaximum(pointsTotal, yTrack, pointsTotal / 2, pointsTotal));

  auto* hTrackX = new TH1D("hTOFsimulatedx", "TOF X Histo Simulated", 100, xMinimum, xMaximum);
  auto* hTrackY = new TH1D("hTOFsimulatedy", "TOF Y Histo Simulated", 100, yMinimum, yMaximum);

  auto* hTOFX = new TH1D("hTOFx", "TOF X Histo", 100, FindMinimum(pointsTotal, xTOF, pointsTotal / 2, pointsTotal), FindMaximum(pointsTotal, xTOF, pointsTotal / 2, pointsTotal));
  auto* hTOFY = new TH1D("hTOFy", "TOF Y Histo", 100, FindMinimum(pointsTotal, yTOF, pointsTotal / 2, pointsTotal), FindMaximum(pointsTotal, yTOF, pointsTotal / 2, pointsTotal));

  for (Int_t i = 0; i < pointsTotal / 2; i++)
  {
    //if(xSub[i] < xMinimum)
     //cout << "Overriden xMinimum" << endl;

    hSubX->Fill(xSub[i]);
    hSubY->Fill(ySub[i]);
  }

  for (Int_t i = pointsTotal / 2; i < pointsTotal; i++)
  {
    hTOFX -> Fill(xTOF[i]);
    hTOFY -> Fill(yTOF[i]);
    hTrackX -> Fill(xTrack[i]);
    hTrackY -> Fill(yTrack[i]);
  }

  TF1 *fx = new TF1("fx", "gaus", xMinimum, xMaximum);
  TF1 *fy = new TF1("fy", "gaus", yMinimum, yMaximum);

  TCanvas *c = new TCanvas("c", "c", 1000, 1000);
  // c -> Draw();
  // c -> cd();

  TPad *xPad = new TPad("xPad", "xPad", 0, 0, 0.5, 1);
  TPad *yPad = new TPad("yPad", "yPad", 0.5, 0, 1, 1);
  
  xPad -> Draw();
  yPad -> Draw();

  xPad -> cd();
  hSubX -> Fit(fx);

  yPad -> cd();
  hSubY -> Fit(fy);

  hSubXSimulated -> FillRandom("fx");
  hSubYSimulated -> FillRandom("fy");

  hTrackX -> Add(hSubXSimulated);
  hTrackY -> Add(hSubYSimulated);

  hTrackX -> SetFillStyle(3001);
  hTrackX -> SetFillColor(46);
  hTrackY -> SetFillStyle(3001);
  hTrackY -> SetFillColor(46);
  hTOFX -> SetFillColor(30);
  hTOFY -> SetFillColor(30);

  xPad -> cd();
  //hSubX -> Draw();
  //hSubXSimulated -> Draw("SAME");
  hTOFX -> Draw();
  hTrackX -> Draw("SAME");

  yPad -> cd();
  //hSubY -> Draw();
  //hSubYSimulated -> Draw("SAME");
  hTOFY -> Draw();
  hTrackY -> Draw("SAME");

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