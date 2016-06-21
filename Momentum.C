/***********************************************************
 Programmer: Connor Simpson
 Purpose:
 - Plots the momentum vs radius and momentum distribution from a root file
 - Only implemented for dummy detector right now

 - Also will write out the initial conditions via the ExtractInitalConditions.h
 file

 Entry Conditions: Root files name, Detector
 Date: 06-20-2016
 Modified: 06-20-2016
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
#define MOMENTUM_UNIT Units->MeV
#define RADIUS_UNIT Units-> m
#define ANGLE_UNIT Units-> rad

std::string MOMENTUM_UNIT_NAME = "MeV";  //units for momentum
std::string RADIUS_UNIT_NAME = "m";  //units for radius
std::string ANGLE_UNIT_NAME = "rad";  //units for lab angle

//define the TChain
TChain* HMOLPOL_CHAIN;

//define histograms
//DUMMY DET
std::vector<TH1D*> h_MOMENTUM_DUMMY;  //[primary e]
//magnitude of the momentum for each primary electron

TH2D* h_MOMENTUM_VS_RADIUS_DUMMY;  //[primary e]
//magnitude of the momentum vs radius for each primary electron

std::vector<TH2D*> h_X_MOMENTUM_VS_LAB_THETA_DUMMY;  //[primary e]
// x component of the momentum vs lab scattering angle

std::vector<TH2D*> h_Y_MOMENTUM_VS_LAB_THETA_DUMMY;
// y component of the momentum vs lab scattering angle

static const Int_t NUM_PRIMARIES = 3;  //number of primary electrons + 1

std::string INDEX_TO_NUM_PRIMARIES_TO_NAME[NUM_PRIMARIES] = {
    "coincidence of primaries", "primary 1", "primary 2" };

//string for the root file name to be shared with 2 functions
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
    "/home/csimpson/workspace/HMolPol/Simulation_Analysis/commit_dbaceca/BeamE_2GeV/Phi0to360/Momentum";

//HMolPol include - Must be here as it needs to know about OUTPUT_PREFIX
#include "ExtractInitalConditions.h"

void Momentum(std::string filename, std::string DetectorType)
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
 - h_MOMENTUM_DUMMY - vector of histograms for momentum
 - h_MOMENTUM_VS_RADIUS_DUMMY - 2D histogram of momentum magnitude vs radius
 - h_X_MOMENTUM_VS_LAB_THETA_DUMMY - vector of histograms for x momentum vs lab scattering angle
 - h_X_MOMENTUM_VS_LAB_THETA_DUMMY - vector of histograms for y momentum vs lab scattering angle

 - MOMENTUM_UNIT_NAME - name of units for momentum
 - RADIUS_UNIT_NAME - name of units for radius
 - ANGLE_UNIT_NAME - name of units for lab angle

 Exit Conditions: none
 Called By: Momentum
 Date: 06-20-2016
 Modified: 06-20-2016
 *********************************************************/
