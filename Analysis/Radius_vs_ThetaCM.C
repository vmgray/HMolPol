/***********************************************************
 Programmer: Valerie Gray
 Purpose:
 - To take a root file and plots radius of the niave primary hits on the detector
 plane vs the Center of Mass theta angle
 - It will work for BOTH dummy and left right detectors :) When the LeftRight is
 set up.

 - Also will write out the initial conditions via the ExtractInitalConditions.h
 file

 Entry Conditions: Root files name, Detector
 Date: 11-07-2016
 Modified: ??-??-2016
 Assisted By: Me!
 *********************************************************/

//Root includes
#include <TSystem.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLatex.h>
#include <TStyle.h>
#include <TString.h>
#include <TFile.h>
#include <TH2D.h>

//system includes
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>

//HMolPol includes
#include "HMolPolEventGenericDetector.hh"
#include "HMolPolEventUnits.hh"
#include "HMolPolEventPrimary.hh"
#include "HMolPolEventPrimaryConditions.hh"
#include "HMolPolRunInformation.hh"

//HMolPol Macro includes
#include "ExtractFromRunInfo.h"

//Function definitions
void GetGitInfo();
//Dummy Detector
void Define_Histograms_Dummy();
void Loop_Through_Tree_Dummy();
void Plot_Dummy();
//Left Right Detector
void Define_Histograms_LeftRight();
void Loop_Through_Tree_LeftRight();
void Plot_LeftRight();

//Globals
#define ANGLE_UNIT Units->deg
#define LENGTH_UNIT Units->m

//std::string ANGLE_UNIT_NAME = "deg";  //units for angle
std::string ANGLE_UNIT_NAME = "°";  //units for angle
std::string LENGTH_UNIT_NAME = "m";  //units for length

//define the TChain
TChain* HMOLPOL_CHAIN;

//define histograms
std::vector<TH2D*> h_RADIUS_VS_THETA_CM_DUMMY;  //[primary e]
//radius distribution vs theta CM for naive primary hits for each primary Dummy Det

//define histograms
std::vector<TH2D*> h_RADIUS_VS_THETA_CM_LEFTRIGHT;  //[primary e]
//radius distribution vs theta CM for naive primary hits for each primary Dummy Det

static const Int_t NUM_PRIMARIES = 3;  //number of primary electrons + 1

std::string INDEX_TO_NUM_PRIMARIES_TO_NAME[NUM_PRIMARIES] = {
    "coincidence of primaries", "primary 1", "primary 2" };

//string for the root flle name to be shared with 2 functions
static std::string FILENAME;

//string for the where the input file is stored.
/*static std::string INPUT_PREFIX =
 "/home/vmgray/workspace/HMolPol/Simulation_Analysis/commit_06d91f8";*/
static std::string INPUT_PREFIX = "/home/vmgray/workspace/HMolPol";

//string for the where the output will go.
/*static std::string OUTPUT_PREFIX =
 "/home/vmgray/workspace/HMolPol";*/
static std::string OUTPUT_PREFIX =
    "/home/vmgray/workspace/HMolPol/Simulation_Analysis/commit_5a398bd_Thesis/11GeV_Mag8T_HallAOpticsStandard_NoDipole/Radius_vs_ThetaCM";
// 11GeV_Mag8T_HallAOpticsStandard_NoDipole 11GeV_Mag8T_NoOptics

//HMolPol include - Must be here as it needs to know about OUTPUT_PREFIX
#include "ExtractInitalConditions.h"

