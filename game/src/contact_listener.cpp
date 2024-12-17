#include "archer.hpp"
#include "attack_tower.hpp"
#include "box2d/b2_contact.h"
#include "box2d/b2_fixture.h"
#include "castle.hpp"
#include "collider_user_data.h"
#include "collectible.hpp"
#include "fire_ball.hpp"
#include "lightning_tower.hpp"
#include "spell.hpp"
#include "warrior.hpp"
#include "enemy.hpp"
#include "defense_tower.hpp"
#include "arrow.hpp"
// #include <iostream>

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
            warrior->addContactAttack(enemy);
            // enemy->tryAttack(warrior);
            enemy->addContactAttack(warrior);
            return;
        }

        bool eaCondition1 = typeA == ColliderUserData::Type::Archer &&
                            typeB == ColliderUserData::Type::Enemy;
        bool eaCondition2 = typeA == ColliderUserData::Type::Enemy &&
                            typeB == ColliderUserData::Type::Archer;

        if (eaCondition1 || eaCondition2) {
            std::shared_ptr<Archer> archer;
            std::shared_ptr<Enemy> enemy;
            if (eaCondition1) {
                archer = std::dynamic_pointer_cast<Archer>(dataA->obj);
                enemy = std::dynamic_pointer_cast<Enemy>(dataB->obj);
            } else {
                archer = std::dynamic_pointer_cast<Archer>(dataB->obj);
                enemy = std::dynamic_pointer_cast<Enemy>(dataA->obj);
            }
            // warrior->addContactAttack(enemy);
            // enemy->tryAttack(warrior);
            enemy->addContactAttack(archer);
            return;
        }

        bool buildingAttackCondition1 =
            typeA == ColliderUserData::Type::Enemy &&
            typeB == ColliderUserData::Type::Building;
        bool buildingAttackCondition2 = typeA == ColliderUserData::Building &&
                                        typeB == ColliderUserData::Enemy;
        if (buildingAttackCondition1 || buildingAttackCondition2) {
            std::shared_ptr<Enemy> enemy;
            std::shared_ptr<Building> building;
            if (buildingAttackCondition1) {
                enemy = std::dynamic_pointer_cast<Enemy>(dataA->obj);
                building = std::dynamic_pointer_cast<Building>(dataB->obj);
            } else {
                enemy = std::dynamic_pointer_cast<Enemy>(dataB->obj);
                building = std::dynamic_pointer_cast<Building>(dataA->obj);
            }
            // std::cout << "enemy building collision" << std::endl;
            // enemy->buildingAttack(building);
            enemy->addContactAttack(building);
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

        bool atSensorCondition1 =
            typeA == ColliderUserData::Type::AttackTowerSensor &&
            typeB == ColliderUserData::Type::Enemy;
        bool atSensorCondition2 =
            typeA == ColliderUserData::Type::Enemy &&
            typeB == ColliderUserData::Type::AttackTowerSensor;
        if (atSensorCondition2 || atSensorCondition1) {
            std::shared_ptr<AttackTower> atower;
            std::shared_ptr<Enemy> enemy;
            if (atSensorCondition1) {
                atower = std::dynamic_pointer_cast<AttackTower>(dataA->obj);
                enemy = std::dynamic_pointer_cast<Enemy>(dataB->obj);
            } else {
                atower = std::dynamic_pointer_cast<AttackTower>(dataB->obj);
                enemy = std::dynamic_pointer_cast<Enemy>(dataA->obj);
            }
            atower->addEnemy(enemy);
            return;
        }

        bool spSensorCondition1 = typeA == ColliderUserData::Type::Spell &&
                                  typeB == ColliderUserData::Type::Enemy;
        bool spSensorCondition2 = typeA == ColliderUserData::Type::Enemy &&
                                  typeB == ColliderUserData::Type::Spell;
        if (spSensorCondition2 || spSensorCondition1) {
            std::shared_ptr<Spell> spell;
            std::shared_ptr<Enemy> enemy;
            if (spSensorCondition1) {
                spell = std::dynamic_pointer_cast<Spell>(dataA->obj);
                enemy = std::dynamic_pointer_cast<Enemy>(dataB->obj);
            } else {
                spell = std::dynamic_pointer_cast<Spell>(dataB->obj);
                enemy = std::dynamic_pointer_cast<Enemy>(dataA->obj);
            }
            spell->addEnemy(enemy);
            return;
        }

        bool ctSensorCondition1 =
            typeA == ColliderUserData::Type::CastleSensor &&
            typeB == ColliderUserData::Type::Enemy;
        bool ctSensorCondition2 = typeA == ColliderUserData::Type::Enemy &&
                                  typeB == ColliderUserData::Type::CastleSensor;
        if (ctSensorCondition2 || ctSensorCondition1) {
            std::shared_ptr<Castle> castle;
            std::shared_ptr<Enemy> enemy;
            if (ctSensorCondition1) {
                castle = std::dynamic_pointer_cast<Castle>(dataA->obj);
                enemy = std::dynamic_pointer_cast<Enemy>(dataB->obj);
            } else {
                castle = std::dynamic_pointer_cast<Castle>(dataB->obj);
                enemy = std::dynamic_pointer_cast<Enemy>(dataA->obj);
            }
            castle->addEnemy(enemy);
            return;
        }

        bool acSensorCondition1 =
            typeA == ColliderUserData::Type::ArcherSensor &&
            typeB == ColliderUserData::Type::Enemy;
        bool acSensorCondition2 = typeA == ColliderUserData::Type::Enemy &&
                                  typeB == ColliderUserData::Type::ArcherSensor;
        if (acSensorCondition2 || acSensorCondition1) {
            std::shared_ptr<Archer> archer;
            std::shared_ptr<Enemy> enemy;
            if (acSensorCondition1) {
                archer = std::dynamic_pointer_cast<Archer>(dataA->obj);
                enemy = std::dynamic_pointer_cast<Enemy>(dataB->obj);
            } else {
                archer = std::dynamic_pointer_cast<Archer>(dataB->obj);
                enemy = std::dynamic_pointer_cast<Enemy>(dataA->obj);
            }
            archer->addEnemy(enemy);
            return;
        }

        bool ltSensorCondition1 =
            typeA == ColliderUserData::Type::LightningTowerSensor &&
            typeB == ColliderUserData::Type::Enemy;
        bool ltSensorCondition2 =
            typeA == ColliderUserData::Type::Enemy &&
            typeB == ColliderUserData::Type::LightningTowerSensor;
        if (ltSensorCondition2 || ltSensorCondition1) {
            std::shared_ptr<LightningTower> ltower;
            std::shared_ptr<Enemy> enemy;
            if (ltSensorCondition1) {
                ltower = std::dynamic_pointer_cast<LightningTower>(dataA->obj);
                enemy = std::dynamic_pointer_cast<Enemy>(dataB->obj);
            } else {
                ltower = std::dynamic_pointer_cast<LightningTower>(dataB->obj);
                enemy = std::dynamic_pointer_cast<Enemy>(dataA->obj);
            }
            ltower->addEnemy(enemy);
            return;
        }

        bool wSensorCondition1 =
            typeA == ColliderUserData::Type::WarriorSensor &&
            typeB == ColliderUserData::Type::Enemy;
        bool wSensorCondition2 = typeA == ColliderUserData::Type::Enemy &&
                                 typeB == ColliderUserData::Type::WarriorSensor;
        if (wSensorCondition2 || wSensorCondition1) {
            std::shared_ptr<Warrior> warrior;
            std::shared_ptr<Enemy> enemy;
            if (wSensorCondition1) {
                warrior = std::dynamic_pointer_cast<Warrior>(dataA->obj);
                enemy = std::dynamic_pointer_cast<Enemy>(dataB->obj);
            } else {
                warrior = std::dynamic_pointer_cast<Warrior>(dataB->obj);
                enemy = std::dynamic_pointer_cast<Enemy>(dataA->obj);
            }
            warrior->addInRangeEnemyUnit(enemy);
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

        bool fbCondition1 = typeA == ColliderUserData::Type::Enemy &&
                            typeB == ColliderUserData::Type::FireBall;
        bool fbCondition2 = typeA == ColliderUserData::Type::FireBall &&
                            typeB == ColliderUserData::Type::Enemy;
        if (fbCondition1 || fbCondition2) {
            std::shared_ptr<Enemy> enemy;
            std::shared_ptr<FireBall> fireball;
            if (fbCondition1) {
                enemy = std::dynamic_pointer_cast<Enemy>(dataA->obj);
                fireball = std::dynamic_pointer_cast<FireBall>(dataB->obj);
            } else {
                enemy = std::dynamic_pointer_cast<Enemy>(dataB->obj);
                fireball = std::dynamic_pointer_cast<FireBall>(dataA->obj);
            }
            enemy->takeAttack(fireball->getDamage());
            return;
        }

        bool bSensorCondition1 =
            typeA == ColliderUserData::Type::BuildingSensor &&
            typeB == ColliderUserData::Type::Formation;
        bool bSensorCondition2 =
            typeA == ColliderUserData::Type::Formation &&
            typeB == ColliderUserData::Type::BuildingSensor;
        if (bSensorCondition2 || bSensorCondition1) {
            std::shared_ptr<Building> building;
            if (bSensorCondition1) {
                building = std::dynamic_pointer_cast<Building>(dataA->obj);
            } else {
                building = std::dynamic_pointer_cast<Building>(dataB->obj);
            }
            building->startContact();
            // std::cout << "Building in contact" << std::endl;
            return;
        }

        bool sSensorCondition1 =
            typeA == ColliderUserData::Type::SummonSensor &&
            typeB == ColliderUserData::Type::Formation;
        bool sSensorCondition2 = typeA == ColliderUserData::Type::Formation &&
                                 typeB == ColliderUserData::Type::SummonSensor;
        if (sSensorCondition2 || sSensorCondition1) {
            std::shared_ptr<Building> building;
            if (sSensorCondition1) {
                building = std::dynamic_pointer_cast<Building>(dataA->obj);
            } else {
                building = std::dynamic_pointer_cast<Building>(dataB->obj);
            }
            building->startSummonContact();
            // std::cout << "Building in contact" << std::endl;
        }
    }
}

