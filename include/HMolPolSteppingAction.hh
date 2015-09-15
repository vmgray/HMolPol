#ifndef HMOLPOLSTEPPINGACTION_HH_
#define HMOLPOLSTEPPINGACTION_HH_

// root include
#include <TObject.h>

#include <G4UserSteppingAction.hh>
#include <G4Types.hh>

class HMolPolSteppingAction: public G4UserSteppingAction {
  public:
    HMolPolSteppingAction();
    virtual ~HMolPolSteppingAction() {};
    void UserSteppingAction(const G4Step* step);

  private:
    G4double fMaxTransportTime; ///< Max transportation time before track is killed
};

#endif /* HMOLPOLSTEPPINGACTION_HH_ */
