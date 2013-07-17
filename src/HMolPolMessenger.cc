/********************************************
 * \author <b>Programmer:</b> Valerie Gray
 * \author <b>Assisted By:</b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This file is a messenger file for HMolPol.
 *
 * \details The messenger file is is the interface between the gui/user and the
 * simulation
 *
 * \date 0<b>Date:</b> 6-27-2013
 * \date <b>Modified:</b> 07-06-2013
 *
 * \note <b>Entry Conditions:</b>
 ********************************************/

//geant4 includes
#include <G4UImessenger.hh>
#include <G4UIcommand.hh>
#include <G4UImanager.hh>

#include <G4RunManager.hh>

#include <G4GDMLParser.hh>
#include <G4VPhysicalVolume.hh>

//#include <G4UIcmdWithAnInteger.hh>
//#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
//#include <G4UIcmdWithABool.hh>

//HMolPol includes
#include "HMolPolPrimaryGeneratorAction.hh"
#include "HMolPolMessenger.hh"

#include <iostream>

/********************************************
 * Programmer: Valerie Gray
 * Function: HMolPolMessenger
 *
 * Purpose:
 * Construction of the HMolPolMessenger
 *
 * Global:
 * Entry Conditions: none
 * Return: Nothing
 * Called By:
 * Date: 06-27-2013
 * Modified:
 ********************************************/
  /// \todo have someone (Wouter) help me figure what this all does
HMolPolMessenger::HMolPolMessenger(){

  //Raster information
  rasXCmd = new G4UIcmdWithADoubleAndUnit("/HMolPol/Raster/SetRasterSizeX",this);
  rasXCmd->SetGuidance("Square raster width in x (horizontal)");
  rasXCmd->SetParameterName("rasterx", false);

  rasYCmd = new G4UIcmdWithADoubleAndUnit("/HMolPol/Raster/SetRasterSizeY",this);
  rasYCmd->SetGuidance("Square raster width y (vertical)");
  rasYCmd->SetParameterName("rastery", false);
}

/********************************************
 * Programmer: Valerie Gray
 * Function: ~HMolPolMessenger
 *
 * Purpose:
 * Destructor of the HMolPolMessenger
 *
 * Global:
 * Entry Conditions: none
 * Return: Nothing
 * Called By:
 * Date: 06-27-2013
 * Modified:
 ********************************************/
HMolPolMessenger::~HMolPolMessenger(){
}

/// \todo have someone (Wouter) help me figure what this all does

void HMolPolMessenger::SetNewValue(G4UIcommand* cmd, G4String newValue){

//  //raster size
//  if( cmd == rasXCmd ){
//      G4double x = rasXCmd->GetNewDoubleValue(newValue);
//      fBeamTarg->fRasterX = x;
//  }
//
//  if( cmd == rasYCmd ){
//      G4double y = rasYCmd->GetNewDoubleValue(newValue);
//      fBeamTarg->fRasterY = y;
//  }

}
