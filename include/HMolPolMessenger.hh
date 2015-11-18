/********************************************
 * \author <b>Programmer:<\b> Valerie Gray
 * \author <b>Assisted By:<\b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is header file for the HMolPolMessenger.
 *      The messenger talks to the interface (user) and changes variables
 *      of the simulation
 *
 * \date <b>Date:</b> 07-06-2013
 * \date <b>Modified:</b> 07-25-2013
 *
 * \note <b>Entry Conditions:</b> none
 *
********************************************/

#ifndef HMOLPOLMESSENGER_HH_
#define HMOLPOLMESSENGER_HH_

//geant4 includes
#include <G4UImessenger.hh>
#include <G4UIcommand.hh>

//geant4 classes included
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithABool;

//HMolPol classes included
class HMolPolPrimaryGeneratorAction;
class HMolPolRunAction;
class HMolPolEventAction;
class HMolPolAnalysis;
class HMolPolDetectorConstruction;
class HMolPolSteppingAction;

class HMolPolMessenger : public G4UImessenger
{

  public:

    HMolPolMessenger(HMolPolPrimaryGeneratorAction*,
        HMolPolRunAction*,
        HMolPolEventAction*,
        HMolPolAnalysis*,
        HMolPolDetectorConstruction*,
        HMolPolSteppingAction*);   ///< constructor of the HMolPolMessenger
    virtual ~HMolPolMessenger();   ///< destructor of the HMolPolMessenger

    void SetNewValue(G4UIcommand* cmd, G4String newValue);   ///< function to
      /// set value from a command and gets its value

  private:

    // Pointers to the relevant classes
    HMolPolPrimaryGeneratorAction*      fPrimaryGeneratorAction;   ///< pointer
      /// to the primary Generator action class
    HMolPolRunAction*                   fRunAction;   ///< pointer
      /// to the Run Action class
    HMolPolEventAction*                 fEventAction;   ///< pointer
      /// to the Event Action class
    HMolPolAnalysis*                    fAnalysis;   ///< pointer
      /// to the Analysis class
    HMolPolDetectorConstruction*        fDetectorConstruction; ///< pointer
      /// to the DetectorConstruction class

    HMolPolSteppingAction*              fSteppingAction; ///< pointer to the
     /// SteppingAction class

    //Create different directories
      //These directories are the
    G4UIdirectory*        fAnalysisDir;   ///< create the Analysis Directory
      /// all analysis related variables (ROOT file names)
    G4UIdirectory*        fBeamDir;       ///< create the Beam Directory
      /// - all beam related variables

/// Analysis related
    //create unique ROOT file names in Macro/interactively
    G4UIcmdWithAString*   fRootFileStemCmd;  ///< Root file stem name
    G4UIcmdWithAString*   fRootFileNameCmd;  ///< Root file name

/// Beam related variables
    //create the raster size for the beam
    G4UIcmdWithADoubleAndUnit* fRasXCmd; ///< raster y size
    G4UIcmdWithADoubleAndUnit* fRasYCmd; ///< raster x size

    G4UIcmdWithADoubleAndUnit* fBeamECmd; ///< beam energy

/// Geometry related variables
    G4UIdirectory*        fGeometryDir;       ///< create the Geometry Directory
    G4UIcmdWithAString*   fGeometryFileNameCmd;  ///< Geometry File Name

/// Tracking/Step related variables
    G4UIdirectory*        fTrackingDir;      ///< create Tracking directory
    G4UIcmdWithABool*     fTrackPrimariesCmd; ///< Should primaries be tracked?
};

#endif // HMOLPOLMESSENGER_HH_