void MyContactListener::EndContact(b2Contact *contact) {
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
        bool bSensorCondition1 =
            typeA == ColliderUserData::Type::BuildingSensor &&
            typeB == ColliderUserData::Type::Formation;
        bool bSensorCondition2 =
            typeA == ColliderUserData::Type::Formation &&
            typeB == ColliderUserData::Type::BuildingSensor;
        if (bSensorCondition2 || bSensorCondition1) {
            std::shared_ptr<Building> building;
            if (bSensorCondition1) {
                building = std::dynamic_pointer_cast<Building>(dataA->obj);
            } else {
                building = std::dynamic_pointer_cast<Building>(dataB->obj);
            }
            building->endContact();
            // std::cout << "Building not in contact" << std::endl;
            return;
        }

        bool sSensorCondition1 =
            typeA == ColliderUserData::Type::SummonSensor &&
            typeB == ColliderUserData::Type::Formation;
        bool sSensorCondition2 = typeA == ColliderUserData::Type::Formation &&
                                 typeB == ColliderUserData::Type::SummonSensor;
        if (sSensorCondition2 || sSensorCondition1) {
            std::shared_ptr<Building> building;
            if (sSensorCondition1) {
                building = std::dynamic_pointer_cast<Building>(dataA->obj);
            } else {
                building = std::dynamic_pointer_cast<Building>(dataB->obj);
            }
            building->endSummonContact();
            // std::cout << "Building in contact" << std::endl;
            return;
        }

        bool buildingAttackCondition1 =
            typeA == ColliderUserData::Type::Enemy &&
            typeB == ColliderUserData::Type::Building;
        bool buildingAttackCondition2 = typeA == ColliderUserData::Building &&
                                        typeB == ColliderUserData::Enemy;
        if (buildingAttackCondition1 || buildingAttackCondition2) {
            std::shared_ptr<Enemy> enemy;
            std::shared_ptr<Building> building;
            if (buildingAttackCondition1) {
                enemy = std::dynamic_pointer_cast<Enemy>(dataA->obj);
                building = std::dynamic_pointer_cast<Building>(dataB->obj);
            } else {
                enemy = std::dynamic_pointer_cast<Enemy>(dataB->obj);
                building = std::dynamic_pointer_cast<Building>(dataA->obj);
            }
            // std::cout << "enemy building collision" << std::endl;
            // enemy->buildingAttack(building);
            enemy->removeContactAttack(building);
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
            warrior->removeContactAttack(enemy);
            // enemy->tryAttack(warrior);
            enemy->removeContactAttack(warrior);
            return;
        }

        bool eaCondition1 = typeA == ColliderUserData::Type::Archer &&
                            typeB == ColliderUserData::Type::Enemy;
        bool eaCondition2 = typeA == ColliderUserData::Type::Enemy &&
                            typeB == ColliderUserData::Type::Archer;

        if (eaCondition1 || eaCondition2) {
            std::shared_ptr<Archer> archer;
            std::shared_ptr<Enemy> enemy;
            if (eaCondition1) {
                archer = std::dynamic_pointer_cast<Archer>(dataA->obj);
                enemy = std::dynamic_pointer_cast<Enemy>(dataB->obj);
            } else {
                archer = std::dynamic_pointer_cast<Archer>(dataB->obj);
                enemy = std::dynamic_pointer_cast<Enemy>(dataA->obj);
            }
            // archer->removeContactAttack(enemy);
            // enemy->tryAttack(warrior);
            enemy->removeContactAttack(archer);
            return;
        }

        bool ltSensorCondition1 =
            typeA == ColliderUserData::Type::LightningTowerSensor &&
            typeB == ColliderUserData::Type::Enemy;
        bool ltSensorCondition2 =
            typeA == ColliderUserData::Type::Enemy &&
            typeB == ColliderUserData::Type::LightningTowerSensor;
        if (ltSensorCondition2 || ltSensorCondition1) {
            std::shared_ptr<LightningTower> ltower;
            std::shared_ptr<Enemy> enemy;
            if (ltSensorCondition1) {
                ltower = std::dynamic_pointer_cast<LightningTower>(dataA->obj);
                enemy = std::dynamic_pointer_cast<Enemy>(dataB->obj);
            } else {
                ltower = std::dynamic_pointer_cast<LightningTower>(dataB->obj);
                enemy = std::dynamic_pointer_cast<Enemy>(dataA->obj);
            }
            ltower->removeEnemy(enemy);
            return;
        }

        bool wSensorCondition1 =
            typeA == ColliderUserData::Type::WarriorSensor &&
            typeB == ColliderUserData::Type::Enemy;
        bool wSensorCondition2 = typeA == ColliderUserData::Type::Enemy &&
                                 typeB == ColliderUserData::Type::WarriorSensor;
        if (wSensorCondition2 || wSensorCondition1) {
            std::shared_ptr<Warrior> warrior;
            std::shared_ptr<Enemy> enemy;
            if (wSensorCondition1) {
                warrior = std::dynamic_pointer_cast<Warrior>(dataA->obj);
                enemy = std::dynamic_pointer_cast<Enemy>(dataB->obj);
            } else {
                warrior = std::dynamic_pointer_cast<Warrior>(dataB->obj);
                enemy = std::dynamic_pointer_cast<Enemy>(dataA->obj);
            }
            warrior->removeInRangeEnemyUnit(enemy);
            return;
        }
    }
}
