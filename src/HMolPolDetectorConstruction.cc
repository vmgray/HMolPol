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
#include "HMolPolAnalysis.hh"

/********************************************
 * Programmer: Valerie Gray
 * Function: Construct
 *
 * Purpose:
 *
 * To construct the geometry of the simulation.
 *    The world
 *    The H target and the Al cell
 *    detectors
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
  G4NistManager* NistManager = G4NistManager::Instance();  ///< manages the NIST
  // variables
  NistManager->SetVerbose(1);

  // Open file with NIST materials
  std::ifstream nist_materials;
  nist_materials.open("nist_materials.txt");
  // Check whether materials file could be opened correctly
  if (nist_materials.good())
  {
    //add all the NIST materials we want to use
    std::string material;
    // Loop over all lines in the file (ie all the needed NIST materials)
    while (! nist_materials.eof())
    {
      nist_materials >> material;
      // Try to load the NIST material
      G4bool success = NistManager->FindOrBuildMaterial(material);
      // And complain if we can't find the material
      if (! success)
      {
        G4cout << "Material " << material << " not found!" << G4endl;
      }
    }
  } else
  {
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
  if (fGDMLParser)
  {
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

  // define and auxiliary map which has all the above info in it
  const G4GDMLAuxMapType* auxmap = fGDMLParser->GetAuxMap();

  //if it finds auxiliary info tell us
  G4cout << "Found " << auxmap->size()
                                     << " volume(s) with auxiliary information."
                                     << G4endl << G4endl;

  //this for loop iterates over all of the auxiliary info.  Starting at the
  // first one defined till there are no more.  Then it outputs what the volume
  // is and what type of auxiliary information it has and what the
  // corresponding value id
  for(G4GDMLAuxMapType::const_iterator
      iter  = auxmap->begin();
      iter != auxmap->end(); iter++)
  {
    G4cout << "Volume " << ((*iter).first)->GetName()
                    << " has the following list of auxiliary information: "<< G4endl;
    for (G4GDMLAuxListType::const_iterator
        vit  = (*iter).second.begin();
        vit != (*iter).second.end(); vit++)
    {
      G4cout << "--> Type: " << (*vit).type
          << ", value: "   << (*vit).value << std::endl;

      // Support for the auxiliary tag "Visibility" that can be
      // "true" to show the volume
      // "false" to hide the volume
      // "wireframe" to show the volume as wireframe only

      //if there is a visibility attribute then set it
      //The defult it to be seen with a wireframe
      if ((*vit).type == "Visibility")
      {
        G4Colour colour(1.0,1.0,1.0);
        // get old color (this is just the standard Geant4 color)
        const G4VisAttributes* visAttribute_old =
            ((*iter).first)->GetVisAttributes();
        //get the new color that is specified by the GDML code
        if (visAttribute_old)
          colour = visAttribute_old->GetColour();
        // create new visibility attributes
        G4VisAttributes visAttribute_new(colour);
        if ((*vit).value == "true")  // if we want the object to be seen
          visAttribute_new.SetVisibility(true);
        if ((*vit).value == "false")  // if we want to hide the object
          visAttribute_new.SetVisibility(false);
        if ((*vit).value == "wireframe") // if we want wireframe
          visAttribute_new.SetForceWireframe(true);
        // set new visibility attributes
        ((*iter).first)->SetVisAttributes(visAttribute_new);
      }

      // Support for the auxiliary tag "Color" that can be any of black,
      // blue, brown, cyan, gray, green, grey, magenta, red, white, yellow
      if ((*vit).type == "Color")
      {
        G4Colour colour(1.0,1.0,1.0);
        // get requested color, if it exists
        if (G4Colour::GetColour((*vit).value, colour))
        {
          G4cout << "Setting color to " << (*vit).value << "." << G4endl;
          // create new visibility attributes
          G4VisAttributes visAttribute(colour);
          // set new visibility attributes
          ((*iter).first)->SetVisAttributes(visAttribute);
        } else
        {
          //if color not in the above list
          G4cout << "Colour " << (*vit).value << " is not known." << G4endl;
        }
      }

      // Support for the auxiliary tag "Alpha" to set opacity,
      // a value of 0.0 is fully transparent, 1.0 is fully opaque
      if ((*vit).type == "Alpha")
      {
        // get the old color - Geant4 standard color
        G4Colour colour(1.0,1.0,1.0);
        // get old color
        const G4VisAttributes* visAttribute_old =
            ((*iter).first)->GetVisAttributes();

        if (visAttribute_old)
          colour = visAttribute_old->GetColour();
        // create new color with alpha channel (// \bug TODO input not checked?? Wouter?)
        // those color is the same as the color set by color (above)
        G4Colour colour_new(
            colour.GetRed(),
            colour.GetGreen(),
            colour.GetBlue(),
            std::atof((*vit).value.c_str())); //this is the alpha value
        // create new visibility attributes
        G4VisAttributes visAttribute_new(colour_new);
        // set new visibility attributes
        ((*iter).first)->SetVisAttributes(visAttribute_new);
      }

      // Support for the auxiliary tag "SensDet" to set sensitive detector type
      if ((*vit).type == "SensDet")
      {
        // Get specified sensitive detector type
        G4String detectortype = (*vit).value;

        // Form name of sensitive detector
        G4String detectorname = "hmolpol/" + detectortype;

        // Get pointer to sensitive detector manager
        G4SDManager* SDman = G4SDManager::GetSDMpointer();

        // Find pointer to current sensitive detector of that name
        G4VSensitiveDetector* sensitivedetector =
            SDman->FindSensitiveDetector(detectorname);

        // Check whether the sensitive detector was found
        if (sensitivedetector == 0)
        {
          // Sensitive detector doesn't exist yet, so create a new one
          /// \todo let HMolPolGenericDetector keep track of number using static
          /// (ask wdc for help if necessary)
          HMolPolGenericDetector* sensitivedetector =
              new HMolPolGenericDetector(detectorname);
          sensitivedetector->SetVolumeName(((*iter).first)->GetName());
          G4cout << "Creating sensitive detector " << detectortype
                 << " for volume " << ((*iter).first)->GetName()
                 <<  G4endl;

          // Add sensitive detector to analysis
          fAnalysis->AddNewDetector(detectortype);

          // Add sensitive detector to manager
          SDman->AddNewDetector(sensitivedetector);
        }

        // Set sensitive detector for the physical volume
        ((*iter).first)->SetSensitiveDetector(sensitivedetector);
      }
    }
  }
  G4cout << G4endl << G4endl;

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
