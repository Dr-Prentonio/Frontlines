//
// Created by tobias on 24.10.26.
//

#include "../../header/debugLogging.hpp"
#include "../../header/generalClasses.hpp"
#include "../../header/terrainClasses.hpp"

void TERRAIN::FOLIAGE::GENERAL::init() {

    std::string oakAdr, spruceAdr, cottonAdr;
    SDL_Renderer* rend = window.rend;

    auto makeTexture = [rend](std::string addr, SDL_Texture*& texture) {

        // create surface
        SDL_Surface* surf = IMG_Load(addr.c_str());
        if (surf == nullptr) {
            LOG::INIT_ERR("Couldnt load image " + addr + ": " + IMG_GetError());
            return -1;
        }

        // create texture from surface
        texture = SDL_CreateTextureFromSurface(rend, surf);
        if (texture == nullptr) {
            LOG::INIT_ERR("Couldnt create texture " + addr + ": " + SDL_GetError());
            return -1;
        }

        SDL_FreeSurface(surf);
        LOG::INIT_SUCCESS("Loaded texture " + addr);
        return 0;
    };



    // trees
    oakAdr = "assets/Fields/oakTreeIcon.png";
    spruceAdr = "assets/Fields/spruceTreeIcon.png";

    if (makeTexture(oakAdr, TERRAIN::FOLIAGE::GENERAL::textures.oak) != 0)         LOG::WARN("Err in oakTreeIcon");
    if (makeTexture(spruceAdr, TERRAIN::FOLIAGE::GENERAL::textures.spruce) != 0)   LOG::WARN("Err in spruceTreeIcon");



    // cotton
    cottonAdr = "assets/Fields/cottonFieldIcon.png";

    if (makeTexture(cottonAdr, TERRAIN::FOLIAGE::GENERAL::textures.cotton) != 0)   LOG::WARN("Err in cottonFieldIcon");



    LOG::INIT_SUCCESS("Created Textures successfully");
}