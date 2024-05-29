#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4LogicalVolume;
class G4Material;
class G4VPhysicalVolume;
class DetectorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
    DetectorConstruction();
   ~DetectorConstruction() override;
  
  public:
    G4VPhysicalVolume* Construct() override;  
    //void ConstructSDandField() override;
    void SetSize     (G4double);              
    void SetMaterial (G4String);
  
     const
     G4VPhysicalVolume* GetWorld()      {return fPBox;};
     G4double           GetSize()       {return fBoxSize;};      
     G4Material*        GetMaterial()   {return fMaterial;};
     
     void               PrintParameters();
  private:
     G4VPhysicalVolume* fPBox;
     G4LogicalVolume*   fLBox;
     
     G4double           fBoxSize = 0.;
     G4Material*        fMaterial;
     DetectorMessenger* fDetectorMessenger = nullptr;
  private:   
     void DefineMaterials();
     G4bool fCheckOverlaps = true; 
     G4VPhysicalVolume* ConstructVolumes();    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#endif

