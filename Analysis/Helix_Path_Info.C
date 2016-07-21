/***********************************************************
 Programmer: Valerie Gray
 Purpose:
 - To take a root file and find information on the helix path of the
 electrons, specifically the radius and angle of rotation through the B field
 - It will work for BOTH dummy and left right detectors :) When left and right are added.

 - Also will write out the initial conditions via the ExtractInitalConditions.h
 file

 Entry Conditions: Root files name, Detector
 Date: 07-18-2016
 Modified: 07-21-2016
 Assisted By: Me!
 *********************************************************/

/**********************************************************
 * Helix Information
 *
 * Equation for a Helix
 * x = r cos(t)
 * y = r sin(t)
 * z = P t
 *
 * where
 * - t is the angle which the helix rotates in rad. This is not
 * related or the same as the scattering angle.
 * - P is the pitch of the helix. The Z distance between 2 points
 * at the same x and y
 * - r is the radius of rotation
 *
 * r = p_perp/(QB)
 * P = (2 * pi * p_par)/(QB)
 *
 * where
 * - Q is the charge of the particle (C)
 * - B is the Magnetic Field Strength
 * - p is the momentum
 * - p_perp is the perpendicular momentum
 * - p_par is the parallel momentum
 **********************************************************/

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
#define ANGLE_UNIT Units->deg
#define LENGTH_UNIT Units->cm
#define MOMENTUM_UNIT Units->GeV

std::string ANGLE_UNIT_NAME = "deg";  //units for angle
std::string LENGTH_UNIT_NAME = "cm";  //units for length
std::string MOMENTUM_UNIT_NAME = "GeV";  //units for length

//define the TChain
TChain* HMOLPOL_CHAIN;

//define historgrams
std::vector<TH1D*> h_RADIUS_OF_ROTATION_DUMMY;  //[primary e]
//radius that the electron rotates around the field lines

std::vector<TH1D*> h_TOTAL_ANGLE_ROTATED_THROUGH_DUMMY;  //[primary e]
//the total angle which the primary rotates through

std::vector<TH2D*> h_TOTAL_ANGLE_ROTATED_VS_ZPOS_THROUGH_DUMMY;  //[primary e]
//the total angle which the primary rotates through vs Z position

std::vector<TH2D*> h_TOTAL_ANGLE_ROTATED_VS_MOMENTUM_DUMMY;  //[primary e]
//the total angle which the primary rotates through vs Z position

static const Int_t NUM_PRIMARIES = 4;  //number of primary electrons + 2
//since coincidence has one from each primary

std::string INDEX_TO_NUM_PRIMARIES_TO_NAME[NUM_PRIMARIES] = {
    "Primary 1 in a coincidence of naive primaries",
    "Primary 2 in a coincidence of naive primaries", "primary 1", "primary 2" };

//define the constants
static Double_t TARGET_LENGTH = 20;  //cm
static Double_t MAG_FIELD = 8.0;  //Tesla
static Double_t ELECTRIC_CHARGE = 1.602e-19;  //Coulumb
static Double_t GEV_TO_QBR = 5.344e-17;  // (kg * cm)/s
//or 1 GeV = 5.344e-19 (kg * m)/s
static Double_t MEV_TO_QBR = 5.344e-20;  // (kg * cm)/s
//or 1MeV = 5.344e-22 (kg * m)/s

//string for the root flle name to be shared with 2 functions
static std::string FILENAME;

//string for the where the input file is stored.
/*static std::string INPUT_PREFIX =
 "/home/vmgray/workspace/HMolPol/Simulation_Analysis/commit_06d91f8";*/
static std::string INPUT_PREFIX = "/home/vmgray/workspace/HMolPol";

//string for the where the output will go.
//For the git and inital conditions OUTPUT_PREFIX
//For all files made here OUTPUT_PREFIX_SCRIPT
/*static std::string OUTPUT_PREFIX =
 "/home/vmgray/workspace/HMolPol/Simulation_Analysis/commit_06d91f8";*/
static std::string OUTPUT_PREFIX = "/home/vmgray/workspace/HMolPol";

static std::string OUTPUT_PREFIX_SCRIPT = "/home/vmgray/workspace/HMolPol";

