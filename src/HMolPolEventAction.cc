/*
 * HMolPolEventAction.cc
 *
 *  Created on: Jul 11, 2013
 *      Author: vmgray
 */


// geant4 includes
#include <G4Event.hh>
#include <G4UserEventAction.hh>
#include <G4SDManager.hh>

// HMolPol includes
#include "HMolPolAnalysis.hh"
#include "HMolPolEvent.hh"
#include "HMolPolEventAction.hh"
#include "HMolPolGenericDetectorHit.hh"

HMolPolEventAction::HMolPolEventAction (HMolPolAnalysis* a)
: fAnalysis(a)
{

}

HMolPolEventAction::~HMolPolEventAction()
{

}

void HMolPolEventAction::BeginOfEventAction(const G4Event* /*evt*/)
{
  G4cout << "At begin of event" << G4endl;

  G4SDManager* SDman = G4SDManager::GetSDMpointer();

  // check for existing Collection ID (if it's -1 it will be assigned)
  if (CollID == -1) {
      CollID = SDman->GetCollectionID("SD/Collection");
  }
}

void HMolPolEventAction::EndOfEventAction(const G4Event* evt)
{
  G4cout << "At end of event" << G4endl;

  // Get the event number
  fAnalysis->fEvent->fEventNumber = evt->GetEventID();

  // Fill the tree
  fAnalysis->FillRootTree();

  // Print some progress information
  if (evt->GetEventID() % 1000 == 0)
      fAnalysis->AutoSaveRootTree();
}
