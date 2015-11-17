#ifndef HMOLPOLSTEPPINGACTION_HH_
#define HMOLPOLSTEPPINGACTION_HH_

// root include
#include <TObject.h>

#include <G4UserSteppingAction.hh>
#include <G4Types.hh>
class HMolPolAnalysis;

class HMolPolSteppingAction: public G4UserSteppingAction {
  public:
    HMolPolSteppingAction( HMolPolAnalysis *analysis );
    virtual ~HMolPolSteppingAction() {};
    void UserSteppingAction(const G4Step* step);
    void SetTrackPrimaries(G4bool trackPrimaries) {
      fTrackPrimaries = trackPrimaries;
    }

  private:
    HMolPolAnalysis *fAnalysis; ///< Pointer to the Analysis instance
    G4double fMaxTransportTime; ///< Max transportation time before track is killed
    G4bool   fTrackPrimaries;   ///< Flag to track primaries or not primary hits.
};

#endif /* HMOLPOLSTEPPINGACTION_HH_ */
