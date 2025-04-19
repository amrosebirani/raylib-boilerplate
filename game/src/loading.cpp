#include "loading.h"
#include "constants.h"
#include "firebase.hpp"
#include "game.h"
#include "globals.h"
#include "raylib.h"
#include "sprite_holder.hpp"
#include "utils.h"

void initBuildingSprites() {
    getSpriteHolder()->initSprites({
        new SpriteConfig(
            ARCHERY1_SPRITE_ID,
            getAssetPath("textures/buildings/archery/archery1.png"), 0, 0),

        new SpriteConfig(
            ARCHERY2_SPRITE_ID,
            getAssetPath("textures/buildings/archery/archery2.png"), 0, 0),
        new SpriteConfig(
            ARCHERY3_SPRITE_ID,
            getAssetPath("textures/buildings/archery/archery3.png"), 0, 0),
        new SpriteConfig(
            BARRACKS1_SPRITE_ID,
            getAssetPath("textures/buildings/barracks/barrack1.png"), 0, 0),
        new SpriteConfig(
            BARRACKS2_SPRITE_ID,
            getAssetPath("textures/buildings/barracks/barrack2.png"), 0, 0),
        new SpriteConfig(
            BARRACKS3_SPRITE_ID,
            getAssetPath("textures/buildings/barracks/barrack3.png"), 0, 0),
        new SpriteConfig(
            BARRACKS4_SPRITE_ID,
            getAssetPath("textures/buildings/barracks/barrack4.png"), 0, 0),
        new SpriteConfig(CASTLE2_SPRITE_ID,
                         getAssetPath("textures/buildings/castle/castle2.png"),
                         0, 0),
        new SpriteConfig(CASTLE1_SPRITE_ID,
                         getAssetPath("textures/buildings/castle/castle1.png"),
                         0, 0),
        new SpriteConfig(CASTLE3_SPRITE_ID,
                         getAssetPath("textures/buildings/castle/castle3.png"),
                         0, 0),
        new SpriteConfig(CASTLE4_SPRITE_ID,
                         getAssetPath("textures/buildings/castle/castle4.png"),
                         0, 0),
        new SpriteConfig(HOUSE1_SPRITE_ID,
                         getAssetPath("textures/buildings/houses/house1.png"),
                         0, 0),
        new SpriteConfig(HOUSE2_SPRITE_ID,
                         getAssetPath("textures/buildings/houses/house2.png"),
                         0, 0),
        new SpriteConfig(HOUSE3_SPRITE_ID,
                         getAssetPath("textures/buildings/houses/house3.png"),
                         0, 0),
        new SpriteConfig(HOUSE4_SPRITE_ID,
                         getAssetPath("textures/buildings/houses/house4.png"),
                         0, 0),
        new SpriteConfig(
            WIZARDRY1_SPRITE_ID,
            getAssetPath("textures/buildings/wizardry/wizardry1.png"), 0, 0),
        new SpriteConfig(
            WIZARDRY2_SPRITE_ID,
            getAssetPath("textures/buildings/wizardry/wizardry2.png"), 0, 0),
        new SpriteConfig(
            WIZARDRY3_SPRITE_ID,
            getAssetPath("textures/buildings/wizardry/wizardry3.png"), 0, 0),
        new SpriteConfig(
            LIGHTNING_TOWER_SPRITE_ID,
            getAssetPath("textures/buildings/towers/lightning_tower.png"), 0,
            0),
        new SpriteConfig(
            BLOCK_TOWER1_SPRITE_ID,
            getAssetPath("textures/buildings/defence_blocks/block_04a.png"), 0,
            0),
        new SpriteConfig(
            BLOCK_TOWER2_SPRITE_ID,
            getAssetPath("textures/buildings/defence_blocks/block_06a.png"), 0,
            0),

    });
}

