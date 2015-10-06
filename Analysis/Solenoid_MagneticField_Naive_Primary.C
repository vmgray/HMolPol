/***********************************************************
 Programmer: Valerie Gray
 Purpose:
 - Take a root file and create a plot at the dummy detector
 of the Y vs X axis from the primary particles.

 - Create a plot of the Momentums of the primary particles at the
 dummy detector.

 - For both of these, also make plots that cut on the CM theta scattering
 angle.

 - Taking all the rootfiles for the different Solenoid Magnetic fields
 strengths

 - NOTE: to get information on only the primary particles look at
 the first hit of any event in the dummy detector. This is NOT a true primary
 rater a naive primary. True primaries will come later.

 Entry Conditions: none
 Date: 08-25-2015
 Modified: 09-02-2015
 Assisted By:
 *********************************************************/

//Root includes
#include <TSystem.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TH2D.h>
#include <TH1D.h>
#include <TMath.h>
#include <TLatex.h>
#include <TF1.h>
#include <TStyle.h>

//system includes
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>

//HMolPol includes
#include "HMolPolEventGenericDetector.hh"
#include "HMolPolEventUnits.hh"
#include "HMolPolEventPrimary.hh"

//Function definitions
void Define_Histograms();
void Loop_Through_Tree(TChain * hmolpol_chain, int Mag_Index);
void Plot(int Mag_Index);

//define units used
#define LENGTH_UNIT Units->m
#define ANGLE_UNIT Units->deg
#define MOMENTUM_UNIT Units->GeV

std::string LENGTH_UNIT_NAME = "m";  //units for length
std::string ANGLE_UNIT_NAME = "deg";  //units for angle
std::string MOMENTUM_UNIT_NAME = "GeV";  //units for Momentum

std::vector<TChain*> HMOLPOL_CHAIN;

std::vector<TH2D*> h_XY_PLANE_DISTRIBUTION;  //histogram for the XY plane
//distribution at the dummy detector

std::vector<TH2D*> h_XY_PLANE_DISTRIBUTION_THETA_CUT;  //histogram for the XY plane
//distribution at the dummy detector with a cut on CM theta angle

std::vector<TH2D*> h_RADIUS_VS_MOMENTUM;  //histogram for the radius vs Momentum
//distribution at the dummy detector

std::vector<TH2D*> h_RADIUS_VS_MOMENTUM_THETA_CUT;  //histogram for the radius
//vs Momentum at the dummy detector with a cut on CM theta angle

std::vector<TH1D*> h_MOMENTUM_DISTRIBUTION;  //histogram for the XY plane
//distribution at the dummy detector

std::vector<TH1D*> h_MOMENTUM_DISTRIBUTION_THETA_CUT;  //histogram for the XY plane
//distribution at the dummy detector with a cut on CM theta angle

//Fit functions for radius vs. Mometum plots
/*
std::vector<TF1*> f_RADIUS_VS_MOMENTUM;  //fit function for the radius vs Momentum
//distribution at the dummy detector
*/

std::vector<TF1*> f_RADIUS_VS_MOMENTUM_THETA_CUT;  //fit function for the radius
//vs Momentum at the dummy detector with a cut on CM theta angle

Int_t CM_THETA_MIN = 89;  //deg
Int_t CM_THETA_MAX = 91;  //deg

static const Int_t NUM_FILES = 7;  //Number of ROOT files aka magnetic fields of interst
Int_t MAG_FIELD_STRENGTH[NUM_FILES] = { 4, 8, 25, 50, 100, 200, 1000 };

