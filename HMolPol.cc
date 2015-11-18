/***********************************************************
\author <b>Programmer:<\b> Valerie Gray
\author <b>Assisted By:<\b> Wouter Deconinck

\brief <b>Purpose:</b> This is the main function for the Atomic Hydrogen
    Moller Geant4 simulation or HMolPol it should put everything together...

\note <b>Entry Conditions:</b> none

\date <b>Date:</b> 05-15-2013
\date <b>Modified:</b> 07-15-2015
 *********************************************************/

#include <iostream>

//all the Geant4 includes
#include <G4RunManager.hh>
#include <G4PhysListFactory.hh>
#include <G4VModularPhysicsList.hh>

#include <G4VisManager.hh>
#include <G4UImanager.hh>
#include <G4UIExecutive.hh>
#include <G4UIQt.hh>

//all the HMollerPol specific includes
//all of the HMolPol headers must be included
#include "HMolPolAnalysis.hh"
#include "HMolPolDetectorConstruction.hh"
#include "HMolPolEventAction.hh"
#include "HMolPolSteppingAction.hh"
#include "HMolPolStackingAction.hh"
#include "HMolPolRunAction.hh"
#include "HMolPolPrimaryGeneratorAction.hh"
#include "HMolPolMessenger.hh"

#ifdef G4VIS_USE
#include <G4VisExecutive.hh>
#endif

///< TODO Doxygen doesn't work at all. This needs to get fixed
///< and then comments in that form and README and mainpage.dox

/** \defgroup root Variables include in the ROOT file
     *  This group contains all variables that are included in the ROOT file.
*/

/** \defgroup see References to where things are found, papers and such
     *  This contains all references in the code to papers and other important
     *  things
*/

