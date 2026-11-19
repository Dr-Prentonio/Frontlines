#include "../header/buildingsClasses.hpp"
#include "../header/generalClasses.hpp"
#include "../header/generalFuncs.hpp"
#include "../header/debugLogging.hpp"
#include "../header/UIClasses.hpp"

int FACTORY::init() {
    // Fabric Init
    TOOLBOX::loadTexture(icons.clothingTex, "assets/Icon/Fabric.png");

    // Metal Init
    TOOLBOX::loadTexture(icons.steelTex, "assets/Icon/Metal.png");

    // Oil Init
    TOOLBOX::loadTexture(icons.oilTex, "assets/Icon/Oil.png");

    // Stone Init
    TOOLBOX::loadTexture(icons.stoneTex, "assets/Icon/Stone.png");

    // Wood Init
    TOOLBOX::loadTexture(icons.woodTex, "assets/Icon/Wood.png");

    // Weapon Init
    TOOLBOX::loadTexture(icons.weaponTex, "assets/Icon/Weapon.png");

    LOG::INIT_SUCCESS("Loaded factory icons successfully");
    return 0;
}

void FACTORY::update() {
    // every sec update
    static float timer {};
    timer += SDL::MAIN::deltaTime;

    if (timer >= 1) {

        for (auto& factory : factorys) {
            int multiplier {1};

            if (factory.onSpecField) multiplier = 2;

            auto& playerRes = PLAYER::resources;

            // apply factory production
            switch (factory.facType) {
                case clothing:
                    playerRes.clothes  += factory.level * 5 * multiplier;
                    break;
                case wood:
                    playerRes.wood     += factory.level * 6 * multiplier;
                    break;
                case stone:
                    playerRes.stone    += factory.level * 6 * multiplier;
                    break;
                case steel:
                    playerRes.steel    += factory.level * 4 * multiplier;
                    break;
                case oil:
                    playerRes.oil      += factory.level * 3 * multiplier;
                    break;
                case weapon:
                    playerRes.weapons  += factory.level * 4;
                    break;
                default: break;
            }
        }

        timer = 0;
    }
}

void removeResources(int type) {
    switch (type) {
        case 1: {
            TOOLBOX::updatePlayerRes(-20, -20, -50);
            break;
        }
        case 2: {
            TOOLBOX::updatePlayerRes(-20, -50, -25);
            break;
        }
        case 3: {
            TOOLBOX::updatePlayerRes(-30, -20, -50);
            break;
        }
        case 4: {
            TOOLBOX::updatePlayerRes(-30, -50, -50);
            break;
        }
        case 5: {
            TOOLBOX::updatePlayerRes(-30, -200, -100);
            break;
        }
        case 6: {
            TOOLBOX::updatePlayerRes(-75, -125, -125);
            break;
        }
        default: break;
    }
}

// returns true if not enough resources
bool checkForResources(int type) {
    auto& playerRes = PLAYER::resources;

    switch (type) {
        case 1: {
            if (playerRes.wood < 20 ||
                playerRes.stone < 20 ||
                playerRes.steel < 50) return false;
            break;
        }
        case 2: {
            if (playerRes.wood < 20 ||
                playerRes.steel < 25 ||
                playerRes.stone < 50) return false;
            break;
        }
        case 3: {
            if (playerRes.wood < 30 ||
                playerRes.steel < 50 ||
                playerRes.stone < 20) return false;
            break;
        }
        case 4: {
            if (playerRes.wood < 30 ||
                playerRes.steel < 50 ||
                playerRes.stone < 50) return false;
            break;
        }
        case 5: {
            if (playerRes.wood < 30 ||
                playerRes.steel < 100 ||
                playerRes.stone < 200) return false;
            break;
        }
        case 6: {
            if (playerRes.wood < 75 ||
                playerRes.steel < 125 ||
                playerRes.stone < 125) return false;
            break;
        }
        default: break;
    }

    return true;
}