void initWarriorSprites() {
    getSpriteHolder()->initSprites({

        new SpriteConfig(ARCHER_ATTACKS_SPRITE_ID,
                         getAssetPath("textures/archer_all_roe_attack.png"),
                         160, 160),
        new SpriteConfig(ARCHER_BUST_SPRITE_ID,
                         getAssetPath("textures/archer_all_roe_bust.png"), 0,
                         0),
        new SpriteConfig(ARCHER_IDLE_BOW_SPRITE_ID,
                         getAssetPath("textures/archer_all_roe_idle_bow.png"),
                         160, 160),
        new SpriteConfig(ARCHER_IDLE_SPRITE_ID,
                         getAssetPath("textures/archer_all_ROE_Idle2.png"), 160,
                         160),
        // add for crossbowman
        new SpriteConfig(
            CROSSBOWMAN_ATTACKS_SPRITE_ID,
            getAssetPath("textures/crossbowman/crossbowman_attack.png"), 128,
            128),
        new SpriteConfig(
            CROSSBOWMAN_BUST_SPRITE_ID,
            getAssetPath("textures/crossbowman/crossbowman_bust.png"), 0, 0),
        new SpriteConfig(
            CROSSBOWMAN_IDLE_SPRITE_ID,
            getAssetPath("textures/crossbowman/crossbowman_idle.png"), 128,
            128),
        new SpriteConfig(
            CROSSBOWMAN_PAPERDOLL_SPRITE_ID,
            getAssetPath("textures/crossbowman/crossbowman_paperdoll.png"), 0,
            0),
        new SpriteConfig(
            CROSSBOWMAN_DEAD_SPRITE_ID,
            getAssetPath("textures/crossbowman/crossbowman_dead.png"), 128,
            128),
        new SpriteConfig(
            CROSSBOWMAN_RUN_SPRITE_ID,
            getAssetPath("textures/crossbowman/crossbowman_run.png"), 128, 128),
        // add for javeliner
        new SpriteConfig(
            JAVELINER_ATTACKS_SPRITE_ID,
            getAssetPath("textures/javeliner/javeliner_attack.png"), 128, 128),
        new SpriteConfig(JAVELINER_BUST_SPRITE_ID,
                         getAssetPath("textures/javeliner/javeliner_bust.png"),
                         0, 0),
        new SpriteConfig(JAVELINER_IDLE_SPRITE_ID,
                         getAssetPath("textures/javeliner/javeliner_idle.png"),
                         128, 128),
        new SpriteConfig(
            JAVELINER_PAPERDOLL_SPRITE_ID,
            getAssetPath("textures/javeliner/javeliner_paperdoll.png"), 0, 0),
        new SpriteConfig(JAVELINER_DEAD_SPRITE_ID,
                         getAssetPath("textures/javeliner/javeliner_dead.png"),
                         128, 128),
        new SpriteConfig(JAVELINER_RUN_SPRITE_ID,
                         getAssetPath("textures/javeliner/javeliner_run.png"),
                         128, 128),
        //
        new SpriteConfig(FOOTMAN_IDLE_SPRITE_ID,
                         getAssetPath("textures/footman/footman_idle.png"), 160,
                         160),
        new SpriteConfig(FOOTMAN_ATTACKS_SPRITE_ID,
                         getAssetPath("textures/footman/footman_attack.png"),
                         160, 160),
        new SpriteConfig(FOOTMAN_BUST_SPRITE_ID,
                         getAssetPath("textures/footman/footman_bust.png"), 0,
                         0),
        new SpriteConfig(FOOTMAN_RUN_SPRITE_ID,
                         getAssetPath("textures/footman/footman_run.png"), 160,
                         160),
        new SpriteConfig(FOOTMAN_PAPERDOLL_SPRITE_ID,
                         getAssetPath("textures/footman/footman_paperdoll.png"),
                         0, 0),
        new SpriteConfig(FOOTMAN_DEAD_SPRITE_ID,
                         getAssetPath("textures/footman/footman_dead.png"), 160,
                         160),
        // for swordsman, similar to footman
        new SpriteConfig(SWORDSMAN_IDLE_SPRITE_ID,
                         getAssetPath("textures/swordsman/swordsman_idle.png"),
                         160, 160),
        new SpriteConfig(
            SWORDSMAN_ATTACKS_SPRITE_ID,
            getAssetPath("textures/swordsman/swordsman_attack.png"), 160, 160),
        new SpriteConfig(SWORDSMAN_BUST_SPRITE_ID,
                         getAssetPath("textures/swordsman/swordsman_bust.png"),
                         0, 0),
        new SpriteConfig(SWORDSMAN_RUN_SPRITE_ID,
                         getAssetPath("textures/swordsman/swordsman_run.png"),
                         160, 160),
        new SpriteConfig(
            SWORDSMAN_PAPERDOLL_SPRITE_ID,
            getAssetPath("textures/swordsman/swordsman_paperdoll.png"), 0, 0),
        new SpriteConfig(SWORDSMAN_DEAD_SPRITE_ID,
                         getAssetPath("textures/swordsman/swordsman_dead.png"),
                         160, 160),
        // for axeman, give all 6 lines at once
        new SpriteConfig(AXEMAN_IDLE_SPRITE_ID,
                         getAssetPath("textures/axeman/axeman_idle.png"), 160,
                         160),
        new SpriteConfig(AXEMAN_ATTACKS_SPRITE_ID,
                         getAssetPath("textures/axeman/axeman_attack.png"), 160,
                         160),
        new SpriteConfig(AXEMAN_BUST_SPRITE_ID,
                         getAssetPath("textures/axeman/axeman_bust.png"), 0, 0),
        new SpriteConfig(AXEMAN_RUN_SPRITE_ID,
                         getAssetPath("textures/axeman/axeman_run.png"), 160,
                         160),
        new SpriteConfig(AXEMAN_PAPERDOLL_SPRITE_ID,
                         getAssetPath("textures/axeman/axeman_paperdoll.png"),
                         0, 0),
        new SpriteConfig(AXEMAN_DEAD_SPRITE_ID,
                         getAssetPath("textures/axeman/axeman_dead.png"), 160,
                         160),
        // for berserker
        new SpriteConfig(BERSERKER_IDLE_SPRITE_ID,
                         getAssetPath("textures/berserker/berserker_idle.png"),
                         160, 160),
        new SpriteConfig(
            BERSERKER_ATTACKS_SPRITE_ID,
            getAssetPath("textures/berserker/berserker_attack.png"), 160, 160),
        new SpriteConfig(BERSERKER_BUST_SPRITE_ID,
                         getAssetPath("textures/berserker/berserker_bust.png"),
                         0, 0),
        new SpriteConfig(BERSERKER_RUN_SPRITE_ID,
                         getAssetPath("textures/berserker/berserker_run.png"),
                         160, 160),
        new SpriteConfig(
            BERSERKER_PAPERDOLL_SPRITE_ID,
            getAssetPath("textures/berserker/berserker_paperdoll.png"), 0, 0),
        new SpriteConfig(BERSERKER_DEAD_SPRITE_ID,
                         getAssetPath("textures/berserker/berserker_dead.png"),
                         160, 160),
        //
        new SpriteConfig(SPEARMAN_IDLE_SPRITE_ID,
                         getAssetPath("textures/spearman/spearman_idle.png"),
                         128, 128),
        new SpriteConfig(SPEARMAN_ATTACKS_SPRITE_ID,
                         getAssetPath("textures/spearman/spearman_attack.png"),
                         128, 128),
        new SpriteConfig(SPEARMAN_BUST_SPRITE_ID,
                         getAssetPath("textures/spearman/spearman_bust.png"), 0,
                         0),
        new SpriteConfig(SPEARMAN_RUN_SPRITE_ID,
                         getAssetPath("textures/spearman/spearman_run.png"),
                         128, 128),
        new SpriteConfig(
            SPEARMAN_PAPERDOLL_SPRITE_ID,
            getAssetPath("textures/spearman/spearman_paperdoll.png"), 0, 0),
        new SpriteConfig(SPEARMAN_DEAD_SPRITE_ID,
                         getAssetPath("textures/spearman/spearman_dead.png"),
                         128, 128),
        new SpriteConfig(SPEARMAN_KO_SPRITE_ID,
                         getAssetPath("textures/spearman/spearman_ko.png"), 128,
                         128),
        new SpriteConfig(KNIGHT_IDLE_SPRITE_ID,
                         getAssetPath("textures/knight/knight_idle.png"), 160,
                         160),
        new SpriteConfig(KNIGHT_ATTACKS_SPRITE_ID,
                         getAssetPath("textures/knight/knight_attack.png"), 160,
                         160),
        new SpriteConfig(KNIGHT_BUST_SPRITE_ID,
                         getAssetPath("textures/knight/knight_bust.png"), 0, 0),
        new SpriteConfig(KNIGHT_RUN_SPRITE_ID,
                         getAssetPath("textures/knight/knight_run.png"), 160,
                         160),
        new SpriteConfig(KNIGHT_PAPERDOLL_SPRITE_ID,
                         getAssetPath("textures/knight/knight_paperdoll.png"),
                         0, 0),
        new SpriteConfig(KNIGHT_DEAD_SPRITE_ID,
                         getAssetPath("textures/knight/knight_dead.png"), 160,
                         160),
        new SpriteConfig(MAGE_IDLE_SPRITE_ID,
                         getAssetPath("textures/mage/mage_idle.png"), 160, 160),
        new SpriteConfig(MAGE_ATTACKS_SPRITE_ID,
                         getAssetPath("textures/mage/mage_attack.png"), 160,
                         160),
        new SpriteConfig(MAGE_BUST_SPRITE_ID,
                         getAssetPath("textures/mage/mage_bust.png"), 0, 0),
        new SpriteConfig(MAGE_RUN_SPRITE_ID,
                         getAssetPath("textures/mage/mage_run.png"), 160, 160),
        new SpriteConfig(MAGE_PAPERDOLL_SPRITE_ID,
                         getAssetPath("textures/mage/mage_paperdoll.png"), 0,
                         0),
        new SpriteConfig(MAGE_DEAD_SPRITE_ID,
                         getAssetPath("textures/mage/mage_dead.png"), 160, 160),
    });
}

