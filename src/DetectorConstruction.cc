// \file B1/src/DetectorConstruction.cc
/// \brief Implementation of the B1::DetectorConstruction class
#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Element.hh"
#include "G4Isotope.hh"
#include "G4Material.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4PVReplica.hh"
#include "G4PVDivision.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
namespace B1
{
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  // Envelope parameters
  //
  G4double cubeX = 5*cm, cubeY=5*cm,cubeZ = 5*cm;
  G4double screenX = 5.*cm, screenY=5*cm,screenZ = 0.262*mm;
  
  G4double layer1X = (cubeX+screenX), layer1Y=(cubeY+screenY),layer1Z = (cubeZ+screenZ);
  G4double envelop1X = 5.*cm, envelop1Y=5*cm,envelop1Z = (cubeZ+screenZ)*3;
  
  G4double layer2X = envelop1X, layer2Y=envelop1Y,layer2Z = envelop1Z;
  G4double envelop2X = envelop1X, envelop2Y=3*envelop1Y, envelop2Z = envelop1Z ;
  
  G4double layer3X = envelop2X, layer3Y=envelop2Y,layer3Z = envelop2Z;
  G4double envelop3X = 3*envelop2X, envelop3Y=envelop2Y, envelop3Z = envelop2Z ;

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;
  //
  // World
  //
  G4double world_sizeX = 10*cubeX;
  G4double world_sizeY = 10*cubeY;
  G4double world_sizeZ = 10*cubeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  auto solidWorld = new G4Box("World", 0.5 * world_sizeX, 0.5 * world_sizeY, 0.5 * world_sizeZ); 
  auto logicWorld = new G4LogicalVolume(solidWorld, world_mat,"World"); 
  auto physWorld = new G4PVPlacement(nullptr,G4ThreeVector(),logicWorld,"World", nullptr,false,0,checkOverlaps);




  //
  // Cube
  //
  
  
 
//---------------Material properties table for PVT EJ 200---------------------
	
	//( parts from https://github.com/murffer/DetectorSim/blob/master/ScintillationSlab/src/Material.cc)
	
	
	G4Material* cube_mat =nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
	// Index of Reflection (146 nm to 1570 nm)
	const G4int nRINDEX = 2;
	G4double photonEnergyRINDEX[nRINDEX] = {0.7973*eV,8.5506*eV};
	G4double RefractiveIndexGlass[nRINDEX]= {1.58, 1.58};
	// Absorbition Length
	const G4int nABS=2;
	G4double photonEnergyABS[nABS] = {1.75*eV,3.5*eV};
	G4double AbsLengthGlass[nABS] = {380*cm, 380*cm};
	// Setting sctintillation to be the emission spectra
	const G4int nEM = 6;
	G4double photonEnergyEM[nEM] = {2.49,2.58,2.70,2.82,2.92,3.01};
	G4double emEJ200[nEM]={0.05,0.18,0.45,0.75,1.0,0};
	// Creating the materials property table and adding entries into properties table
	G4MaterialPropertiesTable* EJ200_mt = new G4MaterialPropertiesTable();
	cube_mat->SetMaterialPropertiesTable(EJ200_mt);
	
	// Setting  Scintillation Properties
	EJ200_mt->AddProperty("SCINTILLATIONCOMPONENT1",photonEnergyEM,emEJ200,nEM);
	EJ200_mt->AddProperty("RINDEX",photonEnergyRINDEX,RefractiveIndexGlass,nRINDEX);
	EJ200_mt->AddProperty("ABSLENGTH",photonEnergyABS,AbsLengthGlass,nABS);
	EJ200_mt->AddConstProperty("SCINTILLATIONYIELD", 10200. / MeV);
	EJ200_mt->AddConstProperty("RESOLUTIONSCALE", 1.0);
	EJ200_mt->AddConstProperty("SCINTILLATIONTIMECONSTANT1",2.5*ns);
	EJ200_mt->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
	


