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
#include <TPaletteAxis.h>

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
//Dummy detector
//
std::vector<TH2D*> h_WHOLE_LENGTH_XY_DUMMY;
//xy distribution for primaries scattered from the whole length of the target

std::vector<TH2D*> h_WHOLE_LENGTH_NAIVE_XY_DUMMY;
//xy distribution for naive primaries scattered from the whole length of the target

std::vector<TH2D*> h_CELL_LENGTH_XY_DUMMY;
//xy distribution for primaries scattered from the length of the cell

std::vector<TH2D*> h_CELL_LENGTH_NAIVE_XY_DUMMY;
//xy distribution for naive primaries scattered from the length of the cell

std::vector<TH2D*> h_GAS_LENGTH_XY_DUMMY;
//xy distribution for primaries scattered from the effective length of the hydrogen gas

std::vector<TH2D*> h_GAS_LENGTH_NAIVE_XY_DUMMY;
//xy distribution for naive primaries scattered from the effective length of the hydrogen gas


static const Int_t NUM_PRIMARIES = 3;  //number of primary electrons + 1

std::string INDEX_TO_NUM_PRIMARIES_TO_NAME[NUM_PRIMARIES] = {
    "coincidence of primaries", "primary 1", "primary 2" };

//string for the root flle name to be shared with 2 functions
static std::string FILENAME;

//string for the where the input file is stored.
/*static std::string INPUT_PREFIX =
 "/home/vmgray/workspace/HMolPol/Simulation_Analysis/commit_06d91f8";*/
static std::string INPUT_PREFIX =
    "/home/csimpson/workspace/HMolPol/data_Simulation_Analysis/commit_dbaceca/BeamE_2GeV/Mag_8T/Phi-10to10";

//string for GetGitInfo to read
/*static std::string OUTPUT_PREFIX =
 "/home/vmgray/workspace/HMolPol/Simulation_Analysis/commit_06d91f8";*/
static std::string OUTPUT_PREFIX =
    "/home/csimpson/workspace/HMolPol/Simulation_Analysis/commit_dbaceca/BeamE_2GeV/Phi-10to10";
//string for where the files get saved to
static std::string OUTPUT_PREFIX_SCRIPT =
    "/home/csimpson/workspace/HMolPol/Simulation_Analysis/commit_dbaceca/BeamE_2GeV/Phi-10to10/Target_Z_Cut_Distribution_Symmetric";

//HMolPol include - Must be here as it needs to know about OUTPUT_PREFIX
#include "ExtractInitalConditions.h"