void Define_Histograms_Dummy()
{
  //resize histograms
  h_MOMENTUM_DUMMY.resize(NUM_PRIMARIES);
  h_X_MOMENTUM_VS_LAB_THETA_DUMMY.resize(NUM_PRIMARIES);
  h_Y_MOMENTUM_VS_LAB_THETA_DUMMY.resize(NUM_PRIMARIES);

  //finish defining histograms
  for (int i = 0; i < NUM_PRIMARIES; i++)
  {
    //momentum
    h_MOMENTUM_DUMMY[i] = new TH1D(
        Form("h_MOMENTUM_DUMMY[%d]", i),
        Form("Momentum magnitude for %s on the Dummy Detector",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()),
        600, 0, 1300);

    h_MOMENTUM_DUMMY[i]->GetXaxis()->SetTitle(
        Form("Momentum of %s (%s)", INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str(),
             MOMENTUM_UNIT_NAME.c_str()));

    //momentum vs scattering angle
    //x component
    h_X_MOMENTUM_VS_LAB_THETA_DUMMY[i] = new TH2D(
        Form("h_X_MOMENTUM_VS_LAB_THETA_DUMMY[%d]", i),
        Form("X Momentum vs Lab Theta for %s on the Dummy Detector",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()),
        600, 1.54, 1.56, 600, 0, 1300);

    h_X_MOMENTUM_VS_LAB_THETA_DUMMY[i]->GetXaxis()->SetTitle(
        Form("Lab Scattering Angle (%s)", ANGLE_UNIT_NAME.c_str()));

    h_X_MOMENTUM_VS_LAB_THETA_DUMMY[i]->GetYaxis()->SetTitle(
        Form("X Momentum (%s)", MOMENTUM_UNIT_NAME.c_str()));

    //y component
    h_Y_MOMENTUM_VS_LAB_THETA_DUMMY[i] = new TH2D(
        Form("h_Y_MOMENTUM_VS_LAB_THETA_DUMMY[%d]", i),
        Form("Y Momentum vs Lab Theta for %s on the Dummy Detector",
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()),
        600, 1.54, 1.56, 600, 0, 900);

    h_Y_MOMENTUM_VS_LAB_THETA_DUMMY[i]->GetXaxis()->SetTitle(
        Form("Lab Scattering Angle (%s)", ANGLE_UNIT_NAME.c_str()));

    h_Y_MOMENTUM_VS_LAB_THETA_DUMMY[i]->GetYaxis()->SetTitle(
        Form("Y Momentum (%s)", MOMENTUM_UNIT_NAME.c_str()));
  }
  //momentum vs radius
  h_MOMENTUM_VS_RADIUS_DUMMY = new TH2D(
      "h_MOMENTUM_VS_RADIUS_DUMMY",
      "Momentum vs Radius of primary hits on the Dummy Detector", 600, 0, .3,
      600, 0, 1300);

  h_MOMENTUM_VS_RADIUS_DUMMY->GetYaxis()->SetTitle(
      Form("Momentum (%s)", MOMENTUM_UNIT_NAME.c_str()));

  h_MOMENTUM_VS_RADIUS_DUMMY->GetXaxis()->SetTitle(
      Form("Radius (%s)", RADIUS_UNIT_NAME.c_str()));
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

 - h_MOMENTUM_DUMMY - vector of histograms for momentum
 - h_MOMENTUM_VS_RADIUS_DUMMY - 2D histogram of momentum magnitude vs radius
 - h_X_MOMENTUM_VS_LAB_THETA_DUMMY - vector of histograms for x momentum vs lab scattering angle
 - h_X_MOMENTUM_VS_LAB_THETA_DUMMY - vector of histograms for y momentum vs lab scattering angle

 - MOMENTUM_UNIT_NAME - name of units for momentum
 - RADIUS_UNIT_NAME - name of units for radius
 - ANGLE_UNIT_NAME - name of units for lab angle


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
          //debugging
          //std::cout << "In the Hits loop" << std::endl;

          //If we have a primary electron ie. it has no parent
          if (Dummy_Det->fHits[i_hits].fParentID == 0)
          {
            h_MOMENTUM_DUMMY[0]->Fill(
                Dummy_Det->fHits[i_hits].fMomentum.Mag() / MOMENTUM_UNIT);

            h_MOMENTUM_VS_RADIUS_DUMMY->Fill(
                sqrt(
                    Dummy_Det->fHits[i_hits].fPosition.X() * Dummy_Det->fHits[i_hits].fPosition.X() + Dummy_Det->fHits[i_hits].fPosition.Y()
                        * Dummy_Det->fHits[i_hits].fPosition.Y())
                / RADIUS_UNIT,
                Dummy_Det->fHits[i_hits].fMomentum.Mag() / MOMENTUM_UNIT);

            //What primary e- is this? The first?
            if (Dummy_Det->fHits[i_hits].fPrimaryID == 1)
            {
              //increase the number of hit by one
              num_Primary_1_Hit++;
              h_MOMENTUM_DUMMY[1]->Fill(
                  Dummy_Det->fHits[i_hits].fMomentum.Mag() / MOMENTUM_UNIT);

              h_X_MOMENTUM_VS_LAB_THETA_DUMMY[1]->Fill(
                  Primary->fThetaLab1 / ANGLE_UNIT,
                  Dummy_Det->fHits[i_hits].fMomentum.X() / MOMENTUM_UNIT);

              h_Y_MOMENTUM_VS_LAB_THETA_DUMMY[1]->Fill(
                  Primary->fThetaLab1 / ANGLE_UNIT,
                  Dummy_Det->fHits[i_hits].fMomentum.Y() / MOMENTUM_UNIT);

              h_X_MOMENTUM_VS_LAB_THETA_DUMMY[0]->Fill(
                  Primary->fThetaLab1 / ANGLE_UNIT,
                  Dummy_Det->fHits[i_hits].fMomentum.X() / MOMENTUM_UNIT);

              h_Y_MOMENTUM_VS_LAB_THETA_DUMMY[0]->Fill(
                  Primary->fThetaLab1 / ANGLE_UNIT,
                  Dummy_Det->fHits[i_hits].fMomentum.Y() / MOMENTUM_UNIT);

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
              h_MOMENTUM_DUMMY[2]->Fill(
                  Dummy_Det->fHits[i_hits].fMomentum.Mag() / MOMENTUM_UNIT);

              h_X_MOMENTUM_VS_LAB_THETA_DUMMY[2]->Fill(
                  Primary->fThetaLab2 / ANGLE_UNIT,
                  Dummy_Det->fHits[i_hits].fMomentum.X() / MOMENTUM_UNIT);

              h_Y_MOMENTUM_VS_LAB_THETA_DUMMY[2]->Fill(
                  Primary->fThetaLab2 / ANGLE_UNIT,
                  Dummy_Det->fHits[i_hits].fMomentum.Y() / MOMENTUM_UNIT);

              h_X_MOMENTUM_VS_LAB_THETA_DUMMY[0]->Fill(
                  Primary->fThetaLab2 / ANGLE_UNIT,
                  Dummy_Det->fHits[i_hits].fMomentum.X() / MOMENTUM_UNIT);

              h_Y_MOMENTUM_VS_LAB_THETA_DUMMY[0]->Fill(
                  Primary->fThetaLab2 / ANGLE_UNIT,
                  Dummy_Det->fHits[i_hits].fMomentum.Y() / MOMENTUM_UNIT);

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
 - h_MOMENTUM_DUMMY - vector of histograms for momentum
 - h_MOMENTUM_VS_RADIUS_DUMMY - 2D histogram of momentum magnitude vs radius
 - h_X_MOMENTUM_VS_LAB_THETA_DUMMY - vector of histograms for x momentum vs lab scattering angle
 - h_X_MOMENTUM_VS_LAB_THETA_DUMMY - vector of histograms for y momentum vs lab scattering angle

 - MOMENTUM_UNIT_NAME - name of units for momentum
 - RADIUS_UNIT_NAME - name of units for radius
 - ANGLE_UNIT_NAME - name of units for lab angle

 Exit Conditions: none
 Called By: Momentum
 Date: 06-20-2016
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
    //momentum
    TCanvas* c_Momentum = new TCanvas(
        "c_Momentum", Form("Momentum for primary %d on the dummy detector", i));

    h_MOMENTUM_DUMMY[i]->Draw();

    c_Momentum->SaveAs(
        Form("%s/Dummy_Momentum_Magnitude_primary_%d.png",
             OUTPUT_PREFIX.c_str(), i));
    c_Momentum->SaveAs(
        Form("%s/Dummy_Momentum_Magnitude_primary_%d.C", OUTPUT_PREFIX.c_str(),
             i));

    //momentum vs lab theta
    //x
    TCanvas* c_X_Momentum_vs_Theta = new TCanvas(
        "c_X_Momentum_vs_Theta",
        Form("X momentum vs lab theta for primary %d on the dummy detector",
             i));

    h_X_MOMENTUM_VS_LAB_THETA_DUMMY[i]->Draw("colz");

    c_X_Momentum_vs_Theta->SaveAs(
        Form("%s/Dummy_X_Momentum_vs_Lab_Theta_primary_%d.png",
             OUTPUT_PREFIX.c_str(), i));
    c_X_Momentum_vs_Theta->SaveAs(
        Form("%s/Dummy_X_Momentum_vs_Lab_Theta_primary_%d.png",
             OUTPUT_PREFIX.c_str(), i));
    //y
    TCanvas* c_Y_Momentum_vs_Theta = new TCanvas(
        "c_Y_Momentum_vs_Theta",
        Form("Y momentum vs lab theta for primary %d on the dummy detector",
             i));

    h_Y_MOMENTUM_VS_LAB_THETA_DUMMY[i]->Draw("colz");

    c_Y_Momentum_vs_Theta->SaveAs(
        Form("%s/Dummy_Y_Momentum_vs_Lab_Theta_primary_%d.png",
             OUTPUT_PREFIX.c_str(), i));
    c_Y_Momentum_vs_Theta->SaveAs(
        Form("%s/Dummy_Y_Momentum_vs_Lab_Theta_primary_%d.png",
             OUTPUT_PREFIX.c_str(), i));
  }
  //momentum vs radius in color
  TCanvas* c_Momentum_vs_Radius = new TCanvas("c_Momentum_vs_Radius",
                                              "Momentum vs Radius");

  h_MOMENTUM_VS_RADIUS_DUMMY->Draw("colz");

  c_Momentum_vs_Radius->SaveAs(
      Form("%s/Dummy_Momentum_vs_Radius.png", OUTPUT_PREFIX.c_str()));
  c_Momentum_vs_Radius->SaveAs(
      Form("%s/Dummy_Momentum_vs_Radius.C", OUTPUT_PREFIX.c_str()));

  //momentum vs radius in black and white
  TCanvas* c_Momentum_vs_Radius_BW = new TCanvas("c_Momentum_vs_Radius_BW",
                                                 "Momentum vs Radius BW");

  h_MOMENTUM_VS_RADIUS_DUMMY->Draw();

  c_Momentum_vs_Radius_BW->SaveAs(
      Form("%s/Dummy_Momentum_vs_Radius_BW.C", OUTPUT_PREFIX.c_str()));
  c_Momentum_vs_Radius_BW->SaveAs(
      Form("%s/Dummy_Momentum_vs_Radius_BW.png", OUTPUT_PREFIX.c_str()));
}

