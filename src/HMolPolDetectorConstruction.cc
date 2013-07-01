/********************************************
 * Programmer: Valerie Gray
 * Purpose:
 *
 * This is the file is in charge
 *
 * Entry Conditions:
 * Date: 06-15-2013
 * Modified:
 * Assisted By: Wouter Deconinck
********************************************/

//Geant4 specific includes
#include <G4Colour.hh>
#include <G4VisAttributes.hh>
#include <G4NistManager.hh>
#include <G4FieldManager.hh>
#include <G4TransportationManager.hh>

//HMolPol includes
#include "HMolPolDetectorConstruction.hh"
#include "HMolPolHSolenoidMagField.hh"

/********************************************
 * Programmer: Valerie Gray
 * Function: Construct
 *
 * Purpose:
 *
 * To construct the geometry of the simulation.
 *    The world
 *    The H target and the Al cell
 *
 * Define the NIST materials
 *
 * Take any auxiliary information form the GDML files and make apply it
 *
 * Read in and apply the magnetic field
 *
 * Global:
 * Entry Conditions: none
 * Return:
 * Called By:
 * Date: 06-25-2013
 * Modified:
********************************************/

G4VPhysicalVolume* HMolPolDetectorConstruction::Construct()
{
  G4cout << "In HMollerPolDetectorConstruction::Construct()..." << G4endl;

  G4VPhysicalVolume *worldVolume;

  //for getting NIST materials
  // WTFM is the G4NistManager do, I should make the name better too
  G4NistManager* NistManager = G4NistManager::Instance();
  NistManager->SetVerbose(1);
  //add all the NIST materials we want to use
  NistManager->FindOrBuildMaterial("G4_Al");
  NistManager->FindOrBuildMaterial("G4_AIR");
  //this is vacuum
  NistManager->FindOrBuildMaterial("G4_Galactic");

  //if the simulation is using GDML
  //why don't we need #include <G4GDMLParser.hh>
  //what's a parser
  if (fGDMLParser) {
    //why are you deleting the G4GDMLParser right away
    delete fGDMLParser;
  }
  //then are you recreating the G4GDMLParser - this makes no sense
  fGDMLParser = new G4GDMLParser();
  fGDMLParser->Clear();
  //is this to check if the geometry overlaps?? - then why are then is there
  // no geometry yet?!?
  fGDMLParser->SetOverlapCheck(true);

  //read in the geometry - by reading the Mother Volume
  ///all of the Geometry will be read in since they are all
  //MotherVolume.gdml uses all they other ones, they will all be read
  // *CHANGE* so that HMolPolMotherVolume is not hard-coded and other geometries
  // can be able to read in easily
  fGDMLParser->Read("geometry/HMolPolMotherVolume.gdml");

  //the world volume is set as the world volume??
  //but how does this become the MotherVolume??
  worldVolume = fGDMLParser->GetWorldVolume();

  //==========================
  // List auxiliary info
  // this includes:
  //    color
  //    alpha (or opaqueness)
  //    detector type - if it is a sensitive detector
  //==========================

//Wouter broke this - he needs to fix
  //I am not sure what anything from this point on does.  HELP!!
  const G4GDMLAuxMapType* auxmap = fGDMLParser->GetAuxMap();

  G4cout << "Found " << auxmap->size()
                         << " volume(s) with auxiliary information."
                         << G4endl << G4endl;
  for(G4GDMLAuxMapType::const_iterator
      iter  = auxmap->begin();
      iter != auxmap->end(); iter++) {
    G4cout << "Volume " << ((*iter).first)->GetName()
                          << " has the following list of auxiliary information: "<< G4endl;
    for (G4GDMLAuxListType::const_iterator
        vit  = (*iter).second.begin();
        vit != (*iter).second.end(); vit++) {
      G4cout << "--> Type: " << (*vit).type
          << ", value: "   << (*vit).value << std::endl;

      if ((*vit).type == "Color") {
        G4Colour colour(1.0,0.0,0.0);
        if (G4Colour::GetColour((*vit).value, colour)) {
          G4cout << "Setting color to " << (*vit).value << "." << G4endl;
          G4VisAttributes* visAttribute = new G4VisAttributes(colour);
          ((*iter).first)->SetVisAttributes(visAttribute);
        } else {
          G4cout << "Colour " << (*vit).value << " is not known." << G4endl;
        }
      }

      if ((*vit).type == "Alpha") {
        const G4VisAttributes* visAttribute_old = ((*iter).first)->GetVisAttributes();
        G4Colour colour_old = visAttribute_old->GetColour();
        G4Colour colour_new(colour_old.GetRed(), colour_old.GetGreen(), colour_old.GetBlue(), 0.5);
        G4VisAttributes* visAttribute_new = new G4VisAttributes(colour_new);
        ((*iter).first)->SetVisAttributes(visAttribute_new);
      }
    }
  }
  G4cout << G4endl<< G4endl;

  //==========================
  // Add in the Magnetic field to world volume
  //==========================

  //get the field??
  HMolPolSolenoidMagField* HTargetSolenoidMagField = new HMolPolSolenoidMagField;

  //tell Geant 4 to use this field - how??
  G4FieldManager* HTargetSolenoidMagFieldMgr =
       G4TransportationManager :: GetTransportationManager()->GetFieldManager();

  //set the field for the HTargetSolenoidFieldMgr
  HTargetSolenoidMagFieldMgr->SetDetectorField(HTargetSolenoidMagField);

  //create the Chord finder (this is responsible for moving the particles
  //through the field
  HTargetSolenoidMagFieldMgr->CreateChordFinder(HTargetSolenoidMagField);


  //==========================
  // Return world volume
  //==========================

  delete fGDMLParser;

  return worldVolume;
}
