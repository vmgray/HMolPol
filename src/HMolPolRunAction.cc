/********************************************
 * \author <b>Programmer:</b> Valerie Gray
 * \author <b>Assisted By:</b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is the file is to do the begin and end
 *      of run actions
 *
 *      Run is a set of n beam events that are stored in a ROOT file
 *
 * \date <b>Date:</b> 07-16-2013
 * \date <b>Modified:</b> 07-26-2013
 *
 * \note <b>Entry Conditions:</b>
 *
 ********************************************/

#include "HMolPolRunAction.hh"

// geant4 includes
#include <G4Run.hh>

// HMolPol includes
#include "HMolPolAnalysis.hh"

/********************************************
 * Programmer: Valerie Gray
 * Function: HMolPolRunAction
 *
 * Purpose: Construct the HMolPolRunAction
 *
 * Global:
 * Entry Conditions: HMolPolAnalysis
 * Return:
 * Called By:
 * Date: 07-16-2013
 * Modified:
 ********************************************/
HMolPolRunAction::HMolPolRunAction(HMolPolAnalysis* a)
: fAnalysis(a),runID(0)
{

}

/********************************************
 * Programmer: Valerie Gray
 * Function: ~HMolPolRunAction
 *
 * Purpose: Destruct the HMolPolRunAction
 *
 * Global:
 * Entry Conditions: none
 * Return:
 * Called By:
 * Date: 07-16-2013
 * Modified:
 ********************************************/
HMolPolRunAction::~HMolPolRunAction()
{

}

/********************************************
 * Programmer: Valerie Gray
 * Function: BeginOfRunAction
 *
 * Purpose: function of what to do at the beginning of a run
 *
 * Global:
 * Entry Conditions: none
 * Return:
 * Called By:
 * Date: 07-16-2013
 * Modified:
 ********************************************/
void HMolPolRunAction::BeginOfRunAction(const G4Run* aRun)
{
  // Get run number
  runID = aRun->GetRunID();

  // Print run number
  G4cout << "### Start of Run " << runID << G4endl;

  // Start analysis at begin of run
  fAnalysis->BeginOfRun(aRun);
}

/********************************************
 * Programmer: Valerie Gray
 * Function: EndOfRunAction
 *
 * Purpose: function of what to do at the end of the run
 *
 * Global:
 * Entry Conditions: none
 * Return:
 * Called By:
 * Date: 07-16-2013
 * Modified:
 ********************************************/
void HMolPolRunAction::EndOfRunAction(const G4Run* aRun)
{
  // Print run number
  G4cout << "### End of Run " << runID << G4endl;

  // End analysis procedure at end of run
  fAnalysis->EndOfRun(aRun);
}
