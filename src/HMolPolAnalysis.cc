/********************************************
 * \author <b>Programmer:</b> Valerie Gray
 * \author <b>Assisted By:</b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This file is in charge of getting everything set
 *      up so that the simulation will have results in a ROOT file.
 *
 * \date <b>Date:</b> 07-11-2013
 * \date <b>Modified:</b> 07-017-2013
 *
 * \note <b>Entry Conditions:</b>
 *
 * \todo comment the comment blocks to get the how this is all put together
 *
 ********************************************/

// geant4 includes
#include <G4Run.hh>

// HMolPol includes
#include "HMolPolAnalysis.hh"
#include "HMolPolEvent.hh"
#include "HMolPolRunInformation.hh"

/// \todo make this so the file names can change with things like generator etc
/// ie HMolPol_eleastic HMolPol_Moller, etc

/********************************************
 * Programmer: Valerie Gray
 * Function: HMolPolAnalysis(): fRootFileStem("HMolPol"),fRootFileName("")
 *
 * Purpose:  To initialize the default Root file name and everything in the
 *      Tree to null.
 *
 * Global:
 * Entry Conditions: none
 * Return:
 * Called By:
 * Date: 07-11-2013
 * Modified:
 ********************************************/
HMolPolAnalysis::HMolPolAnalysis(): fRootFileStem("HMolPol"),fRootFileName("")
{
  G4cout << "###### Calling HMolPolAnalysis::HMolPolAnalysis()" << G4endl;

  // Initialize everything to nothing
  fRootTree   = NULL;
  fRootBranch = NULL;
  fRootFile   = NULL;

  // Create event structure
  fEvent = new HMolPolEvent();
}

/********************************************
 * Programmer: Valerie Gray
 * Function: ~HMolPolAnalysis()
 *
 * Purpose:  To deconsturuct the default Root file name and everything in the
 *      Tree.
 *
 *
 * Global:
 * Entry Conditions:
 * Return:
 * Called By:
 * Date: 07-11-2013
 * Modified:
 ********************************************/
HMolPolAnalysis::~HMolPolAnalysis()
{
  // Delete ROOT objects
  if (fEvent)        delete fEvent;
  if (fRootTree)     delete fRootTree;
  if (fRootBranch)   delete fRootBranch;
  if (fRootFile)     delete fRootFile;
}

/********************************************
 * Programmer: Valerie Gray
 * Function: BeginOfRun
 *
 * Purpose:  Set up what should be done at the beginning of a run.
 *
 * \define run is a set of events for which a whole root file is produced.
 *      i.e. if one has /run/beamOn/ 1000 then a run is 100 fired electrons
 *
 *
 * Global:
 * Entry Conditions: const G4Run* aRun
 * Return:
 * Called By:
 * Date: 07-11-2013
 * Modified:
 ********************************************/
void HMolPolAnalysis::BeginOfRun(const G4Run* aRun)
{
  G4cout << "At begin of run" << G4endl;

  // Get run number
  G4int runID = aRun->GetRunID();

  // Construct file name if not set explicitly
  // This is from the default name and the number of times one had
  // simulated a run in the present session of HMolPol (starting with 0)
  if (fRootFileName.size() == 0)
    fRootFileName = fRootFileStem + "_" + Form("%d",runID) + ".root";

  // Create ROOT file
  G4cout << "###### Analysis: creating ROOT file " << fRootFileName << G4endl;
  fRootFile = new TFile(fRootFileName,"RECREATE","HMolPol ROOT file");
  ConstructRootTree();
}

/********************************************
 * Programmer: Valerie Gray
 * Function: EndOfRun
 *
 * Purpose:  Say you are at the end of a run
 *
 * Global:
 * Entry Conditions: const G4Run* aRun
 * Return:
 * Called By:
 * Date: 07-11-2013
 * Modified:
 ********************************************/
void HMolPolAnalysis::EndOfRun(const G4Run* aRun)
{
  G4cout << "At end of run" << G4endl;

  // Autosave one last time
  AutoSaveRootTree();

  // Write the data to the ROOT file
  G4cout << "###### Analysis: closing ROOT file " << fRootFileName << G4endl;
  fRootFile->Write(0,TObject::kOverwrite);
  fRootFile->Close();
  fRootFileName = "";
}

/********************************************
 * Programmer: Valerie Gray
 * Function: ConstructRootTree
 *
 * Purpose:  Construct the actual ROOT tree in the ROOT file that
 *      was created earlier
 *
 * Global:
 * Entry Conditions:
 * Return:
 * Called By:
 * Date: 07-11-2013
 * Modified:
 ********************************************/
void HMolPolAnalysis::ConstructRootTree()
{
  // Create ROOT tree and give it a name
  // the tree name will be HMolPol_Tree in the ROOT file
  fRootTree = new TTree("HMolPol_Tree","HMolPol Simulation Tree");

  // Save the file after so many bytes. Avoids complete data loss after crash
  //fRootTree ->SetAutoSave(1000000);  //AutoSave after every 1 Mbyte written to disk

// \todo I have no idea what the next lines means (till end)
  // Instance of data structure to be written into ROOT file
  //fRootEvent  = new HMolPolEvent();

  // Create a branch with the data structure defined by fRootEvent
  // THE QWEAK WAY OF LIFE
  int bufsize = 64000;
  int split   = 99;
  fRootBranch = fRootTree->Branch("HMolPolEvent",
      "HMolPolEvent", &fEvent, bufsize, split);

  // Write run data
  //pUserRunInformation->Write();
}
/********************************************
 * Programmer: Valerie Gray
 * Function: AutoSaveRootTree
 *
 * Purpose:  To Auto Save the root tree as writing too it goes along
 *      Then take all the "small" trees and combine it into one large one.
 *
 * Global:
 * Entry Conditions:
 * Return:
 * Called By:
 * Date: 07-11-2013
 * Modified:
 ********************************************/
void HMolPolAnalysis::AutoSaveRootTree()
{
  // save the current ROOT Tree:
  // In case your program crashes before closing the file holding this tree,
  // the file will be automatically recovered when you will connect the file
  // in UPDATE mode.
  // The Tree will be recovered at the status corresponding to the last AutoSave.
  //
  // if option contains "SaveSelf", gDirectory->SaveSelf() is called.
  // This allows another process to analyze the Tree while the Tree is being filled.
  //
  /// \see http://root.cern.ch/root/html/TTree.html#TTree:AutoSave

  // fRootTree -> AutoSave("SaveSelf");
  fRootTree -> AutoSave();
  gDirectory -> Purge(); //Purge old trees

}
