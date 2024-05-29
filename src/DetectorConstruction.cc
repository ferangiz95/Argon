#include "DetectorConstruction.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "DetectorMessenger.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RunManager.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4PhysicalConstants.hh"
#include "G4Tubs.hh"

#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
{
  fBoxSize = 2.0*m;
  DefineMaterials();
  SetMaterial("G4_AIR");
  fDetectorMessenger = new DetectorMessenger(this); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{  delete fDetectorMessenger;}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  return ConstructVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
 
  G4NistManager* man = G4NistManager::Instance();
  
  G4int ncomponents;
  G4bool isotopes = false;
  G4double fractionmass;
  
  G4Element* C = new G4Element("Carbon", "C", 6, 12.011*g/mole);
  G4Element* Cr = new G4Element("Chromium","Cr", 24., 52.00*g/mole);
  G4Element* Fe = new G4Element("Ferrum","Fe", 26., 55.85*g/mole);
  G4Element* Ni = new G4Element("Nickel","Ni", 28., 58.70*g/mole);
  G4Element* Ti = new G4Element("Titanium", "Ti", 22., 47.88*g/mole);

  // build materials
  //

  auto steel_mat = 
  new G4Material("steel_mat", 7.9 * g / cm3, ncomponents=5, kStateSolid, 293.*kelvin, 1.*atmosphere);
  
    steel_mat->AddElement(C, fractionmass=0.008);
    steel_mat->AddElement(Cr, fractionmass=0.18);
  
    steel_mat->AddElement(Fe, fractionmass=0.707);
    steel_mat->AddElement(Ni, fractionmass=0.10);
    steel_mat->AddElement(Ti, fractionmass=0.005); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::ConstructVolumes()
{

  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  
  /*G4double world_sizeX = 2.0*m;
  G4double world_sizeY = 2.0*m;
  G4double world_sizeZ = 2.0*m;*/
  
  G4NistManager* man = G4NistManager::Instance();

  G4Box*
  sBox = new G4Box("Container",                         //its name
                   fBoxSize/2,fBoxSize/2,fBoxSize/2);   //its dimensions

  fLBox = new G4LogicalVolume(sBox,                     //its shape
                             fMaterial,                 //its material
                             fMaterial->GetName());     //its name

  fPBox = new G4PVPlacement(0,                          //no rotation
                            G4ThreeVector(),            //at (0,0,0)
                            fLBox,                      //its logical volume
                            fMaterial->GetName(),       //its name
                            0,                          //its mother  volume
                            false,                      //no boolean operation
                            0);                         //copy number

                            
  G4double TargetLength      = 75.5*cm; 
  G4double TargetRadius1     = 7.75*cm;
  G4double TargetRadius2      = 13.0*cm;
  G4double DetectorLength    = 75.5*cm; 
  G4double DetectorThickness = 0.1*cm;
  G4double WorldRadius;
  WorldRadius = TargetRadius2 + DetectorThickness;  
   
  G4Material *world_mat = man->FindOrBuildMaterial("G4_AIR"); 
      
  G4Tubs* 
  empty = new G4Tubs("Empty",                                   //name
                       0., TargetRadius1, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicEmpty = new G4LogicalVolume(empty,           //shape
                             world_mat,              //material
                             "Empty");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicEmpty,                //logical volume
                           "Empty",                    //name
                           fLBox,                 //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy number 
                               
  G4Material *TargetMater = man->FindOrBuildMaterial("G4_Ca");
                 
  G4Tubs* 
  sTarget = new G4Tubs("Target",                                   //name
                       TargetRadius1, TargetRadius2, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicTarget = new G4LogicalVolume(sTarget,           //shape
                             TargetMater,              //material
                             "Target");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget,                //logical volume
                           "Target",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy number
  
  G4Material* steel_mat = man->FindOrBuildMaterial("steel_mat"); 
  
  // Detector
  //
  G4Tubs* 
  sDetector = new G4Tubs("Detector",  
                         TargetRadius2, WorldRadius, 0.5*DetectorLength, 0.,twopi);


  G4LogicalVolume *LogicDetector = new G4LogicalVolume(sDetector,       //shape
                             steel_mat,            //material
                             "Detector");               //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicDetector,              //logical volume
                           "Detector",                  //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy number

  G4VisAttributes *blue = new G4VisAttributes(G4Colour::Blue());
  blue->SetVisibility(true);
  blue->SetForceWireframe(true);
  LogicDetector->SetVisAttributes(blue);
  
  G4VisAttributes *green = new G4VisAttributes(G4Colour::Green());
  green->SetVisibility(true);
  green->SetForceWireframe(true);
  LogicTarget->SetVisAttributes(green);                         
  
  
  G4VisAttributes *red = new G4VisAttributes(G4Colour::Red());
  red->SetVisibility(true);
  red->SetForceWireframe(true);
  LogicEmpty->SetVisAttributes(red); 
  
  G4VisAttributes *yellow = new G4VisAttributes(G4Colour::Yellow());
  yellow->SetVisibility(true);
  yellow->SetForceWireframe(true);
  fLBox->SetVisAttributes(yellow); 
  
  //always return the root volume
  //
  return fPBox;
}

void DetectorConstruction::PrintParameters()
{
  G4cout << "\n The Box is " << G4BestUnit(fBoxSize,"Length")
         << " of " << fMaterial->GetName() 
         << "\n \n" << fMaterial << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial =
     G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);   
  
  if (pttoMaterial) { 
    if(fMaterial != pttoMaterial) {
      fMaterial = pttoMaterial;
      if(fLBox) { fLBox->SetMaterial(pttoMaterial); }
      G4RunManager::GetRunManager()->PhysicsHasBeenModified();
    }
  } else {
    G4cout << "\n--> warning from DetectorConstruction::SetMaterial : "
           << materialChoice << " not found" << G4endl;
  }              
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetSize(G4double value)
{
  fBoxSize = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

