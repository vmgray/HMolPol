/*********************************************************
 * Programmer: Connor Simpson
 * Purpose:
 * -Outputs the color plot of hits on the dummy detector
 * -Also outputs the z frequency distribution
 * -ONLY IMPLEMENTED FOR DUMMY DETECTOR SO DONT TRY IT
 *
 * -Writes out initial conditions via ExtractInitialConditions.h
 * file
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

//Globals
//define units used
#define POSITION_UNIT Units->m

std::string POSITION_UNIT_NAME = "m";

//define the TChain
TChain* HMOLPOL_CHAIN;

//define histograms
//Dummy Detector

std::vector<TH2D*> h_XY_DIST_DUMMY;  //[primary e]
//X vs Y position for every primary electron

std::vector<TH1D*> h_Z_DIST_DUMMY;  //[primary e]
//Z position for the initial primary electron

std::vector<TH1D*> h_Z_DIST_ALL_PRIMARIES;  //[primary e]
//Z position for all primary electrons

TH1D* h_Z_ALL_HITS_DUMMY;
//Z position for every electron

TH2D* h_XY_ALL_HITS_DUMMY;
//X vs Y position for every electron

static const Int_t NUM_PRIMARIES = 3;  //number of primary electrons + 1

std::string INDEX_TO_NUM_PRIMARIES_TO_NAME[NUM_PRIMARIES] = {
    "coincidence of primaries", "primary 1", "primary 2" };

//string for the root flle name to be shared with 2 functions
static std::string FILENAME;

//string for the where the input file is stored.
/*static std::string INPUT_PREFIX =
 "/home/vmgray/workspace/HMolPol/Simulation_Analysis/commit_06d91f8";*/
static std::string INPUT_PREFIX =
    "/home/csimpson/workspace/HMolPol/data_Simulation_Analysis/commit_dbaceca/BeamE_2GeV/Phi0to360/Rootfiles";

//string for the where the output will go.
/*static std::string OUTPUT_PREFIX =
 "/home/vmgray/workspace/HMolPol/Simulation_Analysis/commit_06d91f8";*/
static std::string OUTPUT_PREFIX =
    "/home/csimpson/workspace/HMolPol/Simulation_Analysis/commit_dbaceca/BeamE_2GeV/Phi0to360/Hit_XYdist_and_Zdist";

//HMolPol include - Must be here as it needs to know about OUTPUT_PREFIX
#include "ExtractInitalConditions.h"

