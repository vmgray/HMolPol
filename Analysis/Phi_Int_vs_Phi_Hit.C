/***********************************************************
 Programmer: Valerie Gray
 Purpose:
 - To take a root file (that now only has the dummy det in it.
 And plot the difference in the phi angle that is at the interaction of the
 scatter and then the phi angle that it has at the detector when it reaches
 it and makes a hit.

 - NOTE: to get information on only the primary particles look at
 the first hit of any event in the on a detector. This is NOT a true primary
 rater a naive primary. True primaries will come later.

 Entry Conditions: Root files name
 Date: 02-18-2016
 Modified: 03-08-2016
 Assisted By: Me!
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
#include "HMolPolEventPrimaryConditions.hh"
#include "HMolPolRunInformation.hh"

//HMolPol Macro includes
#include "ExtractFromRunInfo.h"

//Function definitions
void GetGitInfo();
void Define_Histograms();
void Loop_Through_Tree();
void Plot();

//Globals
//Length is for debugging at this point :)
//define units used
#define ANGLE_UNIT Units->deg
#define LENGTH_UNIT Units->m
#define ENERGY_UNIT Units->GeV

std::string ANGLE_UNIT_NAME = "deg";  //units for angle
std::string LENGTH_UNIT_NAME = "m";  //units for length
std::string ENERGY_UNIT_NAME = "GeV";  //units for length

//define the TChain
TChain* HMOLPOL_CHAIN;

std::vector<std::vector<TH1D*> > h_PHI_HIT_1_2_DIFFERECE;  //[primary e][CM theta]
//histogram for the difference between the phi hit for particle 1 and 2

std::vector<TH2D*> h_PHI_HIT_1_2_DIFFERECE_VS_THETA_CM;  //primary e-
//histogram of the difference between the phi hit for particle 1 and 2 vs
//the center of mass theta angle

std::vector<TH2D*> h_PHI_HIT_1_2_DIFFERECE_VS_THETA_CM_ZOOM;  //primary e-
//histogram of the difference between the phi hit for particle 1 and 2 vs
//the center of mass theta angle

std::vector<TH2D*> h_PHI_HIT_1_2_DIFFERECE_VS_ENERGY_INT;  //primary e-
//histogram of the difference between the phi hit for particle 1 and 2 vs
//the interaction energy

std::vector<TH2D*> h_PHI_HIT_1_2_DIFFERECE_VS_ENERGY_INT_ZOOM;  //primary e-
//histogram of the difference between the phi hit for particle 1 and 2 vs
//the interaction energy

static const Int_t NUM_PRIMARIES = 3;  //number of primary electrons + 1

//Get the CM angles,
//min and max of the simulation, the # of angles it covers
Double_t CM_THETA_MIN = 75;  //deg
Double_t CM_THETA_MAX = 105;  //deg
Double_t CM_THETA_RANGE = CM_THETA_MAX - CM_THETA_MIN;

//the number of CM theta angles we will have histograms of
static const Int_t NUM_CM_ANGLES = 6;  //Number of ROOT files aka magnetic fields of interst

//CM angle intervals amount
Int_t CM_ANGLES_INTERVAL = CM_THETA_RANGE / NUM_CM_ANGLES;
Int_t CM_ANGLES_BUFFER = CM_ANGLES_INTERVAL / 2;  //the amount around the CM_ANGLE value snap shot will be

Int_t CM_ANGLES[NUM_CM_ANGLES + 1];

//string for the root flle name to be shared with 2 functions
static std::string FILENAME;

//string for the where the input file is stored.
static std::string INPUT_PREFIX =
    "/home/vmgray/workspace/HMolPol/Simulation_Analysis/commit_90fe353";

//string for the where the output will go.
static std::string OUTPUT_PREFIX =
    "/home/vmgray/workspace/HMolPol/Simulation_Analysis/commit_90fe353";

//HMolPol include - Must be here as it needs to know about OUTPUT_PREFIX
#include "ExtractInitalConditions.h"

void Phi_int_vs_Phi_Hit(std::string filename)
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

  HMOLPOL_CHAIN->Add(
      "/home/vmgray/workspace/HMolPol/Simulation_Analysis/commit_90fe353/*.root");

  //HMOLPOL_CHAIN->Add(Form("%s", FILENAME.c_str()));

  //fill the Theta CM array
  for (int i = 0; i < NUM_CM_ANGLES + 1; i++)
  {
    CM_ANGLES[i] = CM_THETA_MIN + (i * CM_ANGLES_INTERVAL);
  }

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

/***********************************************************
 Function: Define_Histograms
 Purpose:
 To define all the histograms,
 ie. name, axis labels

 Entry Conditions:
 none

 Global:

 - h_PHI_HIT_1_2_DIFFERECE - histogram for the difference between the
 phi hit for particle 1 and 2
 - h_PHI_HIT_1_2_DIFFERECE_VS_THETA_CM - histogram of the difference between
 the phi hit for particle 1 and 2 vs the center of mass theta angle
 - h_PHI_HIT_1_2_DIFFERECE_VS_THETA_CM_ZOOM - histogram of the difference between
 the phi hit for particle 1 and 2 vs the center of mass theta angle
  - h_PHI_HIT_1_2_DIFFERECE_VS_ENERGY_INT - histogram of the difference between
 the phi hit for particle 1 and 2 vs the interaction
 - h_PHI_HIT_1_2_DIFFERECE_VS_ENERGY_INT_ZOOM - histogram of the difference between
 the phi hit for particle 1 and 2 vs the interaction energy
 - ANGLE_UNIT - units for length
 - NUM_PRIMARIES - Number of the Primaries e-
 - NUM_CM_ANGLES - Num of CM theta angles
 - CM_ANGLES_BUFFER - the amount around the CM_ANGLE value snap shot will be

 Exit Conditions: none
 Called By: Phi_int_vs_Phi_Hit
 Date: 02-18-2016
 Modified: 03-08-2016
 *********************************************************/
