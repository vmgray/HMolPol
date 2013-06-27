/********************************************
 * Programmer: Valerie Gray
 * Purpose:
 *
 * This is header file which for the detector construction.
 *
 * Entry Conditions:
 * Date: 06-15-2013
 * Modified:
 * Assisted By: Wouter Deconinck
********************************************/

//Geant4 includes
#include <G4VUserDetectorConstruction.hh>
#include <G4VPhysicalVolume.hh>
#include <G4GDMLParser.hh>

class HMollerPolDetectorConstruction: public G4VUserDetectorConstruction
{
  private:

    G4GDMLParser* fGDMLParser; ///< GDML parser object

  public:

    HMollerPolDetectorConstruction() {
      fGDMLParser = 0;
    };
    virtual ~HMollerPolDetectorConstruction() { };

  public:

    G4VPhysicalVolume* Construct();

};
