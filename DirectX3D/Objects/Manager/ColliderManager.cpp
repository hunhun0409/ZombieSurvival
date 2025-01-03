#include "Framework.h"

void ColliderManager::Update()
{
	if (KEY_DOWN(VK_TAB))
	{
		quadtreeCheck = !quadtreeCheck;
	}

	checkInterval += DELTA;
	if (checkInterval >= CHECK_INTERVAL)
	{
		quadTree->Update();
		CheckCollisions();

		checkInterval -= CHECK_INTERVAL;
	}
	
}

void ColliderManager::Render()
{
	quadTree->Render();
}

void ColliderManager::GUIRender()
{
	int count = totalColliders.size();

	quadTree->GUIRender();
	ImGui::Text("Collision Check Count : %d", collisionCheckCount);
}

void ColliderManager::Add(Collider* collider)
{
	if (totalColliders.count(collider) != 0) return;
	totalColliders[collider] = collider->Active();
	if (collider->Active())
	{
		quadTree->Insert(collider);
	}
}

void ColliderManager::Remove(Collider* collider)
{
	if (quadTree == nullptr) return;
	quadTree->Remove(collider);
	totalColliders.erase(collider);
}

void ColliderManager::UpdateColliderState(Collider* collider)
{
	if (totalColliders.count(collider) == 0) return;
	totalColliders[collider] = collider->Active();

	if (quadTree == nullptr) return;

	if (collider->Active())
	{
		quadTree->Insert(collider);
	}
	else
	{
		quadTree->Delete(collider);
	}
}

void ColliderManager::Clear()
{
	quadTree->Clear();
	totalColliders.clear();
}

void ColliderManager::CheckCollisions()
{
	collisionCheckCount = 0;
	if (quadtreeCheck)
	{
		for (pair<Collider*, bool> pair : totalColliders)
		{
			if (pair.second = false) continue;
			
			Collider* collider = pair.first;
			vector<Collider*> potentialColliders = 
				quadTree->GetPotentialColliders(collider);
			
			for (Collider* other : potentialColliders)
			{
				if (!other->Active() || other == collider)continue;

				collisionCheckCount++;

				if (collider->IsCollision(other))
				{
					HandleCollision(collider, other);
				}
			}
		}
	}
	else
	{
		for (pair<Collider*, bool> pair1 : totalColliders)
		{
			if (pair1.second = false) continue;
			Collider* collider1 = pair1.first;

			for (pair<Collider*, bool> pair2 : totalColliders)
			{
				if (pair2.second = false) continue;
				Collider* collider2 = pair2.first;

				collisionCheckCount++;

				if (collider1->IsCollision(collider2))
				{
					HandleCollision(collider1, collider2);
				}
			}
		}
	}
}

void ColliderManager::HandleCollision(Collider* a, Collider* b)
{
	a->OnCollision(b);
	b->OnCollision(a);
}
