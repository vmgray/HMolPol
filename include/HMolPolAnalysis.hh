/*
 * HMolPolAnalysis.hh
 *
 *  Created on: Jul 11, 2013
 *      Author: vmgray
 */

#ifndef HMOLPOLANALYSIS_HH_
#define HMOLPOLANALYSIS_HH_

// geant4 includes
#include <G4Types.hh>
#include <G4String.hh>

// root includes
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>

// geant4 classes
class G4Run;

//HMolPol classes
class HMolPolMainEvent;
class HMolPolRunInformation;

class HMolPolAnalysis {

  public:
    HMolPolAnalysis();
    virtual ~HMolPolAnalysis();
    void BeginOfRun(const G4Run* aRun);
    void EndOfRun(const G4Run* aRun);
    void EndOfEvent(G4int flag);

    // Set ROOT file stem or name
    void SetRootFileStem(const G4String& stem) { fRootFileStem = stem; }
    void SetRootFileName(const G4String& name) { fRootFileName = name; }

    void FillRootTree() { fRootTree->Fill(); }
    void AutoSaveRootTree();


  private:

    void ConstructRootTree();

    TTree*   fRootTree;
    TBranch* fRootBranch;
    TFile*   fRootFile;

    G4String fRootFileStem;
    G4String fRootFileName;

  public:
    HMolPolMainEvent* fRootEvent;

  private:
    HMolPolRunInformation* pUserRunInformation;
};

#endif /* HMOLPOLANALYSIS_HH_ */
