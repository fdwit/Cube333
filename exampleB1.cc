#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#include "G4RunManagerFactory.hh"
#include "G4PhysListFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4UImanager.hh"
#include "QBBC.hh"

#include "G4EmDNAPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4IonPhysics.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"

using namespace B1;

int main(int argc,char** argv)
{
  G4UIExecutive* ui = nullptr;
  //if ( argc == 1 ) { ui = new G4UIExecutive(argc, argv); }
  ui = new G4UIExecutive(argc, argv);
  G4int precision = 4;
  G4SteppingVerbose::UseBestUnit(precision);

  auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

  runManager->SetUserInitialization(new DetectorConstruction());

  G4PhysListFactory factory;
  //auto* physList = factory.GetReferencePhysList("QGSP_BERT_HP");
  //auto* physList = factory.GetReferencePhysList("QGSP_BIC_HP");
  auto* physList = factory.GetReferencePhysList("FTFP_BERT_EMZ");
  physList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physList);

  runManager->SetUserInitialization(new ActionInitialization());

  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  G4UImanager* UImanager = G4UImanager::GetUIpointer();

//  if ( ! ui ) {
    // batch mode
    //G4String command = "/control/execute ";
    //G4String fileName = argv[1];
    //UImanager->ApplyCommand(command+fileName);
//  }
//  else {
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
//  }

  delete visManager;
  delete runManager;
}
