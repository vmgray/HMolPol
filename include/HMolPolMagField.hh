/********************************************
 * \author <b>Programmer:<\b> Wouter Deconinck
 * \author <b>Assisted By:<\b>
 *
 * \brief <b>Purpose:</b> This is header file for a magnetic field
 *
 * \date <b>Date:</b> 05-04-2016
 * \date <b>Modified:</b>
 *
 * \note <b>Entry Conditions:</b> none
 *
********************************************/

#ifndef HMOLPOLMAGFIELD_HH_
#define HMOLPOLMAGFIELD_HH_

// Geant4 specific includes
#include <G4LogicalVolume.hh>
#include <G4MagneticField.hh>
#include <G4GenericMessenger.hh>

class HMolPolMagField: public G4MagneticField {

  public:

    // Constructor with logical volume and gdml settings
    HMolPolMagField(G4LogicalVolume* volume, std::map<G4String,G4String> gdml)
    : fField(0),fVolume(volume),fMessenger(0),
      fScale(1.0),fTranslation(0.0,0.0,0.0),fRotation(0.0,0.0,0.0)
    {
      // If no magnetic field scale tag is found, just leave it at 1.0
      try {
        // Read the scale from the input string
        if (gdml.count("MagFieldScale")) {
          std::stringstream(gdml["MagFieldScale"]) >> fScale;
        }
        else
          G4cout << "    Warning: Specify MagFieldScale as a dimensionless number" << G4endl;
        G4cout << "    Scale factor " << fScale << G4endl;
      } catch (const std::exception& ex) {
        G4cout << "    Could not parse " << gdml["MagFieldScale"] << G4endl;
      }

      // If no magnetic field value tag is found, just leave at zero field
      G4ThreeVector origin(0.0,0.0,0.0);
      try {
        // Read the origin value from the input string
        G4ThreeVector relative_origin;
        if (gdml.count("MagFieldOrigin"))
          std::stringstream(gdml["MagFieldOrigin"]) >> relative_origin;
        else
          G4cout << "    Warning: Specify MagFieldOrigin as vector (x,y,z) "
            << "in units of m relative to physical volume position" << G4endl;
        // Assume this is in units of tesla/m
        relative_origin *= CLHEP::m;
        G4cout << "    Relative origin " << relative_origin / CLHEP::m << " m" << G4endl;
        // Add relative origin to origin of volume
        origin += relative_origin;
      } catch (const std::exception& ex) {
        G4cout << "    Could not parse " << gdml["MagFieldOrigin"] << G4endl;
      }

      // If no magnetic field value tag is found, just leave at zero field
      try {
        // Read the Euler angles from the input string
        G4ThreeVector euler_angles;
        if (gdml.count("MagFieldEulerAngles"))
          std::stringstream(gdml["MagFieldEulerAngles"]) >> euler_angles;
        else
          G4cout << "    Warning: Specify MagFieldEulerAngles as vector (phi,theta,psi) "
          << "in units of degrees relative to the physical volume orientation" << G4endl;
        euler_angles *= CLHEP::degree;
        G4cout << "    Euler angles " << euler_angles / CLHEP::degree << " deg" << G4endl;
        fRotation.set(euler_angles.x(), euler_angles.y(), euler_angles.z());
      } catch (const std::exception& ex) {
        G4cout << "    Could not parse " << gdml["MagFieldEulerAngles"] << G4endl;
      }

      // Create messenger for this field
      G4String name = "/HMolPol/Fields/" + volume->GetName() + "/";
      G4String desc = "Field control for " + volume->GetName();
      fMessenger = new G4GenericMessenger(this, name, desc);

      // Create scale command
      fMessenger->DeclareProperty("scale",fScale,
          "Scale factor to apply to the magnetic field " + volume->GetName());

      // Create translation commands
      fMessenger->DeclareMethodWithUnit("setTranslationX","m",&HMolPolMagField::SetTranslationX,
          "Set the X component of the translation of the magnetic field " + volume->GetName());
      fMessenger->DeclareMethodWithUnit("setTranslationY","m",&HMolPolMagField::SetTranslationY,
          "Set the Y component of the translation of the magnetic field " + volume->GetName());
      fMessenger->DeclareMethodWithUnit("setTranslationZ","m",&HMolPolMagField::SetTranslationZ,
          "Set the Z component of the translation of the magnetic field " + volume->GetName());

      // Create rotation commands
      fMessenger->DeclareMethodWithUnit("setRotationPhi","degree",&HMolPolMagField::SetRotationPhi,
          "Set the Euler angle phi of the rotation of the magnetic field " + volume->GetName());
      fMessenger->DeclareMethodWithUnit("setRotationTheta","degree",&HMolPolMagField::SetRotationTheta,
          "Set the Euler angle theta of the rotation of the magnetic field " + volume->GetName());
      fMessenger->DeclareMethodWithUnit("setRotationPsi","degree",&HMolPolMagField::SetRotationPsi,
          "Set the Euler angle psi of the rotation of the magnetic field " + volume->GetName());

    }

    /// Destructor for the HMolPolMagField
    virtual ~HMolPolMagField() {
      // Delete the messenger again
      delete fMessenger;
    }

    /// Print info
    virtual void Print() {
      G4cout << "Scale factor = " << fScale << G4endl;
      G4cout << "Translation = " << fTranslation / CLHEP::m << " m" << G4endl;
      G4cout << "Rotation = " <<
          G4ThreeVector(fRotation.phi(),fRotation.theta(),fRotation.psi()) / CLHEP::degree << " deg" << G4endl;
    };

    /// Function to get the field value
    virtual void GetFieldValue(const G4double Point[4],
                                     G4double *Bfield) const {
      // Get the field
      fField->GetFieldValue(Point, Bfield);

      // Scale the field
      Bfield[0] *= fScale;
      Bfield[1] *= fScale;
      Bfield[2] *= fScale;
    }

    // Create or recreate the field
    virtual void CreateField() = 0;

    // Change the field translation
    void SetTranslationX(G4double x) { fTranslation.setX(x); }
    void SetTranslationY(G4double y) { fTranslation.setY(y); }
    void SetTranslationZ(G4double z) { fTranslation.setZ(z); }

    // Change the field rotation
    void SetRotationPhi(G4double phi)     { fRotation.setPhi(phi);     }
    void SetRotationTheta(G4double theta) { fRotation.setTheta(theta); }
    void SetRotationPsi(G4double psi)     { fRotation.setPsi(psi);     }

  protected:

    /// Magnetic field
    G4MagneticField* fField;

    /// Physical volume
    G4LogicalVolume* fVolume;

    /// Messenger
    G4GenericMessenger* fMessenger;

  private:

    /// Scale factor
    G4double fScale;

    /// Translation
    G4ThreeVector fTranslation;
    /// Rotation
    G4RotationMatrix fRotation;
};

#endif // HMOLPOLMAGFIELD_HH_
