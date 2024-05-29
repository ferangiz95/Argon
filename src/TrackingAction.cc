#include "TrackingAction.hh"

#include "Run.hh"
#include "HistoManager.hh"
#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4HadronicProcessType.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::TrackingAction(DetectorConstruction* det)
:G4UserTrackingAction(),
 fDetector(det), fNbStep1(0),fNbStep2(0),fTrackLen1(0.),fTrackLen2(0.),fTime1(0.),fTime2(0.)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PreUserTrackingAction(const G4Track*)
{
  fNbStep1 = fNbStep2 = 0;
  fTrackLen1 = fTrackLen2 = 0.;
  fTime1 = fTime2 = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::UpdateTrackInfo(G4double ekin, G4double trackl, G4double time, G4double edepStep)
{

  
  Run* run = static_cast<Run*>(
        G4RunManager::GetRunManager()->GetNonConstCurrentRun());    
  
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  const G4double thermal = 1*eV;
  if (ekin > thermal) {
    fNbStep1++; fTrackLen1 = trackl; fTime1 = time;    
  } else {
    fNbStep2++; fTrackLen2 = trackl - fTrackLen1; fTime2 = time - fTime1;  
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PostUserTrackingAction(const G4Track* track)
{
 // keep only primary neutron
 //
 G4int trackID = track->GetTrackID();
 if (trackID > 1) return;
 
 Run* run 
    = static_cast<Run*>(
        G4RunManager::GetRunManager()->GetNonConstCurrentRun()); 
 run->SumTrackLength(fNbStep1,fNbStep2,fTrackLen1,fTrackLen2,fTime1,fTime2);
 
 // histograms
 //
 G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
 analysisManager->FillH1(1,fNbStep1);
 analysisManager->FillH1(2,fTrackLen1);
 analysisManager->FillH1(3,fTime1); 
 analysisManager->FillH1(4,fNbStep2);
 analysisManager->FillH1(5,fTrackLen2);
 analysisManager->FillH1(6,fTime2);    
}
