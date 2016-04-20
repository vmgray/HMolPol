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

#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWithABool.hh>

//HMolPol includes
#include "HMolPolPrimaryGeneratorAction.hh"
#include "HMolPolAnalysis.hh"
#include "HMolPolMessenger.hh"
#include "HMolPolDetectorConstruction.hh"
#include "HMolPolSteppingAction.hh"

//standard includes
#include <iostream>

/********************************************
 * Programmer: Valerie Gray
 * Function: HMolPolMessenger
 *
 * Purpose:
 * Construction of the HMolPolMessenger
 *
 * Global:
 * Entry Conditions: HMolPolPrimaryGeneratorAction,
 *      HMolPolRunAction,
 *      HMolPolEventAction,
 *      HMolPolAnalysis
 * Return: Nothing
 * Called By:
 * Date: 06-27-2013
 * Modified:
 ********************************************/
HMolPolMessenger::HMolPolMessenger(
                                   HMolPolPrimaryGeneratorAction* primaryGeneratorAction,
                                     HMolPolRunAction* runAction,
                                     HMolPolEventAction* eventAction,
                                     HMolPolAnalysis* analysis,
                                     HMolPolDetectorConstruction* detectorConstruction,
                                     HMolPolSteppingAction* steppingAction) :
    fPrimaryGeneratorAction(primaryGeneratorAction),  //initialize variables
    fRunAction(runAction), fEventAction(eventAction), fAnalysis(analysis),
    fDetectorConstruction(detectorConstruction), fSteppingAction(steppingAction)
{
  //------------------------------------------------------------------------------
  // create a new directory for all the analysis related stuff
  fAnalysisDir = new G4UIdirectory("/HMolPol/Analysis/");
  //What is this directory for
  fAnalysisDir->SetGuidance("Analysis control");

  //ROOT file Name
  fRootFileStemCmd = new G4UIcmdWithAString("/HMolPol/Analysis/RootFileStem",
                                            this);
  fRootFileStemCmd->SetGuidance("Set stem of file with output ROOT tree");
  fRootFileStemCmd->SetParameterName("RootFileStem", false);
  fRootFileStemCmd->SetDefaultValue("HMolPol");
  fRootFileStemCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fRootFileNameCmd = new G4UIcmdWithAString("/HMolPol/Analysis/RootFileName",
                                            this);
  fRootFileNameCmd->SetGuidance("Set file name of output ROOT tree");
  fRootFileNameCmd->SetParameterName("RootFileName", false);
  fRootFileNameCmd->SetDefaultValue("HMolPol.root");
  fRootFileNameCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  //------------------------------------------------------------------------------
  /**********
   * create a new directory for all the Beam related properties
   * & commands to set them
  **********/
  fBeamDir = new G4UIdirectory("/HMolPol/Beam/");
  //What is this directory for
  fBeamDir->SetGuidance("Beam control");

  //Raster information
  fRasXCmd = new G4UIcmdWithADoubleAndUnit("/HMolPol/Beam/SetRasterSizeX",
                                           this);
  fRasXCmd->SetGuidance("Square raster width in x (horizontal)");
  fRasXCmd->SetParameterName("RasterX", false);

  fRasYCmd = new G4UIcmdWithADoubleAndUnit("/HMolPol/Beam/SetRasterSizeY",
                                           this);
  fRasYCmd->SetGuidance("Square raster width y (vertical)");
  fRasYCmd->SetParameterName("RasterY", false);

  // Beam Energy
  fBeamECmd = new G4UIcmdWithADoubleAndUnit("/HMolPol/Beam/SetBeamEnergy",
                                            this);
  fBeamECmd->SetGuidance("Beam Energy");
  fBeamECmd->SetParameterName("BeamEnergy", false);

  /**********
   * create a new directory for all the CM Angle related properties
   * & commands to set them
  **********/
  fAngleDir = new G4UIdirectory("/HMolPol/CMAngles/");
  //What is this directory for
  fAngleDir->SetGuidance("Center of Mass Angle range");
  // Theta CM trowen range
  //Min
  fThetaComMinCmd = new G4UIcmdWithADoubleAndUnit("/HMolPol/CMAngles/SetThetaComMin",
                                            this);
  fThetaComMinCmd->SetGuidance("Center of Mass Min Theta angle");
  fThetaComMinCmd->SetParameterName("ThetaComMin", false);

  //Max
  fThetaComMaxCmd = new G4UIcmdWithADoubleAndUnit("/HMolPol/CMAngles/SetThetaComMax",
                                            this);
  fThetaComMaxCmd->SetGuidance("Center of Mass Max Theta angle");
  fThetaComMaxCmd->SetParameterName("ThetaComMax", false);

  // Phi CM trowen range
  //Min
  fPhiComMinCmd = new G4UIcmdWithADoubleAndUnit("/HMolPol/CMAngles/SetPhiComMin",
                                            this);
  fPhiComMinCmd->SetGuidance("Center of Mass Min Phi angle");
  fPhiComMinCmd->SetParameterName("PhiComMin", false);

  //Max
  fPhiComMaxCmd = new G4UIcmdWithADoubleAndUnit("/HMolPol/CMAngles/SetPhiComMax",
                                            this);
  fPhiComMaxCmd->SetGuidance("Center of Mass Max Phi angle");
  fPhiComMaxCmd->SetParameterName("PhiComMax", false);

  /**********
   * create a new directory for all the geometry related properties
   * & commands to set them
  **********/
  // Directory for all Geometry related commands
  fGeometryDir = new G4UIdirectory("/HMolPol/Geometry/");
  fGeometryDir->SetGuidance("Geometry control");

  // Set Geometry file name
  fGeometryFileNameCmd = new G4UIcmdWithAString(
      "/HMolPol/Geometry/GeometryFileName", this);
  fGeometryFileNameCmd->SetGuidance("File Name and path of Geometry GDML file");
  fGeometryFileNameCmd->SetParameterName("GeometryFileName", false);

  /**********
   * create a new directory for all the Tracking/step related properties
   * & commands to set them
  **********/
  // Directory for all Tracking/Step related commands
  fTrackingDir = new G4UIdirectory("/HMolPol/Tracking/");
  fTrackingDir->SetGuidance("HMolPol Tracking control");

  // Enable primary tracker? (tracks and stores all steps taken by a primary)
  fTrackPrimariesCmd = new G4UIcmdWithABool("/HMolPol/Tracking/TrackPrimaries",
                                            this);
  fTrackPrimariesCmd->SetGuidance("Track and store all steps of primaries?");
  fTrackPrimariesCmd->SetParameterName("TrackPrimaries", false);

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
HMolPolMessenger::~HMolPolMessenger()
{
  //delete naming of Root file stuff
  if (fRootFileStemCmd)
    delete fRootFileStemCmd;
  if (fRootFileNameCmd)
    delete fRootFileNameCmd;

  //delete any directories one created
  if (fAnalysisDir)
    delete fAnalysisDir;
  if (fBeamDir)
    delete fBeamDir;
  if (fTrackingDir)
    delete fTrackingDir;
  if (fAngleDir)
    delete fAngleDir;

  //delete the raster info
  if (fRasXCmd)
    delete fRasXCmd;
  if (fRasYCmd)
    delete fRasYCmd;

  //Delete beam energy info
  if (fBeamECmd)
    delete fBeamECmd;

  //delete the angle info
  if (fThetaComMinCmd)
    delete fThetaComMinCmd;
  if (fThetaComMaxCmd)
    delete fThetaComMaxCmd;

  if (fPhiComMinCmd)
    delete fPhiComMinCmd;
  if (fPhiComMaxCmd)
    delete fPhiComMaxCmd;

  //delete the Primary Tracker info
  if (fTrackPrimariesCmd)
    delete fTrackPrimariesCmd;

  // Delete Geometry related variables
  if (fGeometryDir)
    delete fGeometryDir;
  if (fGeometryFileNameCmd)
    delete fGeometryFileNameCmd;
}

/********************************************
 * Programmer: Valerie Gray
 * Function: SetNewValue
 *
 * Purpose:
 * To set that new value for a certain command
 *
 * Global:
 * Entry Conditions: G4UIcommand,
 *      G4String
 * Return: Nothing
 * Called By:
 * Date: 06-27-2013
 * Modified:
 ********************************************/
/// \todo have someone (Wouter) help me figure what this all does
// Whenever a command from this messenger is called, for example the command
// /HMolPol/Analysis/RootFileName, then this function is called with as first
// argument the command and as second argument a string with the value that
// was passed, for example SetNewValue(fRootFileNameCmd,"run.root"). It is
// the job of this function to figure out what to do for this command, to
// convert the value to its proper data type (for example string to int), and
// to test whether the value was even an allowed value.
void HMolPolMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{

// change the different possible variables allowed in the messenger

  //-----------------------------------------------------------
  //change name of ROOT file stuff - if given
  if (command == fRootFileStemCmd)
  {
    G4cout << "#### Messenger: Setting Analysis "
           "ROOT file stem to "
           << newValue << G4endl;
           fAnalysis->SetRootFileStem(newValue);
  }
  if (command == fRootFileNameCmd)
  {
    G4cout << "#### Messenger: Setting Analysis "
    "ROOT file name to " << newValue << G4endl;
    fAnalysis->SetRootFileName(newValue);
  }

  //-----------------------------------------------------------
  //raster size
  if( command == fRasXCmd )
  {
    G4cout << "#### Messenger: Setting Beam X Raster Size to "
    << newValue << G4endl;
    fPrimaryGeneratorAction->SetRasterX(fRasXCmd->GetNewDoubleValue(newValue));
  }

  if( command == fRasYCmd )
  {
    G4cout << "#### Messenger: Setting Beam Y Raster Size to "
        << newValue << G4endl;
    fPrimaryGeneratorAction->SetRasterY(fRasYCmd->GetNewDoubleValue(newValue));
  }

  //-----------------------------------------------------------
  //beam Energy
  if( command == fBeamECmd )
  {
    G4cout << "#### Messenger: Setting Beam Energy to "
        << newValue << G4endl;
    fPrimaryGeneratorAction->SetBeamE(fBeamECmd->GetNewDoubleValue(newValue));
  }

  //-----------------------------------------------------------
  // Geometry file name
  if( command == fGeometryFileNameCmd )
  {
    G4cout << "#### Messenger: Setting Geometry file to "
        << newValue << G4endl;
    fDetectorConstruction->SetGeometryFileName(newValue);
  }

  //-----------------------------------------------------------
  // Track Primaries command
  if( command == fTrackPrimariesCmd )
  {
    G4cout << "#### Messenger: Setting Tracking Primaries to "
        << newValue << G4endl;
    fSteppingAction->SetTrackPrimaries(
        fTrackPrimariesCmd->GetNewBoolValue(newValue));
  }

  //-----------------------------------------------------------
  //Theta CM values
  if( command == fThetaComMinCmd )
  {
    G4cout << "#### Messenger: Setting Theta CM Min value to "
        << newValue << G4endl;
    fPrimaryGeneratorAction->SetThetaComMin(fThetaComMinCmd->GetNewDoubleValue(newValue));
  }

  if( command == fThetaComMaxCmd )
  {
    G4cout << "#### Messenger: Setting Theta CM Max value to "
        << newValue << G4endl;
    fPrimaryGeneratorAction->SetThetaComMax(fThetaComMaxCmd->GetNewDoubleValue(newValue));
  }

  //-----------------------------------------------------------
  //Phi CM values
  if( command == fPhiComMinCmd )
  {
    G4cout << "#### Messenger: Setting Phi CM Min value to "
        << newValue << G4endl;
    fPrimaryGeneratorAction->SetPhiComMin(fPhiComMinCmd->GetNewDoubleValue(newValue));
  }

  if( command == fPhiComMaxCmd )
  {
    G4cout << "#### Messenger: Setting Phi CM Max value to "
        << newValue << G4endl;
    fPrimaryGeneratorAction->SetPhiComMax(fPhiComMaxCmd->GetNewDoubleValue(newValue));
  }


  return;
}
