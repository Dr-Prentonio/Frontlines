//
// Created by tobias on 15.11.26.
//

#include "../header/generalClasses.hpp"
#include "../header/generalFuncs.hpp"
#include "../header/terrainClasses.hpp"

void PLAYER::MINING::mouseClick() {
    glm::ivec2 mousePos     = TOOLBOX::findMousePos();
    glm::ivec2 mouseGridPos {mousePos * TERRAIN::SURFACE::tileSize};

    bool finalBreak {};

    for (const auto forest : TERRAIN::FOLIAGE::FOREST::forestTiles) {
        if (TOOLBOX::checkMouseCollision(mouseGridPos, forest)) {
            TOOLBOX::updatePlayerRes(1, 0, 0, 0, 0, 0);
            finalBreak = true;
            break;
        }
    }

    if (finalBreak) return;

    for (const auto stone : TERRAIN::FOLIAGE::STONE::stoneTiles) {
        if (TOOLBOX::checkMouseCollision(mouseGridPos, stone)) {

            int chance = TOOLBOX::randomInt(0, 5);
            int num = 0;
            if (chance >= 5) num = 1;

            TOOLBOX::updatePlayerRes(0, 1, num, 0, 0, 0);
            finalBreak = true;
            break;
        }
    }
}
