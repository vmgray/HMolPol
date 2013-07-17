/*
 * HMolPolRunAction.cc
 *
 *  Created on: Jul 16, 2013
 *      Author: vmgray
 */

#include "HMolPolRunAction.hh"

// geant4 includes
#include <G4Run.hh>

// HMolPol includes
#include "HMolPolAnalysis.hh"

HMolPolRunAction::HMolPolRunAction(HMolPolAnalysis* a)
: fAnalysis(a),runID(0)
{

}

HMolPolRunAction::~HMolPolRunAction()
{

}

void HMolPolRunAction::BeginOfRunAction(const G4Run* aRun)
{
  // Get run number
  runID = aRun->GetRunID();

  // Print run number
  G4cout << "### Start of Run " << runID << G4endl;

  // Start analysis at begin of run
  fAnalysis->BeginOfRun(aRun);
}

void HMolPolRunAction::EndOfRunAction(const G4Run* aRun)
{
  // Print run number
  G4cout << "### End of Run " << runID << G4endl;

  // End analysis procedure at end of run
  fAnalysis->EndOfRun(aRun);
}
