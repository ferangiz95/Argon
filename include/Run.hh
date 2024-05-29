#ifndef Run_h
#define Run_h 1

#include "G4Run.hh"
#include "G4VProcess.hh"
#include "globals.hh"
#include <map>

class DetectorConstruction;
class G4ParticleDefinition;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class Run : public G4Run
{
  public:
    Run(DetectorConstruction*);
   ~Run() override = default;

  public:
    void CountProcesses(const G4VProcess* process);                  
    void ParticleCount(G4String, G4double);
    void SumTrackLength (G4int,G4int,G4double,G4double,G4double,G4double);
    
    void SetPrimary(G4ParticleDefinition* particle, G4double energy);    
    void EndOfRun(); 
            
    void Merge(const G4Run*) override;
    void AddEdep (G4double edep);
   
   
  private:
    struct ParticleData {
     ParticleData()
       : fCount(0), fEmean(0.), fEmin(0.), fEmax(0.) {}
     ParticleData(G4int count, G4double ekin, G4double emin, G4double emax)
       : fCount(count), fEmean(ekin), fEmin(emin), fEmax(emax) {}
     G4int     fCount;
     G4double  fEmean;
     G4double  fEmin;
     G4double  fEmax;
    };
     
  private:
    DetectorConstruction* fDetector = nullptr;
    G4ParticleDefinition* fParticle = nullptr;
    G4double              fEkin = 0.;
        
    std::map<G4String,G4int>        fProcCounter;            
    std::map<G4String,ParticleData> fParticleDataMap;
        
    G4int    fNbStep1 = 0, fNbStep2 = 0;
    G4double fTrackLen1 = 0., fTrackLen2 = 0.;
    G4double fTime1 = 0., fTime2 = 0.;    
};

#endif