void Define_Histograms()
{
  //resize vectors
  h_PHI_HIT_1_2_DIFFERECE_VS_THETA_CM.resize(NUM_PRIMARIES);
  h_PHI_HIT_1_2_DIFFERECE_VS_THETA_CM_ZOOM.resize(NUM_PRIMARIES);
  h_PHI_HIT_1_2_DIFFERECE_VS_ENERGY_INT.resize(NUM_PRIMARIES);
  h_PHI_HIT_1_2_DIFFERECE_VS_ENERGY_INT_ZOOM.resize(NUM_PRIMARIES);
  h_PHI_HIT_1_2_DIFFERECE.resize(NUM_PRIMARIES);
  for (int i = 0; i < NUM_PRIMARIES; i++)
  {
    //fill in the names of the h_PHI_HIT_1_2_DIFFERECE_VS_THETA_CM histograms
    h_PHI_HIT_1_2_DIFFERECE_VS_THETA_CM[i] =
        new TH2D(
            Form("h_PHI_HIT_1_2_DIFFERECE_VS_THETA_CM[%d]", i),
            Form(
                "Difference in the #phi angle at the detector hit for the primary electron 1 and 2 vs #theta_{CM} for primary %d",
                i),
            CM_THETA_RANGE + 6, CM_THETA_MIN - 3, CM_THETA_MAX + 3, 370, -5,
            365);

    h_PHI_HIT_1_2_DIFFERECE_VS_THETA_CM[i]->GetXaxis()->SetTitle(
        Form("#theta_{CM} of primary %d (%s)", i, ANGLE_UNIT_NAME.c_str()));

    h_PHI_HIT_1_2_DIFFERECE_VS_THETA_CM[i]->GetYaxis()->SetTitle(
        Form("#phi_{1} -  #phi_{2} (%s)", ANGLE_UNIT_NAME.c_str()));

    //fill in the names of the h_PHI_HIT_1_2_DIFFERECE_VS_THETA_CM histograms
    h_PHI_HIT_1_2_DIFFERECE_VS_THETA_CM_ZOOM[i] =
        new TH2D(
            Form("h_PHI_HIT_1_2_DIFFERECE_VS_THETA_CM_ZOOM[%d]", i),
            Form(
                "Difference in the #phi angle at the detector hit for the primary electron 1 and 2 vs #theta_{CM} for primary %d",
                i),
            CM_THETA_RANGE + 6, CM_THETA_MIN - 3, CM_THETA_MAX + 3, 140, 145,
            215);

    h_PHI_HIT_1_2_DIFFERECE_VS_THETA_CM_ZOOM[i]->GetXaxis()->SetTitle(
        Form("#theta_{CM} of primary %d (%s)", i, ANGLE_UNIT_NAME.c_str()));

    h_PHI_HIT_1_2_DIFFERECE_VS_THETA_CM_ZOOM[i]->GetYaxis()->SetTitle(
        Form("#phi_{1} -  #phi_{2} (%s)", ANGLE_UNIT_NAME.c_str()));

    //fill in the names of the h_PHI_HIT_1_2_DIFFERECE_VS_ENERGY_INT histograms
    h_PHI_HIT_1_2_DIFFERECE_VS_ENERGY_INT[i] =
        new TH2D(
            Form("h_PHI_HIT_1_2_DIFFERECE_VS_ENERGY_INT[%d]", i),
            Form(
                "Difference in the #phi angle at the detector hit for the primary electron 1 and 2 vs interaction energy for primary %d",
                i),
                600, 0.7, 1.3, 370, -5, 365);

    h_PHI_HIT_1_2_DIFFERECE_VS_ENERGY_INT[i]->GetXaxis()->SetTitle(
        Form("Interaction energy of primary %d (%s)", i, ENERGY_UNIT_NAME.c_str()));

    h_PHI_HIT_1_2_DIFFERECE_VS_ENERGY_INT[i]->GetYaxis()->SetTitle(
        Form("#phi_{1} -  #phi_{2} (%s)", ANGLE_UNIT_NAME.c_str()));

    //fill in the names of the h_PHI_HIT_1_2_DIFFERECE_VS_ENERGY_INT_ZOOM histograms
    h_PHI_HIT_1_2_DIFFERECE_VS_ENERGY_INT_ZOOM[i] =
        new TH2D(
            Form("h_PHI_HIT_1_2_DIFFERECE_VS_ENERGY_INT_ZOOM[%d]", i),
            Form(
                "Difference in the #phi angle at the detector hit for the primary electron 1 and 2 vs Interaction energy for primary %d",
                i),
                600, 0.7, 1.3, 140, 145, 215);

    h_PHI_HIT_1_2_DIFFERECE_VS_ENERGY_INT_ZOOM[i]->GetXaxis()->SetTitle(
        Form("Interaction energy of primary %d (%s)", i, ENERGY_UNIT_NAME.c_str()));

    h_PHI_HIT_1_2_DIFFERECE_VS_ENERGY_INT_ZOOM[i]->GetYaxis()->SetTitle(
        Form("#phi_{1} -  #phi_{2} (%s)", ANGLE_UNIT_NAME.c_str()));

    //resize vectors
    h_PHI_HIT_1_2_DIFFERECE[i].resize(NUM_CM_ANGLES + 1);
    //add the title and label axises
    for (int j = 0; j < NUM_CM_ANGLES + 1; j++)
    {
      h_PHI_HIT_1_2_DIFFERECE[i][j] =
          new TH1D(
              Form("h_PHI_HIT_1_2_DIFFERECE[%d][%d]", i, j),
              Form(
                  "Difference in the #phi angle at the detector hit for "
                  "the primary electron 1 and 2 scattered #theta_{CM} of primary %d = %d #pm %d^{#circ}",
                  i, CM_ANGLES[j], CM_ANGLES_BUFFER),
              370, -5, 365);

      h_PHI_HIT_1_2_DIFFERECE[i][j]->GetXaxis()->SetTitle(
          Form("#phi_{1} -  #phi_{2} (%s)", ANGLE_UNIT_NAME.c_str()));
    }
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
 - h_PHI_HIT_1_2_DIFFERECE - histogram for the difference between the
 phi hit for particle 1 and 2
 - h_PHI_HIT_1_2_DIFFERECE_VS_THETA_CM - histogram of the difference between
 the phi hit for particle 1 and 2 vs the center of mass theta angle
 - h_PHI_HIT_1_2_DIFFERECE_VS_THETA_CM_ZOOM - histogram of the difference between
 the phi hit for particle 1 and 2 vs the center of mass theta angle
 - h_PHI_HIT_1_2_DIFFERECE_VS_ENERGY_INT - histogram of the difference between
 the phi hit for particle 1 and 2 vs the interaction
 - h_PHI_HIT_1_2_DIFFERECE_VS_ENERGY_INT_ZOOM - histogram of the difference between
 the phi hit for particle 1 and 2 vs the interaction energy
 - ANGLE_UNIT - units for length
 - NUM_CM_ANGLES - Num of CM theta angles
 - CM_ANGLES - the CM theta angles that the "snap shots" will be at
 - CM_ANGLES_BUFFER - the amount around the CM_ANGLE value snap shot will be

 Exit Conditions: none
 Called By: Phi_int_vs_Phi_Hit
 Date: 02-19-2016
 Modified: 03-08-2016
 *********************************************************/
void Loop_Through_Tree()
{
  /*
   //TODO debugging
   std::ofstream fout;

   fout.open(Form("%s/Angle_debugging.txt", OUTPUT_PREFIX.c_str()));
   //fout.open(OUTPUT_PREFIX + "Angle_debugging.txt");
   if (!fout.is_open())
   std::cout << "File not opened" << std::endl;
   */

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

    //the phi angle for hit 1 and 2
    Double_t phi_hit_Primary_1_Val = 0.0;
    Double_t phi_hit_Primary_2_Val = 0.0;

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
                /*
                 std::cout << "X Position " << Dummy_Det->fHits[i_hits].fPosition.X()
                 << " " << Dummy_Det->fHits[i_hits].fPosition.X() / LENGTH_UNIT
                 << " m " << std::endl << "Y Position "
                 << Dummy_Det->fHits[i_hits].fPosition.Y() / LENGTH_UNIT
                 << " m " << std::endl;
                 */

                //Now for the main event!!!
                //Calculate the Hit phi value for primary 1 :)
                Double_t phi_Primary_1 =
                    TMath::ATan(
                        (Dummy_Det->fHits[i_hits].fPosition.Y() / LENGTH_UNIT) / (Dummy_Det->fHits[i_hits].fPosition.X()
                            / LENGTH_UNIT))
                    / ANGLE_UNIT;

                //If the X position is positive
                if (Dummy_Det->fHits[i_hits].fPosition.X() / LENGTH_UNIT > 0
                    / LENGTH_UNIT)
                {
                  //If the Y position is positive
                  if (Dummy_Det->fHits[i_hits].fPosition.Y() / LENGTH_UNIT > 0
                      / LENGTH_UNIT)
                  {
                    //we are in the first quadrant
                    /*********
                     * phi is POSITIVE here and doesn't need to change
                     * Then phi doesn't change phi_hit_Primary_1 = phi_hit_Primary_1
                     *********/
                    phi_hit_Primary_1_Val = phi_Primary_1;

                    //debug
                    if (!(phi_hit_Primary_1_Val >= 0
                        && phi_hit_Primary_1_Val <= 90))
                    {
                      std::cout << "Primary 1 Quadrant 1, phi: "
                                << phi_hit_Primary_1_Val << std::endl;
                    }

                  } else if (Dummy_Det->fHits[i_hits].fPosition.Y()
                      / LENGTH_UNIT
                             < 0 / LENGTH_UNIT)
                  {
                    //y is negative
                    /*********
                     * we are in the second quadrant. phi is NEGATIVE here.
                     * We want it to be between 0 and 360.
                     * so phi_hit_Primary_1 = 180 - abs(phi_Primary_1)
                     *
                     * or specifically here
                     * phi_hit_Primary_1 = 180 + phi_Primary_1
                     *********/
                    phi_hit_Primary_1_Val = 180 + phi_Primary_1;

                    //debug
                    if (!(phi_hit_Primary_1_Val >= 90
                        && phi_hit_Primary_1_Val <= 180))
                    {
                      std::cout << "Primary 1 Quadrant 2, phi: "
                                << phi_hit_Primary_1_Val << std::endl;
                    }
                  }  //end of second quad if for primary 1
                } else if (Dummy_Det->fHits[i_hits].fPosition.X() / LENGTH_UNIT
                    < 0 / LENGTH_UNIT)
                {
                  //the X position is negative

                  if (Dummy_Det->fHits[i_hits].fPosition.Y() / LENGTH_UNIT < 0
                      / LENGTH_UNIT)
                  {
                    //y is negative
                    /*********
                     * we are in the third quadrant. phi is POSITIVE here.
                     * We want it to be between 0 and 360.
                     * so phi_hit_Primary_1 = 180 + abs(phi_Primary_1)
                     *
                     * or specifically:
                     * phi_hit_Primary_1 = 180 + phi_Primary_1;
                     *********/
                    phi_hit_Primary_1_Val = 180 + phi_Primary_1;

                    //debug
                    if (!(phi_hit_Primary_1_Val >= 180
                        && phi_hit_Primary_1_Val <= 270))
                    {
                      std::cout << "Primary 1 Quadrant 3, phi: "
                                << phi_hit_Primary_1_Val << std::endl;
                    }

                  } else if (Dummy_Det->fHits[i_hits].fPosition.Y()
                      / LENGTH_UNIT
                             > 0 / LENGTH_UNIT)
                  {
                    //y is negative
                    /*********
                     * we are in the fourth quadrant. phi is NEGATIVE here.
                     * We want it to be between 0 and 360.
                     * so phi_hit_Primary_1 = 360 - abs(phi_Primary_1)
                     *
                     * or specifically:
                     * phi_hit_Primary_1 = 360 + phi_Primary_1;
                     *********/
                    //we are in the fourth quadrant
                    //phi is positive here and doesn't need to change
                    //Then phi doesn't change phi_hit_Primary_1 = phi_hit_Primary_1
                    phi_hit_Primary_1_Val = 360 + phi_Primary_1;

                    //debug
                    if (!(phi_hit_Primary_1_Val >= 270
                        && phi_hit_Primary_1_Val <= 360))
                    {
                      std::cout << "Primary 1 Quadrant 4, phi: "
                                << phi_hit_Primary_1_Val << std::endl;
                    }
                  }  //end of if 4th quad primary 1
                }  //end of if x pos < 0 for primary 1
              }  //if first hit for Primary 1
            }  //if hit from primary 1

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
                /*
                 std::cout << "X Position " << Dummy_Det->fHits[i_hits].fPosition.X()
                 << " " << Dummy_Det->fHits[i_hits].fPosition.X() / LENGTH_UNIT
                 << " m " << std::endl << "Y Position "
                 << Dummy_Det->fHits[i_hits].fPosition.Y() / LENGTH_UNIT
                 << " m " << std::endl;
                 */

                //Now for the main event!!!
                //Calculate the Hit phi value for primary 1 :)
                Double_t phi_Primary_2 =
                    TMath::ATan(
                        (Dummy_Det->fHits[i_hits].fPosition.Y() / LENGTH_UNIT) / (Dummy_Det->fHits[i_hits].fPosition.X()
                            / LENGTH_UNIT))
                    / ANGLE_UNIT;

                //If the X position is positive
                if (Dummy_Det->fHits[i_hits].fPosition.X() / LENGTH_UNIT > 0
                    / LENGTH_UNIT)
                {
                  //If the Y position is positive
                  if (Dummy_Det->fHits[i_hits].fPosition.Y() / LENGTH_UNIT > 0
                      / LENGTH_UNIT)
                  {
                    //we are in the first quadrant
                    /*********
                     * phi is POSITIVE here and doesn't need to change
                     * Then phi doesn't change phi_hit_Primary_2 = phi_hit_Primary_2
                     *********/
                    phi_hit_Primary_2_Val = phi_Primary_2;

                    //debug
                    if (!(phi_hit_Primary_2_Val >= 0
                        && phi_hit_Primary_2_Val <= 90))
                    {
                      std::cout << "Primary 2 Quadrant 1, phi: "
                                << phi_hit_Primary_2_Val << std::endl;
                    }

                  } else if (Dummy_Det->fHits[i_hits].fPosition.Y()
                      / LENGTH_UNIT
                             < 0 / LENGTH_UNIT)
                  {
                    //y is negative
                    /*********
                     * we are in the second quadrant. phi is NEGATIVE here.
                     * We want it to be between 0 and 360.
                     * so phi_hit_Primary_2 = 180 - abs(phi_Primary_2)
                     *
                     * or specifically here
                     * phi_hit_Primary_2 = 180 + phi_Primary_2
                     *********/
                    phi_hit_Primary_2_Val = 180 + phi_Primary_2;

                    //debug
                    if (!(phi_hit_Primary_2_Val >= 90
                        && phi_hit_Primary_2_Val <= 180))
                    {
                      std::cout << "Primary 2 Quadrant 2, phi: "
                                << phi_hit_Primary_2_Val << std::endl;
                    }

                  }  //end second quad primary 2
                } else if (Dummy_Det->fHits[i_hits].fPosition.X() / LENGTH_UNIT
                    < 0 / LENGTH_UNIT)
                {
                  //the X position is negative

                  if (Dummy_Det->fHits[i_hits].fPosition.Y() / LENGTH_UNIT < 0
                      / LENGTH_UNIT)
                  {
                    //y is negative
                    /*********
                     * we are in the third quadrant. phi is POSITIVE here.
                     * We want it to be between 0 and 360.
                     * so phi_hit_Primary_2 = 180 + abs(phi_Primary_2)
                     *
                     * or specifically:
                     * phi_hit_Primary_2 = 180 + phi_Primary_2;
                     *********/
                    phi_hit_Primary_2_Val = 180 + phi_Primary_2;

                    //debug
                    if (!(phi_hit_Primary_2_Val >= 180
                        && phi_hit_Primary_2_Val <= 270))
                    {
                      std::cout << "Primary 2 Quadrant 3, phi: "
                                << phi_hit_Primary_2_Val << std::endl;
                    }

                  } else if (Dummy_Det->fHits[i_hits].fPosition.Y()
                      / LENGTH_UNIT
                             > 0 / LENGTH_UNIT)
                  {
                    //y is negative
                    /*********
                     * we are in the fourth quadrant. phi is NEGATIVE here.
                     * We want it to be between 0 and 360.
                     * so phi_hit_Primary_2 = 360 - abs(phi_Primary_2)
                     *
                     * or specifically:
                     * phi_hit_Primary_2 = 360 + phi_Primary_2;
                     *********/
                    //we are in the fourth quadrant
                    //phi is positive here and doesn't need to change
                    //Then phi doesn't change phi_hit_Primary_2 = phi_hit_Primary_2
                    phi_hit_Primary_2_Val = 360 + phi_Primary_2;

                    //debug
                    if (!(phi_hit_Primary_2_Val >= 270
                        && phi_hit_Primary_2_Val <= 360))
                    {
                      std::cout << "Primary 2 Quadrant 4, phi: "
                                << phi_hit_Primary_2_Val << std::endl;
                    }
                  }  //end if for quad 4 primary 2
                }  //end if x negative primary 2
              }  //end of if first hit for primary 2
            }  //end of if hit from primary 2
          }  //end of if parentID==0
        }  //loop over hits in the dummy
      }  // if dummy det has a hit
    }  // if we have a hit in the dummy det

    /********
     * Fil histograms There is one entry for each event
     *
     * IFF there is both a phi hit value can these be stored
     ********/

    //calculate the difference in the phi angles
    /*
     Double_t phi_hit_Diff_Val = TMath::Abs(
     phi_hit_Primary_1_Val - phi_hit_Primary_2_Val);
     //Double_t phi_hit_Diff_Val = phi_hit_Primary_1_Val - phi_hit_Primary_2_Val;
     */

    /********
     * if (phi_hit_Primary_1_Val - phi_hit_Primary_2_Val) > 0
     * then
     *   phi_hit_Diff_Val = phi_hit_Primary_1_Val - phi_hit_Primary_2_Val
     * else
     *   phi_hit_Diff_Val = (phi_hit_Primary_1_Val - phi_hit_Primary_2_Val) + 360
     *********/

    Double_t phi_hit_Diff_Val =
        ((phi_hit_Primary_1_Val - phi_hit_Primary_2_Val) > 0) ?
            (phi_hit_Primary_1_Val - phi_hit_Primary_2_Val) :
            (phi_hit_Primary_1_Val - phi_hit_Primary_2_Val) + 360;

    if (Primary_1_Hit_Num != 1000 && Primary_2_Hit_Num != 1000)
    {
      /*
       //debugging
       if (!(TMath::Abs(phi_hit_Primary_1_Val - phi_hit_Primary_2_Val) >= 0
       && TMath::Abs(phi_hit_Primary_1_Val - phi_hit_Primary_2_Val) <= 360))
       {
       fout << "Entry #: " << i_entry << std::endl << "  Phi 1 - Phi 2 "
       << phi_hit_Primary_1_Val << " - " << phi_hit_Primary_2_Val << " = "
       << phi_hit_Primary_1_Val - phi_hit_Primary_2_Val << std::endl;
       }
       */

      //For the theta snap shots
      for (int i_theta = 0; i_theta < NUM_CM_ANGLES + 1; i_theta++)
      {
        //Primary 1
        if (Primary->fThetaCenterOfMass1 / ANGLE_UNIT < (CM_ANGLES[i_theta]
            + CM_ANGLES_BUFFER)
            && Primary->fThetaCenterOfMass1 / ANGLE_UNIT > (CM_ANGLES[i_theta]
                - CM_ANGLES_BUFFER))
        {
          //fill the phi histogram snap shots
          h_PHI_HIT_1_2_DIFFERECE[1][i_theta]->Fill(phi_hit_Diff_Val);

          /*
           h_PHI_HIT_1_2_DIFFERECE[1][i_theta]->Fill(
           phi_hit_Primary_1_Val - phi_hit_Primary_2_Val);
           */

        }  //end of if this is a theta angle of interest for primary 1

        //Primary 2
        if (Primary->fThetaCenterOfMass2 / ANGLE_UNIT < (CM_ANGLES[i_theta]
            + CM_ANGLES_BUFFER)
            && Primary->fThetaCenterOfMass2 / ANGLE_UNIT > (CM_ANGLES[i_theta]
                - CM_ANGLES_BUFFER))
        {
          //fill the phi histogram snap shots
          h_PHI_HIT_1_2_DIFFERECE[2][i_theta]->Fill(phi_hit_Diff_Val);

          /*
           h_PHI_HIT_1_2_DIFFERECE[2][i_theta]->Fill(
           phi_hit_Primary_1_Val - phi_hit_Primary_2_Val);
           */

        }  //end of if this is a theta angle of interest for primary 2
      }  // end of loop over theta CM intervals

      //Now for the histogram of just the differences vs theta CM for particle 1
      h_PHI_HIT_1_2_DIFFERECE_VS_THETA_CM[1]->Fill(
          Primary->fThetaCenterOfMass1 / ANGLE_UNIT, phi_hit_Diff_Val);
      /*
       h_PHI_HIT_1_2_DIFFERECE_VS_THETA_CM[1]->Fill(
       Primary->fThetaCenterOfMass1 / ANGLE_UNIT,
       phi_hit_Primary_1_Val - phi_hit_Primary_2_Val);
       */

      h_PHI_HIT_1_2_DIFFERECE_VS_THETA_CM_ZOOM[1]->Fill(
          Primary->fThetaCenterOfMass1 / ANGLE_UNIT, phi_hit_Diff_Val);

      //energy plots
      h_PHI_HIT_1_2_DIFFERECE_VS_ENERGY_INT[1]->Fill(
          Primary->fInteractionVertexMomentum1.Mag() / ENERGY_UNIT, phi_hit_Diff_Val);

      h_PHI_HIT_1_2_DIFFERECE_VS_ENERGY_INT_ZOOM[1]->Fill(
          Primary->fInteractionVertexMomentum1.Mag() / ENERGY_UNIT, phi_hit_Diff_Val);

      //Now for the histogram of just the differences vs theta CM for particle 2
      h_PHI_HIT_1_2_DIFFERECE_VS_THETA_CM[2]->Fill(
          Primary->fThetaCenterOfMass2 / ANGLE_UNIT, phi_hit_Diff_Val);

      /*
       h_PHI_HIT_1_2_DIFFERECE_VS_THETA_CM[2]->Fill(
       Primary->fThetaCenterOfMass2 / ANGLE_UNIT,
       phi_hit_Primary_1_Val - phi_hit_Primary_2_Val);
       */

      h_PHI_HIT_1_2_DIFFERECE_VS_THETA_CM_ZOOM[2]->Fill(
          Primary->fThetaCenterOfMass2 / ANGLE_UNIT, phi_hit_Diff_Val);

      //energy plots
      h_PHI_HIT_1_2_DIFFERECE_VS_ENERGY_INT[2]->Fill(
          Primary->fInteractionVertexMomentum2.Mag() / ENERGY_UNIT, phi_hit_Diff_Val);

      h_PHI_HIT_1_2_DIFFERECE_VS_ENERGY_INT_ZOOM[2]->Fill(
          Primary->fInteractionVertexMomentum2.Mag() / ENERGY_UNIT, phi_hit_Diff_Val);

    }  // end of the do we have info for both primaries
  }  //end loop over entries in the tree

  return;
}

