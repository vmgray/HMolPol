/***********************************************************
 Programmer: Valerie Gray
 Purpose:
 - Take a root file with the left right detectors and get
 some info from it

 - XY distribution for all events with hits in the left detector, the right
 detector and then both in councidence

 - The distribution of the Center of mass theta

 - Momentum of the scattered particle

 - NOTE: to get information on only the primary particles look at
 the first hit of any event in the on a detector. This is NOT a true primary
 rater a naive primary. True primaries will come later.

 Entry Conditions: none
 Date: 09-22-2015
 Modified:
 Assisted By: Me! && Juan Carlos Cornejo
 *********************************************************/

//Root includes
#include <TSystem.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TH2D.h>
#include <TMath.h>
#include <TLatex.h>
#include <TF1.h>
#include <TStyle.h>
#include <TString.h>
#include <TFile.h>
#include <TH1D.h>

//system includes
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>

//HMolPol includes
#include "HMolPolEventGenericDetector.hh"
#include "HMolPolEventUnits.hh"
#include "HMolPolEventPrimary.hh"
#include "HMolPolRunInformation.hh"

//HMolPol Macro includes
#include "ExtractFromRunInfo.h"

//Function definitions
void GetGitInfo();
void Define_Histograms();
void Loop_Through_Tree();
void Plot();

//Globals
//define units used
#define LENGTH_UNIT Units->m
#define ANGLE_UNIT Units->deg
#define MOMENTUM_UNIT Units->GeV

std::string LENGTH_UNIT_NAME = "m";  //units for length
std::string ANGLE_UNIT_NAME = "deg";  //units for angle
std::string MOMENTUM_UNIT_NAME = "GeV";  //units for Momentum

//define histograms
std::vector<TH2D*> h_XY_PLANE_DISTRIBUTION;  //histogram for the XY plane
//distribution at the detectorss
std::vector<TH1D*> h_THETA_CM_DISTRIBUTION;  //histogram for the Theta CM
//distribution at the detectors
std::vector<TH1D*> h_MOMENTUM_DISTRIBUTION;  //histogram for the XY plane
//distribution at the detectors

//define the TChain
TChain* HMOLPOL_CHAIN;

//define constants
const Int_t NUM_DET_COMBINATIONS = 3;

std::string INDEX_TO_DET_COMBINATIONS_TO_NAME[NUM_DET_COMBINATIONS] = {
    "Right", "Left", "Coincidence" };

//string for the root flle name to be shared with 2 functions
static std::string FILENAME;

void Left_Right_Det_Test(std::string filename)
{
  FILENAME = Form("~/workspace/HMolPol/%s", filename.c_str());

  //Get the Git info for the simulation
  GetGitInfo();

  //Grab the file(s)
  HMOLPOL_CHAIN = new TChain("HMolPol_Tree");
  //add the file

  /*
   HMOLPOL_CHAIN->Add(
   "~/workspace/HMolPol/HMolPol_Left_Right_2GeV_MagField8T.root");
   */
  HMOLPOL_CHAIN->Add(Form("%s", FILENAME.c_str()));

  //define the histograms
  Define_Histograms();
  //Loop through tree
  Loop_Through_Tree();
  //Plot
  Plot();

  return;
}

/***********************************************************
 Function: GetGitInfo
 Purpose:
 To read the git information from the file and print it to a file

 Entry Conditions:
 none

 Global:
 None

 Exit Conditions: none
 Called By: Left_Right_Det_Test
 Date: 09-28-2015
 Modified:
 Brilliance By: Juan Carlos Cornejo
 *********************************************************/
