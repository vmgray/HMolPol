/********************************************
 * \author <b>Programmer:<\b> Valerie Gray
 * \author <b>Assisted By:<\b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is header file for the detector construction.
 *
 * \date <b>Date:</b> 07-11-2013
 * \date <b>Modified:</b> 08-26-2013
 *
 * \note <b>Entry Conditions:</b> none
 *
********************************************/

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

    HMolPolEventAction(HMolPolAnalysis* a);  ///< Constructor for
    // HMolPolEventAction

    virtual ~HMolPolEventAction();  ///< Destructor for HMolPolEventAction

    void BeginOfEventAction(const G4Event* evt);  ///< function BeginOfEventAction
    void EndOfEventAction(const G4Event* evt);  ///< function EndOfEventAction

  private:
    void Initialize();  ///< function Initialize

    HMolPolAnalysis* fAnalysis;  ///< the Analysis object

    G4int CollID;  ///< the colection Id number
};


#endif /* HMOLPOLEVENTACTION_HH_ */