void XYdist_and_Zdist(std::string filename, std::string DetectorType)
{
  FILENAME = Form("%s/%s", INPUT_PREFIX.c_str(), filename.c_str());

  //Get the Git info for the simulation
  GetGitInfo();

  //Grab the file(s)
  HMOLPOL_CHAIN = new TChain("HMolPol_Tree");
  //add the file

  HMOLPOL_CHAIN->Add(
      "~/workspace/HMolPol/data_Simulation_Analysis/commit_dbaceca/BeamE_2GeV/Phi0to360/Rootfiles/"
      "HMolPol_Dummy_G4Pb_2GeV_Mag8T_NoRaster_75Theta105_0Phi360_dbaceac_*.root");

  //HMOLPOL_CHAIN->Add(Form("%s", FILENAME.c_str()));

  //loop through tree then plot dummy
  if (DetectorType.std::string::compare("Dummy") == 0)
  {
    Define_Histograms_Dummy();
    Loop_Through_Tree_Dummy();
    Plot_Dummy();
  }
  //loop through tree then plot left right (well, not yet)
  else if (DetectorType.std::string::compare("LeftRight") == 0)
  {
    std::cout << "That's not implemented yet. Leave a message at the beep."
    << std::endl;
  } else
    std::cout << "Hey, you!  Yes, you.  You have chosen the wrong detector.  Try again.  Options are Dummy or "
    "LeftRight."
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
 Date: 06-10-2016
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

/*************
 * DUMMY STUFF
 *************/

/***********************************************************
 Function: Define_Histograms_Dummy
 Purpose:
 To define all the histograms,
 ie. name, axis labels

 Entry Conditions:
 none

 Global:
 - h_XY_DIST_DUMMY - X vs Y distribution for primary hits on the Dummy Detector
 - h_Z_DIST_DUMMY - Z frequency distribution for primary hits in the Dummy Detector
 - h_Z_ALL_HITS_DUMMY - Z distribution for every hit in the Dummy Detector
 - h_XY_ALL_HITS_DUMMY - X vs Y distribution for all hits on the Dummy Detector
 - h_Z_DIST_ALL_PRIMARIES - Z distribution for every primary hit in the Dummy Detector

 - POSITION_UNIT_NAME - units for position
 - NUM_PRIMARIES - Number of the Primaries e-

 Exit Conditions: none
 Called By: Hit_XYdist_and_Zdist
 Date: 06-10-2016
 *********************************************************/
void Define_Histograms_Dummy()
{
  h_XY_DIST_DUMMY.resize(NUM_PRIMARIES);
  h_Z_DIST_DUMMY.resize(NUM_PRIMARIES);
  h_Z_DIST_ALL_PRIMARIES.resize(NUM_PRIMARIES);

  //finish defining histograms
  for (int i = 0; i < NUM_PRIMARIES; i++)
  {
    //x vs y
    //positions in meters
    h_XY_DIST_DUMMY[i] = new TH2D(
        Form("h_XY_DIST_DUMMY[%d]", i),
        Form("Hit X vs Y for %s on the Dummy Detector",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()),
        680, -.5, .5, 680, -.5, .5);

    h_XY_DIST_DUMMY[i]->GetXaxis()->SetTitle(
        Form("Hit X vs Y of %s (%s)", INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str(),
             POSITION_UNIT_NAME.c_str()));

    //z
    h_Z_DIST_DUMMY[i] = new TH1D(
        Form("h_Z_DIST_DUMMY[%d]", i),
        Form("Hit Z frequency for %s in the Dummy Detector",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()),
        600, 7, 7.5);

    h_Z_DIST_DUMMY[i]->GetXaxis()->SetTitle(
        Form("Hit Z frequency of %s (%s)",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str(),
             POSITION_UNIT_NAME.c_str()));

    h_Z_DIST_ALL_PRIMARIES[i] = new TH1D(
        Form("h_Z_DIST_ALL_PRIMARIES[%d]", i),
        Form("Hit Z frequency for all %s in the Dummy Detector",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()),
        600, 7.2, 7.7);

  }

  //all z
  h_Z_ALL_HITS_DUMMY = new TH1D("h_Z_ALL_HITS_DUMMY",
                                "Hit Z distribution for all hits in detector",
                                600, 7, 7.5);

  //all x and y
  h_XY_ALL_HITS_DUMMY = new TH2D("h_XY_ALL_HITS_DUMMY",
                                 "Hit XY distribution for all hits in detector",
                                 400, -.5, .5, 400, -.5, .5);

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

 - h_XY_DIST_DUMMY - X vs Y distribution for primary hits on the Dummy Detector
 - h_Z_DIST_DUMMY - Z frequency distribution for hits in the Dummy Detector
 - h_Z_ALL_HITS_DUMMY - Z distribution for every hit in the Dummy Detector
 - h_ZY_ALL_HITS_DUMMY - X vs Y distribution for all hits in the Dummy Detector
 - h_Z_DIST_ALL_PRIMARIES - Z distribution for every primary hit in the Dummy Detector

 - POSITION_UNIT_NAME - units for position
 - NUM_PRIMARIES - Number of the Primaries e-


 Exit Conditions: none
 Called By: Hit_XYdist_and_Zdist
 Date: 06-10-2016
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

          //Fill histograms for all cases

          //Fill Z Distribution
          h_Z_ALL_HITS_DUMMY->Fill(
              Dummy_Det->fHits[i_hits].fPosition.Z() / POSITION_UNIT);

          //Fill XY Distribution
          h_XY_ALL_HITS_DUMMY->Fill(
              Dummy_Det->fHits[i_hits].fPosition.X() / POSITION_UNIT,
              Dummy_Det->fHits[i_hits].fPosition.Y() / POSITION_UNIT);

          //If we have a primary electron ie. it has no parent
          if (Dummy_Det->fHits[i_hits].fParentID == 0)
          {
            h_Z_DIST_ALL_PRIMARIES[0]->Fill(
                Dummy_Det->fHits[i_hits].fPosition.Z() / POSITION_UNIT);

            //What primary e- is this? The first?
            if (Dummy_Det->fHits[i_hits].fPrimaryID == 1)
            {
              //increase the number of hit by one
              num_Primary_1_Hit++;

              h_Z_DIST_ALL_PRIMARIES[1]->Fill(
                  Dummy_Det->fHits[i_hits].fPosition.Z() / POSITION_UNIT);

              /*
               //debug
               std::cout <<"we have a primery ID= 2, Hit #" << num_Primary_2_Hit << std::endl;
               */

              //If we are the first hit of this primary (naive primary)
              if (num_Primary_1_Hit == 1)
              {
                //What hit number is this for this event? Store it! It is of interest
                Primary_1_Hit_Num = i_hits;

                //debugging
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

              h_Z_DIST_ALL_PRIMARIES[2]->Fill(
                  Dummy_Det->fHits[i_hits].fPosition.Z() / POSITION_UNIT);

              /*
               //debug
               std::cout <<"we have a primary ID= 2, Hit #" << num_Primary_2_Hit << std::endl;
               */

              //If we are the first hit of this primary (naive primary)
              if (num_Primary_2_Hit == 1)
              {
                //What hit number is this for this event? Store it! It is of interest
                Primary_2_Hit_Num = i_hits;

                //debugging
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

    //Fill histograms for Primary 1
    if (Primary_1_Hit_Num != -1000)
    {
      //Fill XY Plot
      h_XY_DIST_DUMMY[1]->Fill(
          Dummy_Det->fHits[Primary_1_Hit_Num].fPosition.X() / POSITION_UNIT,
          Dummy_Det->fHits[Primary_1_Hit_Num].fPosition.Y() / POSITION_UNIT);

      //Fill Z Distribution
      h_Z_DIST_DUMMY[1]->Fill(
          Dummy_Det->fHits[Primary_1_Hit_Num].fPosition.Z() / POSITION_UNIT);
    }

    //Fill histograms for Primary 2
    if (Primary_2_Hit_Num != -1000)
    {
      //Fill XY Plot
      h_XY_DIST_DUMMY[2]->Fill(
          Dummy_Det->fHits[Primary_2_Hit_Num].fPosition.X() / POSITION_UNIT,
          Dummy_Det->fHits[Primary_2_Hit_Num].fPosition.Y() / POSITION_UNIT);

      //Fill Z Distribution
      h_Z_DIST_DUMMY[2]->Fill(
          Dummy_Det->fHits[Primary_2_Hit_Num].fPosition.Z() / POSITION_UNIT);
    }

    //Fill histograms for coincidence
    if (Primary_1_Hit_Num != -1000 && Primary_2_Hit_Num != -1000)
    {
      //Fill XY Plot for Primary 1
      h_XY_DIST_DUMMY[0]->Fill(
          Dummy_Det->fHits[Primary_1_Hit_Num].fPosition.X() / POSITION_UNIT,
          Dummy_Det->fHits[Primary_1_Hit_Num].fPosition.Y() / POSITION_UNIT);

      //Fill XY Plot for Primary 2
      h_XY_DIST_DUMMY[0]->Fill(
          Dummy_Det->fHits[Primary_2_Hit_Num].fPosition.X() / POSITION_UNIT,
          Dummy_Det->fHits[Primary_2_Hit_Num].fPosition.Y() / POSITION_UNIT);

      //Fill Z Distribution for Primary 1
//      h_Z_DIST_DUMMY[0]->Fill(
//          Dummy_Det->fHits[Primary_1_Hit_Num].fPosition.Z() / POSITION_UNIT);

      //Fill Z Distribution Primary 2
      h_Z_DIST_DUMMY[0]->Fill(
          Dummy_Det->fHits[Primary_2_Hit_Num].fPosition.Z() / POSITION_UNIT);
    }

  }  //closes loop over tree/events
  return;
}

/***********************************************************
 Function: Plot_Dummy
 Purpose:
 To plot the graphs and save those plots

 Entry Conditions:
 none

 Global:
 - h_XY_DIST_DUMMY - X vs Y distribution for primary hits on the Dummy Detector
 - h_Z_DIST_DUMMY - Z frequency distribution for primary hits in the Dummy Detector
 - h_Z_ALL_HITS_DUMMY - Z distribution for every hit in the Dummy Detector
 - h_XY_ALL_HITS_DUMMY - X vs Y distribution for all hits in the Dummy Detector
 - h_Z_DIST_ALL_PRIMARIES - Z distribution for every primary hit in the Dummy Detector

 - NUM_PRIMARIES - Number of Primaries

 Exit Conditions: none
 Called By: CrossSection
 Date: 06-12-2016
 Modified: 6-14-2016
 *********************************************************/
void Plot_Dummy()
{
  for (int i = 0; i < NUM_PRIMARIES; i++)
  {
    //x vs y
    TCanvas* c_XY_Dist = new TCanvas(
        "c_XY_Dist", Form("XY Distribution for primary %d on the detector", i));

    h_XY_DIST_DUMMY[i]->Draw("colz");

    c_XY_Dist->SaveAs(
        Form("%s/Dummy_XY_Distribution_primary_%d.png", OUTPUT_PREFIX.c_str(),
             i));
    c_XY_Dist->SaveAs(
        Form("%s/Dummy_XY_Distribution_primary_%d.C", OUTPUT_PREFIX.c_str(),
             i));


    TCanvas*c_XY_Dist_BW = new TCanvas(
        "c_XY_Dist_BW", Form("Black and White XY primary %d distribution", i));

    h_XY_DIST_DUMMY[i]->Draw();

    c_XY_Dist_BW->SaveAs(
        Form("%s/Dummy_XY_BW_Distribution_primary_%d.png", OUTPUT_PREFIX.c_str(),
             i));
    c_XY_Dist_BW->SaveAs(
        Form("%s/Dummy_XY_BW_Distribution_primary_%d.C", OUTPUT_PREFIX.c_str(),
             i));

    //z
    TCanvas* c_Z_Dist = new TCanvas(
        "c_Z_Dist", Form("Z Distribution for primary %d on the detector", i));

    h_Z_DIST_DUMMY[i]->Draw();

    c_Z_Dist->SaveAs(
        Form("%s/Dummy_Z_Distribution_primary_%d.png", OUTPUT_PREFIX.c_str(),
             i));
    c_Z_Dist->SaveAs(
        Form("%s/Dummy_Z_Distribution_primary_%d.C", OUTPUT_PREFIX.c_str(), i));
    TCanvas* c_Z_All_Primary_Dist = new TCanvas(
        "c_Z_All_Primary_Dist",
        Form("Z Distribution for all primaries %d on the detector", i));

    h_Z_DIST_ALL_PRIMARIES[i]->Draw();

    c_Z_All_Primary_Dist->SaveAs(
        Form("%s/Dummy_Z_Distribution_all_primary_%d.png",
             OUTPUT_PREFIX.c_str(), i));
    c_Z_All_Primary_Dist->SaveAs(
        Form("%s/Dummy_Z_Distribution_all_primary_%d.png",
             OUTPUT_PREFIX.c_str(), i));
  }

  //z for all
  TCanvas* c_Z_All_Dist = new TCanvas(
      "c_Z_All_Dist", "Z Distribution for all hits in detector");

  h_Z_ALL_HITS_DUMMY->Draw();

  c_Z_All_Dist->SaveAs(Form("%s/Dummy_Z_Distribution_all.png", OUTPUT_PREFIX.c_str()));
  c_Z_All_Dist->SaveAs(Form("%s/Dummy_Z_Distribution_all.C", OUTPUT_PREFIX.c_str()));

  //x vs y for all
  TCanvas* c_XY_All_Dist = new TCanvas(
      "c_XY_All_dist", "XY Distribution for all hits in detector");

  h_XY_ALL_HITS_DUMMY->Draw("colz");

  c_XY_All_Dist->SaveAs(
      Form("%s/Dummy_XY_Distribution_all.png", OUTPUT_PREFIX.c_str()));
  c_XY_All_Dist->SaveAs(
      Form("%s/Dummy_XY_Distribution_all.C", OUTPUT_PREFIX.c_str()));

}

