/********************************************
 * \author <b>Programmer:<\b> Valerie Gray
 * \author <b>Assisted By:<\b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is header file for
 *   the HMolPolAnalysis Class, which prints out
 *   the ROOT files
 *
 *   So it must include all the root tree variables
 * \todo wdc should expand this
 *
 * \date <b>Date:</b> 07-11-2013
 * \date <b>Modified:</b> 04-21-2015
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

    // functions that take care of what happens at the start of end of a run or event
    void BeginOfRun(const G4Run* aRun); ///< begin of run function
    void EndOfRun(const G4Run* aRun);   ///< End of run function

    ///<  Set ROOT file stem
    ///< RootfileStem is the standard rootfile name, that gets incremented
    ///< i.e. fRootFileStem = HMolPol then rootfiles are named HMolPol_%d.root
    ///< %d is an natural number
    void SetRootFileStem(const G4String& stem) { fRootFileStem = stem; }
    ///< Set ROOT file name
    ///< RootFileName is the name that the user gives a simulation, works for that
    ///< simulation and is not incremented.
    void SetRootFileName(const G4String& name) { fRootFileName = name; }

    ///< functions to take care of the ROOT file
    ///< Fill the ROOT file
    void FillRootTree() { fRootTree->Fill(); }
    ///< Save the ROOT file
    void AutoSaveRootTree();

    ///< Function to add a new detector to the ROOT Tree.
    // This is so each detector has its own branch
    //detectors are stored in a vector, thus the push_back function
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

    HMolPolEventUnits* fUnits;    ///< Primary event structure
    TBranch* fUnitsBranch;              ///< Primary event ROOT branch

    HMolPolEventPrimary* fPrimary;      ///< Primary event structure
    TBranch* fPrimaryBranch;            ///< Primary event ROOT branch

    std::vector<HMolPolEventGenericDetector*> fDetector; ///< Detector event structure
    std::vector<TBranch*> fDetectorBranch;              ///< ROOT file branches
    std::vector<G4String> fDetectorName;                ///< Detector names

  private:

    HMolPolRunInformation* pUserRunInformation;  ///< the HMolPolRunInformation structure
    ///< contain the information from GitHub, so the version of the simulation is
    ///< knowen
};

#endif /* HMOLPOLANALYSIS_HH_ */
