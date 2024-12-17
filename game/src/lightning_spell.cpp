#include "lightning_spell.hpp"
#include "collider_factory.hpp"
#include "constants.h"
#include "enemy.hpp"
#include "globals.h"
#include "lightning_line.hpp"
#include "magic_types.hpp"

LightningSpell::LightningSpell(int level, float x, float y)
    : Spell(level, MagicType::LIGHTNING_SPELL, x, y) {
}

void LightningSpell::execute() {
    int total_hit = 0;
    int enemies_to_hit = get_max_enemy_count(MagicType::LIGHTNING_SPELL, level);
    int damage = get_spell_damage(MagicType::LIGHTNING_SPELL, level);
    for (auto &enemy : enemies) {
        std::shared_ptr<Enemy> enemy_ptr =
            std::dynamic_pointer_cast<Enemy>(enemy);
        if (enemy->isAlive()) {
            std::shared_ptr<LightningLine> ll =
                std::make_shared<LightningLine>(x, y, enemy->x, enemy->y);
            getContainer()->addGameObject(ll);
            enemy_ptr->takeAttack(damage);
            total_hit++;
        }
        if (total_hit >= enemies_to_hit) {
            break;
        }
    }
    if (total_hit > 0) {
        getAudioManager()->playSound("thunder");
    }
}

void LightningSpell::init() {
    sdata = new ColliderUserData();
    sdata->obj = get_shared_ptr();
    sdata->type = ColliderUserData::Type::Spell;
    enemySensor = ColliderFactory::newCircleSensor(
        sdata, x, y, get_spell_range(MagicType::LIGHTNING_SPELL, level),
        b2_staticBody, CATEGORY_BUILDING_SENSOR, CATEGORY_ENEMY,
        getContainer()->getWorld());
}

void LightningSpell::draw() {
}

bool LightningSpell::isAlive() {
    return alive;
}

void LightningSpell::update(float dt) {
    if (!alive) {
        return;
    }
    updateCalled++;
    if (updateCalled == 1) {
        return;
    }
    if (updateCalled > 1) {
        execute();
        die();
    }
}

void LightningSpell::cleanupData() {
    sdata->obj = nullptr;
}

void LightningSpell::die() {
    alive = false;
    enemies.clear();
}

LightningSpell::~LightningSpell() {
    delete sdata;
    enemySensor = nullptr;
}

void LightningSpell::addEnemy(std::shared_ptr<GameObject> enemy) {
    if (!enemy->isAlive()) {
        return;
    }
    enemies.push_back(enemy);
}