void GetGitInfo()
{
  //print to file
  //this file and everything related to it is fout
  std::ofstream fout;

  //open a file
  fout.open("/home/vmgray/workspace/HMolPol/GitInformation.txt");

  if (!fout.is_open())
    std::cout << "File not opened" << std::endl;

  //Read the ROOT file
  //TFile* file = new TFile("HMolPol_0.root", "READ");
  TFile* file = new TFile(Form("%s", FILENAME.c_str()), "READ");

  // Get Pointer to the Run Information class instance
  HMolPolRunInformation* runInfo = (HMolPolRunInformation*) file->Get(
      "HMolPolRunInformation");

  // Loop over the runInfo list until we get the information we want
  TString buffer;  //String with the lines of information we are intersted in
  // it will help give us the number of characters we are ignoring
  TString gitRevision;  //The git revision number
  TString gitLastAuthor;  //The last author to edit the simulation for this branch
  TString gitBranch;  //The branch of git the simulation is on

  //Loop through all the lines in the Run information
  for (Int_t i = 0; i < runInfo->Conditions.GetSize(); i++)
  {

    // Get the string at position i in the Conditions list - the i^th
    // entry in the list
    buffer = ((TObjString*) runInfo->Conditions.At(i))->GetString();

    // Get "Revision: " (case sensitive, and space is important)
    ExtractFromRunInfo(buffer, "Revision: ", gitRevision);

    // Repeat with other info we want

    //Get "Last changed author: " (case sensitive, and space is important)
    ExtractFromRunInfo(buffer, "Last changed author: ", gitLastAuthor);

    //Get "Current Branch: " (case sensitive, and space is important)
    ExtractFromRunInfo(buffer, "Current Branch: ", gitBranch);

  }

  // Print to screen to show that it works
  fout << "git revision number: " << gitRevision << std::endl;
  fout << "last changed author: " << gitLastAuthor << std::endl;
  fout << "Current branch: " << gitBranch << std::endl;

  //debugging
  /*
   std::cout << "Found git revision number: " << gitRevision << std::endl;
   std::cout << "Found last changed author: " << gitLastAuthor << std::endl;
   std::cout << "Found current branch: " << gitBranch << std::endl;
   */

  /*
   //debugging
   // As an example, we'll print out the entire runInfo, so you can see what we have
   // extracted.
   runInfo->Conditions.Print();
   */

  return;
}

/***********************************************************
 Function: Define_Histograms
 Purpose:
 To define all the histograms,
 ie. name, axis labels

 Entry Conditions:
 none

 Global:

 - h_XY_PLANE_DISTRIBUTION - Vector of Historgrams for XY dis
 - LENGHT_UNIT_NAME - units for length
 - NUM_DET_COMBINATIONS - Number of the detectors combinations possible
 - INDEX_TO_DET_COMBINATIONS_TO_NAME - Name of detector combinations

 Exit Conditions: none
 Called By: Left_Right_Det_Test
 Date: 09-22-2015
 Modified:
 *********************************************************/
void Define_Histograms()
{
  //resize vectors
  h_XY_PLANE_DISTRIBUTION.resize(NUM_DET_COMBINATIONS);
  h_THETA_CM_DISTRIBUTION.resize(NUM_DET_COMBINATIONS);
  h_MOMENTUM_DISTRIBUTION.resize(NUM_DET_COMBINATIONS);

  //loop over the detector combinations
  for (int i = 0; i < NUM_DET_COMBINATIONS; i++)
  {
    //XY distribution
    h_XY_PLANE_DISTRIBUTION[i] = new TH2D(
        Form("h_XY_PLANE_DISTRIBUTION[%d]", i),
        Form("Y vs X hit distribution at the %s detector",
             INDEX_TO_DET_COMBINATIONS_TO_NAME[i].c_str()),
        100, -0.3, 0.3, 100, -0.3, 0.3);

    h_XY_PLANE_DISTRIBUTION[i]->GetYaxis()->SetTitle(
        Form("Y Position (%s)", LENGTH_UNIT_NAME.c_str()));
    h_XY_PLANE_DISTRIBUTION[i]->GetXaxis()->SetTitle(
        Form("X Position (%s)", LENGTH_UNIT_NAME.c_str()));

    //CM Theta Distribution
    h_THETA_CM_DISTRIBUTION[i] = new TH1D(
        Form("h_THETA_CM_DISTRIBUTION[%d]", i),
        Form("#theta Center of Mass hit distribution at the %s detector",
             INDEX_TO_DET_COMBINATIONS_TO_NAME[i].c_str()),
        180, 50, 130);

    h_THETA_CM_DISTRIBUTION[i]->GetXaxis()->SetTitle(
        Form("#theta_{CM} (%s)", ANGLE_UNIT_NAME.c_str()));

    //CM Theta Distribution
    h_MOMENTUM_DISTRIBUTION[i] = new TH1D(
        Form("h_MOMENTUM_DISTRIBUTION[%d]", i),
        Form("Momentum hit distribution at the %s detector",
             INDEX_TO_DET_COMBINATIONS_TO_NAME[i].c_str()),
        100, 0.5, 1.5);

    h_MOMENTUM_DISTRIBUTION[i]->GetXaxis()->SetTitle(
        Form("Momentum (%s)", MOMENTUM_UNIT_NAME.c_str()));
  }

  return;
}

