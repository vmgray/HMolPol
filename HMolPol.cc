/***********************************************************
Programmer: Valerie Gray
Purpose:

This is the main function for the H moller Geant4 simulation
it should put everything together...

Entry Conditions:
Date: 05-15-2013
Modified: 07-11-2013
Assisted By: Wouter Deconinck
 *********************************************************/

#include <iostream>

//all the Geant4 includes
#include <G4RunManager.hh>
#include <G4PhysListFactory.hh>
#include <G4VModularPhysicsList.hh>

#include <G4VisManager.hh>
#include <G4VisExecutive.hh>
#include <G4UImanager.hh>
#include <G4UIExecutive.hh>
#include <G4UIQt.hh>

//all the HMollerPol specific includes
#include "HMolPolDetectorConstruction.hh"
#include "HMolPolPrimaryGeneratorAction.hh"
#include "HMolPolMessenger.hh"
#include "HMolPolAnalysis.hh"
#include "HMolPolEventAction.hh"
#include "HMolPolRunAction.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

//why is this not a void function? Why is it getting passed anything?
//what is the point of whatever it is getting passed
//if this is the number of things passed and the commands, then how does it work
//

/** \defgroup root Variables include in the ROOT file
     *
     *  This group contains all variables that are included in the ROOT file.
*/

/** \defgroup see References to where things are found, papers and such
     *
     *  This contains all references in the code to papers and other important
     *  things
*/

int main (int argc, char** argv)
{
  //Need to put in random seed info... not the remoll way as it is not good

  //-------------------------------
  // Initialization of Run manager
  // This is need so that all the "events" are looped through in a run
  //-------------------------------
  G4cout << "RunManager construction starting...." << G4endl;
  G4RunManager* runManager = new G4RunManager;

  // Physics we want to use
  G4int verbose = 0;
  //physics list factory allows one to use the standard geant4 physics list
  G4PhysListFactory* factory = new G4PhysListFactory;

  /// use the FTFP-BERT
  ///(FTFP_BERT
  // - http://geant4.cern.ch/support/proc_mod_catalog/physics_lists/hadronic/FTFP_BERT.html)
  ///(FTFP - http://geant4.cern.ch/support/proc_mod_catalog/physics_lists/hadronic/FTFP.html)
  /// \note physics list - FTRP-BERT is replacing the LHEP physics list
  /// \note standard EM processes are include in all list
  /// \bug FTFP_bert may be no the right physics list for Mainz energy,
  /// but should be for JLab
  G4VModularPhysicsList* physlist = factory->GetReferencePhysList("FTFP_BERT");
  physlist->SetVerboseLevel(verbose);
  // give the run manager the physics list
  runManager->SetUserInitialization(physlist);

  // add the global messenger - this will talk with all of
  //the files and the user
  HMolPolMessenger* HMolPolMess = new HMolPolMessenger();


  // Detector geometry
  //pass the geometry of the HMolPol to the Geant4 class G4VUserDetectorConstruction
  G4VUserDetectorConstruction* myHMolPolDetector =
      new HMolPolDetectorConstruction();
  // give the run manager the geometry
  runManager->SetUserInitialization(myHMolPolDetector);


  // Analysis (interface with ROOT file)
  HMolPolAnalysis* myHMolPolAnalysis  = new HMolPolAnalysis();

  //give run manager the stuff that is done at every event (write to rootfile)
  G4UserEventAction* myHMolPolEventAction =
      new HMolPolEventAction(myHMolPolAnalysis);
  runManager->SetUserAction(myHMolPolEventAction);
  // give the run manager stuff that is done in each run (save rootfile)
  G4UserRunAction* myHMolPolRunAction =
      new HMolPolRunAction(myHMolPolAnalysis);
  runManager->SetUserAction(myHMolPolRunAction);


//FIX ME!!!!  do something to get the messenger involved
//  HMolPolMess->SetDetCon( ((HMolPolDetectorConstruction *) detector) );
//  HMolPolMess->SetMagField(
//      ((HMolPolDetectorConstruction *) detector)->GetGlobalField() );


  //beam
  G4VUserPrimaryGeneratorAction* myHMolPolPrimaryGeneratorAction =
      new HMolPolPrimaryGeneratorAction();
  runManager->SetUserAction(myHMolPolPrimaryGeneratorAction);
//  HMolPolMess->SetPriGen(primaryGeneratorAction);


  // Initialize Run manager
  // we can either us this or have /run/initialize staring all out macros...
  // I have taken it our so that different geometries configurations
  // can be used with ease
  //runManager->Initialize();

  //----------------
  // Visualization:
  //----------------

  G4UIsession* session = 0;
  if (argc==1)   // Define UI session for interactive mode.
  {
    session = new G4UIQt(argc,argv);
  }

  #ifdef G4VIS_USE
    //this is the initializing the visualization manager
    G4VisManager* visManager = new G4VisExecutive;
    visManager ->Initialize();
  #endif

  //get the pointer to the User Interface manager
  G4UImanager * UI = G4UImanager::GetUIpointer();

  if (session)   // Define UI session for interactive mode.
  {

    // Customize the G4UIQt menubar with a macro file :
    UI->ApplyCommand("/control/execute gui.mac");

    session->SessionStart();
    delete session;
  }
  else           // Batch mode - not using the GUI
  {
    #ifdef G4VIS_USE
      visManager->SetVerboseLevel("quiet");
    #endif
    //these line will execute a macro without the GUI
    //in GEANT4 a macro is executed when it is passed to the command,
    // /control/execute
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UI->ApplyCommand(command+fileName);
  }

  //if one used the GUI then delete it
  #ifdef G4VIS_USE
    delete visManager;
  #endif

  //delete the runManager
  delete runManager;

  //now that everything has ran and is deleted - the simulation can be closed
  return 0;
}