void initTreeSprites() {
    getSpriteHolder()->initSprites({
        new SpriteConfig(TREE1_SPRITE_ID, getAssetPath("textures/tree_02.png"),
                         0, 0),
        new SpriteConfig(TREE2_SPRITE_ID, getAssetPath("textures/tree_03.png"),
                         0, 0),
        new SpriteConfig(TREE3_SPRITE_ID, getAssetPath("textures/tree_04.png"),
                         0, 0),
        new SpriteConfig(TREE4_SPRITE_ID, getAssetPath("textures/tree_05.png"),
                         0, 0),
        new SpriteConfig(TREE5_SPRITE_ID, getAssetPath("textures/tree_06.png"),
                         0, 0),
        new SpriteConfig(TREE6_SPRITE_ID, getAssetPath("textures/tree_08.png"),
                         0, 0),
        new SpriteConfig(TREE7_SPRITE_ID, getAssetPath("textures/tree_09.png"),
                         0, 0),
    });
}

int getSpriteGridMetric(std::string enemyId) {
    if (enemyId == "rat" || enemyId == "zombie_giant" ||
        enemyId == "skeleton_prince") {
        return 256;
    }
    if (enemyId == "wolf") {
        return 96;
    }
    return 128;
}

void initEnemySprites() {
    std::vector<std::string> enemyIds2 = {
        "ghoul",        "necromancer", "skeleton", "skeleton_prince", "zombie1",
        "zombie2",      "zombie3",     "zombie4",  "zombie5",         "zombie6",
        "zombie_giant", "buck",        "rat",      "spider",          "wolf"};
    for (auto id : enemyIds2) {
        std::vector<SpriteConfig *> spriteConfigs;
        std::vector<std::string> spriteIds = {"attack", "move", "idle", "dead"};
        int metric = getSpriteGridMetric(id);
        for (auto spriteId : spriteIds) {
            std::string path = "textures/enemy/" + id + "/" + spriteId + ".png";
            spriteConfigs.push_back(new SpriteConfig(id + "_" + spriteId,
                                                     getAssetPath(path.c_str()),
                                                     metric, metric));
        }
        spriteConfigs.push_back(new SpriteConfig(
            id + "_bust",
            getAssetPath(("textures/enemy/" + id + "/bust.png").c_str()), 0,
            0));
        getSpriteHolder()->initSprites(spriteConfigs);
    }
    getSpriteHolder()->initSprites({
        new SpriteConfig(ENEMY1_BUST_SPRITE_ID,
                         getAssetPath("textures/enemy1/enemy1_bust.png"), 0, 0),
        new SpriteConfig(ENEMY1_ATTACKS_SPRITE_ID,
                         getAssetPath("textures/enemy1/enemy1_attack.png"), 128,
                         128),
        new SpriteConfig(ENEMY1_RUN_SPRITE_ID,
                         getAssetPath("textures/enemy1/enemy1_run.png"), 128,
                         128),
        new SpriteConfig(ENEMY1_DEAD_SPRITE_ID,
                         getAssetPath("textures/enemy1/enemy1_dead.png"), 128,
                         128),
        new SpriteConfig(ENEMY1_PAPERDOLL_SPRITE_ID,
                         getAssetPath("textures/enemy1/enemy1_paperdoll.png"),
                         0, 0),
    });
}

