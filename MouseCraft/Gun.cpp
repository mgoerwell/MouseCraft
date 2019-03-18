#include "Gun.h"

#include "Mice.h"
#include "DamageOnCollision.h"
#include "TimedDestruction.h"

Gun::Gun()
{
}


Gun::~Gun()
{
}

bool Gun::use() {
	Contraption::use();

	std::cout << "GUN is being used" << std::endl;

	auto dir = GetEntity()->transform.getWorldForward();
	auto pos = GetEntity()->transform.getWorldPosition() + dir * 5.0f;

	// entity 
	auto e_bullet = EntityManager::Instance().Create();
	e_bullet->t().pos(pos);
	e_bullet->t().face2D(dir);
	e_bullet->t().rotate(glm::vec3(M_PI / 2.0f, 0, 0));

	// visuals
	auto c_render = ComponentManager<Renderable>::Instance().Create<Renderable>();
	c_render->setModel(*ModelLoader::loadModel("res/models/screw.obj"));
	e_bullet->AddComponent(c_render);
	// destroy after lifetime 
	auto c_timed = ComponentManager<UpdatableComponent>::Instance().Create<TimedDestruction>();
	c_timed->delay = BULLET_LIFETIME;
	e_bullet->AddComponent(c_timed);
	// damage + physics ... 
	std::set<PhysObjectType::PhysObjectType> targets;
	PhysObjectType::PhysObjectType floor;
	bool isUp = GetEntity()->GetParent()->GetComponent<PhysicsComponent>()->isUp;
	if (isUp)
	{
		targets.insert(PhysObjectType::CAT_UP);
		targets.insert(PhysObjectType::OBSTACLE_UP);
		floor = PhysObjectType::CONTRAPTION_UP;
	}
	else
	{
		targets.insert(PhysObjectType::CAT_DOWN);
		targets.insert(PhysObjectType::OBSTACLE_DOWN);
		floor = PhysObjectType::CONTRAPTION_DOWN;
	}
	// ... damage on hit 
	auto c_damage = ComponentManager<DamageOnCollision>::Instance().Create<DamageOnCollision>(targets);
	e_bullet->AddComponent(c_damage);
	// ... physics 
	auto c_phys = PhysicsManager::instance()->createObject(pos.x, pos.z, BULLET_SIZE, BULLET_SIZE, 0, floor);
	c_phys->velocity = Vector2D(glm::vec2(dir.x, dir.z) * BULLET_SPEED);
	e_bullet->AddComponent(c_phys);
	
	OmegaEngine::Instance().GetRoot()->AddChild(e_bullet);

	// cleanup
	this->GetEntity()->Destroy();
	
	return true;
}

void Gun::show() {
	Contraption::show();
}