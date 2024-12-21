#include "wizardry_summon.hpp"
#include "firebase.hpp"
#include "globals.h"
#include "lightning_spell.hpp"
#include "magic_types.hpp"

WizardrySummon::WizardrySummon(Vector2 postion,
                               std::shared_ptr<SummonCard> card) {
    sendFirebaseEvent("MagicSummon",
                      {{"level", TextFormat("%d", card->level)},
                       {"magic_type", getMagicText(card->magic_type)}});
    if (card->magic_type == MagicType::LIGHTNING_SPELL) {
        std::shared_ptr<LightningSpell> spell =
            std::make_shared<LightningSpell>(card->level, postion.x, postion.y);
        spell->init();
        getContainer()->addGameObject(spell);
    }
}

WizardrySummon::~WizardrySummon() {
}