void initSprites() {

    getSpriteHolder()->initSprites({
        new SpriteConfig(BOW_SPRITE_ID, getAssetPath("textures/bow.png"), 128,
                         128),
        new SpriteConfig(ARROW_SPRITE_ID, getAssetPath("textures/arrow.png"), 0,
                         0),
        new SpriteConfig(CASTLE_SPRITE_ID,
                         getAssetPath("textures/castlekeep_14.png"), 0, 0),
        new SpriteConfig(TERRAIN_OP_SPRITE_ID,
                         getAssetPath("textures/ground_02_edit.png"), 0, 0),
        new SpriteConfig(TOWER_SPRITE_ID, getAssetPath("textures/tower.png"), 0,
                         0),
        new SpriteConfig(COINS_AND_GEMS,
                         getAssetPath("textures/combined_gems.png"), 16, 16),
        new SpriteConfig(INFANTRY_ICON,
                         getAssetPath("textures/icons/infantry.png"), 0, 0),
        new SpriteConfig(ARCHERY_ICON,
                         getAssetPath("textures/icons/archers.png"), 0, 0),
        new SpriteConfig(WIZARDRY_ICON,
                         getAssetPath("textures/icons/magic.png"), 0, 0),
        new SpriteConfig(UI_ICONS, getAssetPath("textures/ui-icons.png"), 48,
                         48),
        new SpriteConfig(PANEL_SPRITE_ID,
                         getAssetPath("textures/gradient_panel.png"), 3, 3),
        new SpriteConfig(PANEL_BG, getAssetPath("textures/powersave_bg.png"), 0,
                         0),
        new SpriteConfig(CHARACTER_FRAME_SPRITE_ID,
                         getAssetPath("textures/play_character_frame.png"), 0,
                         0),
        new SpriteConfig(LIGHTNING_ICON,
                         getAssetPath("textures/lightning_icon.png"), 0, 0),
        new SpriteConfig(GOLD_ICON, getAssetPath("textures/icon_gold.png"), 0,
                         0),
        new SpriteConfig(TOWER_ICON, getAssetPath("textures/icon_tower.png"), 0,
                         0),
        new SpriteConfig(LTOWER_ICON, getAssetPath("textures/icon_thunder.png"),
                         0, 0),
        new SpriteConfig(BARRACKS_ICON,
                         getAssetPath("textures/icon_barracks.png"), 0, 0),
        new SpriteConfig(ARCHERYB_ICON,
                         getAssetPath("textures/icon_archery.png"), 0, 0),
        new SpriteConfig(WIZARDRYB_ICON,
                         getAssetPath("textures/icon_wizardry.png"), 0, 0),
        new SpriteConfig(HOUSE_ICON, getAssetPath("textures/icon_house.png"), 0,
                         0),
        new SpriteConfig(CASTLE_ICON, getAssetPath("textures/icon_castle.png"),
                         0, 0),
        new SpriteConfig(UPG_ICON, getAssetPath("textures/icon_arrow_up.png"),
                         0, 0),
        new SpriteConfig(ARROW_BOTTOM,
                         getAssetPath("textures/icon_arrow_bottom.png"), 0, 0),
        new SpriteConfig(UPGRADE_CARD,
                         getAssetPath("textures/upgrade_card.png"), 0, 0),
        new SpriteConfig(ARROW_GO_ICON,
                         getAssetPath("textures/icon_arrow_go.png"), 0, 0),
        new SpriteConfig(ARROW_PREV, getAssetPath("textures/arrow_prev.png"), 0,
                         0),
        new SpriteConfig(ARROW_NEXT, getAssetPath("textures/arrow_next.png"), 0,
                         0),
        new SpriteConfig(BUTTON_ICON, getAssetPath("textures/icon_button.png"),
                         0, 0),
        new SpriteConfig(CLEAR_ICON, getAssetPath("textures/icon_clear.png"), 0,
                         0),
        new SpriteConfig(FIRE_BURNING_SPRITE_ID,
                         getAssetPath("textures/fire_burn_sprites.png"), 160,
                         160),
        new SpriteConfig(FIRE_BALL_SPRITE_ID,
                         getAssetPath("textures/fire_ball.png"), 36, 16),
        new SpriteConfig(WARRIOR_ICONS,
                         getAssetPath("textures/warrior_icons.png"), 256, 256),
        new SpriteConfig(SUMMON_BUTTON,
                         getAssetPath("textures/summon_circle.png"), 0, 0),
        new SpriteConfig(SUMMON_CARD_FRAME,
                         getAssetPath("textures/summon_card_frame.png"), 0, 0),
    });
}

