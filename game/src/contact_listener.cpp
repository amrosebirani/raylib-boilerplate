#include "box2d/b2_contact.h"
#include "box2d/b2_fixture.h"
#include "collider_user_data.h"
#include "collectible.hpp"
#include "warrior.hpp"
#include "enemy.hpp"
#include "defense_tower.hpp"
#include "castle.hpp"
#include "arrow.hpp"

void MyContactListener::BeginContact(b2Contact *contact) {
    b2Fixture *fixtureA = contact->GetFixtureA();
    b2Fixture *fixtureB = contact->GetFixtureB();

    auto *dataA = reinterpret_cast<ColliderUserData *>(
        fixtureA->GetBody()->GetUserData().pointer);
    auto *dataB = reinterpret_cast<ColliderUserData *>(
        fixtureB->GetBody()->GetUserData().pointer);

    if (dataA && dataB) {
        ColliderUserData::Type typeA = dataA->type;
        ColliderUserData::Type typeB = dataB->type;

        if (dataA->obj == nullptr || dataB->obj == nullptr) {
            return;
        }

        bool collectCondition1 = typeA == ColliderUserData::Type::Warrior &&
                                 typeB == ColliderUserData::Type::Collectable;
        bool collectCondition2 = typeA == ColliderUserData::Type::Collectable &&
                                 typeB == ColliderUserData::Type::Warrior;
        if (collectCondition1 || collectCondition2) {
            std::shared_ptr<Collectible> collectible;
            if (collectCondition1) {
                collectible =
                    std::dynamic_pointer_cast<Collectible>(dataB->obj);
            } else {
                collectible =
                    std::dynamic_pointer_cast<Collectible>(dataA->obj);
            }
            collectible->collect();
            return;
        }

        bool enemyCondition1 = typeA == ColliderUserData::Type::Warrior &&
                               typeB == ColliderUserData::Type::Enemy;
        bool enemyCondition2 = typeA == ColliderUserData::Type::Enemy &&
                               typeB == ColliderUserData::Type::Warrior;

        if (enemyCondition1 || enemyCondition2) {
            std::shared_ptr<Warrior> warrior;
            std::shared_ptr<Enemy> enemy;
            if (enemyCondition1) {
                warrior = std::dynamic_pointer_cast<Warrior>(dataA->obj);
                enemy = std::dynamic_pointer_cast<Enemy>(dataB->obj);
            } else {
                warrior = std::dynamic_pointer_cast<Warrior>(dataB->obj);
                enemy = std::dynamic_pointer_cast<Enemy>(dataA->obj);
            }
            warrior->tryAttack(enemy);
            enemy->tryAttack(warrior);
            return;
        }

        bool castleAttackCondition1 = typeA == ColliderUserData::Type::Enemy &&
                                      typeB == ColliderUserData::Type::Castle;
        bool castleAttackCondition2 = typeA == ColliderUserData::Castle &&
                                      typeB == ColliderUserData::Enemy;
        if (castleAttackCondition1 || castleAttackCondition2) {
            std::shared_ptr<Enemy> enemy;
            std::shared_ptr<Castle> castle;
            if (castleAttackCondition1) {
                enemy = std::dynamic_pointer_cast<Enemy>(dataA->obj);
                castle = std::dynamic_pointer_cast<Castle>(dataB->obj);
            } else {
                enemy = std::dynamic_pointer_cast<Enemy>(dataB->obj);
                castle = std::dynamic_pointer_cast<Castle>(dataA->obj);
            }
            enemy->castleAttack(castle);
            return;
        }

        bool sensorCondition1 = typeA == ColliderUserData::Type::DefenseTower &&
                                typeB == ColliderUserData::Type::Enemy;
        bool sensorCondition2 = typeA == ColliderUserData::Type::Enemy &&
                                typeB == ColliderUserData::Type::DefenseTower;
        if (sensorCondition1 || sensorCondition2) {
            std::shared_ptr<DefenseTower> tower;
            std::shared_ptr<Enemy> enemy;
            if (sensorCondition1) {
                tower = std::dynamic_pointer_cast<DefenseTower>(dataA->obj);
                enemy = std::dynamic_pointer_cast<Enemy>(dataB->obj);
            } else {
                tower = std::dynamic_pointer_cast<DefenseTower>(dataB->obj);
                enemy = std::dynamic_pointer_cast<Enemy>(dataA->obj);
            }
            if (fixtureA->IsSensor() || fixtureB->IsSensor()) {
                // the enemy has entered the tower area
                // add to the queue
                tower->addEnemy(enemy);
            } else {
                // the enemy is attacking the tower
                enemy->towerAttack(tower);
            }
            return;
        }

        bool arrowCondition1 = typeA == ColliderUserData::Type::Enemy &&
                               typeB == ColliderUserData::Type::Projectile;
        bool arrowCondition2 = typeA == ColliderUserData::Type::Projectile &&
                               typeB == ColliderUserData::Type::Enemy;
        if (arrowCondition1 || arrowCondition2) {
            std::shared_ptr<Enemy> enemy;
            std::shared_ptr<Arrow> projectile;
            if (arrowCondition1) {
                enemy = std::dynamic_pointer_cast<Enemy>(dataA->obj);
                projectile = std::dynamic_pointer_cast<Arrow>(dataB->obj);
            } else {
                enemy = std::dynamic_pointer_cast<Enemy>(dataB->obj);
                projectile = std::dynamic_pointer_cast<Arrow>(dataA->obj);
            }
            projectile->die();
            enemy->takeAttack(projectile->getDamage());
            return;
        }
    }
}

void MyContactListener::EndContact(b2Contact *contact) {
}
