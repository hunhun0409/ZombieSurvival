#pragma once

class CollisionScene : public Scene
{
public:
    CollisionScene();
    ~CollisionScene();

    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUIRender() override;

private:
    QuadTree* qt;
    vector<Collider*> colliders;
    int index = 0;
    bool b = true;
};