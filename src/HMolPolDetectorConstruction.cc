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
 * \date <b>Modified:</b> 07-15-2015
 *
 * \note <b>Entry Conditions:</b>
 * TODO magnetic fields
 ********************************************/

//Geant4 specific includes
#include <G4Color.hh>
#include <G4VisAttributes.hh>

#include <G4NistManager.hh>

//Magnetic Field related stuff
#include <G4FieldManager.hh>
#include <G4TransportationManager.hh>

#include <G4PhysicalVolumeStore.hh>

// Sensitive detector stuff
#include <G4SDManager.hh>

//system includes
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
 * Modified: 07-15-2015
 ********************************************/

G4VPhysicalVolume* HMolPolDetectorConstruction::Construct()
{
  G4cout << "##### In HMollerPolDetectorConstruction::Construct()... #####" << G4endl;

  //define a world volume
  G4VPhysicalVolume* worldVolume;

  /*****************
   * NIST Materials set up
  *****************/
  //for getting NIST materials
  G4NistManager* NistManager = G4NistManager::Instance();///< manages the NIST
  // variables
  NistManager->SetVerbose(1);

  // Open file with NIST materials
  std::ifstream nist_materials;
  nist_materials.open("nist_materials.txt");
  // Check whether materials file could be opened correctly
  if (nist_materials.good())
  {
/*
    //debugging
    G4cout << "  Nist Material File is open" << G4endl;
*/
    //add all the NIST materials we want to use
    std::string material;
    // Loop over all lines in the file (ie all the needed NIST materials)
    while (! nist_materials.eof())
    {
      nist_materials >> material;
      // Try to load the NIST material
      G4bool success = NistManager->FindOrBuildMaterial(material);
      // And complain if we can't find the material
/*
      //debugging
      if(success)
      {
        G4cout << "  Material " << material << " is added" << G4endl;
      }
*/
      if (! success)
      {
        G4cout << "  Material " << material << " not found!" << G4endl;
      }
    }
  } else
  {
    // If the file cannot be found, load only the vacuum
    NistManager->FindOrBuildMaterial("G4_Galactic");
    G4cout << "File nist_materials.txt not found. "
        << "Only loading NIST vacuum!" << G4endl;
  }
  // Close the file
  nist_materials.close();

  /*****************
   * Read in GDML and make geometry
   *****************/
  //Clear any info from the GDML parser object (initiate it to 0)
  //it was created in the header
  fGDMLParser->Clear();
  //set the flag to check for overlaps as reading in the file to true
  //when it reads geometry it will say if there is an overlap issue
  fGDMLParser->SetOverlapCheck(true);

  //read in the geometry - by reading the Mother Volume
  ///all of the Geometry will be read in since they are all
  //MotherVolume.gdml uses all they other ones, they will all be read
  /// TODO \bug *CHANGE* so that HMolPolMotherVolume is not hard-coded and
  /// other geometries can be able to read in easily
  fGDMLParser->Read("geometry/HMolPolMotherVolume.gdml");

  //function to take the GDML it read in (the experiment)
  //and make it the "World"
  worldVolume = fGDMLParser->GetWorldVolume();

  // Get pointer to sensitive detector manager
  //This can be useful and used throughout all scopes :)
  G4SDManager* SDman = G4SDManager::GetSDMpointer();

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
  const G4GDMLAuxMapType* auxmap = fGDMLParser->GetAuxMap();
  /********
   * G4GDMLAuxMapType is a map from Logical Volume to list of properties
   * we define auxmap to be an object of the type G4GDMLAuxMapType
   * when Geant4 reads in GDML this is filled automatically
  ********/

  //if we find a detector with auxiliary info, count them
  //tell us how many detectors have that have auxiliary info
  G4cout << G4endl << G4endl << "Found " << auxmap->size()
  << " volume(s) with auxiliary information."
  << G4endl << G4endl;

  //this for loop iterates over all of the auxiliary info.  Starting at the
  //first one defined till there are no more.
  //Then writes out what the volume
  //is and what type of auxiliary information it has and what the
  //corresponding value id
  for(G4GDMLAuxMapType::const_iterator iter = auxmap->begin();
      iter != auxmap->end();
      iter++)//This is over the volumes
  {
    //Start with this volume (first component of the map)
    //tells us how many entries there are for this volume
    G4cout << G4endl << "Volume " << ((*iter).first)->GetName()
    << " has " << (*iter).second.size()
    << " items of of auxiliary information."
    << G4endl;
    /*********
     * auxmap.second is the auxiliary information
     * which is a vector of pairs, <type, value>
     * so (*iter).second.size() gives us the number
     * of entries in this vector.
     *********/

    //get the standard Geant4 visibility attributes for the volume, we will change then to
    //to the ones we define in the GDML files when looping over the properties
    const G4VisAttributes* visAttribute_original =
    ((*iter).first)->GetVisAttributes();
    //define a color
    G4Color color_original;
    //if there are original visibility attributes get them
    if (visAttribute_original)
    color_original = visAttribute_original->GetColor();

    /*********
     * create new visibility attributes with the old color
     * or the original geant4 ones - this will be where the
     * the new ones we define will be stored, and then this
     * will be saved and attributed to the detector
     * We will change these when looping over the properties get all
     * the properties for the volume in this and then save it after looping
     * through the properties
    *********/
    G4VisAttributes visAttribute_new(color_original);

    /********
     * for that volume ((*iter).first)->GetName() loop over the properties
     * the properties are in the form (type, value) from the auxmap.second
     *
     * vit is the iterator over the properties of that volume or auxmap.second
     *
     * (*vit) is the object corresponding to (type,value)
     * ie (color, green), (alpha, 0.5) etc)
    *********/
    //loop over all properties
    for (G4GDMLAuxListType::const_iterator vit = (*iter).second.begin();
        vit != (*iter).second.end();
        vit++)//This is over the attributes for said Volume
    {
      //Print what the property is and is value
      G4cout << "  --> Type: " << (*vit).type
      << ", value: " << (*vit).value << std::endl;

      /********
       * Support for the auxiliary tag "Visibility" that can be
       * "true" to show the volume
       * "false" to hide the volume
       * "wireframe" to show the volume as wireframe only
       *
       * if there is a visibility attribute then set it
       * The default is to be seen with a wireframe, other options are
       * full, wireframe and completely off.  Color is not a part of the
       * visibility tag, but it has to be set as part of the VisAttribute
       * in geant4
      ********/
      if ((*vit).type == "Visibility")
      {
        if ((*vit).value == "true")  // if we want the object to be seen
          visAttribute_new.SetVisibility(true);
        if ((*vit).value == "false")// if we want to hide the object
          visAttribute_new.SetVisibility(false);
        if ((*vit).value == "wireframe")// if we want wireframe
          visAttribute_new.SetForceWireframe(true);
      }

      /********
       * Support for the auxiliary tag "Color" that can be any of black,
       * blue, brown, cyan, gray, green, grey, magenta, red, white, yellow
      ********/
      if ((*vit).type == "Color")
      {
        // definition & get the "old" color
        G4Color color = visAttribute_new.GetColour();

        //define another color (color_lookup) that will be a place holder to store the
        //requested color if properly defined.
        G4Color color_lookup(1.0,1.0,1.0);
        //Do or don't we have a Geant4 color in proper def
        G4bool found_color_in_lookup = false;
        // if color is there and digined correctly then  color_lookup that color
        if (G4Color::GetColour((*vit).value, color_lookup))// this fills color_lookup
        {
          //write out the color we are setting the volume too
          G4cout << "    Setting color to " << (*vit).value << "." << G4endl;
          found_color_in_lookup = true;
        } else  //color not right so we complain
        {
          //if color not in the above list
          G4cout << "  Color " << (*vit).value << " is not known." << G4endl;
        }

        //we do have a color that is correct and then we set this as
        //our new color and add that to the visibility attributes.
        //or do nothing as it hasn't found the color
        if (found_color_in_lookup == true)
        {
          G4Color color_new(
            color_lookup.GetRed(),
            color_lookup.GetGreen(),
            color_lookup.GetBlue(),
            color.GetAlpha());  //this is the alpha value from the "old" color
          //this is unchanged

          // change the color in the visibility attributes
          visAttribute_new.SetColor(color_new);
        }
      }

      /********
       * Support for the auxiliary tag "Alpha" to set opacity,
       * a value of 0.0 is fully transparent, 1.0 is fully opaque
      ********/
      if ((*vit).type == "Alpha")
      {
        // definition & get the "old" color - this will contain any changes from above
        G4Color color = visAttribute_new.GetColour();

        //define a double to store the alpha value
        G4double alpha = std::atof((*vit).value.c_str());
        //Bool  and if to check if the alpha value is valid ie between 0 and 1
        G4bool alpha_is_valid = false;
        if (alpha >= 0.0 && alpha <= 1.0)
        {
          //write out the color we are setting the volume too
          G4cout << "    Setting alpha to " << (*vit).value << "." << G4endl;
          alpha_is_valid = true;
        } else  //alpha not right so we complain
        {
          //if alpha not in the above list
          G4cout << "  Alpha " << (*vit).value << " is not known." << G4endl;
        }

        //we do have a alpha that is valid and then we set this as
        //our new color and add that to the visibility attributes.
        //or do nothing as it hasn't found the alpha
        if (alpha_is_valid == true)
        {
          G4Color color_new(
              color.GetRed(),
              color.GetGreen(),
              color.GetBlue(),
              alpha);  //this is the alpha value

          // create new visibility attributes with the Alpha included
          visAttribute_new.SetColor(color_new);
        }
      }

      //Sets the volumes visiablity attribute with both the COLOR and the APLHA
      //As was set (or not) in the last 2 if statements.
      ((*iter).first)->SetVisAttributes(visAttribute_new);

      // Support for the auxiliary tag "SensDet" to set sensitive detector type
      if ((*vit).type == "SensDet")
      {
        // Get specified sensitive detector type: essentially the name
        G4String detectortype = (*vit).value;

        // Form name of sensitive detector (convention is to prefix this with hmolpol/ )
        G4String detectorname = "hmolpol/" + detectortype;

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
          HMolPolGenericDetector* genericdetector =
          new HMolPolGenericDetector(detectorname);

          //give that newly created HMolPolGenericDetector, genericdetector
          //the name if the volume
          genericdetector->SetVolumeName(((*iter).first)->GetName());

          //write out the name if the sensitive detector and the volume
          G4cout << "    Creating sensitive detector " << detectortype
          << " for volume " << ((*iter).first)->GetName()
          << G4endl;

          // Add sensitive detector to analysis
          //therefore adding it to the ROOT file
          fAnalysis->AddNewDetector(detectortype);

          // Add sensitive detector to SDmanager
          // so we can finds and use it
          SDman->AddNewDetector(genericdetector);

          // Lastly, set sensitive detector to this and proceed
          sensitivedetector = genericdetector;
        }

        //debugging
        G4cout << "    Volume Name: " << ((*iter).first)->GetName() << G4endl;

        // Set sensitive detector for the logical volume
        ((*iter).first)->SetSensitiveDetector(sensitivedetector);
      }
    }
  }
  G4cout << G4endl << G4endl;

  /*
   * Now we want to loop over the physical volumes and use there names in the
   * Generaic Detector (and ROOT Tree). Physical are all 100% seperate intities
   * were the logical volumes are not as they can and are reused.
   */
  G4cout << "  Parsing through physical volumes" << G4endl;

  //Get a list of all physical volumes read.
  const G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();
  //defining a new iterator for the physical volumes
  std::vector<G4VPhysicalVolume*>::const_iterator pvciter;
  for( pvciter = pvs->begin(); pvciter != pvs->end(); pvciter++ )
  {
    G4cout << "Physical Volume: " << (*pvciter)->GetName() << " \tHash ID: "
      << (*pvciter)->GetName().hash(G4String::exact) << G4endl;

    //Gets the auxerly info for this physical volume
    G4GDMLAuxListType auxInfo = fGDMLParser->GetVolumeAuxiliaryInformation(
      (*pvciter)->GetLogicalVolume());

    //Loop over auxiliary attributes for physical volumes. THis is all the things
    //that we have in a logical volume GDML file like:
    //<auxiliary auxtype="SensDet" auxvalue="DetectorDet" />
    std::vector<G4GDMLAuxPairType>::const_iterator ipair = auxInfo.begin();
    for( ipair = auxInfo.begin(); ipair != auxInfo.end(); ipair++ )
    {
      /*
       * Only intersted in attributes of sensitive detectors.
       * ipair is the GDML attiributes,
       *
       * This is showen in the GDML by:
       * <auxiliary auxtype="SensDet" auxvalue="DetectorDet" />
       *
       * ipar.type = "SensDet" means we go in the if
       */
      if( ipair->type == "SensDet" )
      {
        // Get specified sensitive detector type: essentially the name
        //Using this GDML example:
        //<auxiliary auxtype="SensDet" auxvalue="DetectorDet" />
        // ipar->valu is DetectorDet
         G4String detectortype = ipair->value;

         // Form name of sensitive detector (convention is to prefix this with hmolpol/ )
         G4String detectorname = "hmolpol/" + detectortype;

         // This is a copy of the sensitive detector we defined and
         //gave attributes before
         //casting this, because it spits out the parent class,
         //G4SensitiveDetector and we need a HMolPolGenericDetector
         HMolPolGenericDetector* sensitivedetector =
             (HMolPolGenericDetector*) SDman->FindSensitiveDetector(detectorname);

         //Make sure this is it is defined
         if(sensitivedetector)
         {
           //register this phyiscal volume with sensitivedetector (the
           //HMolPolGenericDetector defined above
           //This is different then the standard Geant4 way, because we want
           //our detector to know the name of ALL physical volumes of this type.
           //
           // our sensitivedetector is associated to a logical volume. We want
           // this sensitivedetector to know all daughter physical volumes
           // A logical volume could have many daughter physical volumes, and
           //when we write to the ROOT file we want them to be (easily) distigusable
           //ie. Logial volume DetectorDet has 2 phyical volume daughter,
           //Right and Left. So we are telling sensitivedetector that logical volume
           //DetectorDet has 2 daughters and their names
           sensitivedetector->RegisterPhysicalVolume((*pvciter)->GetName());
         }else
         {
           G4cerr << "Sensitive Detector not found: " << detectorname << G4endl;
         }
      }
    }
  }


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
