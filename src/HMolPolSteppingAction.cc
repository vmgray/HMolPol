#include "HMolPolSteppingAction.hh"
#include "HMolPolEventPrimaryHit.hh"
#include "HMolPolAnalysis.hh"
#include <G4Track.hh>


/*
 * TODO: Max transportation time should be configurable through the messenger
 * class.
 */
HMolPolSteppingAction::HMolPolSteppingAction( HMolPolAnalysis *analysis) :
  fAnalysis(analysis), fMaxTransportTime(1*CLHEP::s), fTrackPrimaries(true)
{
}

/*
 * Called at every step by Geant4.
 */
void HMolPolSteppingAction::UserSteppingAction(const G4Step* step)
{
  G4Track *track = step->GetTrack();
  G4double time = track->GetGlobalTime();
  if(time > fMaxTransportTime) {
    // Kill the track, it's probably stuck in a magnetic field somewhere...
    G4cerr << "Particle time too long, forced stop." << G4endl;
    track->SetTrackStatus(fStopAndKill);
  }

  // Should primaries be tracked at every step?
  if(fTrackPrimaries && track->GetParentID() == 0)
  {
    HMolPolEventPrimaryHit hit;
    hit.fPrimaryID = track->GetTrackID();
    hit.fVolumeName = track->GetVolume()->GetName();
    hit.fPosition = TVector3(
        track->GetPosition().x(),
        track->GetPosition().y(),
        track->GetPosition().z());
    hit.fMomentum = TVector3(
        track->GetMomentum().x(),
        track->GetMomentum().y(),
        track->GetMomentum().z());
    fAnalysis->fPrimaryHits->push_back(hit);
  }
}
