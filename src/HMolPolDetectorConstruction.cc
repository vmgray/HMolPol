/********************************************
 * \author <b>Programmer:</b> Valerie Gray
 * \author <b>Assisted By:</b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is the file is in charge reading in the geometry
 * of the simulations and therefore the polarimeter.  This is also sets any
 * attributes of the objects in the simulations, and any magnetic fields.
 *
 * \date <b>Date:</b> 06-25-2013
 * \date <b>Modified:</b> 07-06-2013
 *
 * \note <b>Entry Conditions:</b>
 *
 * \todo comment the comment blocks to get the how this is all put together
 *
********************************************/

//Geant4 specific includes
#include <G4Colour.hh>
#include <G4VisAttributes.hh>
#include <G4NistManager.hh>
#include <G4FieldManager.hh>
#include <G4TransportationManager.hh>
#include <G4SDManager.hh>

#include <fstream>
/// \bug what is the above and what is a singleton?? it is everywhere and I
/// can't heads or tails of it.

//HMolPol includes
#include "HMolPolDetectorConstruction.hh"
#include "HMolPolHSolenoidMagField.hh"
#include "HMolPolGenericDetector.hh"

/// \bug what are these for and why are they defined here, how do they get
/// there values of 200 and 500 respectively
#define __DET_STRLEN 200  //maximum length of string of the detector name
#define __MAX_DETS 500  //maximum number of sensitive detectors

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
  /// \bug G4NistManager do, I should make the name better too
  G4NistManager* NistManager = G4NistManager::Instance();
  NistManager->SetVerbose(1);

  // Open file with NIST materials
  std::ifstream nist_materials;
  nist_materials.open("nist_materials.txt");
  // Check whether materials file could be opened correctly
  if (nist_materials.good()) {
    //add all the NIST materials we want to use
    std::string material;
    // Loop over all lines in the file
    while (! nist_materials.eof()) {
      nist_materials >> material;
      // Try to load the NIST material
      G4bool success = NistManager->FindOrBuildMaterial(material);
      // And complain if we can't find the material
      if (! success) {
        G4cout << "Material " << material << " not found!" << G4endl;
      }
    }
  } else {
    // If the file cannot be found, load only the vacuum
    NistManager->FindOrBuildMaterial("G4_Galactic");
    G4cout << "File nist_materials.txt not found.  "
           << "Only loading NIST vacuum!" << G4endl;
  }
  // Close the file
  nist_materials.close();

  //if the simulation is using GDML
  //why don't we need #include <G4GDMLParser.hh>
  //what's a parser
  if (fGDMLParser) {
    //why are you deleting the G4GDMLParser right away
    delete fGDMLParser;
  }
  /// \todo then are you recreating the G4GDMLParser - this makes no sense
  fGDMLParser = new G4GDMLParser();
  fGDMLParser->Clear();
  //is this to check if the geometry overlaps?? - then why are then is there
  // no geometry yet?!?
  fGDMLParser->SetOverlapCheck(true);

  //read in the geometry - by reading the Mother Volume
  ///all of the Geometry will be read in since they are all
  //MotherVolume.gdml uses all they other ones, they will all be read
  /// \bug *CHANGE* so that HMolPolMotherVolume is not hard-coded and
  /// other geometries can be able to read in easily
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
  /// \todo have someone (Wouter) help me figure what this all does
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

      // Support for the auxiliary tag "Visibility" that can be
      // "true" to show the volume
      // "false" to hide the volume
      // "wireframe" to show the volume as wireframe only
      if ((*vit).type == "Visibility") {
        G4Colour colour(1.0,1.0,1.0);
        // get old color
        const G4VisAttributes* visAttribute_old =
            ((*iter).first)->GetVisAttributes();

        if (visAttribute_old)
          colour = visAttribute_old->GetColour();
        // create new visibility attributes
        G4VisAttributes visAttribute_new(colour);
        if ((*vit).value == "true")
          visAttribute_new.SetVisibility(true);
        if ((*vit).value == "false")
          visAttribute_new.SetVisibility(false);
        if ((*vit).value == "wireframe")
          visAttribute_new.SetForceWireframe(true);
        // set new visibility attributes
        ((*iter).first)->SetVisAttributes(visAttribute_new);
      }

      // Support for the auxiliary tag "Color" that can be any of black,
      // blue, brown, cyan, gray, green, grey, magenta, red, white, yellow
      if ((*vit).type == "Color") {
        G4Colour colour(1.0,1.0,1.0);
        // get requested color, if it exists
        if (G4Colour::GetColour((*vit).value, colour)) {
          G4cout << "Setting color to " << (*vit).value << "." << G4endl;
          // create new visibility attributes
          G4VisAttributes visAttribute(colour);
          // set new visibility attributes
          ((*iter).first)->SetVisAttributes(visAttribute);
        } else {
          G4cout << "Colour " << (*vit).value << " is not known." << G4endl;
        }
      }

      // Support for the auxiliary tag "Alpha" to set opacity,
      // a value of 0.0 is fully transparent, 1.0 is fully opaque
      if ((*vit).type == "Alpha") {
        G4Colour colour(1.0,1.0,1.0);
        // get old color
        const G4VisAttributes* visAttribute_old =
            ((*iter).first)->GetVisAttributes();

        if (visAttribute_old)
          colour = visAttribute_old->GetColour();
        // create new color with alpha channel (TODO input not checked)
        G4Colour colour_new(
            colour.GetRed(),
            colour.GetGreen(),
            colour.GetBlue(),
            std::atof((*vit).value.c_str()));
        // create new visibility attributes
        G4VisAttributes visAttribute_new(colour_new);
        // set new visibility attributes
        ((*iter).first)->SetVisAttributes(visAttribute_new);
      }
    }
  }
  G4cout << G4endl<< G4endl;

