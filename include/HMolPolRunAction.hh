/********************************************
 * \author <b>Programmer:<\b> Valerie Gray
 * \author <b>Assisted By:<\b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is header file for the run action.
 *
 * \date <b>Date:</b> 07-16-2013
 * \date <b>Modified:</b> 07-06-2013
 *
 * \note <b>Entry Conditions:</b> none
 *
********************************************/

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
    HMolPolRunAction(HMolPolAnalysis* a);   ///< Constructor for HMolPolRunAction
    virtual ~HMolPolRunAction();   ///< destructor for HMolPolRunAction

  public:
    void BeginOfRunAction(const G4Run*);   ///< BeginOfRunAction function
    void EndOfRunAction(const G4Run*);   ///< EndOfRunAction function

  private:
    HMolPolAnalysis* fAnalysis;   ///< HMolPolAnalysis object
    G4int runID;   ///< the run id number
};

#endif /* HMOLPOLRUNACTION_HH_ */