void Solenoid_MagneticField_Naive_Primary()
{
  //define the historgrams
  Define_Histograms();

  //get the root files add them to the TChain
  HMOLPOL_CHAIN.resize(NUM_FILES);
  for (int i_files = 0; i_files < NUM_FILES; i_files++)
  //for (int i_files = 1; i_files < 2; i_files++)
  {

    HMOLPOL_CHAIN[i_files] = new TChain("HMolPol_Tree");
    //add the file


    HMOLPOL_CHAIN[i_files]->Add(
        Form(
            "~/workspace/HMolPol/SolenoidMagField_Study/HMolPol_Dummy_2GeV_MagField%dT_PhiCom0.root",
            MAG_FIELD_STRENGTH[i_files]));

    /*
    HMOLPOL_CHAIN[i_files]->Add(
    Form(
    "~/workspace/HMolPol/SolenoidMagField_Study/HMolPol_Dummy_2GeV_MagField%dT.root",
    MAG_FIELD_STRENGTH[i_files]));
    */

    /*
     HMOLPOL_CHAIN[i_files]->Add(
     Form(
     "~/workspace/HMolPol/SolenoidMagField_Study/HMolPol_Dummy_2GeV_MagField%dT_Theta1to179.root",
     MAG_FIELD_STRENGTH[i_files]));
     */

    Loop_Through_Tree(HMOLPOL_CHAIN[i_files], i_files);
    Plot(i_files);
  }

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
 - h_XY_PLANE_DISTRIBUTION_THETA_CUT  - Vector of Historgrams for XY dis with theta cuts
 - h_RADIUS_VS_MOMENTUM - histogram for the radius vs Momentum
 - h_RADIUS_VS_MOMENTUM_THETA_CUT - histogram for the radius vs Momentum CM theta cut
 - h_MOMENTUM_DISTRIBUTION - historgram of mometums
 - h_MOMENTUM_DISTRIBUTION_THETA_CUT - histograms of momentums with theta cuts
 - NUM_FILES - Number of files
 - MAG_FIELD_STRENGTH - Magnetic field values
 - LENGHT_UNIT_NAME - units for length
 - ANGLE_UNIT_NAME - units for angle
 - MOMENTUM_UNIT_NAME - units for Mometum
 - CM_THETA_MIN - Min CM theta angle
 - CM_THETA_MAX - Max CM theta angle

 - f_RADIUS_VS_MOMENTUM - functions momentum vs radius plot
 - f_RADIUS_VS_MOMENTUM_THETA_CUT - functions momentum vs radius plot with theta cut

 Exit Conditions: none
 Called By: Solenoid_MagneticField_Test
 Date: 08-26-2015
 Modified: 09-02-2015
 *********************************************************/
void Define_Histograms()
{
  //resize the vector
  h_XY_PLANE_DISTRIBUTION.resize(NUM_FILES);
  h_XY_PLANE_DISTRIBUTION_THETA_CUT.resize(NUM_FILES);
  h_RADIUS_VS_MOMENTUM.resize(NUM_FILES);
  h_RADIUS_VS_MOMENTUM_THETA_CUT.resize(NUM_FILES);
  h_MOMENTUM_DISTRIBUTION.resize(NUM_FILES);
  h_MOMENTUM_DISTRIBUTION_THETA_CUT.resize(NUM_FILES);

  //also define the fit functions
  //f_RADIUS_VS_MOMENTUM.resize(NUM_FILES);
  f_RADIUS_VS_MOMENTUM_THETA_CUT.resize(NUM_FILES);

  //loop over the magnetic fields
  for (int i = 0; i < NUM_FILES; i++)
  {
    //XY distribution
    h_XY_PLANE_DISTRIBUTION[i] = new TH2D(
        Form("h_XY_PLANE_DISTRIBUTION[%d]", i),
        Form("Y vs X hit distribution at the Dummy detector plan for "
             "a %dT Solenoid Magnetic Field",
             MAG_FIELD_STRENGTH[i]),
        100, -0.5, 0.5, 100, -0.5, 0.5);

    h_XY_PLANE_DISTRIBUTION[i]->GetYaxis()->SetTitle(
        Form("Y Position (%s)", LENGTH_UNIT_NAME.c_str()));
    h_XY_PLANE_DISTRIBUTION[i]->GetXaxis()->SetTitle(
        Form("X Position (%s)", LENGTH_UNIT_NAME.c_str()));

    //XY distribution with theta cut
    h_XY_PLANE_DISTRIBUTION_THETA_CUT[i] = new TH2D(
        Form("h_XY_PLANE_DISTRIBUTION_THETA_CUT[%d]", i),
        Form("Y vs X hit distribution at the Dummy detector plan for "
             "a %dT Solenoid Magnetic Field with CM #theta %d%s to %d%s",
             MAG_FIELD_STRENGTH[i], CM_THETA_MIN, ANGLE_UNIT_NAME.c_str(),
             CM_THETA_MAX, ANGLE_UNIT_NAME.c_str()),
        100, -0.2, 0.2, 100, -0.2, 0.2);

    h_XY_PLANE_DISTRIBUTION_THETA_CUT[i]->GetYaxis()->SetTitle(
        Form("Y Position (%s)", LENGTH_UNIT_NAME.c_str()));
    h_XY_PLANE_DISTRIBUTION_THETA_CUT[i]->GetXaxis()->SetTitle(
        Form("X Position (%s)", LENGTH_UNIT_NAME.c_str()));

    //Momentum vs radius distribution
    h_RADIUS_VS_MOMENTUM[i] = new TH2D(
        Form("h_RADIUS_VS_MOMENTUM[%d]", i),
        Form("Momentum vs radius at the Dummy detector plan for "
             "a %dT Solenoid Magnetic Field",
             MAG_FIELD_STRENGTH[i]),
        100, 1, -1, 100, 1, -1);

    h_RADIUS_VS_MOMENTUM[i]->GetYaxis()->SetTitle(
        Form("Radius (%s)", LENGTH_UNIT_NAME.c_str()));
    h_RADIUS_VS_MOMENTUM[i]->GetXaxis()->SetTitle(
        Form("Momentum (%s)", MOMENTUM_UNIT_NAME.c_str()));

    //Momentum vs radius with theta cut
    h_RADIUS_VS_MOMENTUM_THETA_CUT[i] = new TH2D(
        Form("h_RADIUS_VS_MOMENTUM_THETA_CUT[%d]", i),
        Form("Radius vs. Momentum at the Dummy detector plan for "
             "a %dT Solenoid Magnetic Field with CM #theta %d%s to %d%s",
             MAG_FIELD_STRENGTH[i], CM_THETA_MIN, ANGLE_UNIT_NAME.c_str(),
             CM_THETA_MAX, ANGLE_UNIT_NAME.c_str()),
        100, 1, -1, 100, 1, -1);

    h_RADIUS_VS_MOMENTUM_THETA_CUT[i]->GetYaxis()->SetTitle(
        Form("Radius (%s)", LENGTH_UNIT_NAME.c_str()));
    h_RADIUS_VS_MOMENTUM_THETA_CUT[i]->GetXaxis()->SetTitle(
        Form("Momentum (%s)", MOMENTUM_UNIT_NAME.c_str()));

    //Momentum distribution
    h_MOMENTUM_DISTRIBUTION[i] = new TH1D(
        Form("h_MOMENTUM_DISTRIBUTION[%d]", i),
        Form("Momentum distribution at the Dummy detector plan for "
             "a %dT Solenoid Magnetic Field",
             MAG_FIELD_STRENGTH[i]),
        100, 1, -1);

    h_MOMENTUM_DISTRIBUTION[i]->GetXaxis()->SetTitle(
        Form("Momentum (%s)", MOMENTUM_UNIT_NAME.c_str()));

    //Momentum distribution with theta cut
    h_MOMENTUM_DISTRIBUTION_THETA_CUT[i] = new TH1D(
        Form("h_MOMENTUM_DISTRIBUTION_THETA_CUT[%d]", i),
        Form("Momentum distribution at the Dummy detector plan for "
             "a %dT Solenoid Magnetic Field with CM #theta %d%s to %d%s",
             MAG_FIELD_STRENGTH[i], CM_THETA_MIN, ANGLE_UNIT_NAME.c_str(),
             CM_THETA_MAX, ANGLE_UNIT_NAME.c_str()),
        100, 1, -1);

    h_MOMENTUM_DISTRIBUTION_THETA_CUT[i]->GetXaxis()->SetTitle(
        Form("Momentum (%s)", MOMENTUM_UNIT_NAME.c_str()));

    //functions
    /*
    f_RADIUS_VS_MOMENTUM[i] = new TF1(Form("f_RADIUS_VS_MOMENTUM[%d]", i),
                                      "pol1");
    */

    f_RADIUS_VS_MOMENTUM_THETA_CUT[i] = new TF1(
        Form("f_RADIUS_VS_MOMENTUM_THETA_CUT[%d]", i), "pol1");
  }

  return;
}

/***********************************************************
 Function: Loop_Through_Tree
 Purpose:
 To loop over the event and fill in the values for the
 histograms


 Entry Conditions:
 - TChain - chain
 - Mag_index - the index for the magnetic field

 Global:
 - h_XY_PLANE_DISTRIBUTION - vector of the XY plane hit distribution
 - h_XY_PLANE_DISTRIBUTION_THETA_CUT  - Vector of Historgrams for XY dis with theta cuts
 - h_RADIUS_VS_MOMENTUM - histogram for the radius vs Momentum
 - h_RADIUS_VS_MOMENTUM_THETA_CUT - histogram for the radius vs Momentum CM theta cut
 - h_MOMENTUM_DISTRIBUTION - historgram of mometums
 - h_MOMENTUM_DISTRIBUTION_THETA_CUT - histograms of momentums with theta cuts
 - NUM_FILES - Number of files aka Magnetic fields
 - LENGTH_UNIT - unit that all the lenghts are in
 - ANGLE_UNIT_NAME - units for angle
 - MOMENTUM_UNIT_NAME - units for Mometum
 - CM_THETA_MIN - min CM theta angle
 - CM_THETA_MAX = Max CM theta angle

 Exit Conditions: none
 Called By: Solenoid_MagneticField_Test
 Date: 08-25-2015
 Modified: 09-02-2015
 *********************************************************/
void Loop_Through_Tree(TChain* hmolpol_chain, int Mag_Index)
{
  //get the number of entires in the tree
  Int_t nentries = hmolpol_chain->GetEntries();

  //debugging
  //std::cout << "number of events in tree: " << nentries << std::endl;

  //Get the Units branch, so we can use the units
  HMolPolEventUnits* Units = 0;
  hmolpol_chain->SetBranchAddress("fUnits", &Units);

  //Get the Primary branch, so we can use the units
  HMolPolEventPrimary* Primary = 0;
  hmolpol_chain->SetBranchAddress("fPrimary", &Primary);

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
  hmolpol_chain->SetBranchAddress("fDummyDet", &Dummy_Det);

  //loop over all the events in the tree
  for (int i_entry = 0; i_entry < nentries; i_entry++)
  {
    //get the first entry in the tree
    hmolpol_chain->GetEntry(i_entry);

    //if we have information for the dummy detector we want to look at that
    if (Dummy_Det)
    {
      //get the number of hits that the Dummy Det has for the i_entry in the tree
      Int_t dummy_hits_nentries = Dummy_Det->fHits.size();

      /*
       //debugging
       std::cout << dummy_hits_nentries << " hits in Dummy Det for event "
       << i_entry << std::endl;
       */

      //loop over all the hits and fill the histograms with the information of interest
      //for (int i_hits = 0; i_hits < dummy_hits_nentries; i_hits++)
      //only want the first hit, this is related to the primary electrons
      for (int i_hits = 0; i_hits < 1; i_hits++)
      {
        //debugging
        //std::cout << "In the Hits loop" << std::endl;
        /*
         std::cout << "X Position " << Dummy_Det->fHits[i_hits].fPosition.fX
         << " " << Dummy_Det->fHits[i_hits].fPosition.fX / LENGTH_UNIT
         << " m " << std::endl << "Y Position "
         << Dummy_Det->fHits[i_hits].fPosition.fY / LENGTH_UNIT
         << " m " << std::endl;
         */

        //Fill the histograms
        //XY distribution
        h_XY_PLANE_DISTRIBUTION[Mag_Index]->Fill(
            Dummy_Det->fHits[i_hits].fPosition.fX / LENGTH_UNIT,
            Dummy_Det->fHits[i_hits].fPosition.fY / LENGTH_UNIT);

        h_RADIUS_VS_MOMENTUM[Mag_Index]->Fill(
            Dummy_Det->fHits[i_hits].fMomentum.Mag() / MOMENTUM_UNIT,TMath::Sqrt(
                Dummy_Det->fHits[i_hits].fPosition.fX * Dummy_Det->fHits[i_hits].fPosition.fX + Dummy_Det->fHits[i_hits].fPosition.fY
                    * Dummy_Det->fHits[i_hits].fPosition.fY)
            / LENGTH_UNIT);


        h_MOMENTUM_DISTRIBUTION[Mag_Index]->Fill(
            Dummy_Det->fHits[i_hits].fMomentum.Mag() / MOMENTUM_UNIT);

        /*
         h_MOMENTUM_DISTRIBUTION[Mag_Index]->Fill(
         TMath::Sqrt(
         Dummy_Det->fHits[i_hits].fMomentum.fX * Dummy_Det->fHits[i_hits].fMomentum.fX + Dummy_Det->fHits[i_hits].fMomentum.fY
         * Dummy_Det->fHits[i_hits].fMomentum.fY
         + Dummy_Det->fHits[i_hits].fMomentum.fZ * Dummy_Det->fHits[i_hits].fMomentum.fZ)
         / MOMENTUM_UNIT);
         */

        //Make theta cuts
        if (Primary->fThetaCenterOfMass / ANGLE_UNIT > CM_THETA_MIN && Primary->fThetaCenterOfMass
            / ANGLE_UNIT
                                                                       < CM_THETA_MAX)
        {
          //XY distibution with Theta cut
          h_XY_PLANE_DISTRIBUTION_THETA_CUT[Mag_Index]->Fill(
              Dummy_Det->fHits[i_hits].fPosition.fX / LENGTH_UNIT,
              Dummy_Det->fHits[i_hits].fPosition.fY / LENGTH_UNIT);

          h_RADIUS_VS_MOMENTUM_THETA_CUT[Mag_Index]->Fill(
              Dummy_Det->fHits[i_hits].fMomentum.Mag() / MOMENTUM_UNIT,TMath::Sqrt(
                  Dummy_Det->fHits[i_hits].fPosition.fX * Dummy_Det->fHits[i_hits].fPosition.fX + Dummy_Det->fHits[i_hits].fPosition.fY
                      * Dummy_Det->fHits[i_hits].fPosition.fY)
              / LENGTH_UNIT);

          h_MOMENTUM_DISTRIBUTION_THETA_CUT[Mag_Index]->Fill(
              Dummy_Det->fHits[i_hits].fMomentum.Mag() / MOMENTUM_UNIT);

          /*
           h_MOMENTUM_DISTRIBUTION_THETA_CUT[Mag_Index]->Fill(
           TMath::Sqrt(
           Dummy_Det->fHits[i_hits].fMomentum.fX * Dummy_Det->fHits[i_hits].fMomentum.fX + Dummy_Det->fHits[i_hits].fMomentum.fY
           * Dummy_Det->fHits[i_hits].fMomentum.fY
           + Dummy_Det->fHits[i_hits].fMomentum.fZ * Dummy_Det->fHits[i_hits].fMomentum.fZ)
           / MOMENTUM_UNIT);
           */
        }
      }
    }
  }

  return;
}

/***********************************************************
 Function: Plot
 Purpose:
 To plot the graphs and save those plots

 Entry Conditions:
 - Mag_index - the index for the magnetic field

 Global:
 - h_XY_PLANE_DISTRIBUTION - vector of the XY plane hit distribution
 - h_XY_PLANE_DISTRIBUTION_THETA_CUT  - Vector of Historgrams for XY dis with theta cuts
 - h_RADIUS_VS_MOMENTUM - histogram for the radius vs Momentum
 - h_RADIUS_VS_MOMENTUM_THETA_CUT - histogram for the radius vs Momentum CM theta cut
 - h_MOMENTUM_DISTRIBUTION - historgram of mometums
 - h_MOMENTUM_DISTRIBUTION_THETA_CUT - histograms of momentums with theta cuts
 - NUM_FILES - Number of files aka Magnetic fields
 - MAG_FIELD_STRENGTH - The magnetic fields
 - CM_THETA_MIN - Min CM theta angle
 - CM_THETA_MAX - Max CM theta angle

 - f_RADIUS_VS_MOMENTUM - functions momentum vs radius plot
 - f_RADIUS_VS_MOMENTUM_THETA_CUT - functions momentum vs radius plot with theta cut

 Exit Conditions: none
 Called By: Solenoid_MagneticField_Test
 Date: 08-26-2015
 Modified: 08-31-2015
 *********************************************************/
void Plot(int Mag_Index)
{
  //debugging
  //std::cout << "made it in the make plotting function" << std::endl;

  //define Canvases
  //std::vector<TCanvas*> c_XY_hits_ditribution;  //[Mag Field index]
  //c_XY_hits_ditribution.resize(NUM_FILES);
  TCanvas* c_XY_hits_ditribution = new TCanvas(
      "c_XY_hits_ditribution",
      Form("Y vs X hit distribution at the Dummy detector plan for a %dT "
           "Solenoid Magnetic Field",
           MAG_FIELD_STRENGTH[Mag_Index]),
      600, 500);

  h_XY_PLANE_DISTRIBUTION[Mag_Index]->Draw();

  c_XY_hits_ditribution->SaveAs(
      Form("YX_hit_dis_DummyDet_MagField%dT_PhiCom0_PrimaryOnly.png",
           MAG_FIELD_STRENGTH[Mag_Index]));

  TCanvas* c_XY_hits_ditribution_theta_cut = new TCanvas(
      "c_XY_hits_ditribution_theta_cut",
      Form("Y vs X hit distribution at the Dummy detector plan for a %dT "
           "Solenoid Magnetic Field with CM #theta %d%s to %d%s",
           MAG_FIELD_STRENGTH[Mag_Index], CM_THETA_MIN, ANGLE_UNIT_NAME.c_str(),
           CM_THETA_MAX, ANGLE_UNIT_NAME.c_str()),
      600, 500);

  h_XY_PLANE_DISTRIBUTION_THETA_CUT[Mag_Index]->Draw();

  c_XY_hits_ditribution_theta_cut->SaveAs(
      Form("YX_hit_dis_DummyDet_MagField%dT_theta_Cut_%dto%d_PhiCom0_PrimaryOnly.png",
           MAG_FIELD_STRENGTH[Mag_Index], CM_THETA_MIN, CM_THETA_MAX));

  //*** Radius vs Momentum
  TCanvas* c_Radius_vs_Momentum = new TCanvas(
      "c_Radius_vs_Momentum",
      Form("Radius vs Momentum at the Dummy detector plan for a %dT "
           "Solenoid Magnetic Field",
           MAG_FIELD_STRENGTH[Mag_Index]),
      600, 500);

  /*
  h_RADIUS_VS_MOMENTUM[Mag_Index]->Fit(
      Form("f_RADIUS_VS_MOMENTUM[%d]", Mag_Index));
  */

  h_RADIUS_VS_MOMENTUM[Mag_Index]->Draw();

  c_Radius_vs_Momentum->SaveAs(
      Form("Radius_vs_Momentum_DummyDet_MagField%dT_PhiCom0_PrimaryOnly.png",
           MAG_FIELD_STRENGTH[Mag_Index]));

  TCanvas* c_Radius_vs_Momentum_theta_cut = new TCanvas(
      "c_Radius_vs_Momentum_theta_cut",
      Form("Radius vs Momentum at the Dummy detector plan for a %dT "
           "Solenoid Magnetic Fieldwith CM #theta %d%s to %d%s",
           MAG_FIELD_STRENGTH[Mag_Index], CM_THETA_MIN, ANGLE_UNIT_NAME.c_str(),
           CM_THETA_MAX, ANGLE_UNIT_NAME.c_str()),
      600, 500);

  h_RADIUS_VS_MOMENTUM_THETA_CUT[Mag_Index]->Fit(
      Form("f_RADIUS_VS_MOMENTUM_THETA_CUT[%d]", Mag_Index));

  gStyle->SetStatW(0.13);
  gStyle->SetStatH(0.105);
  gStyle->SetOptFit(1011);

  h_RADIUS_VS_MOMENTUM_THETA_CUT[Mag_Index]->Draw();

  c_Radius_vs_Momentum_theta_cut->SaveAs(
      Form(
          "Radius_vs_Momentum_DummyDet_MagField%dT_theta_Cut_%dto%d_PhiCom0_PrimaryOnly.png",
          MAG_FIELD_STRENGTH[Mag_Index], CM_THETA_MIN, CM_THETA_MAX));

  //*** Momentum
  TCanvas* c_Momentum_ditribution = new TCanvas(
      "c_Momentum_ditribution",
      Form("Momentum distribution at the Dummy detector plan for a %dT "
           "Solenoid Magnetic Field",
           MAG_FIELD_STRENGTH[Mag_Index]),
      600, 500);

  gStyle->SetStatW();
  gStyle->SetStatH();

  h_MOMENTUM_DISTRIBUTION[Mag_Index]->Draw();

  c_Momentum_ditribution->SaveAs(
      Form("Momentum_dis_DummyDet_MagField%dT_PhiCom0_PrimaryOnly.png",
           MAG_FIELD_STRENGTH[Mag_Index]));

  TCanvas* c_Momentum_ditribution_theta_cut = new TCanvas(
      "c_Momentum_ditribution_theta_cut",
      Form("Momentum distribution at the Dummy detector plan for a %dT "
           "Solenoid Magnetic Field with CM #theta %d%s to %d%s",
           MAG_FIELD_STRENGTH[Mag_Index], CM_THETA_MIN, ANGLE_UNIT_NAME.c_str(),
           CM_THETA_MAX, ANGLE_UNIT_NAME.c_str()),
      600, 500);

  h_MOMENTUM_DISTRIBUTION_THETA_CUT[Mag_Index]->Draw();

  c_Momentum_ditribution_theta_cut->SaveAs(
      Form("Momentum_dis_DummyDet_MagField%dT_theta_Cut_%dto%d_PhiCom0_PrimaryOnly.png",
           MAG_FIELD_STRENGTH[Mag_Index], CM_THETA_MIN, CM_THETA_MAX));

  return;
}
