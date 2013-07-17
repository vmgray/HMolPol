/********************************************
 * Programmer: Valerie Gray
 * Purpose:
 *
 * This is header file which for the detector construction.
 *
 * Entry Conditions:
 * Date: 07-06-2013
 * Modified:
 * Assisted By: Wouter Deconinck
********************************************/

#ifndef HMOLPOLMESSENGER_HH_
#define HMOLPOLMESSENGER_HH_

//geant4 includes
#include <G4UImessenger.hh>
#include <G4UIcommand.hh>

//geant4 classes included
//class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
//class G4UIcmdWithAString;
//class G4UIcmdWithABool;

//HMolPol classes included
class HMolPolPrimaryGeneratorAction;

class HMolPolMessenger : public G4UImessenger {

  public:

    //create the constructor and destructor for the messenger
    HMolPolMessenger();
    ~HMolPolMessenger();

    /// \todo What is this??
    //function for IDK
    //void SetPriGen( HMolPolPrimaryGeneratorAction *pg ){ fprigen = pg; }

    /// \todo What is this for??
    //function to set value from a command
    void SetNewValue(G4UIcommand* cmd, G4String newValue);

  private:

    //create the raster size for the beam
    G4UIcmdWithADoubleAndUnit *rasXCmd; ///< raster y size
    G4UIcmdWithADoubleAndUnit *rasYCmd; ///< raster x size
};

#endif // HMOLPOLMESSENGER_HH_
