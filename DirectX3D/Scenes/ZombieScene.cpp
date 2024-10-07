#include "Framework.h"
#include "ZombieScene.h"

ZombieScene::ZombieScene()
{
	Audio::Get()->Add("BG", "Sounds/BG/wind_forest.wav", true, true);
	Audio::Get()->Add("move", "Sounds/Zombie/move.wav", false, false, true);

	Audio::Get()->Play("BG", 0.1f);

	ParticleManager::Get()->Add("BloodExplode", "TextData/Particle/BloodExplode.fx", 20);

}

ZombieScene::~ZombieScene()
{
	delete terrain;
	delete player;

	InstanceCharacterManager::Delete();
}

void ZombieScene::Update()
{
	if (KEY_DOWN('P'))
	{
		InstanceCharacterManager::Get()->Spawn("ZombieWoman", 1);
		InstanceCharacterManager::Get()->Spawn("ZombieMutant", 1);
	}
	aStar->Update();

	player->Update();
	weapon->Update();
	weapon->Shoot(100, 2);
	InstanceCharacterManager::Get()->Update();
	ParticleManager::Get()->Update();
	ProjectileManager::Get()->Update();
}

void ZombieScene::PreRender()
{
}

void ZombieScene::Render()
{
	terrain->Render();
	aStar->Render();

	InstanceCharacterManager::Get()->Render();
	ParticleManager::Get()->Render();
	ProjectileManager::Get()->Render();

	player->Render();
	weapon->Render();
}

void ZombieScene::PostRender()
{
}

void ZombieScene::GUIRender()
{
	player->GUIRender();
	weapon->GUIRender();
	InstanceCharacterManager::Get()->GUIRender();
	ProjectileManager::Get()->GUIRender();
}

void ZombieScene::Start()
{
	terrain = new Terrain("test");
	aStar = new AStar(30, 30);
	aStar->SetNode(terrain);

	player = new Knight();
	player->Pos() = { 100, 0, 100 };

	weapon = new RangeWeapon("Rifle", 0.125);
	weapon->SetParent(player);
	weapon->GetMesh()->Pos().y = 5.0f;
	weapon->SetProjectile("bullet");

	PlayerController::Get()->Possess(player);

	CAM->SetTarget(player);
	CAM->TargetOptionLoad("ZombieSurvivalCamera");
	CAM->LookAtTarget();

	player->SetCamera(CAM);

	InstanceCharacterManager::Get()->Add("ZombieWoman", "ZombieWoman");
	InstanceCharacterManager::Get()->Add("ZombieMutant", "ZombieMutant");
	InstanceCharacterManager::Get()->SetTarget(player);
	InstanceCharacterManager::Get()->SetAstar(aStar);
	InstanceCharacterManager::Get()->SetTerrain(terrain);
	
}

void ZombieScene::End()
{
	InstanceCharacterManager::Get()->Remove("ZombieWoman");
	InstanceCharacterManager::Get()->Remove("ZombieMutant");
}
