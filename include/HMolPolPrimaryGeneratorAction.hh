/********************************************
 * Programmer: Valerie Gray
 * Purpose:
 *
 * This is header file which for the primary action generator.
 * Or the generator of the primary particle
 *
 * Entry Conditions:
 * Date: 06-25-2013
 * Modified:
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
    //create the constructor and deconstructor for the primary particle generator
    HMolPolPrimaryGeneratorAction();
    ~HMolPolPrimaryGeneratorAction();

    //function to generate the primary electrons
    void GeneratePrimaries(G4Event* anEvent);

  private:
    //the primary generation of the primary electrons
    G4ParticleGun* ParticleGun;
};

#endif
