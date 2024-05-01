// \file B1/src/SteppingAction.cc
/// \brief Igh21mplementation of the B1::SteppingAction class

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"



namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction)
: fEventAction(eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  if (!fScoringVolume) {
    const auto detConstruction = static_cast<const DetectorConstruction*>(
      G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detConstruction->GetScoringVolume();
  }

  // get volume of the current step
  G4LogicalVolume* volume
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();
      
  G4AnalysisManager* analysis = G4AnalysisManager::Instance(); 
  
  // check if we are in scoring volume
  if (volume != fScoringVolume) { 
  //analysis->FillH1(0, 0);
  analysis->FillNtupleDColumn(0,0);
  analysis->FillNtupleSColumn(1,"Not in volume");
  analysis->FillNtupleSColumn(2,"Not in volume");
  analysis->FillNtupleSColumn(3,"Not in volume");
  analysis->FillNtupleSColumn(4,"Not in volume");
  }

  // collect energy deposited in this step
  G4double edepStep = step->GetTotalEnergyDeposit();
  fEventAction->AddEdep(edepStep);

  //G4String procName=step->GetTrack()->GetParticleDefinition()->GetParticleName(); 
  //G4StepPoint* postPoint = step->GetPostStepPoint();
  //G4StepPoint* prePoint=step->GetPreStepPoint();
  //const G4VProcess *post_step=postPoint->GetProcessDefinedStep();
  //const G4VProcess *pre_step=prePoint->GetProcessDefinedStep();
  //if( post_step!=nullptr && pre_step!=nullptr){
  //const G4String & processName_post = post_step->GetProcessName();
  //const G4String & processName_pre = pre_step->GetProcessName();
  //}
  
  G4String particleStep=step->GetTrack()->GetParticleDefinition()->GetParticleName();
  
  //G4double t=step->GetTrack()->GetLocalTime();
  
  G4double particleX=step->GetTrack()->GetPosition().x();
  G4double particleY=step->GetTrack()->GetPosition().y();
  G4double particleZ=step->GetTrack()->GetPosition().z();
  
  fEventAction->AddDecayChain(particleStep);
  
 
   
  //analysis->FillH1(0, edepStep/MeV);
  analysis->FillNtupleDColumn(0, edepStep/MeV);
  analysis->FillNtupleSColumn(1,particleStep);
  analysis->FillNtupleDColumn(2,particleX/mm);
  analysis->FillNtupleDColumn(3,particleY/mm);
  analysis->FillNtupleDColumn(4,particleZ/mm);
  
  analysis->AddNtupleRow();
  
  //if(particleX<-2.5*cm or particleY<-2.5*cm or particleZ<-2.5*cm) 
  //{
  //analysis->FillNtupleDColumn(0,0);
  //analysis->FillNtupleSColumn(1,"Out of scintillator");

  //analysis->FillNtupleDColumn(2,0);
  //analysis->FillNtupleDColumn(3,0);
  //analysis->FillNtupleDColumn(4,0);
  //analysis->AddNtupleRow();
     //}
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
