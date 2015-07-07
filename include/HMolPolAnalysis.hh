/********************************************
 * \author <b>Programmer:<\b> Valerie Gray
 * \author <b>Assisted By:<\b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is header file for
 *   the HMolPolAnalysis Class, which prints out
 *   the ROOT files
 *
 *   So it must include all the root tree variables
 *
 * \date <b>Date:</b> 07-11-2013
 * \date <b>Modified:</b> 08-20-2013
 *
 * \note <b>Entry Conditions:</b> none
 *
********************************************/

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
class HMolPolEventGenericDetector;
class HMolPolRunInformation;
class HMolPolMessenger;
class HMolPolEventPrimary;
class HMolPolEventUnits;

class HMolPolAnalysis
{

  public:
    HMolPolAnalysis();             ///< constructor for HMolPolAnalysis class
    virtual ~HMolPolAnalysis();    ///< destructor for HMolPolAnalysis class

    void BeginOfRun(const G4Run* aRun); ///< begin of run function
    void EndOfRun(const G4Run* aRun);   ///< End of run function
    void EndOfEvent();        ///< end of Event function

    ///<  Set ROOT file stem or name
    void SetRootFileStem(const G4String& stem) { fRootFileStem = stem; }
    ///< Set ROOT file name
    void SetRootFileName(const G4String& name) { fRootFileName = name; }

    ///< Fill the ROOT file
    void FillRootTree() { fRootTree->Fill(); }
    ///< Save the ROOT file
    void AutoSaveRootTree();

    ///< Function to add a new detector to the ROOT Tree.
    // This is so each detector has its own branch
    void AddNewDetector(const G4String& name) {
      fDetectorName.push_back(name);
    }

  private:

    void ConstructRootTree(); ///< function to construct the ROOT tree

    TTree*   fRootTree;  ///< ROOT tree variable
    TFile*   fRootFile;  ///< ROOT tree file

    G4String fRootFileStem;     ///< Root file stem
    G4String fRootFileName;     ///< Root file name (if changing it)

  public:

    // ROOT variables that are common for all events in the ROOT tree
    // for example, the run number, beam energy, etc. One run per ROOT
    // tree - single value for the whole run
    Int_t fRunNumber;           ///< Run number
    Double_t fBeamEnergy;       ///< Beam energy

    HMolPolEventUnits* fUnits;    ///< Primary unit structure
    TBranch* fUnitsBranch;              ///< Primary unit ROOT branch

    HMolPolEventPrimary* fPrimary;      ///< Primary event structure
    TBranch* fPrimaryBranch;            ///< Primary event ROOT branch

    std::vector<HMolPolEventGenericDetector*> fDetector; ///< Detector event structure
    std::vector<TBranch*> fDetectorBranch;              ///< ROOT file branches
    std::vector<G4String> fDetectorName;                ///< Detector names
    //wdc: add more infor on the fDetectorName what is etc.

  private:

    HMolPolRunInformation* pUserRunInformation;  ///< the HMolPolRunIOnformation structure
};

#endif /* HMOLPOLANALYSIS_HH_ */