  //
  // Neutron detection screens------------------------------------------------------
  //
  //--------------------------------------------------
  // Enriched Li and LiF
  //--------------------------------------------------
  G4Isotope* Li6 = new G4Isotope("Li6",3,6,6.015*g/mole);
  G4Isotope* Li7 = new G4Isotope("Li7",3,7,7.015*g/mole);
  G4Element* enrichLi  = new G4Element("enriched Lithium","Li",2);
  enrichLi->AddIsotope(Li6,0.95*perCent);
  enrichLi->AddIsotope(Li7,0.05*perCent);

  G4Material* LiF = new G4Material("6LiF",2.64*g/cm3,2);
  LiF->AddElement(enrichLi,1);
  LiF->AddElement(nist->FindOrBuildElement("F"),1);

  G4Material* EJ426 = new G4Material("EJ426",4.1*g/cm3,4);
  EJ426->AddElement(enrichLi,0.081);
  EJ426->AddMaterial(nist->FindOrBuildMaterial("G4_F"),0.253);
  EJ426->AddMaterial(nist->FindOrBuildMaterial("G4_Zn"),0.447);
  EJ426->AddMaterial(nist->FindOrBuildMaterial("G4_S"),0.219);

  //---------------Material properties table for EJ426---------------------

	// Refrative Index
	const G4int nRINDEX_426 = 2;
	G4double photonEnergyRINDEX_426[nRINDEX_426] = {2.75*eV,3.5*eV};
	G4double RefractiveIndexEJ426[nRINDEX_426]= {2.36,2.36};

	// Absorbition Length
	const G4int nABS_426=2;
	G4double photonEnergyABS_426[nABS_426] = {2.75*eV,3.5*eV};
	G4double AbsLengthEJ426[nABS_426]={40*um,40*um};

	// Setting sctintillation to be the emission spectra
	const G4int nEM_426 = 10;
	G4double photonEnergyEM_426[nEM_426] = {2.14*eV,2.30*eV,2.38*eV,2.48*eV,2.58*eV,2.70*eV,2.76*eV,2.82*eV,2.95*eV,3.10*eV};
	G4double emEJ426[nEM_426]={0.0,0.0130,0.0260,0.0519,0.1429,0.2338,0.2597,0.2208,0.0519,0.0};

	// Creating the materials property table and adding entries into properties table
	G4MaterialPropertiesTable* MPTEJ426 = new G4MaterialPropertiesTable();
	MPTEJ426->AddProperty("RINDEX",photonEnergyRINDEX_426,RefractiveIndexEJ426,nRINDEX_426);
	MPTEJ426->AddProperty("ABSLENGTH",photonEnergyABS_426,AbsLengthEJ426,nABS_426);
	// Setting  Scintillation Properties
	MPTEJ426->AddProperty("SCINTILLATIONCOMPONENT1",photonEnergyEM_426,emEJ426,nEM_426);
	MPTEJ426->AddConstProperty("SCINTILLATIONTIMECONSTANT1",200*ns);     
	MPTEJ426->AddConstProperty("SCINTILLATIONYIELD", 72000*MeV);
	MPTEJ426->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
	MPTEJ426->AddConstProperty("RESOLUTIONSCALE", 1.0);
	MPTEJ426->DumpTable();
	EJ426->SetMaterialPropertiesTable(MPTEJ426);




//
//PLACEMENT OF ALL THE OBJECTS IN THE WORLD--------------------------------------
//
//
 // Make scoringvolume
 //
 
G4Material* envelop_mat = nist->FindOrBuildMaterial("G4_Galactic");

	auto solidEnvelop3 = new G4Box("Envelop3", 0.5 * envelop3X, 0.5 * envelop3Y, 0.5 * envelop3Z); 
  	auto logicEnvelop3 = new G4LogicalVolume(solidEnvelop3, envelop_mat,"Envelop3"); 
  	auto physEnvelop3 = new G4PVPlacement(nullptr,G4ThreeVector(0.5*envelop3X,0.5*envelop3Y,0.5*envelop1Z),logicEnvelop3,"Envelop3", logicWorld,false,0,checkOverlaps);



//Placing 3 envelop1 in envelop2