void Radius_vs_ThetaCM(std::string filename, std::string DetectorType)
{
  FILENAME = Form("%s/%s", INPUT_PREFIX.c_str(), filename.c_str());

  //Get the Git info for the simulation
  GetGitInfo();

  //Grab the file(s)
  HMOLPOL_CHAIN = new TChain("HMolPol_Tree");
  //add the file

/*
  HMOLPOL_CHAIN->Add(
      "/home/vmgray/workspace/HMolPol/Simulation_Analysis/commit_5a398bd_Thesis/BeamE_Scan/100MeV/Rootfiles/HMolPol_Dummy_Kryptonite_100MeV_Mag8T_-10GenZ10_0Phi360_75Theta105_NoRaster_NoOptics_5a398bd_*.root");
*/

   HMOLPOL_CHAIN->Add(
   "~/workspace/HMolPol/Simulation_Analysis/commit_5a398bd_Thesis/11GeV_Mag8T_HallAOpticsStandard_NoDipole/Rootfiles/*.root");

//  HMOLPOL_CHAIN->Add(Form("%s", FILENAME.c_str()));

  //define the histograms
  //Loop through tree then Plot Dummy
  if (DetectorType.std::string::compare("Dummy") == 0)
  {
    Define_Histograms_Dummy();
    Loop_Through_Tree_Dummy();
    Plot_Dummy();
  }
  //loop through tree then Plot Left Right
  else if (DetectorType.std::string::compare("LeftRight") == 0)
  {
    std::cout
        << "Sorry this is not implemented yet. So if you need it, please implement. Thank you!"
        << std::endl;

    //Define_Histograms_LeftRight();
    //Loop_Through_Tree_LeftRight();
    //Plot_LeftRight();
  } else
    std::cout
        << "Hey you! Yes, you. You have chosen a wrong detector, Try again! Options are Dummy or LeftRight"
        << std::endl;

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
 Called By: Radius_vs_ThetaCM
 Date: 11-07-2016
 Modified:
 Brilliance By: Juan Carlos Cornejo
 *********************************************************/
void GetGitInfo()
{
  //print to file
  //this file and everything related to it is fout
  std::ofstream fout;

  //get the output file name.
  std::string output_filename = OUTPUT_PREFIX
      + std::string("/GitInformation.txt");

  //open a file
  fout.open(output_filename.c_str());

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
  TString gitLastDate;  //The last date to edit the simulation for this branch
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

    //Get "Last changed author: " (case sensitive, and space is important)
    ExtractFromRunInfo(buffer, "Last changed date: ", gitLastDate);

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

/*********
 * DUMMY STUFF
 *********/
/***********************************************************
 Function: Define_Histograms_Dummy
 Purpose:
 To define all the histograms,
 ie. name, axis labels

 Entry Conditions:
 none

 Global:
 - h_RADUIS_VS_THETA_CM_DUMMY -
 Radius distribution vs theta CM for naive primary hits for each primary Dummy Det

 - LENGTH_UNIT_NAME - units for angle
 - ANGLE_UNIT_NAME - units for angle
 - NUM_PRIMARIES - Number of the Primaries e-
 - INDEX_TO_NUM_PRIMARIES_TO_NAME - name for each primary count

 Exit Conditions: none
 Called By: Radius_vs_ThetaCM
 Date: 11-07-2016
 Modified:
 *********************************************************/
void Define_Histograms_Dummy()
{
  //resize histograms
  h_RADIUS_VS_THETA_CM_DUMMY.resize(NUM_PRIMARIES);

  //finish defining histograms
  for (int i = 0; i < NUM_PRIMARIES; i++)
  {
    // For the MeV plots
    // 160, 70.0, 110.0, 1000, 0.0, 1.05)

    // For the GeV plots
    // 160, 70.0, 110.0, 1000, 0.04, 0.22)

    // For the 11 GeV plots: Solenoid on or off, No Dipole, Quad on or off Combo
    // 160, 70.0, 110.0, 1000, 0.04, 0.1

    h_RADIUS_VS_THETA_CM_DUMMY[i] = new TH2D(
        Form("h_RADIUS_VS_THETA_CM_DUMMY[%d]", i),
        Form("Radius vs. #theta_{CM} for naive %s on the Dummy detector",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()),
        160, 70.0, 110.0, 1000, 0.04, 0.1);

    //    h_XY_DIST_VS_THETA_CM_DUMMY[i]->GetXaxis()->SetTitle(
//        Form("#theta_{CM} [%s]", ANGLE_UNIT_NAME.c_str()));
    h_RADIUS_VS_THETA_CM_DUMMY[i]->GetXaxis()->SetTitle("#theta_{CM} [#circ]");

    h_RADIUS_VS_THETA_CM_DUMMY[i]->GetYaxis()->SetTitle(
        Form("Radius [%s]", LENGTH_UNIT_NAME.c_str()));

  }

  return;
}

/***********************************************************
 Function: Loop_Through_Tree_Dummy
 Purpose:
 To loop over the event and fill in the values for the
 histograms

 Also will write out the initial conditions via the ExtractInitalConditions.h
 file

 Entry Conditions:
 none

 Global:
 - TChain - The TChain

 - h_RADIUS_VS_THETA_CM_DUMMY -
 Radius distribution vs theta CM for naive primary hits for each primary Dummy Det

 - LENGTH_UNIT - units for Lengtj
 - ANGLE_UNIT - units for angle

 Exit Conditions: none
 Called By: Radius_vs_ThetaCM
 Date: 11-07-2016
 Modified: ??-??-2016
 *********************************************************/
void Loop_Through_Tree_Dummy()
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

  //Get the PrimaryConditions branch, so we can use the units
  HMolPolEventPrimaryConditions* PrimaryConditions = 0;
  HMOLPOL_CHAIN->SetBranchAddress("fPrimaryConditions", &PrimaryConditions);

  /*******************************
   * Want information from a Generic Detector.
   * Create a pointer to a HMolPolEventGenericDetector (the object storing the
   * Generic Detector info)
   * Then set a branch address to that generic detector
   *
   * Get the branches that are need for this script
   * This is done by:
   *******************************/
  HMolPolEventGenericDetector* Dummy_Det = 0;
  HMOLPOL_CHAIN->SetBranchAddress("fDummyDet", &Dummy_Det);

  //loop over all the events in the tree
  for (int i_entry = 0; i_entry < nentries; i_entry++)
  {
    //get the first entry in the tree
    HMOLPOL_CHAIN->GetEntry(i_entry);

    //Get the initial conditions and print to file
    if (i_entry == 1)
      ExtractInitalConditions(Units, PrimaryConditions);

    //Count for the number of hits in the dummy detector
    Int_t num_Primary_1_Hit = 0;
    Int_t num_Primary_2_Hit = 0;
    //What hit has first primary
    Int_t Primary_1_Hit_Num = -1000;
    Int_t Primary_2_Hit_Num = -1000;
    //if we have information for the dummy detector we want to look at that
    if (Dummy_Det)
    {
      //get the number of hits that the Detector Det has for the i_entry in the tree
      Int_t detector_hits_nentries = Dummy_Det->fHits.size();

      //debugging
      /*
       std::cout << detector_hits_nentries << " hits in Dummy Det for event "
       << i_entry << std::endl;
       */
      //if there is hits in this entry then carry on
      if (Dummy_Det->fHits.size() != 0)
      {
        //only want the first hit, this is related to the primary electrons
        //for (int i_hits = 0; i_hits < 1; i_hits++)
        //loop over all the hits and fill the histograms with the information of interest
        for (int i_hits = 0; i_hits < detector_hits_nentries; i_hits++)
        {
          //debugging
          //std::cout << "In the Hits loop" << std::endl;

          //If we have a primary electron ie. it has no parent
          if (Dummy_Det->fHits[i_hits].fParentID == 0)
          {
            //What primary e- is this? The first?
            if (Dummy_Det->fHits[i_hits].fPrimaryID == 1)
            {
              //increase the number of hit by one
              num_Primary_1_Hit++;

              /*
               //debug
               std::cout <<"we have a primery ID= 2, Hit #" << num_Primary_2_Hit << std::endl;
               */

              //If we are the first hit of this primary (naive primary)
              if (num_Primary_1_Hit == 1)
              {
                //What hit number is this for this event? Store it! It is of interest
                Primary_1_Hit_Num = i_hits;

                //debuging
                /*
                 std::cout << "Primary_1_Hit_Num: " << Primary_1_Hit_Num
                 << std::endl;
                 */
              }  //End of if first hit primary 1
            }  //end of if it's primary 1
            //What primary e- is this? The second?
            if (Dummy_Det->fHits[i_hits].fPrimaryID == 2)
            {
              //increase the number of hit by one
              num_Primary_2_Hit++;

              /*
               //debug
               std::cout <<"we have a primery ID= 2, Hit #" << num_Primary_2_Hit << std::endl;
               */

              //If we are the first hit of this primary (naive primary)
              if (num_Primary_2_Hit == 1)
              {
                //What hit number is this for this event? Store it! It is of interest
                Primary_2_Hit_Num = i_hits;

                //debuging
                /*
                 std::cout << "Primary_2_Hit_Num: " << Primary_2_Hit_Num
                 << std::endl;
                 */
              }  //End of if first hit primary 2
            }  //end of if it's primary 2
          }  // End of if ParentID == 0
        }  //end of loops over Dummy detector hits

      }  //end of if Dummy_Det hit size > 0
    }  // end of if we have a hit in the Dummy_Det

    //Fill histograms for primary 1
    if (Primary_1_Hit_Num != -1000)
    {

      h_RADIUS_VS_THETA_CM_DUMMY[1]->Fill(
          Primary->fThetaCenterOfMass1 / ANGLE_UNIT,
          Dummy_Det->fHits[Primary_1_Hit_Num].fPosition.Perp() / LENGTH_UNIT);
    }

    //Fill histograms for primary 2
    if (Primary_2_Hit_Num != -1000)
    {

      h_RADIUS_VS_THETA_CM_DUMMY[2]->Fill(
          Primary->fThetaCenterOfMass2 / ANGLE_UNIT,
          Dummy_Det->fHits[Primary_2_Hit_Num].fPosition.Perp() / LENGTH_UNIT);
    }

    //FIll histograms for coincidence
    if (Primary_1_Hit_Num != -1000 && Primary_2_Hit_Num != -1000)
    {
      //add the primary 1 information
      h_RADIUS_VS_THETA_CM_DUMMY[0]->Fill(
          Primary->fThetaCenterOfMass1 / ANGLE_UNIT,
          Dummy_Det->fHits[Primary_1_Hit_Num].fPosition.Perp() / LENGTH_UNIT);

      //Add the primary 2 information
      h_RADIUS_VS_THETA_CM_DUMMY[0]->Fill(
          Primary->fThetaCenterOfMass2 / ANGLE_UNIT,
          Dummy_Det->fHits[Primary_2_Hit_Num].fPosition.Perp() / LENGTH_UNIT);
    }
  }  // end of looping over entry

  return;
}

/***********************************************************
 Function: Plot_Dummy
 Purpose:
 To plot the graphs and save those plots

 Entry Conditions:
 none

 Global:
 - h_RADIUS_VS_THETA_CM_DUMMY -
 Radius distribution vs theta CM for naive primary hits for each primary Dummy Det

 - LENGTH_UNIT_NAME - units for angle
 - ANGLE_UNIT_NAME - units for angle
 - NUM_PRIMARIES - Number of the Primaries e-
 - INDEX_TO_NUM_PRIMARIES_TO_NAME - name for each primary count

 Exit Conditions: none
 Called By: Radius_vs_ThetaCM
 Date: 11-07-2016
 Modified:
 *********************************************************/
void Plot_Dummy()
{
  //debugging
  //std::cout << "made it in the make plotting function" << std::endl;

  //define Canvases
  //loop over primaries
  for (int i = 0; i < NUM_PRIMARIES; i++)
  {
    //create canvas for radius_vs_ThetaCM
    TCanvas* c_radius_vs_thetaCM_dummy = new TCanvas(
        "c_radius_vs_thetaCM_dummy",
        Form("Radius vs. #theta_{CM} for naive %s on the Dummy detector",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()),
        650, 650);

    //Draw histograms
    h_RADIUS_VS_THETA_CM_DUMMY[i]->Draw();

    //offset the Y axis titles
    //For MeV plots
    //h_RADIUS_VS_THETA_CM_DUMMY[i]->GetYaxis()->SetTitleOffset(1.2);
    //For GeV Plots
    h_RADIUS_VS_THETA_CM_DUMMY[i]->GetYaxis()->SetTitleOffset(1.4);

    //remove stat box
    gStyle->SetOptStat(0);
    //removes the histogram title in stat box
    //   gStyle->SetOptStat("emr");  //1110);

    c_radius_vs_thetaCM_dummy->Update();

    //Save the plots
    c_radius_vs_thetaCM_dummy->SaveAs(
        Form("%s/Radius_vs_ThetaCM_primary_%d.C", OUTPUT_PREFIX.c_str(), i));
    c_radius_vs_thetaCM_dummy->SaveAs(
        Form("%s/Radius_vs_ThetaCM_primary_%d.png", OUTPUT_PREFIX.c_str(), i));
  }

  return;
}

/*********
 * LEFTRIGHT STUFF
 *********/
//TO BE ADDED WHEN NEEDED
