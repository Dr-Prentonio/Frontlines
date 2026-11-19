//
// Created by tobias on 24.10.26.
//

#include "../../header/debugLogging.hpp"
#include "../../header/generalClasses.hpp"
#include "../../header/generalFuncs.hpp"
#include "../../header/terrainClasses.hpp"

void TERRAIN::FOLIAGE::GENERAL::init() {
    std::string oakAdr {}, spruceAdr {}, cottonAdr {}, oilAdr {}, stoneAdr {};

    // trees
    oakAdr = "assets/Fields/oakTreeIcon.png";
    spruceAdr = "assets/Fields/spruceTreeIcon.png";
    if (TOOLBOX::loadTexture(textures.oak, oakAdr) != 0)         LOG::WARN("Err in oakTreeIcon");
    if (TOOLBOX::loadTexture(textures.spruce, spruceAdr) != 0)   LOG::WARN("Err in spruceTreeIcon");

    // cotton
    cottonAdr = "assets/Fields/cottonFieldIcon.png";
    if (TOOLBOX::loadTexture(textures.cotton, cottonAdr) != 0)   LOG::WARN("Err in cottonFieldIcon");

    // oil
    oilAdr = "assets/Fields/oilFieldIcon.png";
    if (TOOLBOX::loadTexture(textures.oil, oilAdr) != 0)         LOG::WARN("Err in oilFieldIcon");

    // stone
    stoneAdr = "assets/Fields/stoneFieldIcon.png";
    if (TOOLBOX::loadTexture(textures.stone, stoneAdr) != 0)       LOG::WARN("Err in stoneFieldIcon");

    // init the whole vector (it now uses world cords)
    worldTileUsage.assign(SURFACE::mapSize.x * SURFACE::mapSize.y, false);

    LOG::INIT_SUCCESS("Created Textures successfully");
}