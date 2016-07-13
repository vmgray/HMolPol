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
#include <G4PhysicalVolumeStore.hh>
#include <G4UIcmdWith3Vector.hh>

//Magnetic field related stuff
#include <G4UniformMagField.hh>
#include <G4QuadrupoleMagField.hh>
#include <G4TransportationManager.hh>
#include <G4Mag_UsualEqRhs.hh>
#include <G4FieldManager.hh>
#include <G4UserLimits.hh>
#include <G4AutoDelete.hh>
// Local magnetic field line tracer
#include "G4BlineTracer.hh"

// Sensitive detector stuff
#include <G4SDManager.hh>

//system includes
#include <fstream>
#include <sstream>
#include <algorithm>

//HMolPol includes
#include "HMolPolDetectorConstruction.hh"
#include "HMolPolGenericDetector.hh"
#include "HMolPolAnalysis.hh"
#include "HMolPolMagFieldMap.hh"

// Helper class to use for find_if statement
class containsTagType {
  private:
    // Name of the tag type that should be matched
    G4String fType;
  public:
    // Constructor with name of the tag type
    containsTagType(G4String type)
    : fType(type) { }
    // Operator() that takes a tag as argument and returns true if the tag has
    // the type stored in this object
    bool operator()(const G4GDMLAuxStructType& tag)
    { return tag.type.contains(fType); }
};

// Create static elements
G4ThreadLocal std::vector<G4MagneticField*>    HMolPolDetectorConstruction::fFields;
G4ThreadLocal std::vector<G4FieldManager*>     HMolPolDetectorConstruction::fFieldManagers;
G4ThreadLocal std::vector<G4GenericMessenger*> HMolPolDetectorConstruction::fFieldMessengers;

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
  fGDMLParser->Read(fGeometryFileName);

  //function to take the GDML it read in (the experiment)
  //and make it the "World"
  fWorldVolume = fGDMLParser->GetWorldVolume();

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
    // Get the logical volume which has auxiliary tags
    G4LogicalVolume* logicalVolume = (*iter).first;
    const G4GDMLAuxListType& auxiliaryList = (*iter).second;

    // Start with this volume (first component of the map)
    // tells us how many entries there are for this volume
    G4cout << G4endl << "Volume " << logicalVolume->GetName()
        << " has " << auxiliaryList.size()
        << " items of of auxiliary information."
        << G4endl;
    /*********
     * auxmap.second is the auxiliary information
     * which is a vector of pairs, <type, value>
     * so auxiliaryList.size() gives us the number
     * of entries in this vector.
     *********/

    //get the standard Geant4 visibility attributes for the volume, we will change then to
    //to the ones we define in the GDML files when looping over the properties
    const G4VisAttributes* visAttribute_original =
        logicalVolume->GetVisAttributes();
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
     * for that volume logicalVolume->GetName() loop over the properties
     * the properties are in the form (type, value) from the auxmap.second
     *
     * vit is the iterator over the properties of that volume or auxmap.second
     *
     * (*vit) is the object corresponding to (type,value)
     * ie (color, green), (alpha, 0.5) etc)
    *********/
    //loop over all properties
    for (G4GDMLAuxListType::const_iterator vit = auxiliaryList.begin();
        vit != auxiliaryList.end();
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

      //Sets the volumes visibility attribute with both the COLOR and the APLHA
      //As was set (or not) in the last 2 if statements.
      logicalVolume->SetVisAttributes(visAttribute_new);

    } // end of loop over the auxiliary tags themselves

  } // end of loop over volumes with auxiliary tags
  G4cout << G4endl << G4endl;

  // Return world volume
  return fWorldVolume;
}


/********************************************
 * Programmer: Wouter Deconinck
 * Function: ConstructSDandField
 *
 * Purpose:
 *
 * Take any auxiliary information from the GDML files to construct
 * sensitive detectors
 *
 * Read in and apply the magnetic field
 *
 * Global:
 * Entry Conditions: none
 * Return:
 * Called By:
 * Date: 06-22-2016
 * Modified:
 ********************************************/

