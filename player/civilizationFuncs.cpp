//
// Created by tobias on 18.10.26.
//

#include "../header/buildingsClasses.hpp"
#include "../header/generalClasses.hpp"
#include "../header/generalFuncs.hpp"
#include "../header/debugLogging.hpp"
#include "../header/UIClasses.hpp"

int CITY::init() {
    SDL_Renderer* rend = window.rend;

    SDL_Surface* temp = IMG_Load("assets/Icon/city.png");
    if (temp == nullptr) {
        LOG::INIT_ERR("Couldnt load city.png");
        return -1;
    }

    cityTex = SDL_CreateTextureFromSurface(rend, temp);

    if (cityTex == nullptr) {
        LOG::INIT_ERR("Couldnt create city texture");
        return -1;
    }

    LOG::INIT_SUCCESS("Loaded city icon");
    return 0;
}

void CITY::spawnNew(glm::ivec2 mousePos, bool debug) {
    Building building;

    // texture size
    int width, height;
    SDL_QueryTexture(cityTex, nullptr, nullptr, &width, &height);
    width /= 4;
    height /= 4;

    // find where to place the building
    glm::ivec2 tempPos = {
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

    // check player rec
    auto& playerRes = PLAYER::resources;
    if ( 75 > playerRes.steel or 75 > playerRes.stone or 50 > playerRes.wood) return;

    // check for collisions
    for (auto& boo : FACTORY::factorys) {
        if (TOOLBOX::checkHitboxCollision(building.hitbox, boo.hitbox)) return;
    }
    for (auto& foo : MILITARYCAMP::camps) {
        if (TOOLBOX::checkHitboxCollision(building.hitbox, foo.hitbox)) return;
    }

    // update player
    PLAYER::citizen.citysTotal++;
    PLAYER::citizen.citizen += 400;

    playerRes.stone -= 75;
    playerRes.steel -= 75;
    playerRes.wood  -= 50;

    // |============|
    // |    Draw    |
    // |============|

    // make city rect
    building.cityRect = {
        tempPos.x,
        tempPos.y,
        width,
        height
    };

    // make counter
    TOOLBOX::drawBuildingLevelCounter(
        1,
        building.hitbox,
        building.levelRect,
        building.levelTex
        );

    // update citys
    citys.push_back(building);
}

void CITY::mouseClick() {
    glm::ivec2 mousePos = TOOLBOX::findMousePos();
    glm::ivec2 mouseGridPos = mousePos * TERRAIN::SURFACE::tileSize;

    // if click is an buildings hitbox
    for (auto& factory : FACTORY::factorys) {
        if (TOOLBOX::checkMouseCollision(mouseGridPos, factory.hitbox)) {
            return;
        }
    }
    for (auto& camp : MILITARYCAMP::camps) {
        if (TOOLBOX::checkMouseCollision(mousePos, camp.hitbox)) {
            return;
        }
    }

    // if theres a city -> level up city
    for (auto& city : citys) {
        if (TOOLBOX::checkMouseCollision(mouseGridPos, city.hitbox)) {
            auto& playerRes = PLAYER::resources;

            // checks if player has enough resources
            if (75 > playerRes.steel or 75 > playerRes.stone or 50 > playerRes.wood) return;

            // update player
            TOOLBOX::updatePlayerRes(-50, -75, -75,0,0);

            city.level++;

            PLAYER::citizen.citysTotal++;
            PLAYER::citizen.citizen += 400;

            TOOLBOX::updateBuildingLevelCounter(
                city.level,
                city.hitbox,
                city.levelRect,
                city.levelTex
                );

            return;
        }
    }

    int index = TOOLBOX::findMouseIndex();

    if (TERRAIN::SURFACE::map[index].type <= 1 or TERRITORY::layer[index].ownerID != 1) return;

    spawnNew(mousePos, false);
}

// |=====================|
// |    Military Camp    |
// |=====================|

int MILITARYCAMP::init() {

    // create surface
    SDL_Surface* surf = IMG_Load("assets/Icon/Military.png");
    if (surf == nullptr) {
        std::string err = SDL_GetError();
        if (err.empty()) err = "couldnt get error msg";
        LOG::INIT_ERR("Couldnt create surface: " + err);
        return -1;
    }

    // create texture from surface
    campTex = SDL_CreateTextureFromSurface(window.rend, surf);
    if (campTex == nullptr) {
        std::string err = SDL_GetError();
        if (err.empty()) err = "couldnt get error msg";
        LOG::INIT_ERR("Couldnt create texture: " + err);
        return -1;
    }

    SDL_FreeSurface(surf);
    LOG::INIT_SUCCESS("Loaded military camp icon");
    return 0;
}

void MILITARYCAMP::levelUp(auto& curCamp) {
    // get the resources the player cur has
    auto& playerRes = PLAYER::resources;
    if (playerRes.stone < 100 or playerRes.steel < 150 or playerRes.wood < 125) return;

    // remove resources from player
    TOOLBOX::updatePlayerRes(-125, -100, -100, 0, 0);

    // upgrade level
    curCamp.level++;
    PLAYER::military.militaryCamps++;

    // update counter
    TOOLBOX::updateBuildingLevelCounter(
        curCamp.level,
        curCamp.hitbox,
        curCamp.levelRect,
        curCamp.levelTex);
}

// called 60 times a second
void MILITARYCAMP::update() {
    static float timer = 0;
    timer += gameloop.deltaTime;

    // every second
    if (timer >= 1) {

        for (auto& camp : camps) {
            int offset = TOOLBOX::randomInt(-5, 5);

            // update player
            PLAYER::resources.clothes -= (5 + offset) * camp.level;
            PLAYER::citizen.citizen   -= (5 + offset) * camp.level;
            PLAYER::military.troops   += (5 + offset) * camp.level;
        }

        timer = 0;
    }
}

void MILITARYCAMP::spawnNew() {
    glm::ivec2 mousePos = TOOLBOX::findMousePos();
    Building building;

    // texture size
    int width, height;
    SDL_QueryTexture(campTex, nullptr, nullptr, &width, &height);
    width /= 4;
    height /= 4;

    // find where to place the building
    glm::ivec2 tempPos = {
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

    // check player rec
    auto& playerRes = PLAYER::resources;
    if ( 150 > playerRes.steel or 100 > playerRes.stone or 125 > playerRes.wood) return;

    // check for collisions
    for (auto& boo : FACTORY::factorys) {
        if (TOOLBOX::checkHitboxCollision(building.hitbox, boo.hitbox)) return;
    }
    for (auto& foo : CITY::citys) {
        if (TOOLBOX::checkHitboxCollision(building.hitbox, foo.hitbox)) return;
    }

    // update player
    PLAYER::military.militaryCamps++;

    playerRes.stone -= 100;
    playerRes.steel -= 150;
    playerRes.wood  -= 125;

    // |============|
    // |    Draw    |
    // |============|

    // make city rect
    building.iconRect = {
        tempPos.x,
        tempPos.y,
        width,
        height
    };

    // make counter
    TOOLBOX::drawBuildingLevelCounter(
        1,
        building.hitbox,
        building.levelRect,
        building.levelTex
        );

    // update citys
    camps.push_back(building);
}

void MILITARYCAMP::mouseClick() {
    // mouse pos and mouse pos based on grid size
    glm::ivec2 mousePos = TOOLBOX::findMousePos();
    glm::ivec2 mouseGridPos = mousePos * TERRAIN::SURFACE::tileSize;

    // check for building collision
    for (auto& factory : FACTORY::factorys) {
        if (TOOLBOX::checkMouseCollision(mouseGridPos, factory.hitbox)) return;
    }
    for (auto& cities : CITY::citys) {
        if (TOOLBOX::checkMouseCollision(mousePos, cities.hitbox)) return;
    }

    // level up
    for (auto& camp : camps) {
        if (TOOLBOX::checkMouseCollision(mouseGridPos, camp.hitbox)) {
            levelUp(camp);
            return;
        }
    }

    int index = TOOLBOX::findMouseIndex();

    if (TERRAIN::SURFACE::map.at(index).type <= 1 or TERRITORY::layer.at(index).ownerID != 1) return;

    spawnNew();
}