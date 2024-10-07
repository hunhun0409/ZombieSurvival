#include "Framework.h"


Projectile::Projectile(string name, Transform* transform)
    :name(name), transform(transform)
{
    transform->SetTag(name);
    transform->Load();

    collider = new CapsuleCollider();
    collider->SetTag(name + "_Collider");
    collider->SetParent(transform);
    collider->Load();
    collider->Update();
    
    start = new SphereCollider();
    start->SetTag(name + "_start");
    start->SetParent(transform);
    start->Pos().x = collider->Radius();
    start->UpdateWorld();

    end = new SphereCollider();
    end->SetTag(name + "_end");
    end->SetParent(transform);
    end->Pos().x = -collider->Radius();
    start->UpdateWorld();

    trail = new Trail(L"Textures/Effect/trail.png", start, end, 10, 50.0f);
    
}

Projectile::~Projectile()
{
	delete collider;

    delete start;
    delete end;
    delete trail;
}

void Projectile::Update()
{
    if (!transform->Active()) return;

    curLifeTime += DELTA;

    if (curLifeTime >= maxLifeTime)
    {
        transform->SetActive(false);
    }

    transform->Pos() += transform->Forward() * speed * DELTA;
    transform->UpdateWorld();

    collider->Update();

    start->UpdateWorld();
    end->UpdateWorld();

    trail->Update();
}

void Projectile::Render()
{
    collider->Render();
    trail->Render();
}

void Projectile::GUIRender()
{
    if (!transform->Active()) return;

    transform->GUIRender();
    collider->GUIRender();
    trail->GUIRender();
}

void Projectile::Shoot(Vector3 startPos, Vector3 dir, float speed, float maxLifeTime)
{
    transform->SetActive(true);
    
    this->maxLifeTime = maxLifeTime;
    this->speed = speed;
    
    curLifeTime = 0.0f;
    transform->Pos() = startPos;
    transform->UpdateWorld();
    start->UpdateWorld();
    end->UpdateWorld();
    trail->Reset();

    dir.y *= -1;
    direction = dir.GetNormalized();

    float Yangle = atan2f(dir.x, dir.z);  // Y�� ȸ�� ���� (Yaw)
    float Xangle = atan2f(dir.y, sqrtf(dir.x * dir.x + dir.z * dir.z)); // X�� ȸ�� ���� (Pitch)

    // localRotation�� ������ �������� ����
    transform->Rot().x = Xangle;  // Pitch (���� ȸ��)
    transform->Rot().y = Yangle;  // Yaw (�¿� ȸ��)
}