/*
 * HMolPolEventAction.hh
 *
 *  Created on: Jul 11, 2013
 *      Author: vmgray
 */

#ifndef HMOLPOLEVENTACTION_HH_
#define HMOLPOLEVENTACTION_HH_

// system includes
#include <vector>
#include <map>

// root includes
#include <Rtypes.h>
#include <TString.h>

// geant4 includes
#include <G4Types.hh>
#include <G4String.hh>
#include <G4ThreeVector.hh>
#include <G4UserEventAction.hh>

// HMolPol classes
class HMolPolAnalysis;

class HMolPolEventAction : public G4UserEventAction
{
  public:

    //!Constructor
    HMolPolEventAction(HMolPolAnalysis* AN);

    //!Destructor
    ~HMolPolEventAction();

    void BeginOfEventAction(const G4Event* evt);
    void EndOfEventAction(const G4Event* evt);

  private:
    void Initialize();

    HMolPolAnalysis* analysis;

    G4int CollID;
};


#endif /* HMOLPOLEVENTACTION_HH_ */