//HMolPol include - Must be here as it needs to know about OUTPUT_PREFIX
#include "ExtractInitalConditions.h"

void Helix_Path_Info(std::string filename, std::string DetectorType)
{
  FILENAME = Form("%s/%s", INPUT_PREFIX.c_str(), filename.c_str());

  //Get the Git info for the simulation
  GetGitInfo();

  //Grab the file(s)
  HMOLPOL_CHAIN = new TChain("HMolPol_Tree");
  //add the file
  HMOLPOL_CHAIN->Add(Form("%s", FILENAME.c_str()));

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
 - OUTPUT_PREFIX

 Exit Conditions: none
 Called By: Helix_Path_Info
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
 - h_RADIUS_OF_ROTATION_DUMMY - radius of rotation around field lines
 - h_TOTAL_ANGLE_ROTATED_THROUGH_DUMMY - angle rotated through
 - h_TOTAL_ANGLE_ROTATED_VS_ZPOS_THROUGH_DUMMY - angle rotated through vs Z position
 - h_TOTAL_ANGLE_ROTATED_VS_MOMENTUM_DUMMY

 - ANGLE_UNIT_NAME - units for angle
 - LENGTH_UNIT_NAME
 - NUM_PRIMARIES - Number of the Primaries e-
 - INDEX_TO_NUM_PRIMARIES_TO_NAME

 -OUTPUT_PREFIX

 Exit Conditions: none
 Called By: Helix_Path_Info
 Date: 07-18-2016
 Modified: 07-20-2016
 *********************************************************/
void Define_Histograms_Dummy()
{
  //resize histograms
  h_RADIUS_OF_ROTATION_DUMMY.resize(NUM_PRIMARIES);
  h_TOTAL_ANGLE_ROTATED_THROUGH_DUMMY.resize(NUM_PRIMARIES);
  h_TOTAL_ANGLE_ROTATED_VS_ZPOS_THROUGH_DUMMY.resize(NUM_PRIMARIES);
  h_TOTAL_ANGLE_ROTATED_VS_MOMENTUM_DUMMY.resize(NUM_PRIMARIES);

  for (int i = 0; i < NUM_PRIMARIES; i++)
  {
    //radius
    h_RADIUS_OF_ROTATION_DUMMY[i] =
        new TH1D(
            Form("h_RADIUS_OF_ROTATION_DUMMY[%d]", i),
            Form(
                "Radius of rotation around the magnetic field lines for %s on the Dummy Detector",
                INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()),
            72, 0.90, 0.96);

    h_RADIUS_OF_ROTATION_DUMMY[i]->GetXaxis()->SetTitle(
        Form("Radius of Rotation (%s)", LENGTH_UNIT_NAME.c_str()));

    //angle rotated through
    h_TOTAL_ANGLE_ROTATED_THROUGH_DUMMY[i] =
        new TH1D(
            Form("h_TOTAL_ANGLE_ROTATED_THROUGH_DUMMY[%d]", i),
            Form(
                "Angle of rotation through the magnetic field lines for %s on the Dummy Detector",
                INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()),
            72, -0.2, 7.0);

    h_TOTAL_ANGLE_ROTATED_THROUGH_DUMMY[i]->GetXaxis()->SetTitle(
        Form("Angle rotated through (%s)", ANGLE_UNIT_NAME.c_str()));

    //angle rotated through vs Z interation Position
    h_TOTAL_ANGLE_ROTATED_VS_ZPOS_THROUGH_DUMMY[i] =
        new TH2D(
            Form("h_TOTAL_ANGLE_ROTATED_VS_ZPOS_THROUGH_DUMMY[%d]", i),
            Form(
                "Angle of rotation through the magnetic field lines vs Z interaction position for %s on the Dummy Detector",
                INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()),
            200, -2.0, 1.1 * TARGET_LENGTH, 72, -0.2, 7.0);

    h_TOTAL_ANGLE_ROTATED_VS_ZPOS_THROUGH_DUMMY[i]->GetXaxis()->SetTitle(
        Form("Z Distance traveled in Target (%s)",
             LENGTH_UNIT_NAME.c_str()));

    h_TOTAL_ANGLE_ROTATED_VS_ZPOS_THROUGH_DUMMY[i]->GetYaxis()->SetTitle(
        Form("Angle rotated through (%s)", ANGLE_UNIT_NAME.c_str()));

    //angle rotation vs Momentum
    h_TOTAL_ANGLE_ROTATED_VS_MOMENTUM_DUMMY[i] =
        new TH2D(
            Form("h_TOTAL_ANGLE_ROTATED_VS_MOMENTUM_DUMMY[%d]", i),
            Form(
                "Angle of rotation through the magnetic field lines vs interaction Momentum for %s on the Dummy Detector",
                INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()),
            200, 0.5, 1.5, 72, -0.5, 7.0);

    h_TOTAL_ANGLE_ROTATED_VS_MOMENTUM_DUMMY[i]->GetXaxis()->SetTitle(
        Form("Interaction Momentum (%s)", MOMENTUM_UNIT_NAME.c_str()));

    h_TOTAL_ANGLE_ROTATED_VS_MOMENTUM_DUMMY[i]->GetYaxis()->SetTitle(
        Form("Angle rotated through (%s)", ANGLE_UNIT_NAME.c_str()));

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

 - h_RADIUS_OF_ROTATION_DUMMY - radius of rotation around field lines
 - h_TOTAL_ANGLE_ROTATED_THROUGH_DUMMY - angle rotated through
 - h_TOTAL_ANGLE_ROTATED_VS_ZPOS_THROUGH_DUMMY - angle rotated through vs Z position
 - h_TOTAL_ANGLE_ROTATED_VS_MOMENTUM_DUMMY

 - ANGLE_UNIT
 - LENGTH_UNIT
 - MOMENTUM_UNIT

 - NUM_PRIMARIES - Number of the Primaries e-
 - INDEX_TO_NUM_PRIMARIES_TO_NAME

 - TARGET_LENGTH
 - MAG_FIELD
 - ELECTRIC_CHARGE
 - GEV_TO_QBR

 - ANGLE_UNIT_NAME - units for angle
 - LENGTH_UNIT_NAME
 - MOMENTUM_UNIT_NAME


 Exit Conditions: none
 Called By: Helix_Path_Info
 Date: 07-19-2016
 Modified: 07-21-2016
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

            //prmaryID == 1 first Primary
            if (Dummy_Det->fHits[i_hits].fPrimaryID == 1)
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
            }  //End of if PrimaryID == 1

            //prmaryID == 2, second Primary
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
            }  //End of if PrimaryID == 2

          }  // End of if ParentID == 0
        }  //End of Loops of hits in the Dummy det
      }  //End of loops over Dummy detector hits
    }  //End of loop if have a hit in Dummy det

    //Done getting the hit numbers for this event for naive primaries
    //Fill the histgrams

    //Primary 1
    if (Primary_1_Hit_Num != -1000)
    {
      //Radius
      //r = (Mom.Perp * GEV_TO_QBR)/QB
      h_RADIUS_OF_ROTATION_DUMMY[2]->Fill(
          ((Primary->fInteractionVertexMomentum1.Perp() / MOMENTUM_UNIT) / (MAG_FIELD
              * ELECTRIC_CHARGE))
          * GEV_TO_QBR);

      //angle rotated through
      //t = ((zQB)/(2*Pi*Mom_Par))*(1/GEV_TO_QBR)*(180/Pi)
      //Z dist traveled in B field = (Target_Length / 2) - Pos.Z()
      Double_t z_dist_in_B = (TARGET_LENGTH / 2)
          - (Primary->fInteractionVertexPosition.Z() / LENGTH_UNIT);

      h_TOTAL_ANGLE_ROTATED_THROUGH_DUMMY[2]->Fill(
          ((z_dist_in_B * MAG_FIELD * ELECTRIC_CHARGE) / (2
              * TMath::Pi()
              * (Primary->fInteractionVertexMomentum1.Z() / MOMENTUM_UNIT)))
          * (1 / GEV_TO_QBR) * (180 / TMath::Pi()));

      h_TOTAL_ANGLE_ROTATED_VS_ZPOS_THROUGH_DUMMY[2]->Fill(
          z_dist_in_B,
          ((z_dist_in_B * MAG_FIELD * ELECTRIC_CHARGE) / (2
              * TMath::Pi()
              * (Primary->fInteractionVertexMomentum1.Z() / MOMENTUM_UNIT)))
          * (1 / GEV_TO_QBR) * (180 / TMath::Pi()));

      //angle rotated through vs. momentum
      h_TOTAL_ANGLE_ROTATED_VS_MOMENTUM_DUMMY[2]->Fill(
          Primary->fInteractionVertexMomentum1.Mag() / MOMENTUM_UNIT,
          ((z_dist_in_B * MAG_FIELD * ELECTRIC_CHARGE) / (2
              * TMath::Pi()
              * (Primary->fInteractionVertexMomentum1.Z() / MOMENTUM_UNIT)))
          * (1 / GEV_TO_QBR) * (180 / TMath::Pi()));

    }  //end of if the primary 1
    //fill historgrams for primary 2
    if (Primary_2_Hit_Num != -1000)
    {
      //Radius
      //r = (Mom.Perp * GEV_TO_QBR)/QB
      h_RADIUS_OF_ROTATION_DUMMY[3]->Fill(
          ((Primary->fInteractionVertexMomentum2.Perp() / MOMENTUM_UNIT) / (MAG_FIELD
              * ELECTRIC_CHARGE))
          * GEV_TO_QBR);

      //angle rotated through
      //t = (zQB)/(2*Pi*Mom_Par*GEV_TO_QBR)
      //Z dist traveled in B field = (Target_Length / 2) - Pos.Z()
      Double_t z_dist_in_B = (TARGET_LENGTH / 2)
          - (Primary->fInteractionVertexPosition.Z() / LENGTH_UNIT);

      h_TOTAL_ANGLE_ROTATED_THROUGH_DUMMY[3]->Fill(
          ((z_dist_in_B * MAG_FIELD * ELECTRIC_CHARGE) / (2
              * TMath::Pi()
              * (Primary->fInteractionVertexMomentum2.Z() / MOMENTUM_UNIT)))
          * (1 / GEV_TO_QBR) * (180 / TMath::Pi()));

      h_TOTAL_ANGLE_ROTATED_VS_ZPOS_THROUGH_DUMMY[3]->Fill(
          z_dist_in_B,
          ((z_dist_in_B * MAG_FIELD * ELECTRIC_CHARGE) / (2
              * TMath::Pi()
              * (Primary->fInteractionVertexMomentum2.Z() / MOMENTUM_UNIT)))
          * (1 / GEV_TO_QBR) * (180 / TMath::Pi()));

      //angle rotated through vs. momentum
      h_TOTAL_ANGLE_ROTATED_VS_MOMENTUM_DUMMY[3]->Fill(
          Primary->fInteractionVertexMomentum2.Mag() / MOMENTUM_UNIT,
          ((z_dist_in_B * MAG_FIELD * ELECTRIC_CHARGE) / (2
              * TMath::Pi()
              * (Primary->fInteractionVertexMomentum2.Z() / MOMENTUM_UNIT)))
          * (1 / GEV_TO_QBR) * (180 / TMath::Pi()));

    }  //end of if the primary 2
    //FIll hitogram for coincidence
    if (Primary_1_Hit_Num != -1000 && Primary_2_Hit_Num != -1000)
    {
      //For primary 1 in the coincidence
      //Radius
      //r = (Mom.Perp * GEV_TO_QBR)/QB
      h_RADIUS_OF_ROTATION_DUMMY[0]->Fill(
          ((Primary->fInteractionVertexMomentum1.Perp() / MOMENTUM_UNIT) / (MAG_FIELD
              * ELECTRIC_CHARGE))
          * GEV_TO_QBR);

      //angle rotated through
      //t = (zQB)/(2*Pi*Mom_Par*GEV_TO_QBR)
      //Z dist traveled in B field = (Target_Length / 2) - Pos.Z()
      Double_t z_dist_in_B = (TARGET_LENGTH / 2)
          - (Primary->fInteractionVertexPosition.Z() / LENGTH_UNIT);

      h_TOTAL_ANGLE_ROTATED_THROUGH_DUMMY[0]->Fill(
          ((z_dist_in_B * MAG_FIELD * ELECTRIC_CHARGE) / (2
              * TMath::Pi()
              * (Primary->fInteractionVertexMomentum1.Z() / MOMENTUM_UNIT)))
          * (1 / GEV_TO_QBR) * (180 / TMath::Pi()));

      h_TOTAL_ANGLE_ROTATED_VS_ZPOS_THROUGH_DUMMY[0]->Fill(
          z_dist_in_B,
          ((z_dist_in_B * MAG_FIELD * ELECTRIC_CHARGE) / (2
              * TMath::Pi()
              * (Primary->fInteractionVertexMomentum1.Z() / MOMENTUM_UNIT)))
          * (1 / GEV_TO_QBR) * (180 / TMath::Pi()));

      //angle rotated through vs. momentum
      h_TOTAL_ANGLE_ROTATED_VS_MOMENTUM_DUMMY[0]->Fill(
          Primary->fInteractionVertexMomentum1.Mag() / MOMENTUM_UNIT,
          ((z_dist_in_B * MAG_FIELD * ELECTRIC_CHARGE) / (2
              * TMath::Pi()
              * (Primary->fInteractionVertexMomentum1.Z() / MOMENTUM_UNIT)))
          * (1 / GEV_TO_QBR) * (180 / TMath::Pi()));

      //For primary 2 in the coincidence
      //Radius
      //r = (Mom.Perp * GEV_TO_QBR)/QB
      h_RADIUS_OF_ROTATION_DUMMY[1]->Fill(
          ((Primary->fInteractionVertexMomentum2.Perp() / MOMENTUM_UNIT) / (MAG_FIELD
              * ELECTRIC_CHARGE))
          * GEV_TO_QBR);

      //angle rotated through
      //t = (zQB)/(2*Pi*Mom_Par*GEV_TO_QBR)
      h_TOTAL_ANGLE_ROTATED_THROUGH_DUMMY[1]->Fill(
          ((z_dist_in_B * MAG_FIELD * ELECTRIC_CHARGE) / (2
              * TMath::Pi()
              * (Primary->fInteractionVertexMomentum2.Z() / MOMENTUM_UNIT)))
          * (1 / GEV_TO_QBR) * (180 / TMath::Pi()));

      h_TOTAL_ANGLE_ROTATED_VS_ZPOS_THROUGH_DUMMY[1]->Fill(
          z_dist_in_B,
          ((z_dist_in_B * MAG_FIELD * ELECTRIC_CHARGE) / (2
              * TMath::Pi()
              * (Primary->fInteractionVertexMomentum2.Z() / MOMENTUM_UNIT)))
          * (1 / GEV_TO_QBR) * (180 / TMath::Pi()));

      //angle rotated through vs. momentum
      h_TOTAL_ANGLE_ROTATED_VS_MOMENTUM_DUMMY[1]->Fill(
          Primary->fInteractionVertexMomentum2.Mag() / MOMENTUM_UNIT,
          ((z_dist_in_B * MAG_FIELD * ELECTRIC_CHARGE) / (2
              * TMath::Pi()
              * (Primary->fInteractionVertexMomentum2.Z() / MOMENTUM_UNIT)))
          * (1 / GEV_TO_QBR) * (180 / TMath::Pi()));

    }

  }  //End of loop over events

  return;
}