/***********************************************************
 Function: Plot
 Purpose:
 To plot the graphs and save those plots

 Entry Conditions:
 none

 Global:
 - h_PHI_HIT_1_2_DIFFERECE - histogram for the difference between the
 phi hit for particle 1 and 2
 - h_PHI_HIT_1_2_DIFFERECE_VS_THETA_CM - histogram of the difference between
 the phi hit for particle 1 and 2 vs the center of mass theta angle
 - h_PHI_HIT_1_2_DIFFERECE_VS_THETA_CM_ZOOM - histogram of the difference between
 the phi hit for particle 1 and 2 vs the center of mass theta angle
 - h_PHI_HIT_1_2_DIFFERECE_VS_ENERGY_INT - histogram of the difference between
 the phi hit for particle 1 and 2 vs the interaction
 - h_PHI_HIT_1_2_DIFFERECE_VS_ENERGY_INT_ZOOM - histogram of the difference between
 the phi hit for particle 1 and 2 vs the interaction energy
 - ANGLE_UNIT - units for length
 - ENERGY_UNIT - units for energy
 - NUM_CM_ANGLES - Num of CM theta angles

 Exit Conditions: none
 Called By: Phi_int_vs_Phi_Hit
 Date: 02-22-2016
 Modified: 02/23/2016
 *********************************************************/