	auto solidLayer3 = new G4Box("Layer3", 0.5 * envelop2X, 0.5 * envelop2Y, 0.5 * envelop2Z); 
	auto logicLayer3 = new G4LogicalVolume(solidLayer3, envelop_mat,"Layer3");
	new G4PVReplica( "Layer3", logicLayer3, logicEnvelop3, kXAxis,3,envelop2X);






	auto solidEnvelop2 = new G4Box("Envelop2", 0.5 * envelop2X, 0.5 * envelop2Y, 0.5 * envelop2Z); 
  	auto logicEnvelop2 = new G4LogicalVolume(solidEnvelop2, envelop_mat,"Envelop2"); 
  	//auto physEnvelop2 = new G4PVPlacement(nullptr,G4ThreeVector(0.,0.5*envelop2Y,0.5*envelop1Z),logicEnvelop2,"Envelop2", logicWorld,false,0,checkOverlaps);
  	auto physEnvelop2 = new G4PVPlacement(nullptr,G4ThreeVector(0.,0.,0.),logicEnvelop2,"Envelop2", logicLayer3,false,0,checkOverlaps);


//Placing 3 envelop1 in envelop2

	auto solidLayer2 = new G4Box("Layer2", 0.5 * envelop1X, 0.5 * envelop1Y, 0.5 * envelop1Z); 
	auto logicLayer2 = new G4LogicalVolume(solidLayer2, envelop_mat,"Layer2");
	new G4PVReplica( "Layer2", logicLayer2, logicEnvelop2, kYAxis,3,envelop1Y);

  	auto solidEnvelop1 = new G4Box("Envelop1", 0.5 * envelop1X, 0.5 * envelop1Y, 0.5 * envelop1Z); 
  	auto logicEnvelop1 = new G4LogicalVolume(solidEnvelop1, envelop_mat,"Envelop1"); 
  	auto physEnvelop1 = new G4PVPlacement(nullptr,G4ThreeVector(0.,0.,0.),logicEnvelop1,"Envelop1", logicLayer2,false,0,checkOverlaps);


  
// Making a layer for the replicas
   	auto solidLayer1 = new G4Box("Layer1", 0.5 * layer1X, 0.5 * layer1Y, 0.5 * layer1Z); 
	auto logicLayer1 = new G4LogicalVolume(solidLayer1, envelop_mat,"Layer1");
	new G4PVReplica( "Layer1", logicLayer1, logicEnvelop1, kZAxis,3,layer1Z );


//Placing the scintillator in the layer of the replicas
	auto solidCube = new G4Box("Cube", 0.5 * cubeX, 0.5 * cubeY, 0.5 * cubeZ); 
	auto logicCube = new G4LogicalVolume(solidCube, cube_mat,"Cube");
	auto physCube = new G4PVPlacement(nullptr,G4ThreeVector(0.,0.,-0.5*layer1Z+screenZ+0.5*cubeZ),logicCube,"Cube", logicLayer1,false,0,checkOverlaps);

  //Placing the screens  in the layer of the replicas
	auto solidScreen = new G4Box("Screen", 0.5*screenX, 0.5*screenY, 0.5 *screenZ); 
	auto logicScreen = new G4LogicalVolume(solidScreen, EJ426,"Screen");
	auto physScreen = new G4PVPlacement(nullptr,G4ThreeVector(0.,0.,-0.5*layer1Z+0.5*screenZ),logicScreen,"Screen", logicLayer1,false,0,checkOverlaps);

 // Wrapping the scintillator in tyvek	
 	G4OpticalSurface *tyvekWrap = new G4OpticalSurface("TyvekWrap");
    	G4LogicalSkinSurface *tyvekSurface = new G4LogicalSkinSurface("TyvekSurface", logicCube, tyvekWrap);
    	tyvekWrap->SetType(dielectric_LUT);
    	tyvekWrap->SetModel(LUT);
    	tyvekWrap->SetFinish(polishedtyvekair);
  // Set cube+screen as scoring volume
  //
  fScoringVolume = logicEnvelop2;

  //always return the physical World
  //
  return physWorld;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