void FACTORY::spawnNewFac(glm::ivec2 mousePos, factoryType type, bool debug) {
    // make new building
    Building building {};

    building.facType = type;
    SDL_Texture* temp {};          // temp storage for tex

    // select the correct tex type
    switch (type) {
        case clothing:  temp = icons.clothingTex;   break;
        case wood:      temp = icons.woodTex;       break;
        case stone:     temp = icons.stoneTex;      break;
        case steel:     temp = icons.steelTex;      break;
        case oil:       temp = icons.oilTex;        break;
        case weapon:    temp = icons.weaponTex;     break;
        default: break;
    }

    if (temp != nullptr) building.facTex = temp;
    else return;

    // get tex size and resize it
    int width {}, height {};
    SDL_QueryTexture(temp, nullptr, nullptr, &width, &height);
    width /= 4;
    height /= 4;

    // find where to place the building
    glm::ivec2 tempPos {
        (mousePos.x * TERRAIN::SURFACE::tileSize) - width / 2,
        (mousePos.y * TERRAIN::SURFACE::tileSize) - height / 2,
    };

    // make factory hitbox
    building.hitbox = {
        tempPos.x - 20,
        tempPos.y - 20,
        width + 40,
        height + 40
    };

    // check for colliding hitboxes
    for (const auto boo : factorys) {
        if (TOOLBOX::checkHitboxCollision(building.hitbox, boo.hitbox)) return;
    }
    for (const auto foo : CITY::citys) {
        if (TOOLBOX::checkHitboxCollision(building.hitbox, foo.hitbox)) return;
    }

    // get player resources
    if (PLAYER::citizen.citizen < 75) return;
    if (!checkForResources(type)) return;

    // update player
    PLAYER::citizen.citizen -= 75;

    switch (type) {
        case 1: {
            PLAYER::factorys.clothesFactorys++;
            for (const auto cotton : TERRAIN::FOLIAGE::COTTON::cottonTiles) {
                if (TOOLBOX::checkHitboxCollision(building.hitbox, cotton)) {
                    building.onSpecField = true;
                    break;
                }
            }

            break;
        }
        case 2: {
            PLAYER::factorys.woodFactorys++;
            for (const auto forest : TERRAIN::FOLIAGE::FOREST::forestTiles) {
                if (TOOLBOX::checkHitboxCollision(building.hitbox, forest)) {
                    building.onSpecField = true;
                    break;
                }
            }
            break;
        }
        case 3: {
            PLAYER::factorys.stoneFactorys++;
            for (const auto stone : TERRAIN::FOLIAGE::STONE::stoneTiles) {
                if (TOOLBOX::checkHitboxCollision(building.hitbox, stone)) {
                    building.onSpecField = true;
                    break;
                }
            }
            break;
        }
        case 4: {
            PLAYER::factorys.steelFactorys++;
            for (const auto stone : TERRAIN::FOLIAGE::STONE::stoneTiles) {
                if (TOOLBOX::checkHitboxCollision(building.hitbox, stone)) {
                    building.onSpecField = true;
                    break;
                }
            }
            break;
        }
        case 5: {
            PLAYER::factorys.oilFactorys++;
            for (const auto oil : TERRAIN::FOLIAGE::OIL::oilTiles) {
                if (TOOLBOX::checkHitboxCollision(building.hitbox, oil)) {
                    building.onSpecField = true;
                    break;
                }
            }
            break;
        }
        case 6: {
            PLAYER::factorys.weaponFactorys++;
        }
        default: break;
    }

    removeResources(type);

    // |=====================|
    // |    Setup Drawing    |
    // |=====================|

    // make fac Rect
    building.facRect = {
        tempPos.x,
        tempPos.y,
        width,
        height
    };

    // make the level counter
    TOOLBOX::drawBuildingLevelCounter(
        1,
        building.hitbox,
        building.levelRect,
        building.levelTex
        );

    // push back
    factorys.push_back(building);
}

void FACTORY::mouseClick() {
    glm::ivec2 mousePos     = TOOLBOX::findMousePos();
    glm::ivec2 mouseGridPos {mousePos * TERRAIN::SURFACE::tileSize};

    for (const auto city : CITY::citys) {
        if (TOOLBOX::checkMouseCollision(mouseGridPos, city.hitbox)) {
            return;
        }
    }
    for (const auto camp : MILITARYCAMP::camps) {
        if (TOOLBOX::checkMouseCollision(mouseGridPos, camp.hitbox)) {
            return;
        }
    }

    // checks if theres a factory
    for (auto& building : factorys) {

        // if click is inside an already existing building
        if (TOOLBOX::checkMouseCollision(mouseGridPos, building.hitbox)) {

            // check for resources
            if (PLAYER::citizen.citizen < 75) return;
            if (!checkForResources(building.facType)) return;

            // upgrade
            building.level += 1;

            // update player
            PLAYER::citizen.citizen -= 75;

            switch (building.facType) {
                case 1: PLAYER::factorys.clothesFactorys++;  break;
                case 2: PLAYER::factorys.woodFactorys++;     break;
                case 3: PLAYER::factorys.stoneFactorys++;    break;
                case 4: PLAYER::factorys.steelFactorys++;    break;
                case 5: PLAYER::factorys.oilFactorys++;      break;
                case 6: PLAYER::factorys.weaponFactorys++;   break;
                default: break;
            }

            // update level counter
            TOOLBOX::updateBuildingLevelCounter(
                building.level,
                building.hitbox,
                building.levelRect,
                building.levelTex
                );

            removeResources(static_cast<int>(building.facType));
        }
    }

    int index = TOOLBOX::findMouseIndex();

    if (TERRITORY::layer[index].ownerID != 1) return;

    // if there isnt a factory then spawn a new one
    spawnNewFac(mousePos, static_cast<factoryType>(UI::bottomLeftUi.index), false);
}