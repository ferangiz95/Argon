#include "SteppingAction.hh"

#include "Run.hh"
#include "TrackingAction.hh"
#include "HistoManager.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
   
   
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4LogicalVolume.hh"

#include "DetectorConstruction.hh"
#include "EventAction.hh"
                    
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(DetectorConstruction* det, EventAction* event, TrackingAction* TrAct)
: G4UserSteppingAction(),fDetector(det), fEventAction(event), fTrackingAction(TrAct)
{
  fEventAction = event;
  auto analysisManager = G4AnalysisManager::Instance();
 }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  auto analysisManager = G4AnalysisManager::Instance();
  
  Run* run = static_cast<Run*>(
        G4RunManager::GetRunManager()->GetNonConstCurrentRun());  
  
  G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
  
  const G4String particleName = aStep->GetTrack()->GetDefinition()->GetParticleName();
  
  const G4int particlePDG = aStep->GetTrack()->GetDefinition()->GetPDGEncoding();
  
  const G4String currentPhysicalName = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
  
  G4ThreeVector pos  = aStep->GetPreStepPoint()->GetPosition();
  
    // get volume of the current step
  //
  auto Volume = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume(); 
  
  G4double KineticEnergy = aStep->GetTrack()->GetKineticEnergy();
  
  G4double edepStep = aStep->GetTotalEnergyDeposit();
  if (edepStep <= 0.) return;
    fEventAction->AddEdep(edepStep);
  
  analysisManager->FillNtupleIColumn(0, 0, evt);
  analysisManager->FillNtupleSColumn(0, 1, particleName);
  analysisManager->FillNtupleIColumn(0, 2, particlePDG);
  analysisManager->FillNtupleDColumn(0, 3, edepStep);
  analysisManager->FillNtupleDColumn(0, 4, KineticEnergy);
  analysisManager->FillNtupleIColumn(0, 5, pos[0]);
  analysisManager->FillNtupleIColumn(0, 6, pos[1]);
  analysisManager->FillNtupleIColumn(0, 7, pos[2]);
  analysisManager->AddNtupleRow(0);
  
  // count processes
  // 
  const G4StepPoint* endPoint = aStep->GetPostStepPoint();
  const G4VProcess* process   = endPoint->GetProcessDefinedStep();

  run->CountProcesses(process);

  // incident neutron
  //
  if (aStep->GetTrack()->GetTrackID() == 1) { 
    G4double ekin  = endPoint->GetKineticEnergy();
    G4double trackl = aStep->GetTrack()->GetTrackLength();
    G4double time   = aStep->GetTrack()->GetLocalTime();           
    fTrackingAction->UpdateTrackInfo(ekin,trackl,time, edepStep);
    analysisManager->FillH1(7,ekin);
  }    
  
}