void HMolPolDetectorConstruction::ConstructSDandField()
{
  /*
   * Now we want to loop over the physical volumes and use there names in the
   * Generic Detector (and ROOT Tree). Physical are all 100% separate entities
   * were the logical volumes are not as they can and are reused.
   */
  G4cout << "  Parsing through physical volumes" << G4endl;

  // Get pointer to sensitive detector manager
  // This can be useful and used throughout all scopes :)
  G4SDManager* SDman = G4SDManager::GetSDMpointer();

  // Get a list of all physical volumes read.
  const G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();
  // Defining a new iterator for the physical volumes
  std::vector<G4VPhysicalVolume*>::const_iterator pvciter;
  for (pvciter = pvs->begin(); pvciter != pvs->end(); pvciter++)
  {
    G4cout << "Physical Volume: " << (*pvciter)->GetName() << " \tHash ID: "
        << (*pvciter)->GetName().hash(G4String::exact) << G4endl;

    // Gets the auxiliary info for this physical volume
    G4GDMLAuxListType auxInfo =
        fGDMLParser->GetVolumeAuxiliaryInformation((*pvciter)->GetLogicalVolume());

    // Gets the logical volume for this physical volume
    G4LogicalVolume* logicalVolume = (*pvciter)->GetLogicalVolume();

    // Loop over auxiliary attributes for physical volumes. This is all the things
    // that we have in a logical volume GDML file like:
    //   <auxiliary auxtype="SensDet" auxvalue="DetectorDet" />
    G4GDMLAuxListType::const_iterator ipair = auxInfo.begin();
    for (ipair = auxInfo.begin(); ipair != auxInfo.end(); ipair++)
    {
      /*
       * Only interested in attributes of sensitive detectors.
       * ipair is the GDML attributes,
       *
       * This is showen in the GDML by:
       * <auxiliary auxtype="SensDet" auxvalue="DetectorDet" />
       *
       * ipair.type = "SensDet" means we go in the if
       */
      if (ipair->type == "SensDet")
      {
        // Get specified sensitive detector type: essentially the name
        // Using this GDML example:
        //  <auxiliary auxtype="SensDet" auxvalue="DetectorDet" />
        // ipair->value is DetectorDet
        G4String detectortype = ipair->value;

        // Form name of sensitive detector (convention is to prefix this with hmolpol/ )
        G4String detectorname = "hmolpol/" + detectortype;

        // Find pointer to current sensitive detector of that name
        // in the SDmanager
        // Casting this, because it spits out the parent class,
        // G4SensitiveDetector and we need a HMolPolGenericDetector
        G4VSensitiveDetector* sensitivedetector =
            SDman->FindSensitiveDetector(detectorname);

        // Check whether the sensitive detector was found
        if (sensitivedetector == 0)
        {
          // Sensitive detector doesn't exist yet, so create a new one

          /// \todo let HMolPolGenericDetector keep track of number using static
          /// (ask wdc for help if necessary)

          // Creates a HMolPolGenericDetector with the detector name
          // (has the hmolpol/ in front)
          HMolPolGenericDetector* genericdetector =
              new HMolPolGenericDetector(detectorname);

          // Give that newly created HMolPolGenericDetector, genericdetector
          // the name of the volume
          genericdetector->SetVolumeName(logicalVolume->GetName());

          // Write out the name if the sensitive detector and the volume
          G4cout << "    Creating sensitive detector " << detectortype
              << " for volume " << logicalVolume->GetName() << G4endl;

          // Add sensitive detector to analysis
          //therefore adding it to the ROOT file
          fAnalysis->AddNewDetector(detectortype);

          // Add sensitive detector to SDmanager
          // so we can finds and use it
          SDman->AddNewDetector(genericdetector);

          // Register this physical volume with sensitivedetector (the
          // HMolPolGenericDetector defined above
          // This is different then the standard Geant4 way, because we want
          // our detector to know the name of ALL physical volumes of this type.
          //
          // Our sensitivedetector is associated to a logical volume. We want
          // this sensitivedetector to know all daughter physical volumes
          // A logical volume could have many daughter physical volumes, and
          // when we write to the ROOT file we want them to be (easily) distinguishable
          // i.e. Logical volume DetectorDet has 2 physical volume daughter,
          // Right and Left. So we are telling sensitivedetector that logical volume
          // DetectorDet has 2 daughters and their names
          genericdetector->RegisterPhysicalVolume((*pvciter)->GetName());

          // Lastly, set sensitive detector to this and proceed
          sensitivedetector = genericdetector;
        }

        // Debugging
        G4cout << "    Volume Name: " << logicalVolume->GetName() << G4endl;

        // Set sensitive detector for the logical volume
        logicalVolume->SetSensitiveDetector(sensitivedetector);
      }
    }


    // Create magnetic field line tracer
    fBlineTracer = new G4BlineTracer();


    // Create a map with all types of magnetic field tags for this volume
    std::map<G4String,G4String> magFieldEntryMap;
    // Loop over all auxiliary tag names
    for (G4GDMLAuxListType::const_iterator
        aux = auxInfo.begin();
        aux != auxInfo.end(); aux++) {
      // If the type contains "MagField" then add to map
      if (containsTagType("MagField")(*aux))
        magFieldEntryMap[aux->type] = aux->value;
    }


    // Find if there is a magnetic field type tag for this volume
    if (magFieldEntryMap.count("MagFieldType")) {

      // Create a magnetic field pointer for this volume (this is null until
      // there is an actual magnetic field created based on the gdml file content)
      G4MagneticField* localField = 0;

      // Figure out which type of field this is
      if (magFieldEntryMap["MagFieldType"] == "Uniform") {
        G4cout << "    Creating uniform magnetic field" << G4endl;

        // If no magnetic field value tag is found, just leave at zero field
        G4ThreeVector vector(0.0, 0.0, 0.0);
        try {
          // Read the vector from the input string
          if (magFieldEntryMap.count("MagFieldVector")) {
            std::stringstream(magFieldEntryMap["MagFieldVector"]) >> vector;
          }
          else
            G4cout << "    Warning: Specify MagFieldVector as vector (Bx,By,Bz) in units of Tesla" << G4endl;
          // Assume this is in units of tesla
          vector *= CLHEP::tesla;
          G4cout << "    Field vector " << vector / CLHEP::tesla << " T" << G4endl;
        } catch (const std::exception& ex) {
          G4cout << "    Could not parse " << magFieldEntryMap["MagVieldVector"] << G4endl;
          continue;
        }

        // Create uniform magnetic field with the given field vector
        localField = new G4UniformMagField(vector);

      } else if (magFieldEntryMap["MagFieldType"] == "Quadrupole") {
        G4cout << "    Creating quadrupole magnetic field" << G4endl;

        // If no magnetic field value tag is found, just leave at zero field
        G4double gradient = 0.0;
        try {
          // Read the gradient value from the input string
          if (magFieldEntryMap.count("MagFieldGradient"))
            std::stringstream(magFieldEntryMap["MagFieldGradient"]) >> gradient;
          else
            G4cout << "    Warning: Specify MagFieldGradient in units of Tesla/m" << G4endl;
          // Assume this is in units of tesla/m
          gradient *= CLHEP::tesla / CLHEP::m;
          G4cout << "    Field gradient " << gradient / (CLHEP::tesla / CLHEP::m) << " T/m" << G4endl;
        } catch (const std::exception& ex) {
          G4cout << "    Could not parse " << magFieldEntryMap["MagFieldGradient"] << G4endl;
        }

        // If no magnetic field value tag is found, just leave at zero field
        G4ThreeVector origin = (*pvciter)->GetObjectTranslation();
        try {
          // Read the origin value from the input string
          G4ThreeVector relative_origin;
          if (magFieldEntryMap.count("MagFieldOrigin"))
            std::stringstream(magFieldEntryMap["MagFieldOrigin"]) >> relative_origin;
          else
            G4cout << "    Warning: Specify MagFieldOrigin as vector (x,y,z) "
              << "in units of m relative to physical volume position" << G4endl;
          // Assume this is in units of tesla/m
          relative_origin *= CLHEP::m;
          G4cout << "    Relative origin " << relative_origin / CLHEP::m << " m" << G4endl;
          // Add relative origin to origin of volume
          origin += relative_origin;
        } catch (const std::exception& ex) {
          G4cout << "    Could not parse " << magFieldEntryMap["MagFieldOrigin"] << G4endl;
        }

        // If no magnetic field value tag is found, just leave at zero field
        G4RotationMatrix rotation((*pvciter)->GetObjectRotationValue());
        try {
          // Read the Euler angles from the input string
          G4ThreeVector euler_angles;
          if (magFieldEntryMap.count("MagFieldEulerAngles"))
            std::stringstream(magFieldEntryMap["MagFieldEulerAngles"]) >> euler_angles;
          else
            G4cout << "    Warning: Specify MagFieldEulerAngles as vector (phi,theta,psi) "
            << "in units of degrees relative to the physical volume orientation" << G4endl;
          euler_angles *= CLHEP::degree;
          G4cout << "    Euler angles " << euler_angles / CLHEP::degree << " deg" << G4endl;
          G4RotationMatrix euler_rotation(euler_angles.x(), euler_angles.y(), euler_angles.z());
          // Multiply Euler angle rotation with object rotation
          rotation *= euler_rotation;
        } catch (const std::exception& ex) {
          G4cout << "    Could not parse " << magFieldEntryMap["MagFieldRotation"] << G4endl;
        }

        // Create uniform magnetic field with the given field vector
        localField = new G4QuadrupoleMagField(gradient,origin,new G4RotationMatrix(rotation));

      } else if (magFieldEntryMap["MagFieldType"] == "Map") {
        G4cout << "    Creating mapped magnetic field" << G4endl;

        // Read the filename from the input string
        std::string filename;
        if (magFieldEntryMap.count("MagFieldMapFile"))
          filename = magFieldEntryMap["MagFieldMapFile"];
        else
          G4cout << "    Warning: Specify tag MagFieldMapFile with path to file" << G4endl;
        G4cout << "    Map file " << filename << G4endl;

        // Create uniform magnetic field with the given field vector
        localField = new HMolPolMagFieldMap(filename);

      } else {
        G4cout << "    Magnetic field type not recognized" << G4endl;
        continue;
      }

      // Add a field manager to this logical volume
      // Ref: https://geant4.web.cern.ch/geant4/UserDocumentation/UsersGuides/ForApplicationDeveloper/html/ch04s03.html
      G4FieldManager* localFieldManager = new G4FieldManager();
      localFieldManager->SetDetectorField(localField);
      localFieldManager->CreateChordFinder(localField);
      fFields.push_back(localField);
      fFieldManagers.push_back(localFieldManager);

      // Create messenger for this field
      G4String name = "/HMolPol/Fields/" + logicalVolume->GetName() + "/";
      G4String desc = "Field control for " + logicalVolume->GetName();
      G4GenericMessenger* localFieldMessenger =
          new G4GenericMessenger(this, name, desc);
      fFieldMessengers.push_back(localFieldMessenger);

      // Create command for this field
      name = "/HMolPol/Fields/" + (*pvciter)->GetName() + "/set";
      desc = "Set field strength for " + (*pvciter)->GetName();
      G4UIcmdWith3Vector* localFieldCommand =
          new G4UIcmdWith3Vector(name, localFieldMessenger);
      localFieldCommand->SetGuidance(desc);
      localFieldCommand->SetDefaultValue(G4ThreeVector(0.0,0.0,0.0));

      // Register the field and its manager for deleting when done
      G4AutoDelete::Register(localField);
      G4AutoDelete::Register(localFieldManager);
      G4AutoDelete::Register(localFieldMessenger);
      G4AutoDelete::Register(localFieldCommand);

      // Connect field manager to this logical volume and its daughters
      G4bool forceToAllDaughters = true;
      logicalVolume->SetFieldManager(localFieldManager,forceToAllDaughters);

      // Set step limit in volume with magnetic field
      G4double volume = logicalVolume->GetSolid()->GetCubicVolume();
      G4double area = logicalVolume->GetSolid()->GetSurfaceArea();
      G4double max_step_size = 0.1 * volume / area;
      G4cout << "    Limiting step size to " << max_step_size / CLHEP::mm << " mm" << G4endl;
      G4UserLimits* userLimits = new G4UserLimits();
      userLimits->SetMaxAllowedStep(max_step_size);
      logicalVolume->SetUserLimits(userLimits);
    }
  }
}


void HMolPolDetectorConstruction::DumpGeometry(
    G4bool overlap_check,
    G4VPhysicalVolume* volume,
    G4int depth)
{
  // Null volume
  if (volume == 0) volume = fWorldVolume;

  // Print spaces
  for (int isp = 0; isp < depth; isp++) { G4cout << "  "; }
  // Print name
  G4cout << volume->GetName() << "[" << volume->GetCopyNo() << "] "
         << volume->GetLogicalVolume()->GetName() << " "
         << volume->GetLogicalVolume()->GetNoDaughters() << " "
         << volume->GetLogicalVolume()->GetMaterial()->GetName();
  // Print sensitive detector
  if (volume->GetLogicalVolume()->GetSensitiveDetector())
  {
    G4cout << " " << volume->GetLogicalVolume()->GetSensitiveDetector()
                            ->GetFullPathName();
  }
  // Print end of line
  G4cout << G4endl;

  // Check overlapping volumes
  if (overlap_check) volume->CheckOverlaps();

  // Descend down the tree
  for (int i = 0; i < volume->GetLogicalVolume()->GetNoDaughters(); i++)
  {
    DumpGeometry(overlap_check, volume->GetLogicalVolume()->GetDaughter(i), depth+1);
  }
}
