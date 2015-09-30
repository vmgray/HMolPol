#include "HMolPolStackingAction.hh"
#include <G4Track.hh>


/*
 */
HMolPolStackingAction::HMolPolStackingAction()
{

}

/*
 * Called everytime a new particle is created for tracking
 *
 * We are using it to "tag" any new particles as either primaries or
 * if the particle is a secondary, identify and tag it with its parent primary
 * ID.
 */
G4ClassificationOfNewTrack HMolPolStackingAction::ClassifyNewTrack(
    const G4Track *track)
{
  // Is it a primary? Primaries have no parent
  G4int parentID = track->GetParentID();
  G4int trackID = track->GetTrackID();
  if(parentID==0)
  {
    fPrimaryTrackID.push_back(trackID);
  }
  else
  {
    // This is a secondary particle, and we need to identify its primary
    // parent.  First, see if its parent is a primary directly.
    bool parentFound = false;
    for(size_t i = 0; i < fPrimaryTrackID.size() && !parentFound; i++) {
      if( fPrimaryTrackID[i] == parentID )
      {
        fTrackParentPrimaryID[trackID] = fPrimaryTrackID[i];
        parentFound = true;
      }
    }

    // If this track was not created by a primary, then we can still
    // identify its the primary by using the parent's primary information
    if(!parentFound)
    {
      fTrackParentPrimaryID[trackID] =
        fTrackParentPrimaryID[parentID];
    }
  }

  // Do the default GEANT4 thing
  return G4UserStackingAction::ClassifyNewTrack(track);
}


void HMolPolStackingAction::InitNewEvent()
{
  fTrackParentPrimaryID.clear();
  fPrimaryTrackID.clear();
}
