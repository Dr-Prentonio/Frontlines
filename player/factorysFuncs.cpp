#include "../header/buildingsClasses.hpp"
#include "../header/generalClasses.hpp"
#include "../header/generalFuncs.hpp"
#include "../header/debugLogging.hpp"
#include "../header/UIClasses.hpp"

int FACTORY::init() {

    SDL_Renderer* rend = window.rend;

    // |===================|
    // |    Fabric Init    |
    // |===================|

    SDL_Surface* temp = IMG_Load("assets/Icon/Fabric.png");
    if (temp == nullptr) {
        LOG::INIT_ERR("Couldnt load clothing factory icon");
        return -1;
    }
    icons.clothingTex = SDL_CreateTextureFromSurface(rend, temp);

    // |==================|
    // |    Metal Init    |
    // |==================|

    temp = IMG_Load("assets/Icon/Metal.png");
    if (temp == nullptr) {
        LOG::INIT_ERR("Couldnt load metal factory icon");
        return -1;
    }
    icons.steelTex = SDL_CreateTextureFromSurface(rend, temp);

    // |================|
    // |    Oil Init    |
    // |================|

    temp = IMG_Load("assets/Icon/Oil.png");
    if (temp == nullptr) {
        LOG::INIT_ERR("Couldnt load oil factory icon");
        return -1;
    }
    icons.oilTex = SDL_CreateTextureFromSurface(rend, temp);

    // |==================|
    // |    Stone Init    |
    // |==================|

    temp = IMG_Load("assets/Icon/Stone.png");
    if (temp == nullptr) {
        LOG::INIT_ERR("Couldnt load stone factory icon");
        return -1;
    }
    icons.stoneTex = SDL_CreateTextureFromSurface(rend, temp);

    // |=================|
    // |    Wood Init    |
    // |=================|

    temp = IMG_Load("assets/Icon/Wood.png");
    if (temp == nullptr) {
        LOG::INIT_ERR("Couldnt load wood factory icon");
        return -1;
    }
    icons.woodTex = SDL_CreateTextureFromSurface(rend, temp);

    SDL_FreeSurface(temp);

    return 0;
}

void FACTORY::update() {
    // every sec update
    static float timer = 0;
    timer += gameloop.deltaTime;

    if (timer >= 1) {
        player.resources.clothesAmount  += player.factorys.clothesFactorys  * 5;
        player.resources.woodAmount     += player.factorys.woodFactorys     * 6;
        player.resources.steelAmount    += player.factorys.steelFactorys    * 4;
        player.resources.stoneAmount    += player.factorys.stoneFactorys    * 6;
        player.resources.oilAmount      += player.factorys.oilFactorys      * 3;

        timer = 0;
    }
}

void removeResources(int type) {
    switch (type) {
        case 1: {
            player.resources.woodAmount -= 20;
            player.resources.steelAmount -= 50;
            player.resources.stoneAmount -= 20;
            break;
        }
        case 2: {
            player.resources.woodAmount -= 20;
            player.resources.steelAmount -= 25;
            player.resources.stoneAmount -= 50;
            break;
        }
        case 3: {
            player.resources.woodAmount -= 30;
            player.resources.steelAmount -= 50;
            player.resources.stoneAmount -= 20;
            break;
        }
        case 4: {
            player.resources.woodAmount -= 30;
            player.resources.steelAmount -= 50;
            player.resources.stoneAmount -= 50;
            break;
        }
        case 5: {
            player.resources.woodAmount -= 30;
            player.resources.steelAmount -= 100;
            player.resources.stoneAmount -= 200;
            break;
        }
        default: break;
    }
}

// returns true if not enough resources
bool checkForResources(int type) {

    auto rec = TOOLBOX::getPlayerResources();

    switch (type) {
        case 1: {
            if (rec.woodAmount < 20 || rec.stoneAmount < 20 || rec.steelAmount < 50) return false;
            break;
        }
        case 2: {
            if (rec.woodAmount < 20 || rec.steelAmount < 25 || rec.stoneAmount < 50) return false;
            break;
        }
        case 3: {
            if (rec.woodAmount < 30 || rec.steelAmount < 50 || rec.stoneAmount < 20) return false;
            break;
        }
        case 4: {
            if (rec.woodAmount < 30 || rec.steelAmount < 50 || rec.stoneAmount < 50) return false;
            break;
        }
        case 5: {
            if (rec.woodAmount < 30 || rec.steelAmount < 100 || rec.stoneAmount < 200) return false;
            break;
        }
        default: break;
    }

    return true;
}

void FACTORY::spawnNewFac(glm::ivec2 mousePos, factoryType type, bool debug) {

    // make new building
    Building building;

    building.facType = type;
    SDL_Texture* temp;          // temp storage for tex

    // select the correct tex type
    switch (type) {
        case 1: temp = icons.clothingTex;   break;
        case 2: temp = icons.woodTex;       break;
        case 3: temp = icons.stoneTex;      break;
        case 4: temp = icons.steelTex;      break;
        case 5: temp = icons.oilTex;        break;
        default: temp = nullptr;
    }

    if (temp != nullptr) building.facTex = temp;
    else return;

    // get tex size and resize it
    int width, height;
    SDL_QueryTexture(temp, nullptr, nullptr, &width, &height);
    width /= 4;
    height /= 4;

    // find where to place the building
    glm::ivec2 tempPos = {
        (mousePos.x * TERRAIN::tileSize) - width / 2,
        (mousePos.y * TERRAIN::tileSize) - height / 2,
    };

    // make factory hitbox
    building.hitbox = {
        tempPos.x - 20,
        tempPos.y - 20,
        width + 40,
        height + 40
    };

    // check for colliding hitboxes
    for (auto& boo : factorys) {
        if (TOOLBOX::checkHitboxCollision(building.hitbox, boo.hitbox)) return;
    }
    for (auto& foo : CITY::citys) {
        if (TOOLBOX::checkHitboxCollision(building.hitbox, foo.hitbox)) return;
    }

    // get player resources
    if (player.citizen.citizen < 75) return;
    if (!checkForResources(type)) return;

    // update player
    player.citizen.citizen -= 75;

    switch (type) {
        case 1: player.factorys.clothesFactorys++;  break;
        case 2: player.factorys.woodFactorys++;     break;
        case 3: player.factorys.stoneFactorys++;    break;
        case 4: player.factorys.steelFactorys++;    break;
        case 5: player.factorys.oilFactorys++;      break;
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
    glm::ivec2 mousePos = TOOLBOX::findMousePos();
    glm::ivec2 mouseGridPos = mousePos * TERRAIN::tileSize;

    for (auto& city : CITY::citys) {
        if (TOOLBOX::checkMouseCollision(mouseGridPos, city.hitbox)) {
            return;
        }
    }

    // checks if theres a factory
    for (Building& building : factorys) {

        // if click is inside an already existing building
        if (TOOLBOX::checkMouseCollision(mouseGridPos, building.hitbox)) {

            // check for resources
            if (player.citizen.citizen < 75) return;
            if (!checkForResources(building.facType)) return;

            // upgrade
            building.level += 1;

            // update player
            player.citizen.citizen -= 75;

            switch (building.facType) {
                case 1: player.factorys.clothesFactorys++;  break;
                case 2: player.factorys.woodFactorys++;     break;
                case 3: player.factorys.stoneFactorys++;    break;
                case 4: player.factorys.steelFactorys++;    break;
                case 5: player.factorys.oilFactorys++;      break;
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

    spawnNewFac(mousePos, static_cast<factoryType>(UI::bottomLeftUi.index), false);
}