/***********************************************************
 Function: Plot_Dummy
 Purpose:
 To plot the graphs and save those plots

 Entry Conditions:
 none

 Global:
 - h_RADIUS_OF_ROTATION_DUMMY - radius of rotation around field lines
 - h_TOTAL_ANGLE_ROTATED_THROUGH_DUMMY - angle rotated through
 - h_TOTAL_ANGLE_ROTATED_VS_ZPOS_THROUGH_DUMMY - angle rotated through vs Z position
 - h_TOTAL_ANGLE_ROTATED_VS_MOMENTUM_DUMMY

 - NUM_PRIMARIES - Number of the Primaries e-
 - INDEX_TO_NUM_PRIMARIES_TO_NAME
 - OUTPUT_PREFIX_SCRIPT

 Exit Conditions: none
 Called By: Helix_Path_Info
 Date: 07-18-2016
 Modified: 07-20-2016
 *********************************************************/
void Plot_Dummy()
{
  //debugging
  //std::cout << "made it in the make plotting function" << std::endl;

  //define Canvases
  //loop over primaries
  for (int i = 0; i < NUM_PRIMARIES; i++)
  {
    //radius
    TCanvas* c_Radius_of_rotation =
        new TCanvas(
            "c_Radius_of_rotation",
            Form(
                "Radius of rotation around the magnetic field lines for %s on the Dummy Detector",
                INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()));

    h_RADIUS_OF_ROTATION_DUMMY[i]->Draw();

    //removes the histogram title in stat box
    gStyle->SetOptStat("emr");  //1110);

    c_Radius_of_rotation->SaveAs(
        Form("%s/Dummy_Radius_of_rotation_around_BField_%s.png",
             OUTPUT_PREFIX_SCRIPT.c_str(),
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()));
    c_Radius_of_rotation->SaveAs(
        Form("%s/Dummy_Radius_of_rotation_around_BField_%s.C",
             OUTPUT_PREFIX_SCRIPT.c_str(),
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()));

    //angle rotated through
    TCanvas* c_Total_angle_rotated_through =
        new TCanvas(
            "c_Total_angle_rotated_through",
            Form(
                "Angle of rotation through the magnetic field lines for %s on the Dummy Detector",
                INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()));

    h_TOTAL_ANGLE_ROTATED_THROUGH_DUMMY[i]->Draw();

    c_Total_angle_rotated_through->SaveAs(
        Form("%s/Dummy_Total_angle_rotated_through_%s.png",
             OUTPUT_PREFIX_SCRIPT.c_str(),
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()));
    c_Total_angle_rotated_through->SaveAs(
        Form("%s/Dummy_Total_angle_rotated_through_%s.C",
             OUTPUT_PREFIX_SCRIPT.c_str(),
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()));

    //angle rotated through vs Z interaction Position
    TCanvas* c_Total_angle_rotated_vs_mom=
        new TCanvas(
            "c_Total_angle_rotated_vs_mom",
            Form(
                "Angle of rotation through the magnetic field lines vs interaction Momentum for %s on the Dummy Detector",
                INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()));

    h_TOTAL_ANGLE_ROTATED_VS_MOMENTUM_DUMMY[i]->Draw();

    //move stat box
//    gStyle->SetStatX(0.95);
//    gStyle->SetStatY(0.40);
    //removes the histogram title in stat box
//    gStyle->SetOptStat("emr");  //1110);

    c_Total_angle_rotated_vs_mom->SaveAs(
        Form("%s/Dummy_Total_angle_rotated_vs_Momentum_%s.png",
             OUTPUT_PREFIX_SCRIPT.c_str(),
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()));
    c_Total_angle_rotated_vs_mom->SaveAs(
        Form("%s/Dummy_Total_angle_rotated_vs_Momentum_%s.C",
             OUTPUT_PREFIX_SCRIPT.c_str(),
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()));
  }

  //angle rotated through vs Z interaction Position
  //Seperate so can move the stat box
  for (int i = 0; i < NUM_PRIMARIES; i++)
  {
    //angle rotated through vs Z interaction Position
    TCanvas* c_Total_angle_rotated_vs_zpos =
        new TCanvas(
            "c_Total_angle_rotated_vs_zpos",
            Form(
                "Angle of rotation through the magnetic field lines vs Z interaction position for %s on the Dummy Detector",
                INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()));

    h_TOTAL_ANGLE_ROTATED_VS_ZPOS_THROUGH_DUMMY[i]->Draw();

    //move stat box
    gStyle->SetStatX(0.95);
    gStyle->SetStatY(0.40);
    //removes the histogram title in stat box
    gStyle->SetOptStat("emr");  //1110);

    c_Total_angle_rotated_vs_zpos->SaveAs(
        Form("%s/Dummy_Total_angle_rotated_vs_zpos_%s.png",
             OUTPUT_PREFIX_SCRIPT.c_str(),
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()));
    c_Total_angle_rotated_vs_zpos->SaveAs(
        Form("%s/Dummy_Total_angle_rotated_vs_zpos_%s.C",
             OUTPUT_PREFIX_SCRIPT.c_str(),
             INDEX_TO_NUM_PRIMARIES_TO_NAME[i].c_str()));
  }

  return;
}

/*********
 * LEFT/RIGHT STUFF
 *********/