/***********************************************************
 Function: Loop_Through_Tree
 Purpose:
 To loop over the event and fill in the values for the
 histograms

 Entry Conditions:
 none

 Global:
 - TChain - The TChain
 - h_XY_PLANE_DISTRIBUTION - vector of the XY plane hit distribution
 - LENGTH_UNIT - unit that all the lenghts are in
 - NUM_DET_COMBINATIONS - Number of the detectors combinations possible
 - INDEX_TO_DET_COMBINATIONS_TO_NAME - Name of detector combinations

 Exit Conditions: none
 Called By: Left_Right_Det_Test
 Date: 09-22-2015
 Modified:
 *********************************************************/
void Loop_Through_Tree()
{
  //get the number of entires in the tree
  Int_t nentries = HMOLPOL_CHAIN->GetEntries();

  //debugging
  //std::cout << "number of events in tree: " << nentries << std::endl;

  //Get the Units branch, so we can use the units
  HMolPolEventUnits* Units = 0;
  HMOLPOL_CHAIN->SetBranchAddress("fUnits", &Units);

  //Get the Primary branch, so we can use the units
  HMolPolEventPrimary* Primary = 0;
  HMOLPOL_CHAIN->SetBranchAddress("fPrimary", &Primary);

  /*******************************
   * Want information from a Generic Detector.
   * Create a pointer to a HMolPolEventGenericDetector (the object storing the
   * Generic Detector info)
   * Then set a branch address to that generic detector
   *
   * Get the branches that are need for this script
   * This is done by:
   *******************************/
  HMolPolEventGenericDetector* Detector_Det = 0;
  HMOLPOL_CHAIN->SetBranchAddress("fDetectorDet", &Detector_Det);

  //loop over all the events in the tree
  for (int i_entry = 0; i_entry < nentries; i_entry++)
  {
    //get the first entry in the tree
    HMOLPOL_CHAIN->GetEntry(i_entry);

    //Count for the number of hits in the left/right detector
    Int_t num_Left_det_Hit = 0;
    Int_t num_Right_det_Hit = 0;
    Int_t Left_Det_Hit_Num = -1000;
    Int_t Right_Det_Hit_Num = -1000;

    //debugging
    /*
     std::cout << "Left_Det_Hit_Num: " << Left_Det_Hit_Num << std::endl;
     std::cout << "Right_Det_Hit_Num: " << Right_Det_Hit_Num << std::endl;
     */

    //if we have information for the dummy detector we want to look at that
    if (Detector_Det)
    {
      //get the number of hits that the Detector Det has for the i_entry in the tree
      Int_t detector_hits_nentries = Detector_Det->fHits.size();

      //debugging
      /*
       std::cout << detector_hits_nentries << " hits in Detector Det for event "
       << i_entry << std::endl;
       */

      //if there is hits in this entry then carry on
      if (Detector_Det->fHits.size() != 0)
      {
        //only want the first hit, this is related to the primary electrons
        //for (int i_hits = 0; i_hits < 1; i_hits++)
        //loop over all the hits and fill the histograms with the information of interest
        for (int i_hits = 0; i_hits < detector_hits_nentries; i_hits++)
        {
          //debugging
          //std::cout << "In the Hits loop" << std::endl;
          /*
           std::cout << "X Position " << Detector_Det->fHits[i_hits].fPosition.fX
           << " " << Detector_Det->fHits[i_hits].fPosition.fX / LENGTH_UNIT
           << " m " << std::endl << "Y Position "
           << Detector_Det->fHits[i_hits].fPosition.fY / LENGTH_UNIT
           << " m " << std::endl;
           */

          //if we are in the left detector
          if (Detector_Det->fHits[i_hits].fDetectorName.EqualTo("LeftDetector"))
          {
            num_Left_det_Hit++;

            if (num_Left_det_Hit == 1)
            {

              Left_Det_Hit_Num = i_hits;
              //debuging
              /*
               std::cout << "Left_Det_Hit_Num: " << Left_Det_Hit_Num
               << std::endl;
               */
              /*
               std::cout << "X Position " << Detector_Det->fHits[i_hits].fPosition.fX
               << " " << Detector_Det->fHits[i_hits].fPosition.fX / LENGTH_UNIT
               << " m " << std::endl << "Y Position "
               << Detector_Det->fHits[i_hits].fPosition.fY / LENGTH_UNIT
               << " m " << std::endl;
               */

              //Fill the histograms
              //XY distribution
              h_XY_PLANE_DISTRIBUTION[1]->Fill(
                  Detector_Det->fHits[i_hits].fPosition.fX / LENGTH_UNIT,
                  Detector_Det->fHits[i_hits].fPosition.fY / LENGTH_UNIT);

              //Theta CM Angle
              h_THETA_CM_DISTRIBUTION[1]->Fill(
                  Primary->fThetaCenterOfMass / ANGLE_UNIT);

              //Momentum
              h_MOMENTUM_DISTRIBUTION[1]->Fill(
                  Detector_Det->fHits[i_hits].fMomentum.Mag() / MOMENTUM_UNIT);
            }
          }  //end of if in left detector

          //if we are in the Right detector
          if (Detector_Det->fHits[i_hits].fDetectorName.EqualTo(
              "RightDetector"))
          {
            num_Right_det_Hit++;

            if (num_Right_det_Hit == 1)
            {

              Right_Det_Hit_Num = i_hits;
              /*
               std::cout << "Right_Det_Hit_Num: " << Right_Det_Hit_Num
               << std::endl;
               */
              //debuging
              /*
               std::cout << "X Position " << Detector_Det->fHits[i_hits].fPosition.fX
               << " " << Detector_Det->fHits[i_hits].fPosition.fX / LENGTH_UNIT
               << " m " << std::endl << "Y Position "
               << Detector_Det->fHits[i_hits].fPosition.fY / LENGTH_UNIT
               << " m " << std::endl;
               */

              //Fill the histograms
              //XY distribution
              h_XY_PLANE_DISTRIBUTION[0]->Fill(
                  Detector_Det->fHits[i_hits].fPosition.fX / LENGTH_UNIT,
                  Detector_Det->fHits[i_hits].fPosition.fY / LENGTH_UNIT);

              //Theta CM Angle
              h_THETA_CM_DISTRIBUTION[0]->Fill(
                  Primary->fThetaCenterOfMass / ANGLE_UNIT);

              //Momentum
              h_MOMENTUM_DISTRIBUTION[0]->Fill(
                  Detector_Det->fHits[i_hits].fMomentum.Mag() / MOMENTUM_UNIT);
            }

          }  //end of if in right detector

        }  //end of loop over hits in the detector

        //debugging
        /*
         std::cout << "Left_Det_Hit_Num: " << Left_Det_Hit_Num << std::endl;
         std::cout << "Right_Det_Hit_Num: " << Right_Det_Hit_Num << std::endl;
         */

        //If this event has a counicidence
        if (Left_Det_Hit_Num != -1000 && Right_Det_Hit_Num != -1000)
        {
          //debugging
          //std::cout << "Both the Right and Left Det have a hit And Seg Fault... or not" << std::endl;

          //Fill with the left and right detector information
          h_XY_PLANE_DISTRIBUTION[2]->Fill(
              Detector_Det->fHits[Left_Det_Hit_Num].fPosition.fX / LENGTH_UNIT,
              Detector_Det->fHits[Left_Det_Hit_Num].fPosition.fY / LENGTH_UNIT);

          h_XY_PLANE_DISTRIBUTION[2]->Fill(
              Detector_Det->fHits[Right_Det_Hit_Num].fPosition.fX / LENGTH_UNIT,
              Detector_Det->fHits[Right_Det_Hit_Num].fPosition.fY / LENGTH_UNIT);

          //Theta CM Angle
          h_THETA_CM_DISTRIBUTION[2]->Fill(
              Primary->fThetaCenterOfMass / ANGLE_UNIT);

          //Momentum
          h_MOMENTUM_DISTRIBUTION[2]->Fill(
              Detector_Det->fHits[Left_Det_Hit_Num].fMomentum.Mag() / MOMENTUM_UNIT);

          h_MOMENTUM_DISTRIBUTION[2]->Fill(
                       Detector_Det->fHits[Right_Det_Hit_Num].fMomentum.Mag() / MOMENTUM_UNIT);
        }

      }  //end of if hits in DetectorDet >0

      //debugging
      /*
       std::cout << "Left_Det_Hit_Num: " << Left_Det_Hit_Num << std::endl;
       std::cout << "Right_Det_Hit_Num: " << Right_Det_Hit_Num << std::endl;
       */

    }  //end of if in the detector det

  }  //end of loop over events

  return;
}

