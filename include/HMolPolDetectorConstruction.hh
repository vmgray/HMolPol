/********************************************
 * \author <b>Programmer:<\b> Valerie Gray
 * \author <b>Assisted By:<\b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is header file for the
 *      detector construction, which makes all the detectors
 *
 * \date <b>Date:</b> 06-15-2013
 * \date <b>Modified:</b> 07-14-2013
 *
 * \note <b>Entry Conditions:</b> none
 *
********************************************/

#ifndef HMOLPOLDETECTORCONSTRUCTION_HH_
#define HMOLPOLDETECTORCONSTRUCTION_HH_

//Geant4 includes
#include <G4VUserDetectorConstruction.hh>
#include <G4VPhysicalVolume.hh>
#include <G4MagneticField.hh>
#include <G4FieldManager.hh>
#include <G4GDMLParser.hh>

// HMolPol classes
class HMolPolAnalysis;

class HMolPolDetectorConstruction: public G4VUserDetectorConstruction
{
  private:

    /// GDML parser object included in the
    /// detector construction class
    G4GDMLParser* fGDMLParser;

  public:

    /// Constructor for the Detector construction
    HMolPolDetectorConstruction(HMolPolAnalysis* analysis)
    {
      fAnalysis = analysis;
      fGDMLParser = new G4GDMLParser();

      // Set a default geometry file name
      fGeometryFileName = "geometry/HMolPolMotherVolume.gdml";
    };
    /// Destructor of the Detector construction
    virtual ~HMolPolDetectorConstruction()
    {
      delete fGDMLParser;
    };

    void SetGeometryFileName(G4String fileName)
    {
      fGeometryFileName = fileName;
    }

  public:

    // Analysis
    HMolPolAnalysis* fAnalysis;  ///< the Analysis object

    /// Construct the volume??
    G4VPhysicalVolume* Construct();  ///< construct the physical volume

    // Geometry File name
    G4String fGeometryFileName;

    // List of magnetic fields and managers
    static G4ThreadLocal std::vector<G4MagneticField*> fMagneticFields;
    static G4ThreadLocal std::vector<G4FieldManager*> fFieldManagers;

};

#endif // HMOLPOLDETECTORCONSTRUCTION_HH_