Loading::Loading() {
    sendFirebaseEvent("onLoadingScreen", {});
    loadingText = "Loading Resources";
    loadingL = LoadTexture(getAssetPath("textures/backgrounds/loading_l.png"));
    loadingP = LoadTexture(getAssetPath("textures/backgrounds/loading_p.png"));
    bsh = GetScreenHeight() * 5.0 / 6;
    fs = GetScreenHeight() / 30.0f;
}

bool Loading::update(float dt) {
    if (finished) {
        return false;
    }
    if (firstTime) {
        firstTime = false;

        loadingText = "Spawning Castles & Buildings!";
        return false;
    }
    if (!buildingSpritesInit) {
        initBuildingSprites();
        buildingSpritesInit = true;
        loadingText = "Training Warriors!";
        return false;
    }
    if (!warriorSpritesInit) {
        initWarriorSprites();
        warriorSpritesInit = true;
        loadingText = "Planting Trees!";
        return false;
    }
    if (!treeSpritesInit) {
        initTreeSprites();
        treeSpritesInit = true;
        loadingText = "Summoning Enemies!";
        return false;
    }
    if (!enemySpritesInit) {
        initEnemySprites();
        enemySpritesInit = true;
        loadingText = "Building Towns!";
        return false;
    }
    if (!townSpritesInit) {
        initSprites();
        townSpritesInit = true;
        loadingText = "Loading Sound!!";
        return false;
    }
    if (!soundsLoaded) {
        getAudioManager()->update(dt);
        if (getAudioManager()->areSoundsLoaded()) {
            soundsLoaded = true;
        }
        return false;
    }
    if (buildingSpritesInit && warriorSpritesInit && treeSpritesInit &&
        enemySpritesInit && townSpritesInit && soundsLoaded) {
        // getContainer()->init();
        // getStateStack()->push(getContainer());
        // getStateStack()->push(getWorldState());
        // getStateStack()->push(std::make_shared<TextBox>(
        //     "Welcome to the world of the Kingdom of the Lost! "
        //     "You are the ruler of this kingdom. "
        //     "You must build your kingdom, train your warriors, "
        //     "and defend your kingdom from the enemies that seek to destroy
        //     it. " "Good luck, my liege!", ARCHER_BUST_SPRITE_ID, true, 20,
        //     2));
        //
        // getStateStack()->push(std::make_shared<TextBox>(
        //     "Hello", ARCHER_BUST_SPRITE_ID, false, 20, 2));
        getMainMenu()->reset();
        getStateStack()->push(getMainMenu());
        sendFirebaseEvent("onMainMenu", {});
        finished = true;
    }
    return false;
}

void Loading::draw() {
    float sh = GetScreenHeight();
    float sw = GetScreenWidth();
    DrawRectangle(0, 0, sw, sh, BLACK);
    Texture2D loading;
    if (sw > sh) {
        // draw loading l texture on the entire screen maintaining it's aspect
        drawFullScreenTextureWithAspect(loadingL);
    } else {
        // draw loading p texture on the entire screen maintaining it's aspect
        drawFullScreenTextureWithAspect(loadingP);
    }
    DrawRectangleGradientV(0, sh * .67, sw, sh * .4, {0, 0, 0, 20}, BLACK);
    // DrawRectangle(0, sh * .67, sw, sh * .4, BLACK);
    std::string lt = "Loading!";
    float width = MeasureText(lt.c_str(), fs);
    DrawText(lt.c_str(), sw * 1.0f / 2 - width / 2, bsh, fs, WHITE);
    float mm = MeasureText(loadingText.c_str(), fs);
    DrawText(loadingText.c_str(), sw * 1.0f / 2 - mm / 2, bsh + 3 * fs, fs,
             WHITE);
}

bool Loading::isFinished() {
    return finished;
}

Loading::~Loading() {
    // do nothing
    UnloadTexture(loadingL);
    UnloadTexture(loadingP);
}