void Plot()
{
  //debugging
  //std::cout << "made it in the make plotting function" << std::endl;

  //define Canvases
  //loop over primaries
  for (int i = 1; i < NUM_PRIMARIES; i++)
  {
    //phi 1 2 diff vs Theta CM
    TCanvas* c_Phi_Hit_1_2_Diff_ThetaCM = new TCanvas(
        "c_Phi_Hit_1_2_Diff_ThetaCM",
        Form("Difference in the #phi angle at the detector hit for the "
             "primary electron 1 and 2 vs #theta_{CM} for Primary electron %d",
             i));

    h_PHI_HIT_1_2_DIFFERECE_VS_THETA_CM[i]->Draw();

    c_Phi_Hit_1_2_Diff_ThetaCM->SaveAs(
        Form("%s/Phi_Hit_1_2_Diff_vs_Theta_CM_primary_%d.png",
             OUTPUT_PREFIX.c_str(), i));
    c_Phi_Hit_1_2_Diff_ThetaCM->SaveAs(
        Form("%s/Phi_Hit_1_2_Diff_vs_Theta_CM_primary_%d.C",
             OUTPUT_PREFIX.c_str(), i));

    //phi 1 2 diff vs Theta CM Zoom
    TCanvas* c_Phi_Hit_1_2_Diff_ThetaCM_Zoom = new TCanvas(
        "c_Phi_Hit_1_2_Diff_ThetaCM_Zoom",
        Form("Difference in the #phi angle at the detector hit for the "
             "primary electron 1 and 2 vs #theta_{CM} for Primary electron %d",
             i));

    h_PHI_HIT_1_2_DIFFERECE_VS_THETA_CM_ZOOM[i]->Draw();

    c_Phi_Hit_1_2_Diff_ThetaCM_Zoom->SaveAs(
        Form("%s/Phi_Hit_1_2_Diff_vs_Theta_CM_primary_%d_Zoom.png",
             OUTPUT_PREFIX.c_str(), i));
    c_Phi_Hit_1_2_Diff_ThetaCM_Zoom->SaveAs(
        Form("%s/Phi_Hit_1_2_Diff_vs_Theta_CM_primary_%d_Zoom.C",
             OUTPUT_PREFIX.c_str(), i));
    //ENERGY
    //phi 1 2 diff vs EnergyInt
    TCanvas* c_Phi_Hit_1_2_Diff_EnergyInt = new TCanvas(
        "c_Phi_Hit_1_2_Diff_EnergyInt",
        Form("Difference in the #phi angle at the detector hit for the "
             "primary electron 1 and 2 vs interaction energy for Primary electron %d",
             i));

    h_PHI_HIT_1_2_DIFFERECE_VS_ENERGY_INT[i]->Draw();

    c_Phi_Hit_1_2_Diff_EnergyInt->SaveAs(
        Form("%s/Phi_Hit_1_2_Diff_vs_int_energy_primary_%d.png",
             OUTPUT_PREFIX.c_str(), i));
    c_Phi_Hit_1_2_Diff_EnergyInt->SaveAs(
        Form("%s/Phi_Hit_1_2_Diff_vs_int_energy_primary_%d.C",
             OUTPUT_PREFIX.c_str(), i));

    //phi 1 2 diff vs EnergyInt Zoom
    TCanvas* c_Phi_Hit_1_2_Diff_EnergyInt_Zoom = new TCanvas(
        "c_Phi_Hit_1_2_Diff_EnergyInt_Zoom",
        Form("Difference in the #phi angle at the detector hit for the "
             "primary electron 1 and 2 vs Interaction energy for Primary electron %d",
             i));

    h_PHI_HIT_1_2_DIFFERECE_VS_ENERGY_INT_ZOOM[i]->Draw();

    c_Phi_Hit_1_2_Diff_EnergyInt_Zoom->SaveAs(
        Form("%s/Phi_Hit_1_2_Diff_vs_int_energy_primary_%d_Zoom.png",
             OUTPUT_PREFIX.c_str(), i));
    c_Phi_Hit_1_2_Diff_EnergyInt_Zoom->SaveAs(
        Form("%s/Phi_Hit_1_2_Diff_vs_int_energy_primary_%d_Zoom.C",
             OUTPUT_PREFIX.c_str(), i));

    //loop over CM angles
    //Start with the snap shots
    for (int j = 0; j < NUM_CM_ANGLES + 1; j++)
    {
      //Phi 1 2 Difference
      TCanvas* c_Phi_Hit_1_2_Diff = new TCanvas(
          "c_Phi_Hit_1_2_Diff",
          Form("Difference in the #phi angle for a hit at the detector for the "
               "primary electron 1 and 2 (%s)",
               ANGLE_UNIT_NAME.c_str()));

      h_PHI_HIT_1_2_DIFFERECE[i][j]->Draw();

      c_Phi_Hit_1_2_Diff->SaveAs(
          Form("%s/Phi_Hit_1_2_Diff_for_Theta_CM_Primary%d_%d.png",
               OUTPUT_PREFIX.c_str(), i, CM_ANGLES[j]));
      c_Phi_Hit_1_2_Diff->SaveAs(
          Form("%s/Phi_Hit_1_2_Diff_for_Theta_CM_Primary%d_%d.C",
               OUTPUT_PREFIX.c_str(), i, CM_ANGLES[j]));
    }  //end of the snap shot loop
  }
  return;
}
