/********************************************
 * \author <b>Programmer:<\b> Valerie Gray
 * \author <b>Assisted By:<\b>
 *
 * \brief <b>Purpose:</b> This is header file
 *   for the Initial Conditions.
 *
 *   Must be called
 *   When you loop through the tree and after you
 *   have grabbed the branches and the first entry
 *
 *   it will be used by many Analysis macros
 *   for HMolPol
 *
 * \date <b>Date:</b> 04-21-2016
 * \date <b>Modified:</b> 04-28-2016
 *
 * \note <b>Entry Conditions:</b> HMolPolEventUnits and HMolPolEventPrimaryConditions
 *
 ********************************************/

//HMolPol Includes
#include "HMolPolEventUnits.hh"
#include "HMolPolEventPrimaryConditions.hh"

#include <iostream>

void ExtractInitalConditions(HMolPolEventUnits* Units, HMolPolEventPrimaryConditions* PrimaryConditions)
{
  //this file and everything related to it is fout
  std::ofstream fout;

  //get the output file name.
  std::string output_filename = OUTPUT_PREFIX
      + std::string("/Inital_conditions.txt");

  //open a file
  fout.open(output_filename.c_str());

  if (!fout.is_open())
    std::cout << "File not opened" << std::endl;

  //Beam
  fout << "Beam:" << std::endl;
  fout << "Beam Energy (GeV): " << PrimaryConditions->fBeamE / Units->GeV
  << std::endl;
  fout << "Raster size (x,y) (mm): " << PrimaryConditions->fRasterX / Units->mm
  << "," << PrimaryConditions->fRasterY / Units->mm << std::endl;
  fout << "Z gen location (min, max) (cm): " << PrimaryConditions->fZGenMin / Units->cm
  << "," << PrimaryConditions->fZGenMax / Units->cm << std::endl << std::endl;

  //Angles
  fout << "Angles:" << std::endl;
  fout << "Theta Min (deg): " << PrimaryConditions->fThetaCM_Min / Units->deg
  << std::endl;
  fout << "Theta Max (deg): " << PrimaryConditions->fThetaCM_Max / Units->deg
  << std::endl;
  fout << "Delta Theta (deg): " << PrimaryConditions->fDelta_Theta / Units->deg
  << std::endl;
  fout << "Delta cos(Theta) (deg): " << PrimaryConditions->fDelta_CosTheta
  << std::endl;

  fout << "Phi Min (deg): " << PrimaryConditions->fPhiCM_Min / Units->deg
  << std::endl;
  fout << "Phi Max (deg): " << PrimaryConditions->fPhiCM_Max / Units->deg
  << std::endl;
  fout << "Delta Phi (deg): " << PrimaryConditions->fDelta_Phi / Units->deg
  << std::endl;

  fout << "Delta Omega (sr): " << PrimaryConditions->fDelta_Omega / Units->sr
  << std::endl << std::endl;

  fout << "Nb (Hz/muA): " << PrimaryConditions->fNb << std::endl << std::endl;

  fout << "Number of Entries:" << std::endl;
  fout << "nevents: " << HMOLPOL_CHAIN->GetEntries() << std::endl;

  return;
}
