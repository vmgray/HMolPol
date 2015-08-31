#include "HMolPolSteppingAction.hh"
#include <G4Track.hh>


/*
 * TODO: Max transportation time should be configurable through the messenger
 * class.
 */
HMolPolSteppingAction::HMolPolSteppingAction() :
  fMaxTransportTime(1*CLHEP::s)
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
  return;
}
