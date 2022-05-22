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
#include <TLeaf.h>
#include <TList.h>
#include <TMath.h>
#include <TMultiGraph.h>
#include <TObject.h>
#include <TPad.h>
#include <TProfile.h>
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

void Analysis(Int_t run_number, Short_t nBoards = 1) {

  TString name;

  printf("Start analysis\n");
  TString FileName = "/home/facecerep/Downloads/Merge_RPC_DRS_allTracks (1)"; // NameInFile
  FileName += ".root";                                     // NameInFile

  cout << "\nOpen file \n " << FileName << endl << endl;

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
  cout << "In the file " << nEvForRead << " events \n" << endl;

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

  // TCanvas *c = nullptr;
  // Short_t nPads = 2;
  // std::vector<TPad *> Pads(nPads);
  // if (fDRAW) {
  //   cout << "nPads = " << nPads << endl;
  //   c = new TCanvas("c", "The Canvas", 0, 0, 1440, 1000);
  //   // Pads = ConfigurePads(Pads, "vertical");
  // }

  Int_t ReadBytes = -1;
  Int_t entriesReadTOF = 0;
  Int_t entriesReadTrack = 0;

  TCanvas *c = new TCanvas("c", "c", 1000, 1000);
  //TGraph* grTOF = new TGraph(); 
  //TGraph* grTrack = new TGraph();
  TMultiGraph *mg = new TMultiGraph();
  TGraph* grSub = new TGraph();

  Double_t xTOF[100], yTOF[100];
  Double_t xTrack[100], yTrack[100];

  Double_t xSub[100], ySub[100];

  TString FileToWriteName = "/home/facecerep/diplom/histo.root";
  TFile f(FileToWriteName, "RECREATE");
  cout << "file created at: " << FileToWriteName << endl;
  auto T = new TTree("T", "test");
  T->Branch("graph", &grSub, 10000, 0);

  for (Int_t iEv = 0; iEv < nEvForRead; iEv++) {

    ReadBytes = eveTree->GetEntry(iEv);

    entriesReadTOF = 0;
    entriesReadTrack = 0;

    for (Int_t iDig = 0; iDig < TOF_digi->GetEntriesFast(); iDig++) {
      BmnTOF1Conteiner *digi = (BmnTOF1Conteiner *)TOF_digi->At(iDig);
      xTOF[iDig] = digi->GetX();
      yTOF[iDig] = digi->GetY();
      entriesReadTOF++;
    }

    for (Int_t iDig = 0; iDig < Tracks_digi->GetEntriesFast(); iDig++) {
      Track *digi = (Track *)Tracks_digi->At(iDig);
      xTrack[iDig] = digi->GetfStopPointX();
      yTrack[iDig] = digi->GetfStopPointY();
      entriesReadTrack++;
    }

    if(entriesReadTOF != 0 && entriesReadTrack != 0){
      Int_t minimumEntriesRead = 0;

      if(entriesReadTOF < entriesReadTrack)
        minimumEntriesRead = entriesReadTOF;
      else
        minimumEntriesRead = entriesReadTrack;        

      for(Int_t i = 0; i < minimumEntriesRead; i++){
        xSub[i] = xTOF[i] - xTrack[i];
        ySub[i] = yTOF[i] - yTrack[i]; 
      }

      mg = new TMultiGraph();

      grSub = new TGraph(minimumEntriesRead, xSub, ySub);
      grSub->SetTitle("Sub");
      grSub->SetMarkerStyle(20);

      // grTOF = new TGraph(entriesReadTOF, xTOF, yTOF);
      // grTOF->SetTitle("TOF");
      // grTOF->SetMarkerStyle(20);

      // grTrack = new TGraph(entriesReadTrack, xTrack, yTrack);
      // grTrack->SetMarkerStyle(21);
      // grTrack->SetTitle("Tracks");

      //cout << "Enrties read track: " << entriesReadTrack << endl;
      //cout << "Entries read TOF: " << entriesReadTOF << endl;
      //cout << "Entries read: " << minimumEntriesRead << endl;
      //mg->Add(grSub, "AP");

      //mg->Draw("A pmc plc");
      
      //grSub->Draw("AP");

      T->Fill();
      //T->Print();
      f.Write();
      c->Update();

      //this_thread::sleep_for(chrono::milliseconds(100));
    }
  }

  //getchar();
}
int main(int argc, char **argv) {

  Int_t run_number;

  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " <run number> " << std::endl;
    return 1;
  } else if (argc >= 2)
  run_number = atoi(argv[1]);
  std::cout << "Doing analysis for run " << run_number << "\n";
  TApplication theApp("App", &argc, argv);
  Analysis(run_number);
  std::cout << "Analysis complete!" << std::endl;
  return 0;
}