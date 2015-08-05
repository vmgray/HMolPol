/********************************************
 * \author <b>Programmer:<\b> Valerie Gray
 * \author <b>Assisted By:<\b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is header file for
 *   the HMolPolAnalysis Class, which prints out
 *   the ROOT files
 *
 *   This class includes the top-level ROOT tree variables. For example, if the
 *   ROOT tree contains a branch 'Primary' which is (for each event) an object
 *   of the class HMolPolEventPrimary, then this class must contain a pointer
 *   to HMolPolEventPrimary which is named fPrimary. That branch is then added
 *   to the ROOT tree in the function ConstructRootTree().
 *
 *   For the various detectors this class has a vector of pointers to
 *   HMolPolEventGenericDetector objects. Each pointer corresponds to one
 *   branch in the ROOT tree. If you have a branch 'Detector1' and a second
 *   branch 'Detector2' then you will have a vector with 2 pointers to
 *   HMolPolEventGenericDetector objects.
 *
 * \date <b>Date:</b> 07-11-2013
 * \date <b>Modified:</b> 07-06-2015
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
    HMolPolAnalysis();             ///< Constructor for HMolPolAnalysis class
    virtual ~HMolPolAnalysis();    ///< Destructor for HMolPolAnalysis class

    // functions that take care of what happens at the start of end of a run or event
    void BeginOfRun(const G4Run* aRun); ///< Begin of run function
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

    HMolPolEventUnits* fUnits;    ///< Primary unit structure
    TBranch* fUnitsBranch;              ///< Primary unit ROOT branch

    HMolPolEventPrimary* fPrimary;      ///< Primary event structure
    TBranch* fPrimaryBranch;            ///< Primary event ROOT branch

    /// Event structure for the branches in the ROOT tree corresponding to a
    /// particular detector. There could be several of them so they are stored
    /// in a vector: one element in the vector for each detector that is stored
    /// in the ROOT tree. For example, when there are two branches, 'Detector1'
    /// and 'Detector2', then there will be two elements in this vector. The
    /// pointers in this vector only describe what data to store, not what the
    /// branch will be called.
    std::vector<HMolPolEventGenericDetector*> fDetector;
    /// ROOT file branches associated with the corresponding elements in the
    /// vector fDetector above. The  pointers in this vector only identify to
    /// ROOT where the branches are stored in memory, not what the branch will
    /// be called.
    std::vector<TBranch*> fDetectorBranch;
    /// Name of the detector associated with the corresponding elements in the
    /// vector fDetector above. For example, if you have two sensitive detectors
    /// defined in the geometry, 'Detector1' and 'Detector2', then this vector
    /// will contain the strings 'Detector1' and 'Detector2'. That's all this
    /// vector does: store the name of the branches in the ROOT file.
    std::vector<G4String> fDetectorName;

  private:

    HMolPolRunInformation* pUserRunInformation;  ///< the HMolPolRunInformation structure
    ///< contain the information from GitHub, so the version of the simulation is
    ///< knowen
};

#endif /* HMOLPOLANALYSIS_HH_ */
