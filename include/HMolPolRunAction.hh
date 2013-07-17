/*
 * HMolPolRunAction.hh
 *
 *  Created on: Jul 16, 2013
 *      Author: vmgray
 */

#ifndef HMOLPOLRUNACTION_HH_
#define HMOLPOLRUNACTION_HH_

#include <G4UserRunAction.hh>
#include <G4Run.hh>

// geant4 classes
class G4Run;

// HMolPol classes
class HMolPolAnalysis;

class HMolPolRunAction : public G4UserRunAction
{
  public:
    HMolPolRunAction(HMolPolAnalysis* AN);
    ~HMolPolRunAction();

  public:
    void BeginOfRunAction(const G4Run*);
    void EndOfRunAction(const G4Run*);

  private:
    HMolPolAnalysis* analysis;
    G4int runID;
};

#endif /* HMOLPOLRUNACTION_HH_ */
