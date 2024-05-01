/// \file B1/src/RunAction.cc
/// \brief Implementation of the B1::RunAction class

#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
//#include "Run.hh"
#include <fstream>

#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
{
  G4AnalysisManager* analysis = G4AnalysisManager::Instance();
   
  analysis->SetNtupleMerging(true);
  //analysis->CreateH1(name, title, nbins, vmin, vmax);
  //analysis->CreateH1("Edep", "Energy deposition", 100, 0*MeV, 100*MeV);
   
  analysis->CreateNtuple("CubeNtuple", "Ntuple");
  analysis->CreateNtupleDColumn("Edep");
  analysis->CreateNtupleSColumn("ID");
  analysis->CreateNtupleDColumn("x");
  analysis->CreateNtupleDColumn("y");
  analysis->CreateNtupleDColumn("z");
  analysis->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  char fileName[65];
  sprintf(fileName, "output_run%03d.csv",aRun->GetRunID());
  analysisManager->SetFileName(G4String(fileName));
  analysisManager->OpenFile();
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;   

  G4AnalysisManager* analysis = G4AnalysisManager::Instance();

  analysis->Write();
  analysis->CloseFile();
  
  const auto detConstruction = static_cast<const DetectorConstruction*>(
    G4RunManager::GetRunManager()->GetUserDetectorConstruction());

  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  const auto generatorAction = static_cast<const PrimaryGeneratorAction*>(
    G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
  if (generatorAction)
  {
    const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
    runCondition += particleGun->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    G4double particleEnergy = particleGun->GetParticleEnergy();
    runCondition += G4BestUnit(particleEnergy,"Energy");
  }

  // Print
  //
  if (IsMaster()) {
    G4cout
     << G4endl
     << "--------------------End of Global Run-----------------------";
  }
  else {
    G4cout
     << G4endl
     << "--------------------End of Local Run------------------------";
  }
     
}
   
}
