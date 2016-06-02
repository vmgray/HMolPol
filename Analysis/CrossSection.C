/***********************************************************
 Programmer: Valerie Gray
 Purpose:
 - To take a root file and get the total cross section out of it
 - this is ideal, but not possible right now
 - It will work for BOTH dummy and left right detectors :)

 - Also will write out the initial conditions via the ExtractInitalConditions.h
 file

 Entry Conditions: Root files name, Detector
 Date: 03-24-2016
 Modified: 04-28-2016
 Assisted By: Me!
 *********************************************************/

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
#define ANGLE_UNIT Units->rad
#define CROSS_SECTION_UNIT Units->ubarn
#define SOLID_ANGLE_UNIT Units->sr

std::string ANGLE_UNIT_NAME = "rad";  //units for angle
std::string CROSS_SECTION_UNIT_NAME = "μb";  //units for cross section
std::string DIFF_CROSS_SECTION_UNIT_NAME = "μb/sr";  //units for cross section
std::string SOLID_ANGLE_UNIT_NAME = "sr";  //units for cross section

//define the TChain
TChain* HMOLPOL_CHAIN;

//define histograms
//DUMMY DET
std::vector<TH1D*> h_CROSS_SECTION_CM_DUMMY;  //[primary e]
//Differential cross section in CM frame for each primary Dummy Det

std::vector<TH1D*> h_PHI_CM_DUMMY;  //[primary e]
//Phi center of mass for each primary Dummy Det

std::vector<TH1D*> h_THETA_CM_DUMMY;  //[primary e]
//Theta center of mass for each primary Dummy Det

std::vector<TH1D*> h_CROSS_SECTION_CALC_CM_DUMMY;  //[primary e]
//Differential cross section in CM frame for each primary Dummy Det
//This fill is with 1 weighted by the solid angel tossed over * differental cross section / nentries
//Have to take the integral to get the value

//LEFT RIGHT DET
std::vector<TH1D*> h_CROSS_SECTION_CM_LEFTRIGHT;  //[primary e]
//Differential cross section in CM frame for each primary left Right Det

std::vector<TH1D*> h_PHI_CM_LEFTRIGHT;  //[primary e]
//Phi center of mass for each primary Left right Det

std::vector<TH1D*> h_THETA_CM_LEFTRIGHT;  //[primary e]
//Theta center of mass for each primary Left Right Det

std::vector<TH1D*> h_CROSS_SECTION_CALC_CM_LEFTRIGHT;  //[primary e]
//Differential cross section in CM frame for each primary left Right Det

//Calculate the cross section
std::vector<Double_t> d_CROSS_SECTION_CALC_CM_DUMMY;  //[primary e]
//Differential cross section in CM frame for each primary Dummy Det
//This add up is with differental cross section / nentries * change in phi * change in cos theta
//the sum will be the value

std::vector<Double_t> d_CROSS_SECTION_CALC_CM_LEFTRIGHT;  //[primary e]
//Differential cross section in CM frame for each primary Dummy Det
//This add up is with differental cross section / nentries * change in phi * change in cos theta
//the sum will be the value

static const Int_t NUM_PRIMARIES = 3;  //number of primary electrons + 1

std::string INDEX_TO_NUM_PRIMARIES_TO_NAME[NUM_PRIMARIES] = {
    "coincidence of primaries", "primary 1", "primary 2" };

//string for the root flle name to be shared with 2 functions
static std::string FILENAME;

//string for the where the input file is stored.
/*static std::string INPUT_PREFIX =
 "/home/vmgray/workspace/HMolPol/Simulation_Analysis/commit_06d91f8";*/
static std::string INPUT_PREFIX =
    "/home/vmgray/workspace/HMolPol/Simulation_Analysis/commit_dbaceca";

//string for the where the output will go.
/*static std::string OUTPUT_PREFIX =
 "/home/vmgray/workspace/HMolPol/Simulation_Analysis/commit_06d91f8";*/
static std::string OUTPUT_PREFIX =
    "/home/vmgray/workspace/HMolPol/Simulation_Analysis/commit_dbaceca";

//HMolPol include - Must be here as it needs to know about OUTPUT_PREFIX
#include "ExtractInitalConditions.h"

