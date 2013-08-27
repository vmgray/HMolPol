/********************************************
 * \author <b>Programmer:</b> Valerie Gray
 * \author <b>Assisted By:</b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is the file is in charge the event action.
 *      What happens in the beginning and end of an event.
 *
 * \date <b>Date:</b> 07-11-2013
 * \date <b>Modified:</b> 07-18-2013
 *
 * \note <b>Entry Conditions:</b>
 *
 * \todo comment the comment blocks to get the how this is all put together
 *
 ********************************************/


// geant4 includes
#include <G4Event.hh>
#include <G4UserEventAction.hh>
#include <G4SDManager.hh>

// HMolPol includes
#include "HMolPolAnalysis.hh"
#include "HMolPolEventAction.hh"
#include "HMolPolEventGenericDetector.hh"
#include "HMolPolGenericDetectorHit.hh"

/********************************************
 * Programmer: Valerie Gray
 * Function: HMolPolEventAction
 *
 * Purpose: to construct the HMolPolEventAction
 *
 * Global:
 * Entry Conditions: HMolPolAnalysis object
 * Return:
 * Called By:
 * Date: 07-11-2013
 * Modified:
 ********************************************/

HMolPolEventAction::HMolPolEventAction (HMolPolAnalysis* a)
: fAnalysis(a),CollID(-1)
{

}
/********************************************
 * Programmer: Valerie Gray
 * Function: ~HMolPolEventAction
 *
 * Purpose: to destruct the HMolPolEventAction
 *
 * Global:
 * Entry Conditions:
 * Return:
 * Called By:
 * Date: 07-11-2013
 * Modified:
 ********************************************/
HMolPolEventAction::~HMolPolEventAction()
{

}

/********************************************
 * Programmer: Valerie Gray
 * Function: BeginOfEventAction
 *
 * Purpose: What happens at the beginning of a event
 *
 * Global:
 * Entry Conditions: G4Event
 * Return:
 * Called By:
 * Date: 07-11-2013
 * Modified:
 ********************************************/
void HMolPolEventAction::BeginOfEventAction(const G4Event* /*evt*/)
{
  //Say that an event is beginning
  G4cout << "At begin of event" << G4endl;

/// \bug not sure what this does at this point forward - it is unused!?!
//  G4SDManager* SDman = G4SDManager::GetSDMpointer();

  // check for existing Collection ID (if it's -1 it will be assigned)
  /// \todo reenable hit collection
//  if (CollID == -1) {
//      CollID = SDman->GetCollectionID("SD/Collection");
//  }
}

/********************************************
 * Programmer: Valerie Gray
 * Function: EndOfEventAction
 *
 * Purpose: What happens at the end of a event
 *      - clear all hits
 *      - loop over hit collection
 *      - get information about hits
 *
 * Global:
 * Entry Conditions: G4Event
 * Return:
 * Called By:
 * Date: 07-11-2013
 * Modified:
 ********************************************/
void HMolPolEventAction::EndOfEventAction(const G4Event* evt)
{
  //We are at the end of an event
  //  G4cout << "At end of event" << G4endl;

  // Get the event number
  /// \todo Event number should be branch in ROOT tree
  //fAnalysis->fPrimary->fEventNumber = evt->GetEventID();


  // Clear all hits
  for (size_t i = 0; i < fAnalysis->fDetector.size(); i++)
  {
    G4cout << "Clearing branch " << i << G4endl;
    fAnalysis->fDetector[i]->fHits.clear();
  }

  // Get the hit collections of this event
  G4HCofThisEvent *HCE = evt->GetHCofThisEvent();

  // Loop over all hit collections, sort by output type
  for (int hcidx = 0; hcidx < HCE->GetCapacity(); hcidx++)
  {

    // Get this particular hit collection
    G4VHitsCollection* thiscol = HCE->GetHC(hcidx);

    // This is NULL if nothing is stored
    if (thiscol)
    {
  //I do get in here.
      // Dynamic cast to test for types
      if (HMolPolGenericDetectorHitsCollection* thiscast =
          dynamic_cast<HMolPolGenericDetectorHitsCollection*>(thiscol))
      {
//We never get in this loop
        // Process all hits
        for (unsigned int hidx = 0; hidx < thiscast->GetSize(); hidx++)
        {
          G4cout << "made it too the loop to process all hits" << G4endl;

          HMolPolGenericDetectorHit* thisHit =
              (HMolPolGenericDetectorHit*) thiscast->GetHit(hidx);

          // Create new hit
          HMolPolEventGenericDetectorHit hit;
          hit.fTrackID = thisHit->GetTrackID();
          hit.fDetectorID = thisHit->GetDetectorID();
          hit.fPosition = TVector3(
                            thisHit->GetPosition().x(),
                            thisHit->GetPosition().y(),
                            thisHit->GetPosition().z());
          hit.fMomentum = TVector3(
                            thisHit->GetMomentum().x(),
                            thisHit->GetMomentum().y(),
                            thisHit->GetMomentum().z());
          hit.fParticleName = thisHit->GetParticleName();
          hit.fParticleType = thisHit->GetParticleType();
          hit.fTotalEnergy = thisHit->GetTotalEnergy();
          hit.fKineticEnergy = thisHit->GetKineticEnergy();

          // Add hit
          G4cout << "Adding hit to branch " << hit.fDetectorID << G4endl;
          fAnalysis->fDetector[hit.fDetectorID]->fHits.push_back(hit);
        }
      }
    }
  }

  //Debugging
  G4cout << "At end of event number " << evt->GetEventID() << G4endl;

  // Fill the tree
  fAnalysis->FillRootTree();

  // Print some progress information
  if (evt->GetEventID() % 1000 == 0)
      fAnalysis->AutoSaveRootTree();
}
