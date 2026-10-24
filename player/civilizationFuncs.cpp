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

    // check player rec
    auto rec = TOOLBOX::getPlayerResources();
    if ( 75 > rec.steelAmount or 75 > rec.stoneAmount or 50 > rec.woodAmount) return;

    // check for collisions
    for (auto& boo : FACTORY::factorys) {
        if (TOOLBOX::checkHitboxCollision(building.hitbox, boo.hitbox)) return;
    }
    for (auto& foo : citys) {
        if (TOOLBOX::checkHitboxCollision(building.hitbox, foo.hitbox)) return;
    }

    // update player
    player.citizen.citysTotal++;
    player.citizen.citizen += 400;

    player.resources.stoneAmount -= 75;
    player.resources.steelAmount -= 75;
    player.resources.woodAmount  -= 50;

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
    glm::ivec2 mouseGridPos = mousePos * TERRAIN::tileSize;

    // if click is an buildings hitbox
    for (auto& factory : FACTORY::factorys) {
        if (TOOLBOX::checkMouseCollision(mouseGridPos, factory.hitbox)) {
            return;
        }
    }

    // if theres a city -> level up city
    for (auto& city : citys) {
        if (TOOLBOX::checkMouseCollision(mouseGridPos, city.hitbox)) {

            auto rec = TOOLBOX::getPlayerResources();

            // checks if player has enough resources
            if ( 75 > rec.steelAmount or 75 > rec.stoneAmount or 50 > rec.woodAmount) return;

            // update player
            player.resources.stoneAmount -= 75;
            player.resources.steelAmount -= 75;
            player.resources.woodAmount  -= 50;

            city.level++;

            player.citizen.citysTotal++;
            player.citizen.citizen += 400;

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

    if (TERRAIN::map[index].type <= 1 or TERRITORY::layer[index].ownerID != 1) return;

    spawnNew(mousePos, false);
}