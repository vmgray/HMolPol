/********************************************
 * \author <b>Programmer:<\b> Valerie Gray
 * \author <b>Assisted By:<\b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is header file for the
 *      detector construction, which makes all the detectors
 *
 * \date <b>Date:</b> 06-15-2013
 * \date <b>Modified:</b> 07-06-2013
 *
 * \note <b>Entry Conditions:</b> none
 *
********************************************/

//Geant4 includes
#include <G4VUserDetectorConstruction.hh>
#include <G4VPhysicalVolume.hh>
#include <G4GDMLParser.hh>

// HMolPol classes
class HMolPolAnalysis;

class HMolPolDetectorConstruction: public G4VUserDetectorConstruction
{
  private:

    /// GDML parser object included in the
    /// detector construction class
    G4GDMLParser fGDMLParser;

  public:

    /// Constructor for the Detector construction
    HMolPolDetectorConstruction(HMolPolAnalysis* analysis)
    {
      fAnalysis = analysis;
      fGDMLParser = 0;
    };
    /// Destructor of the Detector construction
    virtual ~HMolPolDetectorConstruction() { };

  public:

    // Analysis
    HMolPolAnalysis* fAnalysis;  ///< the Analysis object

    /// Construct the volume??
    G4VPhysicalVolume* Construct();  ///< construct the physical volume

};
