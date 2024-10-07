#include "Framework.h"

RangeWeapon::RangeWeapon(string name, float interval)
	:Weapon(name), interval(interval)
{
	start = new SphereCollider();
	start->SetTag(name + "_start");
	start->UpdateWorld();
	start->SetParent(mesh);

	end = new SphereCollider();
	end->SetTag(name + "_end");
	end->UpdateWorld();
	end->SetParent(mesh);


	cooldown = interval;
	Load();
}

RangeWeapon::~RangeWeapon()
{
	delete start;
	delete end;
}

void RangeWeapon::Update()
{
	if (!mesh->Active()) return;
	Weapon::Update();

	start->UpdateWorld();
	end->UpdateWorld();

	if (cooldown < interval)
	{
		cooldown += DELTA;
	}
}

void RangeWeapon::Render()
{
	Weapon::Render();
	start->Render();
	end->Render();
}

void RangeWeapon::GUIRender()
{
	Weapon::GUIRender();
	start->GUIRender();
	end->GUIRender();
}

void RangeWeapon::Shoot(float speed, float lifeTime)
{
	if (cooldown < interval) return;
	cooldown = 0.0f;
	ProjectileManager::Get()->Shoot(projectileName, start->GlobalPos(), (end->GlobalPos() - start->GlobalPos()).GetNormalized(), speed, lifeTime);
}

void RangeWeapon::SetProjectile(string name)
{
	projectileName = mesh->GetName() + "_" + name;
	ProjectileManager::Get()->Add(projectileName, name);
}

void RangeWeapon::Load()
{
	mesh->Load();
	start->Load();
	end->Load();
}