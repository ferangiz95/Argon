#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction;

class TrackingAction : public G4UserTrackingAction {

  public:  
    TrackingAction(DetectorConstruction*);
   ~TrackingAction() {};
   
    virtual void PreUserTrackingAction(const G4Track*);   
    virtual void PostUserTrackingAction(const G4Track*);
    
    void UpdateTrackInfo(G4double, G4double, G4double, G4double);    
    
  private:
    DetectorConstruction* fDetector;
    G4int fNbStep1, fNbStep2;
    G4double fTrackLen1, fTrackLen2;
    G4double fTime1, fTime2;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