/*******
 * argc is the number of arguments passed (including the executable name)
 * argv is a vector with all arguments
 * if no arguments are passed (argc == 1) we only create a Qt GUI session
 * if arguments are passed (argc > 1) we execute the first argument passed
*******/
int main (int argc, char** argv)
{
  //< TODO: Need to put in random seed info... not the remoll way as it is not good

  /**********
   * Initialization of Run manager
   * This is need so that all the "events" are looped through in a run
  ***********/
  G4cout << "RunManager construction starting...." << G4endl;
  G4RunManager* runManager = new G4RunManager;

  // Physics we want to use
  // How much output we want to see - 0 is the most
  G4int verbose = 0;

  //physics list factory allows one to use the standard geant4 physics list
  G4PhysListFactory* factory = new G4PhysListFactory;
  /*********
   *  use the FTFP-BERT
   * (FTFP_BERT
   * - http://geant4.cern.ch/support/proc_mod_catalog/physics_lists/hadronic/FTFP_BERT.html)
   * (FTFP - http://geant4.cern.ch/support/proc_mod_catalog/physics_lists/hadronic/FTFP.html)
   *  \note physics list - FTRP-BERT is replacing the LHEP physics list
   *  \note standard EM processes are include in all list
   *  \bug FTFP_bert may be not the right physics list for Mainz energy,
   *  but should be for JLab
   *  \TODO: check to see if these are right and have one for each place
  *********/
  G4VModularPhysicsList* physlist = factory->GetReferencePhysList("FTFP_BERT");
  physlist->SetVerboseLevel(verbose);
  // give the run manager the physics list
  runManager->SetUserInitialization(physlist);

  // Analysis (interface with ROOT file)
  HMolPolAnalysis* myHMolPolAnalysis  = new HMolPolAnalysis();

  // Geant4 has two main mechanisms to allow the user to affect the way it is
  // running: you can pass UserInitialization classes that affect how the
  // simulation is initialized at the beginning, or you can pass UserAction
  // classes which can be called several times (for example for every run or
  // for every event). Since the initialization classes are only executed once,
  // it is not as important that they are fast. For user action classes it is
  // important to make them relatively fast (some things cannot be avoided,
  // for example writing to a ROOT file for each event, but the ROOT file can
  // be kept in memory to avoid having to write to disk for each event; ROOT
  // does this by default already).

  // Add the Detector geometry
  //pass the geometry of the HMolPol to the Geant4 class G4VUserDetectorConstruction
  HMolPolDetectorConstruction* myHMolPolDetector =
      new HMolPolDetectorConstruction(myHMolPolAnalysis);
  // give the run manager the geometry
  runManager->SetUserInitialization(myHMolPolDetector);

  // Register the Stepping Action (called at each step)
  HMolPolSteppingAction *myHMolPolStepping =
    new HMolPolSteppingAction(myHMolPolAnalysis);
  runManager->SetUserAction( myHMolPolStepping );

  // Register the Stacking Action (called at each new particle created)
  HMolPolStackingAction *myHMolPolStacking = new HMolPolStackingAction();
  runManager->SetUserAction(myHMolPolStacking);

  //Add the things that happen at each event
  //give run manager the stuff that is done at every event (write to rootfile)
  HMolPolEventAction* myHMolPolEventAction =
      new HMolPolEventAction(myHMolPolAnalysis,myHMolPolStacking);
  runManager->SetUserAction(myHMolPolEventAction);

  // Add the thing that happen for each run (run==a full simulation of \BeamOn n)
  // give the run manager stuff that is done in each run (save rootfile)
  HMolPolRunAction* myHMolPolRunAction =
      new HMolPolRunAction(myHMolPolAnalysis);
  runManager->SetUserAction(myHMolPolRunAction);

  //< TODO: Add in the magnetic field

/// \todo FIX ME!!!!  do something to get the messenger involved But with what?
  //< what is the point of this part?
//  HMolPolMess->SetDetCon( ((HMolPolDetectorConstruction *) detector) );
//  HMolPolMess->SetMagField(
//      ((HMolPolDetectorConstruction *) detector)->GetGlobalField() );

  // Add in the beam - what happen for each initial interaction
  // give the run manager the beam information
  HMolPolPrimaryGeneratorAction* myHMolPolPrimaryGeneratorAction =
      new HMolPolPrimaryGeneratorAction(myHMolPolAnalysis);
  runManager->SetUserAction(myHMolPolPrimaryGeneratorAction);

  // add the global messenger - this will talk with all of
  // the files and the user - Allows for the interaction and changes
  // in the simulation without going into the code making changes
  //
  // This does not need to be added to the run manager because the messenger
  // class defines user interface commands, and only the user interface needs
  // to know about those. The run manager does not care about the commands that
  // are implemented.
  HMolPolMessenger* myHMolPolMess = new HMolPolMessenger(
      myHMolPolPrimaryGeneratorAction,
      myHMolPolRunAction,
      myHMolPolEventAction,
      myHMolPolAnalysis,
      myHMolPolDetector,
      myHMolPolStepping);

  /*******
   * Initialize Run manager
   * we can either us this or have /run/initialize staring all out macros...
   * I have taken it our so that different geometries configurations
   * can be used with ease, it is better to do this in your macros if you
   * are using GDML
  ********/
  //runManager->Initialize();

  /*******
   *  Visualization:
  *******/
  // Start with out a interactive session
  G4UIsession* session = 0;
  if (argc==1)   // Define UI session for interactive mode (no arguments passed).
  {
    session = new G4UIQt(argc,argv);
  }

  #ifdef G4VIS_USE
    //this is the initializing the visualization manager
    G4VisManager* visManager = new G4VisExecutive;
    /*******
     * Simple graded message scheme - give first letter or a digit:
     * 0) quiet,         // Nothing is printed.
     * 1) startup,       // Startup and endup messages are printed...
     * 2) errors,        // ...and errors...
     * 3) warnings,      // ...and warnings...
     * 4) confirmations, // ...and confirming messages...
     * 5) parameters,    // ...and parameters of scenes and views...
     * 6) all            // ...and everything available.
    ********/
    //visManager -> SetVerboseLevel (1);

    visManager ->Initialize();
  #endif

  //get the pointer to the User Interface manager
  G4UImanager* UI = G4UImanager::GetUIpointer();

  if (session)   // Define UI session for interactive mode.
  {
    // Customize the G4UIQt menu-bar with a macro file :
    UI->ApplyCommand("/control/execute gui.mac");

    //start the interactive session
    session->SessionStart();
    // delete the session
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
    // you pass the name of the macro and the /control/execute is added for free :)
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UI->ApplyCommand(command+fileName);
  }

  //if one used the GUI then delete it
  #ifdef G4VIS_USE
    delete visManager;
  #endif

  //Now delete all the objects we created, must be done in the opposite order as
  //they were created

  //shoot the messenger
  delete myHMolPolMess;
  //delete HMolPolAnalysis
  delete myHMolPolAnalysis;
  //delete the runManager
  delete runManager;
 /******
 * Don't need to delete as deleting the run manager does this
 * //delete HMolPolEventAction
 * delete myHMolPolEventAction;
 * //delete HMolPolDetector
 * delete myHMolPolDetector;
 * //delete the physics list
 * delete physlist;
 * //delete  HMolPolPrimaryGeneratorAction
 * delete myHMolPolPrimaryGeneratorAction;
 * //delete the HMolPolRunAction
 * delete myHMolPolRunAction;
 ********/

  //now that everything has ran and is deleted - the simulation can be closed
  return 0;
}
