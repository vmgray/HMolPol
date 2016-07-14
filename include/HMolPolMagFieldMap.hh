/********************************************
 * \author <b>Programmer:<\b> Wouter Deconinck
 * \author <b>Assisted By:<\b>s
 *
 * \brief <b>Purpose:</b> This is header file for the mapped magnetic field
 *
 * \date <b>Date:</b> 05-04-2016
 * \date <b>Modified:</b>
 *
 * \note <b>Entry Conditions:</b> none
 *
********************************************/

#ifndef HMOLPOLMAGFIELDMAP_HH_
#define HMOLPOLMAGFIELDMAP_HH_

//Geant4 specific includes
#include <G4MagneticField.hh>

//HMolPol includes
#include <HMolPolMagFieldMapBasic.hh>

class HMolPolMagFieldMap: public HMolPolMagField {

  public:

    // Constructor with logical volume and gdml settings
    HMolPolMagFieldMap(G4LogicalVolume* volume, std::map<G4String,G4String> gdml)
    : HMolPolMagField(volume,gdml)
    {
      // Read the filename from the input string
      if (gdml.count("MagFieldMapFile"))
        fFileName = gdml["MagFieldMapFile"];
      else
        G4cout << "    Warning: Specify tag MagFieldMapFile with path to file" << G4endl;
      G4cout << "    Map file " << fFileName << G4endl;

      // Create field
      CreateField();

      // Create print command
      fMessenger->DeclareMethod("print",&HMolPolMagFieldMap::Print,
          "Print information on the magnetic field " + volume->GetName());

      // Create file name commands
      fMessenger->DeclareProperty("setFileName",fFileName,
          "Set the file name for mapped magnetic field " + volume->GetName());
      fMessenger->DeclareMethod("load",&HMolPolMagFieldMap::CreateField,
          "Load the mapped magnetic field " + volume->GetName());
    };

    /// Destructor for the HMolPolMagFieldMap
    virtual ~HMolPolMagFieldMap() {
      // Delete existing field
      if (fField) delete fField;
    };

    void Print() {
      G4cout << "Mapped magnetic field " << fVolume->GetName() << G4endl;
      G4cout << "Filename " << fFileName << G4endl;
      HMolPolMagField::Print();
    }

    /// Create or recreate the field
    void CreateField() {
      // Delete existing field
      if (fField) delete fField;

      // Create uniform magnetic field with the given field vector
      fField = new HMolPolMagFieldMapBasic<float,3>(fFileName);
    }

    /// Set the filename
    void SetFileName(const G4String& filename) { fFileName = filename; }

  private:

    /// Magnetic field map
    HMolPolMagFieldMapBasic<float,3> *fField;

    /// File name
    G4String fFileName;

};

#endif // HMOLPOLMAGFIELDMAP_HH_
