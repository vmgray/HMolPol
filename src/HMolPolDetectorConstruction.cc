/********************************************
 * \author <b>Programmer:</b> Valerie Gray
 * \author <b>Assisted By:</b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is the file is in charge
 *      reading in the geometry of the simulations and
 *      therefore the polarimeter.  This is also sets any
 *      attributes of the objects in the simulations, and
 *      any magnetic fields.
 *
 * \date <b>Date:</b> 06-25-2013
 * \date <b>Modified:</b> 07-06-2013
 *
 * \note <b>Entry Conditions:</b>
 * TODO magnetic fields
 ********************************************/

//Geant4 specific includes
#include <G4Color.hh>
#include <G4VisAttributes.hh>
#include <G4NistManager.hh>
#include <G4FieldManager.hh>
#include <G4TransportationManager.hh>
#include <G4SDManager.hh>

#include <fstream>

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

  //define a world volume
  G4VPhysicalVolume *worldVolume;

  /*****************
   * NIST Materials set up
  *****************/
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

  /*****************
   * Read in GDML and make geometry
  *****************/
  //Clear any info from the GDML parser object (initiate it to 0)
  //it was created in the header
  fGDMLParser.Clear();
  //set the flag to check for overlaps as reading in the file to true
  //when it reads geometry it will say if there is an overlap issue
  fGDMLParser.SetOverlapCheck(true);

  //read in the geometry - by reading the Mother Volume
  ///all of the Geometry will be read in since they are all
  //MotherVolume.gdml uses all they other ones, they will all be read
  /// TODO \bug *CHANGE* so that HMolPolMotherVolume is not hard-coded and
  /// other geometries can be able to read in easily
  fGDMLParser.Read("geometry/HMolPolMotherVolume.gdml");

  //function to take the GDML it read in (the experiment)
  //and make it the "World"
  worldVolume = fGDMLParser.GetWorldVolume();

  /*****************
   * Read in the auxiliary information.
   *
   * List auxiliary info includes:
   *    color
   *    alpha (or opaqueness)
   *    detector type - if it is a sensitive detector
  *****************/

  // define and get auxiliary map  from the Parser
  // which has all the above info in it - it is already there
  //from reading the files, just grabbing it from the parser
  const G4GDMLAuxMapType* auxmap = fGDMLParser.GetAuxMap();
  //Aux map is a map from Logical Volume to list of properties

  //if it finds auxiliary info on a detector
  //tell us how many have that
  G4cout << "Found " << auxmap->size()
         << " volume(s) with auxiliary information."
         << G4endl << G4endl;

  //this for loop iterates over all of the auxiliary info.  Starting at the
  // first one defined till there are no more.  Then it outputs what the volume
  // is and what type of auxiliary information it has and what the
  // corresponding value id
  for(G4GDMLAuxMapType::const_iterator
      iter  = auxmap->begin();
      iter != auxmap->end(); iter++) //This is over the volumes
  {
    //Start with the the the volume (first component of the map)
    G4cout << "Volume " << ((*iter).first)->GetName()
                    << " has the following list of auxiliary information: "<< G4endl;

    //for that volume ((*iter).first)->GetName() loop over the properties
    //the properties are in the form (type, value)
    //(*vit) is the object corresponding to (type,value)
    for (G4GDMLAuxListType::const_iterator
        vit  = (*iter).second.begin();
        vit != (*iter).second.end(); vit++) //This is over the attributes for said Volume
    {
      G4cout << "--> Type: " << (*vit).type
          << ", value: "   << (*vit).value << std::endl;

      // /todo TODO: We should get the visibility attributes before the first

      // get visibility attributes (this is just the standard Geant4 ones)
      //(this is not the ones from the in the GDML file - those we are changing)
      const G4VisAttributes* visAttribute_original =
          ((*iter).first)->GetVisAttributes();
      //define a color
      G4Color color;
      //if there are original visiablity attributes get them
      if (visAttribute_original)
        color = visAttribute_original->GetColor();

      //create new visibility attributes with the old color
      //or the orignal geant4 ones - this will be where the
      //the new ones we define will be stored, and then this
      //will be saved and attributed to the detector
      G4VisAttributes visAttribute_new(color);

      // if statement, modify them in the if statements, and write them
      // to the volume after the last if statement.

      // Support for the auxiliary tag "Visibility" that can be
      // "true" to show the volume
      // "false" to hide the volume
      // "wireframe" to show the volume as wireframe only

      //if there is a visibility attribute then set it
      // The default is to be seen with a wireframe, other options are
      // full, wireframe and completely off.  Color is not a part of the
      // visibility tag, but it has to be set as part of the VisAttribute
      // in geant4
      if ((*vit).type == "Visibility")
      {
        // definition & initialization
        G4Color color(1.0,1.0,1.0);

/*
 * TODO REMOVE when global viablity works
//        // get old color (this is just the standard Geant4 color)
//        //(this is not the color in the GDML file)
//        const G4VisAttributes* visAttribute_original =
//            ((*iter).first)->GetVisAttributes();
        if (visAttribute_original)
          color = visAttribute_original->GetColor();

        // create new visibility attributes with the old color
        G4VisAttributes visAttribute_new(color);
 */       if ((*vit).value == "true")  // if we want the object to be seen
          visAttribute_new.SetVisibility(true);
        if ((*vit).value == "false")  // if we want to hide the object
          visAttribute_new.SetVisibility(false);
        if ((*vit).value == "wireframe") // if we want wireframe
          visAttribute_new.SetForceWireframe(true);
/*
 * TODO REMOVE when global visablity works
        // set new visibility attributes
        ((*iter).first)->SetVisAttributes(visAttribute_new);
*/
      }

      // Support for the auxiliary tag "Color" that can be any of black,
      // blue, brown, cyan, gray, green, grey, magenta, red, white, yellow
      if ((*vit).type == "Color")
      {
        // definition & initialization
        G4Color color(1.0,1.0,1.0);

/*
 * TODO REMOVE when global viablity works
        // get old visibility attributes so that all previously set properties
        // are still going to be valid
        G4VisAttributes* visAttribute_original =
            ((*iter).first)->GetVisAttributes();
*/

        // get requested color, if it exists
        if (G4Color::GetColour((*vit).value, color))
        {
          //write out the color we are setting the volume too
          G4cout << "Setting color to " << (*vit).value << "." << G4endl;
          // change the color in the visibility attributes
          visAttribute_new.SetColor(color);
/*
 * TODO remove when global visablitly works
          // saves this color to the VisAttributes of the volume
          ((*iter).first)->SetVisAttributes(visAttribute);
*/
        } else
        {
          //if color not in the above list
          G4cout << "Color " << (*vit).value << " is not known." << G4endl;
        }
      }

      // Support for the auxiliary tag "Alpha" to set opacity,
      // a value of 0.0 is fully transparent, 1.0 is fully opaque
      if ((*vit).type == "Alpha")
      {
        // definition & initialization
        G4Colour colour(1.0,1.0,1.0);

/*
 * REMOVE when global viablity works
        // get old visibility attributes so that all previously set properties
        // are still going to be valid
        G4VisAttributes* visAttribute =
            ((*iter).first)->GetVisAttributes();
*/

        if (visAttribute_original)
          colour = visAttribute_original->GetColour();
        // create new color with alpha channel (// \bug TODO input not checked?? Wouter?)
        // those color is the same as the color set by color (above)
        G4Colour colour_new(
            colour.GetRed(),
            colour.GetGreen(),
            colour.GetBlue(),
            std::atof((*vit).value.c_str())); //this is the alpha value

        // create new visibility attributes with the Alphaincluded
        G4VisAttributes visAttribute_new(colour_new);

/*
 * Remove when global visablity works
        // saves the alpha value to the visibility attributes for this volume
        ((*iter).first)->SetVisAttributes(visAttribute_new);
*/
      }

      //TODO Does this work??
      // saves the alpha value to the visibility attributes for this volume
              ((*iter).first)->SetVisAttributes(visAttribute_new);


      // Support for the auxiliary tag "SensDet" to set sensitive detector type
      if ((*vit).type == "SensDet")
      {
        // Get specified sensitive detector type: essentially the name
        G4String detectortype = (*vit).value;

        // Form name of sensitive detector (convention is to prefix this with hmolpol/ )
        G4String detectorname = "hmolpol/" + detectortype;

        // Get pointer to sensitive detector manager
        G4SDManager* SDman = G4SDManager::GetSDMpointer();

        // Find pointer to current sensitive detector of that name
        // in the SDmanager
        G4VSensitiveDetector* sensitivedetector =
            SDman->FindSensitiveDetector(detectorname);

        // Check whether the sensitive detector was found
        if (sensitivedetector == 0)
        {
          // Sensitive detector doesn't exist yet, so create a new one

          /// \todo let HMolPolGenericDetector keep track of number using static
          /// (ask wdc for help if necessary)

          //crates a HMolPolGenericDetector with the detector name
          //(has the hmolpol/ in front)
          HMolPolGenericDetector* sensitivedetector =
              new HMolPolGenericDetector(detectorname);

          //give that newly created HMolPolGenericDetector, sensitivedetector
          //the name if the volume
          sensitivedetector->SetVolumeName(((*iter).first)->GetName());

          //write out the name if the sensitive detector and the volume
          G4cout << "Creating sensitive detector " << detectortype
                 << " for volume " << ((*iter).first)->GetName()
                 <<  G4endl;

          // Add sensitive detector to analysis
          //therefore adding it to the ROOT file
          fAnalysis->AddNewDetector(detectortype);

          // Add sensitive detector to SDmanager
          // so we can finds and use it
          SDman->AddNewDetector(sensitivedetector);
        }

        // Set sensitive detector for the physical volume
        ((*iter).first)->SetSensitiveDetector(sensitivedetector);
      }
    }
  }
  G4cout << G4endl << G4endl;

  /*****************
   * Add in the Magnetic field to world volume
  *****************/

  /// TODO \bug this is for the *simple* magnetic field - need to be updated to
  /// something realistic and that might effect the following code
  //get the field??

  //create a new magnetic field HMolPolSolenoidMagField
  //for the  HTargetSolenoid, and store the pointer to it
  HMolPolSolenoidMagField* HTargetSolenoidMagField =
      new HMolPolSolenoidMagField;

  //Make a MagFeildMaanager for this field
  G4FieldManager* HTargetSolenoidMagFieldMgr =
      G4TransportationManager :: GetTransportationManager()->GetFieldManager();

  //add the field (HTargetSolenoidMagField) to the HTargetSolenoidFieldMgr
  //Allows the feild to get used
  HTargetSolenoidMagFieldMgr->SetDetectorField(HTargetSolenoidMagField);

  //create the Chord finder (this is responsible for moving
  // the particles through the field)
  HTargetSolenoidMagFieldMgr->CreateChordFinder(HTargetSolenoidMagField);


  //Return world volume
  return worldVolume;
}