void CrossSection(std::string filename, std::string DetectorType)
{
  FILENAME = Form("%s/%s", INPUT_PREFIX.c_str(), filename.c_str());

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
  //Loop through tree then Plot Dummy
  if (DetectorType.std::string::compare("Dummy") == 0)
  {
    Define_Histograms_Dummy();
    Loop_Through_Tree_Dummy();
//    Plot_Dummy();
  }
  //loop through tree then Plot Left Right
  else if (DetectorType.std::string::compare("LeftRight") == 0)
  {
    Define_Histograms_LeftRight();
    Loop_Through_Tree_LeftRight();
//    Plot_LeftRight();
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
 Called By: CrossSection
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
 - h_CROSS_SECTION_CM_DUMMY - Differential cross section in CM frame for each primary Dummy detector
 - h_PHI_CM_DUMMY - Phi center of mass for each primary Dummy detector
 - h_THETA_CM_DUMMY - Theta center of mass for each primary Dummy Detector
 - h_CROSS_SECTION_CALC_CM_DUMMY

 - CROSS_SECTION_UNIT_NAME - units for angle
 - ANGLE_UNIT_NAME - units for angle
 - NUM_PRIMARIES - Number of the Primaries e-

 Exit Conditions: none
 Called By: CrossSection
 Date: 03-24-2016
 Modified: 03-28-2016
 *********************************************************/
void Define_Histograms_Dummy()
{
  d_CROSS_SECTION_CALC_CM_DUMMY.resize(NUM_PRIMARIES);
  //resize histograms
  h_CROSS_SECTION_CM_DUMMY.resize(NUM_PRIMARIES);
  h_PHI_CM_DUMMY.resize(NUM_PRIMARIES);
  h_THETA_CM_DUMMY.resize(NUM_PRIMARIES);
  h_CROSS_SECTION_CALC_CM_DUMMY.resize(NUM_PRIMARIES);

  //finish defining historgrams
  for (int i = 0; i < NUM_PRIMARIES; i++)
  {
    //cross section
    h_CROSS_SECTION_CM_DUMMY[i] = new TH1D(
        Form("h_CROSS_SECTION_CM_DUMMY[%d]", i),
        Form("Differential cross section for %s on the Dummy detector",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()),
        160, 80, 120);

    h_CROSS_SECTION_CM_DUMMY[i]->GetXaxis()->SetTitle(
        Form("differential cross section of %s (%s)",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str(),
             DIFF_CROSS_SECTION_UNIT_NAME.c_str()));

    //CM Theta
    //Angles in rad
    h_THETA_CM_DUMMY[i] = new TH1D(
        Form("h_THETA_CM_DUMMY[%d]", i),
        Form("Interaction #theta_{CM} for %s on the Dummy detector",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()),
        70, 1.2, 2.0);

    h_THETA_CM_DUMMY[i]->GetXaxis()->SetTitle(
        Form("Interaction #theta_{CM} of %s (%s)",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str(),
             ANGLE_UNIT_NAME.c_str()));

    //CM phi
    h_PHI_CM_DUMMY[i] = new TH1D(
        Form("h_PHI_CM_DUMMY[%d]", i),
        Form("Interaction #phi_{CM} for %s on the Dummy detector",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()),
        628, 0, 6.28);

    h_PHI_CM_DUMMY[i]->GetXaxis()->SetTitle(
        Form("Interaction #phi_{CM} of %s (%s)",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str(),
             ANGLE_UNIT_NAME.c_str()));

    //Cross section calculation
    h_CROSS_SECTION_CALC_CM_DUMMY[i] =
        new TH1D(
            Form("h_CROSS_SECTION_CALC_CM_DUMMY[%d]", i),
            Form(
                "Calculation for the Total cross section for %s on the Dummy detector",
                INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()),
            8, -2, 2);

    //Sum to initalize to zero
    d_CROSS_SECTION_CALC_CM_DUMMY[i] = 0.0;
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

 - h_CROSS_SECTION_CM_DUMMY - Differential cross section in CM frame for each primary Dummy detector
 - h_PHI_CM_DUMMY - Phi center of mass for each primary Dummy detector
 - h_THETA_CM_DUMMY - Theta center of mass for each primary Dummy Detector
 - h_CROSS_SECTION_CALC_CM_DUMMY
 - d_CROSS_SECTION_CALC_CM_DUMMY

 - CROSS_SECTION_UNIT - units for angle
 - ANGLE_UNIT - units for angle


 Exit Conditions: none
 Called By: CrossSection
 Date: 03-24-2016
 Modified: 04-20-2016
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

    /*
     //debugging
     std::cout << "fDelta_Omega"
     << PrimaryConditions->fDelta_Omega / SOLID_ANGLE_UNIT << std::endl
     << "fDelta_Phi" << PrimaryConditions->fDelta_Phi / ANGLE_UNIT
     << std::endl << "fDelta_CosTheta" << PrimaryConditions->fDelta_CosTheta
     << std::endl;
     */
    //Fill histograms for primary 1
    if (Primary_1_Hit_Num != -1000)
    {
      h_CROSS_SECTION_CM_DUMMY[1]->Fill(
          Primary->fCrossSectionCM / CROSS_SECTION_UNIT);
      h_PHI_CM_DUMMY[1]->Fill(Primary->fPhiCenterOfMass1 / ANGLE_UNIT);
      h_THETA_CM_DUMMY[1]->Fill(Primary->fThetaCenterOfMass1 / ANGLE_UNIT);
      h_CROSS_SECTION_CALC_CM_DUMMY[1]->Fill(
          1,
          (PrimaryConditions->fDelta_Omega / SOLID_ANGLE_UNIT) * (1 / nentries)
          * (Primary->fCrossSectionCM / CROSS_SECTION_UNIT));
      d_CROSS_SECTION_CALC_CM_DUMMY[1] +=
          (PrimaryConditions->fDelta_Phi / ANGLE_UNIT) * (PrimaryConditions->fDelta_CosTheta
              / nentries)
          * (Primary->fCrossSectionCM / CROSS_SECTION_UNIT);
    }
    //fill historgrams for primary 2
    if (Primary_2_Hit_Num != -1000)
    {
      h_CROSS_SECTION_CM_DUMMY[2]->Fill(
          Primary->fCrossSectionCM / CROSS_SECTION_UNIT);
      h_PHI_CM_DUMMY[2]->Fill(Primary->fPhiCenterOfMass2 / ANGLE_UNIT);
      h_THETA_CM_DUMMY[2]->Fill(Primary->fThetaCenterOfMass2 / ANGLE_UNIT);

      h_CROSS_SECTION_CALC_CM_DUMMY[2]->Fill(
          1,
          (PrimaryConditions->fDelta_Omega / nentries) * (Primary->fCrossSectionCM
              / CROSS_SECTION_UNIT));
      d_CROSS_SECTION_CALC_CM_DUMMY[2] +=
          (PrimaryConditions->fDelta_Phi / ANGLE_UNIT) * (PrimaryConditions->fDelta_CosTheta
              / nentries)
          * (Primary->fCrossSectionCM / CROSS_SECTION_UNIT);
    }
    //FIll hitogram for coincidence
    if (Primary_1_Hit_Num != -1000 && Primary_2_Hit_Num != -1000)
    {
      h_CROSS_SECTION_CM_DUMMY[0]->Fill(
          Primary->fCrossSectionCM / CROSS_SECTION_UNIT);
      h_PHI_CM_DUMMY[0]->Fill(Primary->fPhiCenterOfMass / ANGLE_UNIT);
      h_THETA_CM_DUMMY[0]->Fill(Primary->fThetaCenterOfMass / ANGLE_UNIT);

      h_CROSS_SECTION_CALC_CM_DUMMY[0]->Fill(
          1,
          (PrimaryConditions->fDelta_Omega / nentries) * (Primary->fCrossSectionCM
              / CROSS_SECTION_UNIT));

      d_CROSS_SECTION_CALC_CM_DUMMY[0] +=
          (PrimaryConditions->fDelta_Phi / ANGLE_UNIT) * (PrimaryConditions->fDelta_CosTheta
              / nentries)
          * (Primary->fCrossSectionCM / CROSS_SECTION_UNIT);

    }
  }  //end of loop over the entries

  /*********
   * print out the info
   * print to file
   *********/
  //this file and everything related to it is fout
  std::ofstream fout;

  //get the output file name.
  std::string output_filename = OUTPUT_PREFIX
      + std::string("/TotalCrossSection_Dummy.txt");

  //open a file
  fout.open(output_filename.c_str());

  if (!fout.is_open())
    std::cout << "File not opened" << std::endl;

  for (int i = 0; i < NUM_PRIMARIES; i++)
  {
    /*
     //debugging
     std::cout << INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str() << ": "
     << h_CROSS_SECTION_CALC_CM_DUMMY[i]->Integral()
     << " " << DIFF_CROSS_SECTION_UNIT_NAME.c_str() << std::endl;
     */

    /*
     fout << INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str() << " : "
     << h_CROSS_SECTION_CALC_CM_DUMMY[i]->Integral() << " "
     << CROSS_SECTION_UNIT_NAME.c_str() << std::endl;
     */

    fout << INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str() << " Sum: "
         << d_CROSS_SECTION_CALC_CM_DUMMY[i] << " "
         << CROSS_SECTION_UNIT_NAME.c_str() << std::endl;

    fout << std::endl << "-----------------" << std::endl << std::endl;
  }

  return;
}

/***********************************************************
 Function: Plot_Dummy
 Purpose:
 To plot the graphs and save those plots

 Entry Conditions:
 none

 Global:
 - h_CROSS_SECTION_CM_DUMMY - Differential cross section in CM frame for each primary Dummy detector
 - h_PHI_CM_DUMMY - Phi center of mass for each primary Dummy detector
 - h_THETA_CM_DUMMY - Theta center of mass for each primary Dummy Detector

 - NUM_PRIMARIES - Number of Primaries

 Exit Conditions: none
 Called By: CrossSection
 Date: 03-24-2016
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
    //cross section
    TCanvas* c_Cross_Section_CM = new TCanvas(
        "c_Cross_Section_CM",
        Form("Differntal cross section for primary %d on the Dummy detector",
             i));

    h_CROSS_SECTION_CM_DUMMY[i]->Draw();

    c_Cross_Section_CM->SaveAs(
        Form("%s/Dummy_Differental_Cross_Section_CM_primary_%d.png",
             OUTPUT_PREFIX.c_str(), i));
    c_Cross_Section_CM->SaveAs(
        Form("%s/Dummy_Differental_Cross_Section_CM_primary_%d.C",
             OUTPUT_PREFIX.c_str(), i));

    //theta
    TCanvas* c_Theta_CM = new TCanvas(
        "c_Theta_CM",
        Form("Interaction #theta CM for primary %d on the Dummy detector", i));

    h_THETA_CM_DUMMY[i]->Draw();

    c_Theta_CM->SaveAs(
        Form("%s/Dummy_Interaction_Theta_CM_primary_%d.png",
             OUTPUT_PREFIX.c_str(), i));
    c_Theta_CM->SaveAs(
        Form("%s/Dummy_Interaction_Theta_CM_primary_%d.C",
             OUTPUT_PREFIX.c_str(), i));
    //phi
    TCanvas* c_Phi_CM = new TCanvas(
        "c_Phi_CM",
        Form("Interaction #phi CM for primary %d on the Dummy detector", i));

    h_PHI_CM_DUMMY[i]->Draw();

    c_Phi_CM->SaveAs(
        Form("%s/Dummy_Interaction_Phi_CM_primary_%d.png",
             OUTPUT_PREFIX.c_str(), i));
    c_Phi_CM->SaveAs(
        Form("%s/Dummy_Interaction_Phi_CM_primary_%d.C",
             OUTPUT_PREFIX.c_str(), i));
  }

  return;
}

/*********
 * LEFT RIGHT STUFF
 *********/

/***********************************************************
 Function: Define_Histograms_LeftRight
 Purpose:
 To define all the histograms,
 ie. name, axis labels

 Entry Conditions:
 none

 Global:
 - h_CROSS_SECTION_CM_LEFTRIGHT - Differential cross section in CM frame for each primary Left RIght detector
 - h_PHI_CM_LEFTRIGHT - Phi center of mass for each primary Left Right detector
 - h_THETA_CM_LEFTRIGHT - Theta center of mass for each primary Left Right Detector
 - h_CROSS_SECTION_CALC_CM_LEFTRIGHT

 - CROSS_SECTION_UNIT_NAME - units for angle
 - ANGLE_UNIT_NAME - units for angle
 - NUM_PRIMARIES - Number of the Primaries e-

 Exit Conditions: none
 Called By: CrossSection
 Date: 03-24-2016
 Modified: 03-28-2016
 *********************************************************/
void Define_Histograms_LeftRight()
{
  //Integral sum size it
  d_CROSS_SECTION_CALC_CM_LEFTRIGHT.resize(NUM_PRIMARIES);

  //resize histograms
  h_CROSS_SECTION_CM_LEFTRIGHT.resize(NUM_PRIMARIES);
  h_PHI_CM_LEFTRIGHT.resize(NUM_PRIMARIES);
  h_THETA_CM_LEFTRIGHT.resize(NUM_PRIMARIES);
  h_CROSS_SECTION_CALC_CM_LEFTRIGHT.resize(NUM_PRIMARIES);

  //finish defining historgrams
  for (int i = 0; i < NUM_PRIMARIES; i++)
  {
    //cross section
    h_CROSS_SECTION_CM_LEFTRIGHT[i] = new TH1D(
        Form("h_CROSS_SECTION_CM_LEFTRIGHT[%d]", i),
        Form("Differential cross section for %s on the Left/Right detector",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()),
        160, 80, 120);

    h_CROSS_SECTION_CM_LEFTRIGHT[i]->GetXaxis()->SetTitle(
        Form("Differential cross section of primary %s (%s)",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str(),
             DIFF_CROSS_SECTION_UNIT_NAME.c_str()));

    //CM Theta
    h_THETA_CM_LEFTRIGHT[i] = new TH1D(
        Form("h_THETA_CM_LEFTRIGHT[%d]", i),
        Form("Interaction #theta_{CM} for %s on the Left/Right detector",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()),
        360, 0, 180);

    h_THETA_CM_LEFTRIGHT[i]->GetXaxis()->SetTitle(
        Form("Interaction #theta_{CM} of %s (%s)",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str(),
             ANGLE_UNIT_NAME.c_str()));

    //CM phi
    h_PHI_CM_LEFTRIGHT[i] = new TH1D(
        Form("h_PHI_CM_LEFTRIGHT[%d]", i),
        Form("interaction #phi_{CM} for %s on the Left/Right detector",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()),
        360, 0, 360);

    h_PHI_CM_LEFTRIGHT[i]->GetXaxis()->SetTitle(
        Form("Interaction #phi_{CM} of primary %s (%s)",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str(),
             ANGLE_UNIT_NAME.c_str()));

    //Cross section calculation
    h_CROSS_SECTION_CALC_CM_LEFTRIGHT[i] =
        new TH1D(
            Form("h_CROSS_SECTION_CALC_CM_LEFTRIGHT[%d]", i),
            Form(
                "Calculation for the Total cross section for %s on the Left/Right detector",
                INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()),
            8, -2, 2);

    h_CROSS_SECTION_CM_LEFTRIGHT[i]->GetXaxis()->SetTitle(
        Form("Total cross section of %s (%s)",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str(),
             CROSS_SECTION_UNIT_NAME.c_str()));

    //Set integral sum
    d_CROSS_SECTION_CALC_CM_LEFTRIGHT[i] = 0.0;
  }

  return;
}

/***********************************************************
 Function: Loop_Through_Tree_LeftRight
 Purpose:
 To loop over the event and fill in the values for the
 histograms

 Also will write out the initial conditions via the ExtractInitalConditions.h
 file

 Entry Conditions:
 none

 Global:
 - TChain - The TChain

 - h_CROSS_SECTION_CM_LEFTRIGHT - Differential cross section in CM frame for each primary Left RIght detector
 - h_PHI_CM_LEFTRIGHT - Phi center of mass for each primary Left Right detector
 - h_THETA_CM_LEFTRIGHT - Theta center of mass for each primary Left Right Detector
 - h_CROSS_SECTION_CALC_CM_LEFTRIGHT

 - CROSS_SECTION_UNIT - units for angle
 - ANGLE_UNIT - units for angle
 - DIFF_CROSS_SECTION_UNIT_NAME


 Exit Conditions: none
 Called By: CrossSection
 Date: 03-28-2016
 Modified: 04-28-2016
 *********************************************************/
void Loop_Through_Tree_LeftRight()
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
  HMolPolEventGenericDetector* Detector_Det = 0;
  HMOLPOL_CHAIN->SetBranchAddress("fDetectorDet", &Detector_Det);

  //loop over all the events in the tree
  for (int i_entry = 0; i_entry < nentries; i_entry++)
  {
    //get the first entry in the tree
    HMOLPOL_CHAIN->GetEntry(i_entry);

    //Get the initial conditions and print to file
    if (i_entry == 1)
      ExtractInitalConditions(Units, PrimaryConditions);

    /*
     //Count for the number of hits in the left/right detector
     Int_t num_Left_det_Hit = 0;
     Int_t num_Right_det_Hit = 0;
     Int_t Left_Det_Hit_Num = -1000;
     Int_t Right_Det_Hit_Num = -1000;
     */
    /*
     //Not using
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
          //If we have a primary electron ie. it has no parent
          if (Detector_Det->fHits[i_hits].fParentID == 0)
          {
            //What primary e- is this? The first?
            if (Detector_Det->fHits[i_hits].fPrimaryID == 1)
            {
              //increase the number of hit by one
              num_Primary_1_Hit++;

              /*
               //debug
               std::cout <<"we have a primery ID= 1, Hit #" << num_Primary_1_Hit << std::endl;
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
            }  //end of primaryID ==1
               //What primary e- is this? The second?
            if (Detector_Det->fHits[i_hits].fPrimaryID == 2)
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

          }  //end of if this hit as parentID == 0

        }  //end of loops over the hits in this event
      }  //End of if there is more than 1 hit in this event
    }  //end of if there is a hit in the detector

    //Fill histograms for primary 1
    if (Primary_1_Hit_Num != -1000)
    {
      h_CROSS_SECTION_CM_LEFTRIGHT[1]->Fill(
          Primary->fCrossSectionCM / CROSS_SECTION_UNIT);
      h_PHI_CM_LEFTRIGHT[1]->Fill(Primary->fPhiCenterOfMass1 / ANGLE_UNIT);
      h_THETA_CM_LEFTRIGHT[1]->Fill(Primary->fThetaCenterOfMass1 / ANGLE_UNIT);
      h_CROSS_SECTION_CALC_CM_LEFTRIGHT[1]->Fill(
          (PrimaryConditions->fDelta_Omega / nentries) * (Primary->fCrossSectionCM
              / CROSS_SECTION_UNIT));

      //Integral sum
      d_CROSS_SECTION_CALC_CM_LEFTRIGHT[1] =
          +(PrimaryConditions->fDelta_Phi / ANGLE_UNIT) * (PrimaryConditions->fDelta_CosTheta
              / nentries)
          * (Primary->fCrossSectionCM / CROSS_SECTION_UNIT);
    }
    //fill historgrams for primary 2
    if (Primary_2_Hit_Num != -1000)
    {
      h_CROSS_SECTION_CM_LEFTRIGHT[2]->Fill(
          Primary->fCrossSectionCM / CROSS_SECTION_UNIT);
      h_PHI_CM_LEFTRIGHT[2]->Fill(Primary->fPhiCenterOfMass2 / ANGLE_UNIT);
      h_THETA_CM_LEFTRIGHT[2]->Fill(Primary->fThetaCenterOfMass2 / ANGLE_UNIT);
      h_CROSS_SECTION_CALC_CM_LEFTRIGHT[2]->Fill(
          1,
          (PrimaryConditions->fDelta_Omega / nentries) * (Primary->fCrossSectionCM
              / CROSS_SECTION_UNIT));

      //Integral sum
      d_CROSS_SECTION_CALC_CM_LEFTRIGHT[2] =
          +(PrimaryConditions->fDelta_Phi / ANGLE_UNIT) * (PrimaryConditions->fDelta_CosTheta
              / nentries)
          * (Primary->fCrossSectionCM / CROSS_SECTION_UNIT);
    }
    //FIll hitogram for coincidence
    if (Primary_1_Hit_Num != -1000 && Primary_2_Hit_Num != -1000)
    {
      h_CROSS_SECTION_CM_LEFTRIGHT[0]->Fill(
          Primary->fCrossSectionCM / CROSS_SECTION_UNIT);
      h_PHI_CM_LEFTRIGHT[0]->Fill(Primary->fPhiCenterOfMass / ANGLE_UNIT);
      h_THETA_CM_LEFTRIGHT[0]->Fill(Primary->fThetaCenterOfMass / ANGLE_UNIT);
      h_CROSS_SECTION_CALC_CM_LEFTRIGHT[0]->Fill(
          (PrimaryConditions->fDelta_Omega / nentries) * (Primary->fCrossSectionCM
              / CROSS_SECTION_UNIT));
      //Integral sum
      d_CROSS_SECTION_CALC_CM_LEFTRIGHT[0] =
          +(PrimaryConditions->fDelta_Phi / ANGLE_UNIT) * (PrimaryConditions->fDelta_CosTheta
              / nentries)
          * (Primary->fCrossSectionCM / CROSS_SECTION_UNIT);
    }
  }  //end of loop over entries

  /*********
   * print out the info
   * print to file
   *********/
  //this file and everything related to it is fout
  std::ofstream fout;

  //get the output file name.
  std::string output_filename = OUTPUT_PREFIX
      + std::string("/TotalCrossSection_LeftRight.txt");

  //open a file
  fout.open(output_filename.c_str());

  if (!fout.is_open())
    std::cout << "File not opened" << std::endl;

  for (int i = 0; i < NUM_PRIMARIES; i++)
  {

    /*
     fout << INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str() << " : "
     << h_CROSS_SECTION_CALC_CM_LEFTRIGHT[i]->Integral() << " "
     << CROSS_SECTION_UNIT_NAME.c_str() << std::endl;
     */

    fout << INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str() << " Sum: "
         << d_CROSS_SECTION_CALC_CM_LEFTRIGHT[i] << " "
         << CROSS_SECTION_UNIT_NAME.c_str() << std::endl;

    fout << std::endl << "-----------------" << std::endl << std::endl;
  }

  return;
}

/***********************************************************
 Function: Plot_LeftRight
 Purpose:
 To plot the graphs and save those plots

 Entry Conditions:
 none

 Global:
 - h_CROSS_SECTION_CM_LEFTRIGHT - Differential cross section in CM frame for each primary Left RIght detector
 - h_PHI_CM_LEFTRIGHT - Phi center of mass for each primary Left Right detector
 - h_THETA_CM_LEFTRIGHT - Theta center of mass for each primary Left Right Detector

 - NUM_PRIMARIES - Number of Primaries

 Exit Conditions: none
 Called By: CrossSection
 Date: 03-24-2016
 Modified:
 *********************************************************/
void Plot_LeftRight()
{
  //debugging
  //std::cout << "made it in the make plotting function" << std::endl;

  //define Canvases
  //loop over primaries
  for (int i = 0; i < NUM_PRIMARIES; i++)
  {
    //cross section
    TCanvas* c_Cross_Section_CM =
        new TCanvas(
            "c_Cross_Section_CM",
            Form(
                "Differntal cross section for primary %d on the Left Right detector",
                i));

    h_CROSS_SECTION_CM_LEFTRIGHT[i]->Draw();

    c_Cross_Section_CM->SaveAs(
        Form("%s/Left_Right_Differental_Cross_Section_CM_primary_%d.png",
             OUTPUT_PREFIX.c_str(), i));
    c_Cross_Section_CM->SaveAs(
        Form("%s/Left_Right_Differental_Cross_Section_CM_primary_%d.C",
             OUTPUT_PREFIX.c_str(), i));

    //theta
    TCanvas* c_Theta_CM = new TCanvas(
        "c_Theta_CM",
        Form("Interaction #theta CM for primary %d on the Left Right detector",
             i));

    h_THETA_CM_LEFTRIGHT[i]->Draw();

    c_Theta_CM->SaveAs(
        Form("%s/Left_Right_Interaction_Theta_CM_primary_%d.png",
             OUTPUT_PREFIX.c_str(), i));
    c_Theta_CM->SaveAs(
        Form("%s/Left_Right_Interaction_Theta_CM_primary_%d.C",
             OUTPUT_PREFIX.c_str(), i));

    //phi
    TCanvas* c_Phi_CM = new TCanvas(
        "c_Phi_CM",
        Form("Interaction #phi CM for primary %d on the Left Right detector",
             i));

    h_PHI_CM_LEFTRIGHT[i]->Draw();

    c_Phi_CM->SaveAs(
        Form("%s/Left_Right_Interaction_Phi_CM_primary_%d.png",
             OUTPUT_PREFIX.c_str(), i));
    c_Phi_CM->SaveAs(
        Form("%s/Left_Right_Interaction_Phi_CM_primary_%d.C",
             OUTPUT_PREFIX.c_str(), i));
  }

  return;
}
