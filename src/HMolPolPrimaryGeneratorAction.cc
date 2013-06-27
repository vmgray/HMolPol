/********************************************
 * Programmer: Valerie Gray
 * Purpose:
 * This file is to create the primary generator action.  This means that
 * the this generates the initial incoming particle.
 *
 * This is invoked at the beginning of each event.  This it generates the
 * primary particles, and its properties (momentum, energy directions, etc.)
 *
 * Entry Conditions:
 * Date: 06-25-2013
 * Modified:
 * Assisted By: Wouter Deconinck
 ********************************************/

//Geant4 includes
#include <G4Event.hh>
#include <G4ParticleGun.hh>

//HMolPol includes
#include "HMolPolPrimaryGeneratorAction.hh"

/********************************************
 * Programmer: Valerie Gray
 * Function: HMolPolPrimaryGeneratorAction
 *
 * Purpose:
 * Construction of the HMolPolPrimaryGeneratorAction
 *
 * Global:
 * Return: Nothing
 * Called By:
 * Date: 06-25-2013
 * Modified:
 ********************************************/
HMolPolPrimaryGeneratorAction::HMolPolPrimaryGeneratorAction()
{
  //set number of particles getting fired at a time
  G4int NubofParticles = 1;

  //set a particle gun with n number of particles
  ParticleGun = new G4ParticleGun(NubofParticles);

  //set particle type
  // *ADD* in functionality for other particles as incoming ones later
  //SetParticleType("e-");

  //set the beam energy

}

/********************************************
 * Programmer: Valerie Gray
 * Function: ~HMolPolPrimaryGeneratorAction
 *
 * Purpose:
 * Destruction of the HMolPolPrimaryGeneratorAction
 *
 * Global:
 * Return: nothing
 * Called By:
 * Date: 06-25-2013
 * Modified:
 ********************************************/
HMolPolPrimaryGeneratorAction::~HMolPolPrimaryGeneratorAction()
{
  if (ParticleGun) delete ParticleGun;
}

/********************************************
 * Programmer: Valerie Gray
 * Function: GeneratePrimaries
 *
 * Purpose:
 *
 * Sets up and generates the primaries
 *
 * Global:
 * Return:
 * Called By:
 * Date: 06-25-2013
 * Modified:
 ********************************************/
void HMolPolPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // *CHANGE* this right now it is HARDCODED which is BAD :(
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="e-");
  ParticleGun->SetParticleDefinition(particle);
  ParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.0,0.0,-1.0));
  ParticleGun->SetParticleEnergy(200.*MeV);
  ParticleGun->SetParticlePosition(G4ThreeVector(0.0*cm,0.0*cm,10.0*m));
  ParticleGun = ParticleGun;

  // finally : fire !!!
  ParticleGun->GeneratePrimaryVertex(anEvent);  // takes an event, generates primary vertex, and associates primary particles with the vertex

  G4cout << "###### Leaving QweakSimPrimaryGeneratorAction::GeneratePrimaries" << G4endl;

}
