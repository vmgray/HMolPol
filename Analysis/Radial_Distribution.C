//Programmer: Connor Simpson

//Root includes
#include <TSystem.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TH1D.h>
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
//define units used
#define ANGLE_UNIT Units-> rad
#define RADIUS_UNIT Units-> m

std::string ANGLE_UNIT_NAME = "rad";
std::string RADIUS_UNIT_NAME = "m";

//define the TChain
TChain* HMOLPOL_CHAIN;

//define histograms
std::vector<TH1D*> h_PHI_0_TO_PI6;
std::vector<TH1D*> h_PHI_PI6_TO_PI3;
std::vector<TH1D*> h_PHI_PI3_TO_PI2;

static const Int_t NUM_PRIMARIES = 3;  //number of primary electrons + 1

std::string INDEX_TO_NUM_PRIMARIES_TO_NAME[NUM_PRIMARIES] = {
    "coincidence of primaries", "primary 1", "primary 2" };

//string for the root file name to be shared with 2 functions
static std::string FILENAME;

//string for the where the input file is stored.
/*static std::string INPUT_PREFIX =
 "/home/vmgray/workspace/HMolPol/Simulation_Analysis/commit_06d91f8";*/
static std::string INPUT_PREFIX =
    "/home/csimpson/workspace/HMolPol/data_Simulation_Analysis/commit_dbaceca/BeamE_2GeV/Phi-5to5/Rootfiles";

//string for the where the output will go.
/*static std::string OUTPUT_PREFIX =
 "/home/vmgray/workspace/HMolPol/Simulation_Analysis/commit_06d91f8";*/
static std::string OUTPUT_PREFIX =
    "/home/csimpson/workspace/HMolPol/Simulation_Analysis/commit_dbaceca/BeamE_2GeV/Phi-5to5/Radial_Distribution";

//HMolPol include - Must be here as it needs to know about OUTPUT_PREFIX
#include "ExtractInitalConditions.h"

void Radial_Distribution(std::string filename, std::string DetectorType)
{
  FILENAME = Form("%s/%s", INPUT_PREFIX.c_str(), filename.c_str());

  //Get the Git info for the simulation
  GetGitInfo();

  //Grab the file(s)
  HMOLPOL_CHAIN = new TChain("HMolPol_Tree");
  //add the file

  HMOLPOL_CHAIN->Add(
      "~/workspace/HMolPol/data_Simulation_Analysis/commit_dbaceca/BeamE_2GeV/Phi-5to5/Rootfiles/"
      "HMolPol_Dummy_G4Pb_2GeV_Mag8T_NoRaster_75Theta105_-5Phi5_dbaceac_*.root");

  //HMOLPOL_CHAIN->Add(Form("%s", FILENAME.c_str()));
  //HMolPol_Dummy_G4Pb_2GeV_Mag8T_NoRaster_75Theta105_0Phi360_dbaceac_0.root

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
    std::cout << "Detector not implemented.  Leave a message after the beep."
    << std::endl;
    //    Define_Histograms_LeftRight();
    //    Loop_Through_Tree_LeftRight();
    //    Plot_LeftRight();
  } else
    std::cout << "Hey you! Yes, you. You have chosen a wrong detector, Try again! Options are Dummy or LeftRight"
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
 Called By: Momentum
 Date: 03-24-2016
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
 - h_PHI_0_TO_PI6 - cuts from 0 to pi/6
 - h_PHI_PI6_TO_PI3 - cuts from pi/6 to pi/3
 - h_PHI_PI3_TO_PI2 - cuts from pi/3 to pi/2

 - ANGLE_UNIT_NAME - the units of angle
 - RADIUS_UNIT_NAME - units of displacement for radius

 Exit Conditions: none
 Called By: Radial_Distribution
 Date: 06-21-2016
 Modified: 06-21-2016
 *********************************************************/

