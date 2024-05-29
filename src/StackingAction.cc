#include "StackingAction.hh"
#include "Run.hh"

#include "G4RunManager.hh"
#include "G4Track.hh"
#include "G4NeutrinoE.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StackingAction::StackingAction()
:G4UserStackingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StackingAction::~StackingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack
StackingAction::ClassifyNewTrack(const G4Track* aTrack)
{
  //keep primary particle
  if (aTrack->GetParentID() == 0) return fUrgent;
  
  //count secondary particles
  G4String name   = aTrack->GetDefinition()->GetParticleName();
  G4double energy = aTrack->GetKineticEnergy();
  Run* run = static_cast<Run*>(
        G4RunManager::GetRunManager()->GetNonConstCurrentRun());    
  run->ParticleCount(name,energy);
  
  //kill all secondaries  
  return fKill;
}
