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
#include <TImage.h>
#include <TLeaf.h>
#include <TList.h>
#include <TMath.h>
#include <TMultiGraph.h>
#include <TObject.h>
#include <TPad.h>
#include <TProfile.h>
#include "TRandom.h"
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
  Double_t toReturn = 100000;
  
  if(finish == 0)
    finish = n;

  for(Int_t i = start; i < finish; i++){
    if(array[i] < toReturn)
      toReturn = array[i];
  }

  return toReturn;
}

Double_t FindMaximum(Int_t n, Double_t *array, Int_t start = 0, Int_t finish = 0){
  Double_t toReturn = -10000;
  
  if(finish == 0)
    finish = n;

  for(Int_t i = start; i < finish; i++){
    if(array[i] > toReturn)
      toReturn = array[i];
  }

  return toReturn;
}

void Analysis(Int_t run_number, Bool_t verbose = kFALSE){
  TString name;

  printf("Start analysis\n");
  TString FileName = "/home/facecerep/Downloads/Merge_RPC_DRS_allTracks (1)"; // NameInFile
  // TString FileName = "/home/facecerep/Downloads/Merge_RPC_DRS_allTracks (1)"; // NameInFile
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

  if (verbose){
    auto *Branches = eveTree->GetListOfBranches();
    for (Int_t iBr = 0; iBr < eveTree->GetNbranches(); ++iBr){
      auto Branch = (TBranch *) Branches->At(iBr);
      Branch->Print();
    }
    getchar();
  }

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
  Int_t plane[100];
  Double_t xTrackTemp[100], yTrackTemp[100], Mx[100], Nx[100], My[100], Ny[100];
  Double_t Z[4] = {868, 885, 901, 901};

  Int_t totalPointsNotLimited = 6624;
  Int_t totalPointsLimited = 1655;

  Double_t xSub[totalPointsNotLimited], ySub[totalPointsNotLimited];
  Double_t xTOF[totalPointsNotLimited], yTOF[totalPointsNotLimited];
  Double_t xTrack[totalPointsNotLimited], yTrack[totalPointsNotLimited];

  Int_t pointsTotal = 0;

  for (Int_t iEv = 0; iEv < nEvForRead; iEv++)
  {
    if (verbose)
      cout << "EVENT #" << iEv << endl;
    ReadBytes = eveTree->GetEntry(iEv);

    entriesReadTOF = 0;
    entriesReadTrack = 0;

    if (verbose && TOF_digi->GetEntriesFast()>0 ) cout << "TOF hits" << endl;
    for (Int_t iDig = 0; iDig < TOF_digi->GetEntriesFast(); iDig++)
    {
      BmnTOF1Conteiner *digi = (BmnTOF1Conteiner *)TOF_digi->At(iDig);
      xTOFTemp[iDig] = digi->GetX();
      yTOFTemp[iDig] = digi->GetY();
      plane[iDig] = digi->GetPlane();
      entriesReadTOF++;
      if (verbose)
        cout << "TOF_x = " << xTOFTemp[iDig] << "\t TOF_y = " << yTOFTemp[iDig] << endl;
    }

    if (verbose && Tracks_digi->GetEntriesFast()>0 ) cout << "Tracks hits" << endl;
    for (Int_t iDig = 0; iDig < Tracks_digi->GetEntriesFast(); iDig++)
    {
      Track *digi = (Track *)Tracks_digi->At(iDig);
      xTrackTemp[iDig] = digi->GetfStopPointX(); // Вычислить как Mx * z (Для каждого плейна всой z) + Nx 
                                                 // Это делается так как у каждого детектора разные координаты по z и это нужно учитв=ывать
      yTrackTemp[iDig] = digi->GetfStopPointY();

      Mx[iDig] = digi -> GetfFitParamMX();
      Nx[iDig] = digi -> GetfFitParamNX();
      My[iDig] = digi -> GetfFitParamMY();
      Ny[iDig] = digi -> GetfFitParamNY();

      // xTrackTemp[iDig] = digi -> GetfFitParamMX() * digi -> GetfFirstPointZ() + digi -> GetfFitParamNX();
      // yTrackTemp[iDig] = digi -> GetfFitParamMY() * digi -> GetfFirstPointZ() + digi -> GetfFitParamNY();

      entriesReadTrack++;
      if (verbose)
        cout << "Tra_x = " << xTrackTemp[iDig] << "\t Tra_y = " << yTrackTemp[iDig] << endl;
    }

    if (entriesReadTOF != 0 && entriesReadTrack != 0)
    {
      Int_t minimumEntriesRead = 0;

      if (entriesReadTOF < entriesReadTrack)
        minimumEntriesRead = entriesReadTOF;
      else
        minimumEntriesRead = entriesReadTrack;

      if (verbose){
        cout << "Readed entries (min, TOF, Tracks)" << endl;
        cout << minimumEntriesRead << "\t" << entriesReadTOF << "\t" << entriesReadTrack << endl;
      }

      for (Int_t i = 0; i < minimumEntriesRead; i++){
        Double_t minimumXSub = 1000000;
        Double_t minimumYSub = 1000000;
        Double_t defaultMinimalDistance = 10000;
        Double_t minimalDistance = defaultMinimalDistance;

        Int_t minimumTOFNum = 10000;
        Int_t minimumTrackNum = 10000;

        Double_t minimalTackXCalculated = 0;
        Double_t minimalTackYCalculated = 0;

        for (Int_t iTOF = 0; iTOF < entriesReadTOF; iTOF++){
          for (Int_t iTrack = 0; iTrack < entriesReadTrack; iTrack++){
            if(xTOFTemp[iTOF] != NULL && xTrackTemp[iTrack] != NULL && plane[iTOF] == 0){
              Double_t x = xTOFTemp[iTOF] - xTrackTemp[iTrack];
              Double_t y = yTOFTemp[iTOF] - yTrackTemp[iTrack];
              // Double_t x = xTOFTemp[iTOF] - (Mx[iTrack] * Z[plane[iTOF]] + Nx[iTrack]);
              // Double_t y = yTOFTemp[iTOF] - (My[iTrack] * Z[plane[iTOF]] + Ny[iTrack]);
              
              Double_t distance = TMath::Sqrt(x * x + y * y);

              if (distance < minimalDistance){
                minimumXSub = x;
                minimumYSub = y;

                minimalDistance = TMath::Sqrt(minimumXSub * minimumXSub + minimumYSub * minimumYSub);

                minimumTOFNum = iTOF;
                minimumTrackNum = iTrack;

                minimalTackXCalculated = Mx[iTrack] * Z[plane[iTOF]] + Nx[iTrack];
                minimalTackYCalculated = My[iTrack] * Z[plane[iTOF]] + Ny[iTrack];
              }
            }
          }
        }

        if(minimalDistance != defaultMinimalDistance){
          xSub[pointsTotal] = minimumXSub;
          ySub[pointsTotal] = minimumYSub;

          xTOF[pointsTotal] = xTOFTemp[minimumTOFNum];
          yTOF[pointsTotal] = yTOFTemp[minimumTOFNum];
          xTrack[pointsTotal] = xTrackTemp[minimumTrackNum];
          yTrack[pointsTotal] = yTrackTemp[minimumTrackNum];
          // xTrack[pointsTotal] = minimalTackXCalculated;
          // yTrack[pointsTotal] = minimalTackYCalculated;

          if (verbose){
            cout << "Calculation of the differences" << endl;
            cout << "Sub_x = " << xSub[pointsTotal] << "\t Sub_y = " << ySub[pointsTotal] << endl;
            cout << "TOF_x = " << xTOFTemp[minimumTOFNum] << "\t Tof_y = " << yTOFTemp[minimumTOFNum] << endl;
            cout << "TOF_x_Num = " << minimumTOFNum << "\t Tof_y_Num = " << minimumTOFNum << endl;
            cout << "Track_x = " << minimalTackXCalculated << "\t Track_y = " << minimalTackYCalculated << endl;
            cout << "Mx = " << Mx[minimumTrackNum] << "\t My = " <<  My[minimumTrackNum] << endl;
            cout << "Nx = " << Nx[minimumTrackNum] << "\t Ny = " <<  Ny[minimumTrackNum] << endl;
            cout << "Track_x_Num = " << minimumTrackNum << "\t Track_y_Num = " << minimumTrackNum << endl;

            getchar();
          }

          xTOFTemp[minimumTOFNum] = NULL;
          yTOFTemp[minimumTOFNum] = NULL;
          xTrackTemp[minimumTrackNum] = NULL;
          yTrackTemp[minimumTrackNum] = NULL;

          pointsTotal++;
        }
        else{
          if(verbose)
            cout << "Skipped" << endl;
        }
      }
    }
  }
  Double_t xMinimum = FindMinimum(pointsTotal, xSub);
  Double_t xMaximum = FindMaximum(pointsTotal, xSub);
  Double_t yMinimum = FindMinimum(pointsTotal, ySub);
  Double_t yMaximum = FindMaximum(pointsTotal, ySub);
  cout << "xMinimum: " << xMinimum << endl;
  //cout << "Points Total: " << pointsTotal << endl;

  auto* hSubX = new TH1D("hsubx", "Sub X Histo", 100, xMinimum, xMaximum);
  // auto* hSubX = new TH1D("hsubx", "Sub X Histo", 100, 100, 300);
  hSubX -> SetFillColor(30);
  //hSubX -> SetFillStyle(4050);
  auto* hSubXSimulated = new TH1D("hsubxsim", "Sub X Histo Simulated", 100, xMinimum, xMaximum);
  hSubXSimulated -> SetFillColor(46);
  hSubXSimulated -> SetFillStyle(3001);

  auto* hSubY = new TH1D("hsuby", "Sub Y Histo", 100, yMinimum, yMaximum);
  // auto* hSubY = new TH1D("hsuby", "Sub Y Histo", 100, -100, 20);
  hSubY -> SetFillColor(30);
  //hSubY -> SetFillStyle(4050);
  auto* hSubYSimulated = new TH1D("hsubysim", "Sub Y Histo Simulated", 100, yMinimum, yMaximum);
  hSubYSimulated -> SetFillColor(46);
  hSubYSimulated -> SetFillStyle(3001);

  auto* hTrackX = new TH1D("hTrackx", "Track X Histo", 100, FindMinimum(pointsTotal, xTrack, pointsTotal / 2, pointsTotal), FindMaximum(pointsTotal, xTrack, pointsTotal / 2, pointsTotal));
  auto* hTrackY = new TH1D("hTracky", "Track Y Histo", 100, FindMinimum(pointsTotal, yTrack, pointsTotal / 2, pointsTotal), FindMaximum(pointsTotal, yTrack, pointsTotal / 2, pointsTotal));
  // auto* hTrackX = new TH1D("hTrackx", "Track X Histo", 100, -2, 2);
  // auto* hTrackY = new TH1D("hTracky", "Track Y Histo", 100, -10, 20);
  hTrackX -> SetFillColor(30);
  hTrackY -> SetFillColor(30);
  
  auto* hTOFX = new TH1D("hTOFx", "TOF X Histo", 100, FindMinimum(pointsTotal, xTOF, pointsTotal / 2, pointsTotal), FindMaximum(pointsTotal, xTOF, pointsTotal / 2, pointsTotal));
  auto* hTOFY = new TH1D("hTOFy", "TOF Y Histo", 100, FindMinimum(pointsTotal, yTOF, pointsTotal / 2, pointsTotal), FindMaximum(pointsTotal, yTOF, pointsTotal / 2, pointsTotal));
  auto* hTOFXSimulated = new TH1D("hTOFx", "TOF X Histo", 100, FindMinimum(pointsTotal, xTOF, pointsTotal / 2, pointsTotal), FindMaximum(pointsTotal, xTOF, pointsTotal / 2, pointsTotal));
  auto* hTOFYSimulated = new TH1D("hTOFy", "TOF Y Histo", 100, FindMinimum(pointsTotal, yTOF, pointsTotal / 2, pointsTotal), FindMaximum(pointsTotal, yTOF, pointsTotal / 2, pointsTotal));

  for (Int_t i = 0; i < pointsTotal; i++) // WHY DIVIDE BY 2 ??????
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

  TF1 *fTOFx = new TF1("fTOFx", "gaus", xMinimum, xMaximum);
  fTOFx -> SetLineColor(3);
  TF1 *fTOFy = new TF1("fTOFy", "gaus", yMinimum, yMaximum);
  fTOFy -> SetLineColor(3);
  TF1 *fTOFSimulatedx = new TF1("fTOFSimulatedx", "gaus", xMinimum, xMaximum);
  fTOFSimulatedx -> SetLineColor(2);
  TF1 *fTOFSimulatedy = new TF1("fTOFSimulatedy", "gaus", yMinimum, yMaximum);
  fTOFSimulatedy -> SetLineColor(2);

  TCanvas *c = new TCanvas("c", "c", 1000, 1000);

  TPad *xPad = new TPad("xPad", "xPad", 0, 0, 0.5, 1);
  TPad *yPad = new TPad("yPad", "yPad", 0.5, 0, 1, 1);
  
  xPad -> Draw();
  yPad -> Draw();

  xPad -> cd();
  cout << "-------- hSubX Fit --------" << endl;
  hSubX -> Fit(fx);

  yPad -> cd();
  cout << "-------- hSubY Fit --------" << endl;
  hSubY -> Fit(fy);

  TRandom *random = new TRandom();
  Double_t fxMean = fx -> GetParameter(1);
  Double_t fxSigma = fx -> GetParameter(2);
  Double_t fyMean = fy -> GetParameter(1);
  Double_t fySigma = fy -> GetParameter(2);

  for (Int_t i = pointsTotal / 2; i < pointsTotal; i++){
    Double_t SubXSimulated = fx -> GetRandom();
    Double_t SubYSimulated = fy -> GetRandom();

    hSubXSimulated -> Fill(SubXSimulated);
    hSubYSimulated -> Fill(SubYSimulated);

    hTOFXSimulated -> Fill(xTrack[i] + SubXSimulated);
    hTOFYSimulated -> Fill(yTrack[i] + SubYSimulated);
  }
  
  cout << "-------- hSubXSimulated Fit --------" << endl;
  hSubXSimulated -> Fit("gaus");
  cout << "-------- hSubYSimulated Fit --------" << endl;
  hSubYSimulated -> Fit("gaus");

  hTOFXSimulated -> SetFillStyle(3001);
  hTOFXSimulated -> SetFillColor(46);
  hTOFYSimulated -> SetFillStyle(3001);
  hTOFYSimulated -> SetFillColor(46);
  hTOFX -> SetFillColor(30);
  hTOFY -> SetFillColor(30);
  
  // TImage *image = TImage::Create();
  // image -> FromPad(c);
  // //Saving TOF canvas
  // xPad -> cd();
  // hTOFX -> Draw();
  // yPad -> cd();
  // hTOFY -> Draw();
  // c -> Modified();
  // c -> Update();
  // image -> WriteImage("TOF.png");

  // getchar();

  //Saving Track canvas
  xPad -> cd();
  hTrackX -> Draw();
  yPad -> cd();
  hTrackY -> Draw();
  c -> Update();
  //image -> WriteImage("Track.png");

  getchar();

  xPad -> cd();
  hSubX -> Draw();
  hSubXSimulated -> Draw("SAME");

  yPad -> cd();
  hSubY -> Draw();
  hSubYSimulated -> Draw("SAME");

  c -> Modified();
  c -> Update();

  getchar();

  xPad -> cd();
  hTOFX -> Draw();
  cout << "-------- hTOFX Fit --------" << endl;
  hTOFX -> Fit("fTOFx");
  hTOFXSimulated -> Draw("SAME");
  cout << "-------- hTOFXSimulated Fit --------" << endl;
  hTOFXSimulated -> Fit("fTOFSimulatedx");

  yPad -> cd();
  hTOFY -> Draw();
  cout << "-------- hTOFY Fit --------" << endl;
  hTOFY -> Fit("fTOFy");
  hTOFYSimulated -> Draw("SAME");
  cout << "-------- hTOFYSimulated Fit --------" << endl;
  hTOFYSimulated -> Fit("fTOFSimulatedy");

  c -> Modified();
  c -> Update();

  Double_t xMeanPrecision = fTOFx -> GetParameter(1) / fTOFSimulatedx -> GetParameter(1);
  if(xMeanPrecision > 1)
    xMeanPrecision = 1 / xMeanPrecision;
  Double_t yMeanPrecision = fTOFy -> GetParameter(1) / fTOFSimulatedy -> GetParameter(1);
  if(yMeanPrecision > 1)
    yMeanPrecision = 1 / yMeanPrecision;
  Double_t xSigmaPrecision = fTOFx -> GetParameter(2) / fTOFSimulatedx -> GetParameter(2);
  if(xSigmaPrecision > 1)
    xSigmaPrecision = 1 / xSigmaPrecision;
  Double_t ySigmaPrecision = fTOFy -> GetParameter(2) / fTOFSimulatedy -> GetParameter(2);
  if(ySigmaPrecision > 1)
    ySigmaPrecision = 1 / ySigmaPrecision;

  cout << "----------------" << endl;
  cout << "xMeanPrecision: " << xMeanPrecision << endl;
  cout << "xSigmaPrecision: " << xSigmaPrecision << endl;
  cout << "yMeanPrecision: " << yMeanPrecision << endl;
  cout << "ySigmaPrecision: " << ySigmaPrecision << endl;

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
  Analysis(run_number, (Bool_t) run_number);
  std::cout << "Analysis complete!" << std::endl;
  return 0;
}