/***********************************************************
 Function: Plot
 Purpose:
 To plot the graphs and save those plots

 Entry Conditions:
 none

 Global:
 - h_XY_PLANE_DISTRIBUTION - vector of the XY plane hit distribution
 - INDEX_TO_DET_COMBINATIONS_TO_NAME - Name of detector combinations
 - NUM_DET_COMBINATIONS - number of detector combinations

 Exit Conditions: none
 Called By: Left_Right_Det_Test
 Date: 09-22-2015
 Modified:
 *********************************************************/
void Plot()
{
  //debugging
  //std::cout << "made it in the make plotting function" << std::endl;

  //define Canvases
  //std::vector<TCanvas*> c_XY_hits_ditribution;  //[Mag Field index]
  //c_XY_hits_ditribution.resize(NUM_FILES);
  for (int i = 0; i < NUM_DET_COMBINATIONS; i++)
  {
    //XY Distribution
    TCanvas* c_XY_hits_ditribution = new TCanvas(
        "c_XY_hits_ditribution",
        Form("Y vs X hit distribution at the for the %s Detector",
             INDEX_TO_DET_COMBINATIONS_TO_NAME[i].c_str()));

    h_XY_PLANE_DISTRIBUTION[i]->Draw();

    c_XY_hits_ditribution->SaveAs(
        Form("YX_hit_dis_Det_%s_PrimaryOnly.png",
             INDEX_TO_DET_COMBINATIONS_TO_NAME[i].c_str()));

    //CM Theta
    TCanvas* c_theta_CM_hits_ditribution = new TCanvas(
        "c_theta_CM_hits_ditribution",
        Form("#theta Center of Mass hit distribution at the %s detector",
             INDEX_TO_DET_COMBINATIONS_TO_NAME[i].c_str()));

    h_THETA_CM_DISTRIBUTION[i]->Draw();

    c_theta_CM_hits_ditribution->SaveAs(
        Form("Theta_CM_hit_dis_Det_%s_PrimaryOnly.png",
             INDEX_TO_DET_COMBINATIONS_TO_NAME[i].c_str()));

    //Momentum
    TCanvas* c_momentum_hits_ditribution = new TCanvas(
        "c_momentum_hits_ditribution",
        Form("Momentum hit  hit distribution at the %s detector",
             INDEX_TO_DET_COMBINATIONS_TO_NAME[i].c_str()));

    h_MOMENTUM_DISTRIBUTION[i]->Draw();

    c_momentum_hits_ditribution->SaveAs(
        Form("Momentum_hit_dis_Det_%s_PrimaryOnly.png",
             INDEX_TO_DET_COMBINATIONS_TO_NAME[i].c_str()));
  }

  return;
}
