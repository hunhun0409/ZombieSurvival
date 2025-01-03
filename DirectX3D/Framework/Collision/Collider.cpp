#include "Framework.h"

bool Collider::isRender = true;

Collider::Collider() : GameObject(L"Basic/Collider.hlsl")
{
    mesh = new Mesh<Vertex>();   

    SetColor(0, 1, 0);
}

Collider::~Collider()
{
    delete mesh;
}

void Collider::Render()
{
    if (!Active()) return;
    if (!isRender) return;

    SetRender();

    mesh->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

bool Collider::IsCollision(Collider* collider)
{
    if (!Active()) return false;
    if (!collider->Active()) return false;

    switch (collider->type)
    {
    case Collider::Type::BOX:
        return IsBoxCollision((BoxCollider*)collider);
    case Collider::Type::SPHERE:
        return IsSphereCollision((SphereCollider*)collider);
    case Collider::Type::CAPSULE:
        return IsCapsuleCollision((CapsuleCollider*)collider);    
    }

    return false;
}

bool Collider::PushCollision(Collider* collider)
{
    if (!IsCollision(collider)) return false;

    Vector3 dir = (collider->GlobalPos() - GlobalPos()).GetNormalized();

    collider->GetParent()->Pos() += dir * PUSH_SPEED * DELTA;

    return true;
}

void Collider::OnCollision(Collider* other)
{
    if (owner == nullptr) return;

    Projectile* projectile = dynamic_cast<Projectile*>(owner);
    if (projectile != nullptr)
    {
        projectile->OnCollision(other);
        return;
    }

    MeleeWeapon* sword = dynamic_cast<MeleeWeapon*>(owner);
    if (sword != nullptr)
    {
        sword->OnCollision(other);
        return;
    }
}

void Collider::SetActive(bool active)
{
    Transform::SetActive(active);
    ColliderManager::Get()->UpdateColliderState(this);
}