void Define_Histograms_Dummy()
{
  //resize histograms
  h_PHI_0_TO_PI6.resize(NUM_PRIMARIES);
  h_PHI_PI6_TO_PI3.resize(NUM_PRIMARIES);
  h_PHI_PI3_TO_PI2.resize(NUM_PRIMARIES);
  //finish defining histograms
  for (int i = 0; i < NUM_PRIMARIES; i++)
  {
    h_PHI_0_TO_PI6[i] = new TH1D(
        Form("h_PHI_0_TO_PI6[%d]", i),
        Form("Radial frequency 0 to pi/6 for %s on the dummy detector",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()),
        600, 0, .5);

    h_PHI_0_TO_PI6[i]->GetXaxis()->SetTitle(
        Form("Radial hit count 0 to pi/6 for %s (%s)",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str(),
             RADIUS_UNIT_NAME.c_str()));

    h_PHI_PI6_TO_PI3[i] = new TH1D(
        Form("h_PHI_PI6_TO_PI3[%d]", i),
        Form("Radial frequency pi/6 to pi/3 for %s on the dummy detector",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()),
        600, 0, .5);
    h_PHI_PI6_TO_PI3[i]->GetXaxis()->SetTitle(
        Form("Radial hit count pi/6 to pi/3 for %s (%s)",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str(),
             RADIUS_UNIT_NAME.c_str()));

    h_PHI_PI3_TO_PI2[i] = new TH1D(
        Form("h_PHI_PI3_TO_PI2[%d]", i),
        Form("Radial frequency pi/3 to pi/2 for %s on the dummy detector",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()),
        600, 0, .5);
    h_PHI_PI3_TO_PI2[i]->GetXaxis()->SetTitle(
        Form("Radial hit count pi/3 to pi/2 for %s (%s)",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str(),
             RADIUS_UNIT_NAME.c_str()));
  }
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

 - h_PHI_0_TO_PI6 - cuts from 0 to pi/6
 - h_PHI_PI6_TO_PI3 - cuts from pi/6 to pi/3
 - h_PHI_PI3_TO_PI2 - cuts from pi/3 to pi/2

 - ANGLE_UNIT_NAME - the units of angle
 - RADIUS_UNIT_NAME - units of displacement for radius


 Exit Conditions: none
 Called By: Momentum
 Date: 06-20-2016
 Modified: 06-20-2016
 *********************************************************/
void Loop_Through_Tree_Dummy()
{
  //get the number of entries in the tree
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

    /*
     //Get the interaction theta value for each primary electron
     Double_t theta_hit_Primary_1_Val = 0.0;
     Double_t theta_hit_Primary_2_Val = 0.0;

     //Get the interaction phi value for each primary electron
     Double_t phi_hit_Primary_1_Val = 0.0;
     Double_t phi_hit_Primary_2_Val = 0.0;
     */

    //Count for the number of hits in the dummy detector
    Int_t num_Primary_1_Hit = 0;
    Int_t num_Primary_2_Hit = 0;
    Int_t Primary_1_Hit_Num = -1000;
    Int_t Primary_2_Hit_Num = -1000;

    //debugging
    /*
     std::cout << "Left_Det_Hit_Num: " << Left_Det_Hit_Num << std::endl;
     std::cout << "Right_Det_Hit_Num: " << Right_Det_Hit_Num << std::endl;
     */

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
          //phi angle of recorded hit on detector
          double hit_phi =
              atan(
                  Dummy_Det->fHits[i_hits].fPosition.Y() / Dummy_Det->fHits[i_hits].fPosition.X())
              / ANGLE_UNIT;

          //distance from the center of the detector of recorded hit on detector
          double hit_radius =
              sqrt(
                  Dummy_Det->fHits[i_hits].fPosition.Y() * Dummy_Det->fHits[i_hits].fPosition.Y() + Dummy_Det->fHits[i_hits].fPosition.X()
                      * Dummy_Det->fHits[i_hits].fPosition.X())
              / RADIUS_UNIT;
          //debugging
          //std::cout << "In the Hits loop" << std::endl;

          //If we have a primary electron ie. it has no parent
          if (Dummy_Det->fHits[i_hits].fParentID == 0)
          {
            //if the angle the electron hit at is between 0 and pi/6
            if ((hit_phi > 0) and (hit_phi < .52))
            {
              h_PHI_0_TO_PI6[0]->Fill(hit_radius);
            }
            //if the angle the electron hit at is between pi/6 and pi/3
            if ((hit_phi > .52) and (hit_phi < 1.04))
            {
              h_PHI_PI6_TO_PI3[0]->Fill(hit_radius);
            }
            //if the angle the electron hit at is between pi/3 and pi/2
            if ((hit_phi > 1.04) and hit_phi < 1.56)
            {
              h_PHI_PI3_TO_PI2[0]->Fill(hit_radius);
            }
            //What primary e- is this? The first?
            if (Dummy_Det->fHits[i_hits].fPrimaryID == 1)
            {
              //increase the number of hit by one
              num_Primary_1_Hit++;
              //if the angle the electron hit at is between 0 and pi/6
              if ((hit_phi > 0) and (hit_phi < .52))
              {
                h_PHI_0_TO_PI6[1]->Fill(hit_radius);
              }
              //if the angle the electron hit at is between pi/6 and pi/3
              if ((hit_phi > .52) and (hit_phi < 1.04))
              {
                h_PHI_PI6_TO_PI3[1]->Fill(hit_radius);
              }
              //if the angle the electron hit at is between pi/3 and pi/2
              if ((hit_phi > 1.04) and hit_phi < 1.56)
              {
                h_PHI_PI3_TO_PI2[1]->Fill(hit_radius);
              }

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
              //if the angle the electron hit at is between 0 and pi/6
              if ((hit_phi > 0) and (hit_phi < .52))
              {
                h_PHI_0_TO_PI6[2]->Fill(hit_radius);
              }
              //if the angle the electron hit at is between pi/6 and pi/3
              if ((hit_phi > .52) and (hit_phi < 1.04))
              {
                h_PHI_PI6_TO_PI3[2]->Fill(hit_radius);
              }
              //if the angle the electron hit at is between pi/3 and pi/2
              if ((hit_phi > 1.04) and hit_phi < 1.56)
              {
                h_PHI_PI3_TO_PI2[2]->Fill(hit_radius);
              }

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

    /*
     //debugging
     std::cout << "fDelta_Omega"
     << PrimaryConditions->fDelta_Omega / SOLID_ANGLE_UNIT << std::endl
     << "fDelta_Phi" << PrimaryConditions->fDelta_Phi / ANGLE_UNIT
     << std::endl << "fDelta_CosTheta" << PrimaryConditions->fDelta_CosTheta
     << std::endl;
     */
  }  //closes loop over trees/events
  return;
}

/***********************************************************
 Function: Plot_Dummy
 Purpose:
 To plot the graphs and save those plots

 Entry Conditions:
 none

 Global:
 - h_PHI_0_TO_PI6 - cuts from 0 to pi/6
 - h_PHI_PI6_TO_PI3 - cuts from pi/6 to pi/3
 - h_PHI_PI3_TO_PI2 - cuts from pi/3 to pi/2

 - ANGLE_UNIT_NAME - the units of angle
 - RADIUS_UNIT_NAME - units of displacement for radius

 Exit Conditions: none
 Called By: Radial_Distribution
 Date: 06-21-2016
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
    //phi_hit from 0 to pi/6
    TCanvas* c_Radius_Phi_0_to_Pi6 = new TCanvas(
        "c_Radius_Phi_0_to_Pi6_Dummy",
        Form("Radius phi 0 to .52 for primary %d on dummy detector", i));

    h_PHI_0_TO_PI6[i]->Draw();

    c_Radius_Phi_0_to_Pi6->SaveAs(
        Form("%s/Radius_Phi_0_to_.52_Dummy_primary_%d.png",
             OUTPUT_PREFIX.c_str(), i));
    c_Radius_Phi_0_to_Pi6->SaveAs(
        Form("%s/Radius_Phi_0_to_.52_Dummy_primary_%d.C", OUTPUT_PREFIX.c_str(),
             i));
    c_Radius_Phi_0_to_Pi6->SaveAs(
        Form("%s/Radius_Phi_0_to_.52_Dummy_primary_%d.pdf",
             OUTPUT_PREFIX.c_str(), i));

    //phi_hit from pi/6 to pi/3
    TCanvas* c_Radius_Phi_Pi6_to_Pi3 = new TCanvas(
        "c_Radius_Phi_.52_to_1.04_Dummy",
        Form("Radius phi .52 to 1.04 for primary %d on dummy detector", i));

    h_PHI_PI6_TO_PI3[i]->Draw();

    c_Radius_Phi_Pi6_to_Pi3->SaveAs(
        Form("%s/Radius_Phi_.52_to_1.04_Dummy_primary_%d.png",
             OUTPUT_PREFIX.c_str(), i));
    c_Radius_Phi_Pi6_to_Pi3->SaveAs(
        Form("%s/Radius_Phi_.52_to_1.04_Dummy_primary_%d.C",
             OUTPUT_PREFIX.c_str(), i));
    c_Radius_Phi_Pi6_to_Pi3->SaveAs(
        Form("%s/Radius_Phi_.52_to_1.04_Dummy_primary_%d.pdf",
             OUTPUT_PREFIX.c_str(), i));

    //phi_hit from pi/3 to pi/2
    TCanvas* c_Radius_Phi_Pi3_to_Pi2 = new TCanvas(
        "c_Radius_Phi_1.04_to_1.56_Dummy",
        Form("Radius phi 1.04 to 1.56 for primary %d on dummy detector", i));

    h_PHI_PI3_TO_PI2[i]->Draw();

    c_Radius_Phi_Pi3_to_Pi2->SaveAs(
        Form("%s/Radius_Phi_1.04_to_1.56_Dummy_primary_%d.png",
             OUTPUT_PREFIX.c_str(), i));
    c_Radius_Phi_Pi3_to_Pi2->SaveAs(
        Form("%s/Radius_Phi_1.04_to_1.56_Dummy_primary_%d.C",
             OUTPUT_PREFIX.c_str(), i));
    c_Radius_Phi_Pi3_to_Pi2->SaveAs(
        Form("%s/Radius_Phi_1.04_to_1.56_Dummy_primary_%d.pdf",
             OUTPUT_PREFIX.c_str(), i));
  }

}

