#ifndef HMOLPOLSTACKINGACTION_HH_
#define HMOLPOLSTACKINGACTION_HH_

// root include
#include <TObject.h>

#include <G4UserStackingAction.hh>
#include <G4Types.hh>

// Standard Lib includes
#include <map>

class HMolPolStackingAction: public G4UserStackingAction {
  public:
    HMolPolStackingAction();
    virtual ~HMolPolStackingAction() {};

    /// Classify New Particles
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track *track);

    void InitNewEvent();

    std::vector<G4int>& GetPrimaries()
    {
      return fPrimaryTrackID;
    }

    std::map<G4int,G4int>& GetTrackParentPrimaries()
    {
      return fTrackParentPrimaryID;
    }

  private:
    std::vector<G4int>    fPrimaryTrackID;
    std::map<G4int,G4int> fTrackParentPrimaryID;
};

#endif /* HMOLPOLSTACKINGACTION_HH_ */
