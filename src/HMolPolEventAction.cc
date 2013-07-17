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
#include "HMolPolMainEvent.hh"
#include "HMolPolEventAction.hh"
#include "HMolPolGenericDetectorHit.hh"

HMolPolEventAction::HMolPolEventAction (HMolPolAnalysis* AN)
: analysis(AN)
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
  analysis->fRootEvent->event_number = evt->GetEventID();
}
