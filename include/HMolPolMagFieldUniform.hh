/*
 * HMolPolMagFieldUniform.hh
 *
 *  Created on: Jul 14, 2016
 *      Author: wdconinc
 */

#ifndef INCLUDE_HMOLPOLMAGFIELDUNIFORM_HH_
#define INCLUDE_HMOLPOLMAGFIELDUNIFORM_HH_

#include <G4UniformMagField.hh>

#include "HMolPolMagField.hh"

class HMolPolMagFieldUniform: public HMolPolMagField {

  public:

    // Constructor with logical volume and gdml settings
    HMolPolMagFieldUniform(G4LogicalVolume* volume, std::map<G4String,G4String> gdml)
    : HMolPolMagField(volume,gdml),fFieldVector(0.0,0.0,0.0)
    {
      // If no magnetic field value tag is found, just leave at zero field
      try {
        // Read the vector from the input string
        if (gdml.count("MagFieldVector")) {
          std::stringstream(gdml["MagFieldVector"]) >> fFieldVector;
        }
        else
          G4cout << "    Warning: Specify MagFieldVector as vector (Bx,By,Bz) in units of Tesla" << G4endl;
        // Assume this is in units of tesla
        fFieldVector *= CLHEP::tesla;
        G4cout << "    Field vector " << fFieldVector / CLHEP::tesla << " T" << G4endl;
      } catch (const std::exception& ex) {
        G4cout << "    Could not parse " << gdml["MagFieldVector"] << G4endl;
      }

      // Create field
      CreateField();

      // Create print command
      fMessenger->DeclareMethod("print",&HMolPolMagFieldUniform::Print,
          "Print information on the magnetic field " + volume->GetName());

      // Create field component commands
      fMessenger->DeclareMethodWithUnit("setFieldX","tesla",&HMolPolMagFieldUniform::SetFieldX,
          "Set the Bx component of uniform magnetic field " + volume->GetName());
      fMessenger->DeclareMethodWithUnit("setFieldY","tesla",&HMolPolMagFieldUniform::SetFieldY,
          "Set the By component of uniform magnetic field " + volume->GetName());
      fMessenger->DeclareMethodWithUnit("setFieldZ","tesla",&HMolPolMagFieldUniform::SetFieldZ,
          "Set the Bz component of uniform magnetic field " + volume->GetName());

    }

    /// Destructor for the HMolPolMagFieldUniform
    virtual ~HMolPolMagFieldUniform() {
      // Delete existing field
      if (fField) delete fField;
    };

    /// Print info
    void Print() {
      G4cout << "Uniform magnetic field with field vector " << fFieldVector / CLHEP::tesla << " T" << G4endl;
      HMolPolMagField::Print();
    }

  private:

    // Change the field components and recreate the fields
    void SetFieldX(G4double Bx) { fFieldVector.setX(Bx); CreateField(); }
    void SetFieldY(G4double By) { fFieldVector.setY(By); CreateField(); }
    void SetFieldZ(G4double Bz) { fFieldVector.setZ(Bz); CreateField(); }

    // Create or recreate the field
    void CreateField() {
      // Delete existing field
      if (fField) delete fField;

      // Create uniform magnetic field with the given field vector
      fField = new G4UniformMagField(fFieldVector);
    }

  private:

    // Field vector
    G4ThreeVector fFieldVector;

};

#endif /* INCLUDE_HMOLPOLMAGFIELDUNIFORM_HH_ */
