/********************************************
 * Programmer: Valerie Gray
 * Purpose:
 *
 * This is header file which for the detector construction.
 *
 * Entry Conditions:
 * Date: 06-15-2013
 * Modified: 07-06-2013
 * Assisted By: Wouter Deconinck
********************************************/


//Geant4 includes
#include <G4VUserDetectorConstruction.hh>
#include <G4VPhysicalVolume.hh>
#include <G4GDMLParser.hh>

class HMolPolDetectorConstruction: public G4VUserDetectorConstruction
{
  private:

    G4GDMLParser* fGDMLParser; ///< GDML parser object

  public:

    //Construct and the destructor of for the Detector construction
    HMolPolDetectorConstruction() {
      fGDMLParser = 0;
    };
    virtual ~HMolPolDetectorConstruction() { };

  public:

    /// Construct the volume??
    G4VPhysicalVolume* Construct();  ///< construct the physical volume

};
