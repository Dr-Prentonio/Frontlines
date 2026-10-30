//
// Created by tobias on 24.10.26.
//

#include "../../header/debugLogging.hpp"
#include "../../header/generalClasses.hpp"
#include "../../header/specializedFieldClasses.hpp"

void FOREST::init() {

    std::string OakAdr, SpruceAdr;
    SDL_Renderer* rend = window.rend;

    // asset address
    OakAdr = "assets/Fields/oakTreeIcon.png";
    SpruceAdr = "assets/Fields/spruceTreeIcon.png";

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

    // init the textures
    if (makeTexture(OakAdr, textures.oak) != 0) LOG::WARN("Err in denseOakTree");
    if (makeTexture(SpruceAdr, textures.spruce) != 0) LOG::WARN("Err in denseSpruceTree");

    LOG::INIT_SUCCESS("Created Textures successfully");
}