/// \todo detector numbers?? do we need them?  How do these auxiliary tags work?

  //==========================
  // Sensitive detectors
  //==========================
/// this is all taken from remoll since that is what WD wanted me to do it work
  /// via magic
  char detectorname[__DET_STRLEN];
  int retval;

  G4int k=0;

  G4GDMLAuxMapType::const_iterator iter;
  G4GDMLAuxListType::const_iterator vit, nit;

  G4cout << "Beginning sensitive detector assignment" << G4endl;

  G4bool useddetnums[__MAX_DETS];
  for( k = 0; k < __MAX_DETS; k++ ){useddetnums[k] = false;}
  k = 0;

  for( iter  = auxmap->begin(); iter != auxmap->end(); iter++) {
      G4LogicalVolume* myvol = (*iter).first;
      G4cout << "Volume " << myvol->GetName() << G4endl;

      for( vit  = (*iter).second.begin(); vit != (*iter).second.end(); vit++) {
          if ((*vit).type == "SensDet") {
              G4String det_type = (*vit).value;

              // Also allow specification of det number ///////////////////
              int det_no = -1;
              for( nit  = (*iter).second.begin(); nit != (*iter).second.end(); nit++) {
                  if ((*nit).type == "DetNo") {
                      det_no= atoi((*nit).value.data());
                      useddetnums[det_no] = true;
                  }
              }
              if( det_no <= 0 ){
                  k = 1;
                  while( useddetnums[k] == true && k < __MAX_DETS ){ k++; }
                  if( k == __MAX_DETS ){
                      G4cerr << __FILE__ << " line " << __LINE__
                             << ": ERROR too many detectors" << G4endl;
                      exit(1);
                  }
                  det_no = k;
                  useddetnums[k] = true;
              }
              /////////////////////////////////////////////////////////////

              retval = snprintf(detectorname, __DET_STRLEN,
                  "hmolpol/det_%s", det_type.c_str());

              assert( 0 < retval && retval < __DET_STRLEN ); // Ensure we're writing reasonable strings

              G4SDManager* SDman = G4SDManager::GetSDMpointer();

              G4VSensitiveDetector* thisdet =
                  SDman->FindSensitiveDetector(detectorname);

              if( thisdet == 0 ) {
                  thisdet = new HMolPolGenericDetector(detectorname, det_no);
                  G4cout << "  Creating sensitive detector " << det_type
                      << " for volume " << myvol->GetName()
                      <<  G4endl << G4endl;

                  SDman->AddNewDetector(thisdet);
              }

              myvol->SetSensitiveDetector(thisdet);
          }
      }
  }

  G4cout << "Completed sensitive detector assignment" << G4endl;

  //==========================
  // Add in the Magnetic field to world volume
  //==========================

  /// \bug this is for the *simple* magnetic field - need to be updated to
  /// something realistic and that might effect the following code
  //get the field??
  HMolPolSolenoidMagField* HTargetSolenoidMagField =
       new HMolPolSolenoidMagField;

  //tell Geant 4 to use this field - how??
  G4FieldManager* HTargetSolenoidMagFieldMgr =
       G4TransportationManager :: GetTransportationManager()->GetFieldManager();

  //set the field for the HTargetSolenoidFieldMgr
  HTargetSolenoidMagFieldMgr->SetDetectorField(HTargetSolenoidMagField);

  //create the Chord finder (this is responsible for moving
  // the particles through the field
  HTargetSolenoidMagFieldMgr->CreateChordFinder(HTargetSolenoidMagField);


  //==========================
  // Return world volume
  //==========================

  delete fGDMLParser;

  return worldVolume;
}
