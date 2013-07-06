/********************************************
 * Programmer: Valerie Gray
 * Purpose:
 *
 * This is header file which for the primary action generator.
 * Or the generator of the primary particle
 *
 * Entry Conditions:
 * Date: 06-25-2013
 * Modified: 07-06-2013
 * Assisted By: Wouter Deconinck
********************************************/

#ifndef HMolPolPrimaryGeneratorAction_h
#define HMolPolPrimaryGeneratorAction_h 1

//Geant4 includes
#include <G4ParticleTable.hh>
#include <G4VUserPrimaryGeneratorAction.hh>

//Geant4 classes
class G4ParticleGun;

class HMolPolPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{

  public:
    //create the constructor and destructor for the primary particle generator
    HMolPolPrimaryGeneratorAction();
    virtual ~HMolPolPrimaryGeneratorAction();

    //function to generate the primary electrons
    void GeneratePrimaries(G4Event* anEvent);


  private:

    // Center of mass angles to generate particles between
    /// \todo make this so that they are passed and in the messenger
    double fTheta_com_min; /// < this is the minimum Theta that electrons
    /// will be generated over
    double fTheta_com_max; /// < this is the minimum Theta that electrons
    /// will be generated over

    //the primary generation of the primary electrons
    /// \todo why does doxygen not want this documented??
    G4ParticleGun* fParticleGun;

};

#endif