void Z_Cut_Distribution_Symmetric(std::string filename, std::string DetectorType)
{
  FILENAME = Form("%s/%s", INPUT_PREFIX.c_str(), filename.c_str());

  //Get the Git info for the simulation
  GetGitInfo();

  //Grab the file(s)
  HMOLPOL_CHAIN = new TChain("HMolPol_Tree");
  //add the file

  HMOLPOL_CHAIN->Add(
      "~/workspace/HMolPol/data_Simulation_Analysis/commit_19095aa/PrimaryGenZ_45cm/Dummy_G4_LEAD_GLASS/200000_Events/"
      "HMolPol_Dummy_Material_G4_GLASS_LEAD_2GeV_Mag8T_NoRaster_Test2.root");

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
 - h_WHOLE_LENGTH_XY_DUMMY: xy distribution for primaries scattered from the whole length of the target
 - h_CELL_LENGTH_XY_DUMMY: xy distribution for primaries scattered from the length of the cell
 - h_GAS_LENGTH_XY_DUMMY: xy distribution for primaries scattered from the effective length of the hydrogen gas

 - POSITION_UNIT_NAME - units for position
 - NUM_PRIMARIES - Number of the Primaries e-

 Exit Conditions: none
 Called By: Hit_XYdist_and_Zdist
 Date: 06-10-2016
 *********************************************************/

void Define_Histograms_Dummy()
{
  //set proper number of histograms
  h_WHOLE_LENGTH_XY_DUMMY.resize(NUM_PRIMARIES);
  h_WHOLE_LENGTH_NAIVE_XY_DUMMY.resize(NUM_PRIMARIES);
  h_CELL_LENGTH_XY_DUMMY.resize(NUM_PRIMARIES);
  h_CELL_LENGTH_NAIVE_XY_DUMMY.resize(NUM_PRIMARIES);
  h_GAS_LENGTH_XY_DUMMY.resize(NUM_PRIMARIES);
  h_GAS_LENGTH_NAIVE_XY_DUMMY.resize(NUM_PRIMARIES);

  for (int i = 0; i < NUM_PRIMARIES; i++)
  {
    //-22.5 to 22.5
    //all primaries
    h_WHOLE_LENGTH_XY_DUMMY[i] = new TH2D(
        Form("h_WHOLE_LENGTH_XY_DUMMY[%d]", i),
        Form("XY distribution for %s generated across the whole target",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()),
        680, -.5, .5, 680, -.5, .5);
    h_WHOLE_LENGTH_XY_DUMMY[i]->GetXaxis()->SetTitle(
        Form("Distance (%s)", POSITION_UNIT_NAME.c_str()));
    h_WHOLE_LENGTH_XY_DUMMY[i]->GetYaxis()->SetTitle(
        Form("Distance (%s)", POSITION_UNIT_NAME.c_str()));

    //naive primaries
    h_WHOLE_LENGTH_NAIVE_XY_DUMMY[i] = new TH2D(
        Form("h_WHOLE_LENGTH_NAIVE_XY_DUMMY[%d]", i),
        Form("XY distribution for naive %s generated across the whole target",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()),
        680, -.5, .5, 680, -.5, .5);
    h_WHOLE_LENGTH_NAIVE_XY_DUMMY[i]->GetXaxis()->SetTitle(
        Form("Distance(%s)", POSITION_UNIT_NAME.c_str()));
    h_WHOLE_LENGTH_NAIVE_XY_DUMMY[i]->GetXaxis()->SetTitle(
        Form("Distance(%s)", POSITION_UNIT_NAME.c_str()));

    //-20 to 20
    //all primaries
    h_CELL_LENGTH_XY_DUMMY[i] = new TH2D(
        Form("h_CELL_LENGTH_XY_DUMMY[%d]", i),
        Form("XY distribution for %s generated across the length of the cell",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()),
        680, -.5, .5, 680, -.5, .5);
    h_CELL_LENGTH_XY_DUMMY[i]->GetXaxis()->SetTitle(
        Form("Distance (%s)", POSITION_UNIT_NAME.c_str()));
    h_CELL_LENGTH_XY_DUMMY[i]->GetYaxis()->SetTitle(
        Form("Distance (%s)", POSITION_UNIT_NAME.c_str()));

    //naive primaries
    h_CELL_LENGTH_NAIVE_XY_DUMMY[i] =
        new TH2D(
            Form("h_CELL_LENGTH_NAIVE_XY_DUMMY[%d]", i),
            Form(
                "XY distribution for naive %s generated across the length of the cell",
                INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()),
            680, -.5, .5, 680, -.5, .5);
    h_CELL_LENGTH_NAIVE_XY_DUMMY[i]->GetXaxis()->SetTitle(
        Form("Distance(%s)", POSITION_UNIT_NAME.c_str()));
    h_CELL_LENGTH_NAIVE_XY_DUMMY[i]->GetXaxis()->SetTitle(
        Form("Distance(%s)", POSITION_UNIT_NAME.c_str()));

    //-10 to 10
    //all primaries
    h_GAS_LENGTH_XY_DUMMY[i] = new TH2D(
        Form("h_GAS_LENGTH_XY_DUMMY[%d]", i),
        Form("XY distribution for %s generated across the effective gas target",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()),
        680, -.5, .5, 680, -.5, .5);
    h_GAS_LENGTH_XY_DUMMY[i]->GetXaxis()->SetTitle(
        Form("Distance (%s)", POSITION_UNIT_NAME.c_str()));
    h_GAS_LENGTH_XY_DUMMY[i]->GetYaxis()->SetTitle(
        Form("Distance (%s)", POSITION_UNIT_NAME.c_str()));

    //naive primaries
    h_GAS_LENGTH_NAIVE_XY_DUMMY[i] =
        new TH2D(
            Form("h_GAS_LENGTH_NAIVE_XY_DUMMY[%d]", i),
            Form(
                "XY distribution for naive %s generated across the effective gas target",
                INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()),
            680, -.5, .5, 680, -.5, .5);
    h_GAS_LENGTH_NAIVE_XY_DUMMY[i]->GetXaxis()->SetTitle(
        Form("Distance(%s)", POSITION_UNIT_NAME.c_str()));
    h_GAS_LENGTH_NAIVE_XY_DUMMY[i]->GetXaxis()->SetTitle(
        Form("Distance(%s)", POSITION_UNIT_NAME.c_str()));

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

 - h_WHOLE_LENGTH_XY_DUMMY: xy distribution for primaries scattered from the whole length of the target
 - h_CELL_LENGTH_XY_DUMMY: xy distribution for primaries scattered from the length of the cell
 - h_GAS_LENGTH_XY_DUMMY: xy distribution for primaries scattered from the effective length of the hydrogen gas

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

  //values to fill histograms with
  double x;
  double y;

  //z position of the primary scattering event
  double z;

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
          //assign values to x and y hit positions and z scattering position
          x = Dummy_Det->fHits[i_hits].fPosition.X() / POSITION_UNIT;
          y = Dummy_Det->fHits[i_hits].fPosition.Y() / POSITION_UNIT;
          z = Primary->fInteractionVertexPosition.z() / POSITION_UNIT;
          //debugging
          //std::cout << "In the Hits loop" << std::endl;

          //If we have a primary electron ie. it has no parent
          if (Dummy_Det->fHits[i_hits].fParentID == 0)
          {
            //fill histograms for coincidence of primaries
            if ((z > -.225) and (z < .225))
            {
              h_WHOLE_LENGTH_XY_DUMMY[0]->Fill(x, y);
            }
            if ((z > -.2) and (z < .2))
            {
              h_CELL_LENGTH_XY_DUMMY[0]->Fill(x, y);
            }
            if ((z > -.1) and (z < .1))
            {
              h_GAS_LENGTH_XY_DUMMY[0]->Fill(x, y);
            }
            //What primary e- is this? The first?
            if (Dummy_Det->fHits[i_hits].fPrimaryID == 1)
            {
              //increase the number of hit by one
              num_Primary_1_Hit++;

              //fill histograms for primary 1
              if ((z > -.225) and (z < .225))
              {
                h_WHOLE_LENGTH_XY_DUMMY[1]->Fill(x, y);
              }
              if ((z > -.2) and (z < .2))
              {
                h_CELL_LENGTH_XY_DUMMY[1]->Fill(x, y);
              }
              if ((z > -.1) and (z < .1))
              {
                h_GAS_LENGTH_XY_DUMMY[1]->Fill(x, y);
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

              //fill histograms for primary 2
              if ((z > -.225) and (z < .225))
              {
                h_WHOLE_LENGTH_XY_DUMMY[2]->Fill(x, y);
              }
              if ((z > -.2) and (z < .2))
              {
                h_CELL_LENGTH_XY_DUMMY[2]->Fill(x, y);
              }
              if ((z > -.1) and (z < .1))
              {
                h_GAS_LENGTH_XY_DUMMY[2]->Fill(x, y);
              }
              //debugging
//              std::cout<<"Hit recorded at "
//                  <<x
//                  <<", "
//                  <<y
//                  <<"."
//                  <<std::endl;

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
      //record naive primary hit location
      x = Dummy_Det->fHits[Primary_1_Hit_Num].fPosition.X() / POSITION_UNIT;
      y = Dummy_Det->fHits[Primary_1_Hit_Num].fPosition.Y() / POSITION_UNIT;

      //fill naive histograms
      if ((z > -.225) and (z < .225))
      {
        h_WHOLE_LENGTH_NAIVE_XY_DUMMY[1]->Fill(x, y);
      }
      if ((z > -.2) and (z < .2))
      {
        h_CELL_LENGTH_NAIVE_XY_DUMMY[1]->Fill(x, y);
      }
      if ((z > -.1) and (z < .1))
      {
        h_GAS_LENGTH_NAIVE_XY_DUMMY[1]->Fill(x, y);
      }

    }

    //Fill histograms for Primary 2
    if (Primary_2_Hit_Num != -1000)
    {
      //record naive primary hit location
      x = Dummy_Det->fHits[Primary_2_Hit_Num].fPosition.X() / POSITION_UNIT;
      y = Dummy_Det->fHits[Primary_2_Hit_Num].fPosition.Y() / POSITION_UNIT;

      //fill naive histograms
      if ((z > -.225) and (z < .225))
      {
        h_WHOLE_LENGTH_NAIVE_XY_DUMMY[2]->Fill(x, y);
      }
      if ((z > -.2) and (z < .2))
      {
        h_CELL_LENGTH_NAIVE_XY_DUMMY[2]->Fill(x, y);
      }
      if ((z > -.1) and (z < .1))
      {
        h_GAS_LENGTH_NAIVE_XY_DUMMY[2]->Fill(x, y);
      }
    }

    //Fill histograms for coincidence
    double x_1;
    double y_1;
    double x_2;
    double y_2;
    if (Primary_1_Hit_Num != -1000 && Primary_2_Hit_Num != -1000)
    {
      //record naive primary 1 hit location
      x_1 = Dummy_Det->fHits[Primary_1_Hit_Num].fPosition.X() / POSITION_UNIT;
      y_1 = Dummy_Det->fHits[Primary_1_Hit_Num].fPosition.Y() / POSITION_UNIT;

      //record naive primary 2 hit location
      x_2 = Dummy_Det->fHits[Primary_2_Hit_Num].fPosition.X() / POSITION_UNIT;
      y_2 = Dummy_Det->fHits[Primary_2_Hit_Num].fPosition.Y() / POSITION_UNIT;

      //fill naive histograms with primary 1 distribution
      if ((z > -.225) and (z < .225))
      {
        h_WHOLE_LENGTH_NAIVE_XY_DUMMY[0]->Fill(x_1, y_1);
      }
      if ((z > -.2) and (z < .2))
      {
        h_CELL_LENGTH_NAIVE_XY_DUMMY[0]->Fill(x_1, y_1);
      }
      if ((z > -.1) and (z < .1))
      {
        h_GAS_LENGTH_NAIVE_XY_DUMMY[0]->Fill(x_1, y_1);
      }

      //fill naive histograms with primary 2 distribution
      if ((z > -.225) and (z < .225))
      {
        h_WHOLE_LENGTH_NAIVE_XY_DUMMY[0]->Fill(x_2, y_2);
      }
      if ((z > -.2) and (z < .2))
      {
        h_CELL_LENGTH_NAIVE_XY_DUMMY[0]->Fill(x_2, y_2);
      }
      if ((z > -.1) and (z < .1))
      {
        h_GAS_LENGTH_NAIVE_XY_DUMMY[0]->Fill(x_2, y_2);
      }
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
 - h_WHOLE_LENGTH_XY_DUMMY: xy distribution for primaries scattered from the whole length of the target
 - h_CELL_LENGTH_XY_DUMMY: xy distribution for primaries scattered from the length of the cell
 - h_GAS_LENGTH_XY_DUMMY: xy distribution for primaries scattered from the effective length of the hydrogen gas

 - POSITION_UNIT_NAME - units for position
 - NUM_PRIMARIES - Number of the Primaries e-

 Exit Conditions: none
 Called By: CrossSection
 Date: 06-12-2016
 Modified: 6-14-2016
 *********************************************************/
void Plot_Dummy()
{
  for (int i = 0; i < NUM_PRIMARIES; i++)
  {
    //whole target length
    //all primaries
    TCanvas* c_Whole_Length_Dummy = new TCanvas(
        "c_Whole_Length_Dummy",
        Form("XY hit distribution for primary %d generated from whole target",
             i),
        650, 650);

    h_WHOLE_LENGTH_XY_DUMMY[i]->Draw("colz");

    gPad->Update();

    //move the stat box
    gStyle->SetStatY(.9);
    gStyle->SetStatX(.9);

    //reformats colored z axis
    TPaletteAxis* p_Whole_Length_XY_Dummy =
        (TPaletteAxis*) h_WHOLE_LENGTH_XY_DUMMY[i]->GetListOfFunctions()->FindObject(
            "palette");
    //moves z axis to right side and width (X2 -X1)
    p_Whole_Length_XY_Dummy->SetX1NDC(0.9);
    p_Whole_Length_XY_Dummy->SetX2NDC(0.91);
    //set z axis tick length
    h_WHOLE_LENGTH_XY_DUMMY[i]->GetZaxis()->SetTickLength(.01);

    gStyle->SetStatY(.9);
    gStyle->SetStatX(.9);

    c_Whole_Length_Dummy->Update();

    c_Whole_Length_Dummy->SaveAs(
        Form("%s/Whole_Target_XY_Distribution_primary_%d.C",
             OUTPUT_PREFIX_SCRIPT.c_str(), i));
    c_Whole_Length_Dummy->SaveAs(
        Form("%s/Whole_Target_XY_Distribution_primary_%d.png",
             OUTPUT_PREFIX_SCRIPT.c_str(), i));

    //naive primaries
    TCanvas* c_Whole_Length_Naive_Dummy =
        new TCanvas(
            "c_Whole_Length_Naive_Dummy",
            Form(
                "XY hit distribution for naive primary %d generated from whole target",
                i),
            650, 650);

    h_WHOLE_LENGTH_NAIVE_XY_DUMMY[i]->Draw("colz");

    gPad->Update();

    //move the stat box
    gStyle->SetStatY(.9);
    gStyle->SetStatX(.9);

    //reformats colored z axis
    TPaletteAxis *p_Whole_Length_Naive_XY_Dummy =
        (TPaletteAxis*) h_WHOLE_LENGTH_NAIVE_XY_DUMMY[i]->GetListOfFunctions()->FindObject(
            "palette");
    //moves z axis to right side and width(X2 - X1)
    p_Whole_Length_Naive_XY_Dummy->SetX1NDC(.9);
    p_Whole_Length_Naive_XY_Dummy->SetX2NDC(.91);
    //set z axis tick length
    h_WHOLE_LENGTH_NAIVE_XY_DUMMY[i]->GetZaxis()->SetTickLength(.01);

    gStyle->SetStatY(.9);
    gStyle->SetStatX(.9);

    c_Whole_Length_Naive_Dummy->Update();

    c_Whole_Length_Naive_Dummy->SaveAs(
        Form("%s/Whole_Target_XY_Distribution_Naive_primary_%d.C",
             OUTPUT_PREFIX_SCRIPT.c_str(), i));
    c_Whole_Length_Naive_Dummy->SaveAs(
        Form("%s/Whole_Target_XY_Distribution_Naive_primary_%d.png",
             OUTPUT_PREFIX_SCRIPT.c_str(), i));

    //cell length
    //all primaries
    TCanvas* c_Cell_Length_Dummy = new TCanvas(
        "c_Cell_Length_Dummy",
        Form("XY hit distribution for primary %d generated from cell", i), 650,
        650);

    h_CELL_LENGTH_XY_DUMMY[i]->Draw("colz");

    gPad->Update();

    //move the stat box
    gStyle->SetStatY(.9);
    gStyle->SetStatX(.9);

    //reformats colored z axis
    TPaletteAxis *p_Cell_Length_XY_Dummy =
        (TPaletteAxis*) h_CELL_LENGTH_XY_DUMMY[i]->GetListOfFunctions()->FindObject(
            "palette");
    //moves z axis to right side and width (X2 -X1)
    p_Cell_Length_XY_Dummy->SetX1NDC(0.9);
    p_Cell_Length_XY_Dummy->SetX2NDC(0.91);
    //set z axis tick length
    h_CELL_LENGTH_XY_DUMMY[i]->GetZaxis()->SetTickLength(.01);

    gStyle->SetStatY(.9);
    gStyle->SetStatX(.9);

    c_Cell_Length_Dummy->Update();

    c_Cell_Length_Dummy->SaveAs(
        Form("%s/Cell_Target_XY_Distribution_primary_%d.C",
             OUTPUT_PREFIX_SCRIPT.c_str(), i));
    c_Cell_Length_Dummy->SaveAs(
        Form("%s/Cell_Target_XY_Distribution_primary_%d.png",
             OUTPUT_PREFIX_SCRIPT.c_str(), i));

    //naive primaries
    TCanvas* c_Cell_Length_Naive_Dummy = new TCanvas(
        "c_Cell_Length_Naive_Dummy",
        Form("XY hit distribution for primary %d generated from cell", i), 650,
        650);

    h_CELL_LENGTH_NAIVE_XY_DUMMY[i]->Draw("colz");

    gPad->Update();

    //move the stat box
    gStyle->SetStatY(.9);
    gStyle->SetStatX(.9);

    //reformats colored z axis
    TPaletteAxis *p_Cell_Length_Naive_XY_Dummy =
        (TPaletteAxis*) h_CELL_LENGTH_NAIVE_XY_DUMMY[i]->GetListOfFunctions()->FindObject(
            "palette");
    //moves z axis to right side and width (X2 -X1)
    p_Cell_Length_Naive_XY_Dummy->SetX1NDC(0.9);
    p_Cell_Length_Naive_XY_Dummy->SetX2NDC(0.91);
    //set z axis tick length
    h_CELL_LENGTH_NAIVE_XY_DUMMY[i]->GetZaxis()->SetTickLength(.01);

    gStyle->SetStatY(.9);
    gStyle->SetStatX(.9);

    c_Cell_Length_Naive_Dummy->Update();

    c_Cell_Length_Naive_Dummy->SaveAs(
        Form("%s/Cell_Target_XY_Distribution_naive_primary_%d.C",
             OUTPUT_PREFIX_SCRIPT.c_str(), i));
    c_Cell_Length_Naive_Dummy->SaveAs(
        Form("%s/Cell_Target_XY_Distribution_naive_primary_%d.png",
             OUTPUT_PREFIX_SCRIPT.c_str(), i));

    //effective gas length
    //all primaries
    TCanvas* c_Gas_Length_Dummy =
        new TCanvas(
            "c_Gas_Length_Dummy",
            Form(
                "XY hit distribution for primary %d generated from effective gas target",
                i),
            650, 650);

    h_GAS_LENGTH_XY_DUMMY[i]->Draw("colz");

    gPad->Update();

    //move the stat box
    gStyle->SetStatY(.9);
    gStyle->SetStatX(.9);

    //reformats colored z axis
    TPaletteAxis *p_Gas_Length_XY_Dummy =
        (TPaletteAxis*) h_GAS_LENGTH_XY_DUMMY[i]->GetListOfFunctions()->FindObject(
            "palette");
    //moves z axis to right side and width (X2 -X1)
    p_Gas_Length_XY_Dummy->SetX1NDC(0.9);
    p_Gas_Length_XY_Dummy->SetX2NDC(0.91);
    //set z axis tick length
    h_GAS_LENGTH_XY_DUMMY[i]->GetZaxis()->SetTickLength(.01);

    gStyle->SetStatY(.9);
    gStyle->SetStatX(.9);

    c_Gas_Length_Dummy->Update();

    c_Gas_Length_Dummy->SaveAs(
        Form("%s/Gas_Target_XY_Distribution_primary_%d.C",
             OUTPUT_PREFIX_SCRIPT.c_str(), i));
    c_Gas_Length_Dummy->SaveAs(
        Form("%s/Gas_Target_XY_Distribution_primary_%d.png",
             OUTPUT_PREFIX_SCRIPT.c_str(), i));

    //naive primaries
    TCanvas* c_Gas_Length_Naive_Dummy =
        new TCanvas(
            "c_Gas_Length_Naive_Dummy",
            Form(
                "XY hit distribution for naive primary %d generated from effective gas target",
                i),
            650, 650);

    h_GAS_LENGTH_NAIVE_XY_DUMMY[i]->Draw("colz");

    gPad->Update();

    //move the stat box
    gStyle->SetStatY(.9);
    gStyle->SetStatX(.9);

    //reformats colored z axis
    TPaletteAxis *p_Gas_Length_Naive_XY_Dummy =
        (TPaletteAxis*) h_GAS_LENGTH_NAIVE_XY_DUMMY[i]->GetListOfFunctions()->FindObject(
            "palette");
    //moves z axis to right side and width (X2 -X1)
    p_Gas_Length_Naive_XY_Dummy->SetX1NDC(0.9);
    p_Gas_Length_Naive_XY_Dummy->SetX2NDC(0.91);
    //set z axis tick length
    h_GAS_LENGTH_NAIVE_XY_DUMMY[i]->GetZaxis()->SetTickLength(.01);

    gStyle->SetStatY(.9);
    gStyle->SetStatX(.9);

    c_Gas_Length_Naive_Dummy->Update();

    c_Gas_Length_Naive_Dummy->SaveAs(
        Form("%s/Gas_Target_XY_Distribution_naive_primary_%d.C",
             OUTPUT_PREFIX_SCRIPT.c_str(), i));
    c_Gas_Length_Naive_Dummy->SaveAs(
        Form("%s/Gas_Target_XY_Distribution_naive_primary_%d.png",
             OUTPUT_PREFIX_SCRIPT.c_str(), i));
  }
}

