//
// Created by tobias on 24.10.26.
//

#include "../../header/debugLogging.hpp"
#include "../../header/generalClasses.hpp"
#include "../../header/specializedFieldClasses.hpp"

void FOREST::init() {

    std::string denseOakAdr, denseSpruceAdr, lightOakAdr, lightSpruceAdr;
    SDL_Renderer* rend = window.rend;

    // all the texture addresses
    denseOakAdr = "assets/Fields/denseOak.png";
    denseSpruceAdr = "assets/Fields/denseSpruce.png";
    lightOakAdr = "assets/Fields/lightOak.png";
    lightSpruceAdr = "assets/Fields/lightSpruce.png";

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
    if (makeTexture(denseOakAdr, textures.denseOak) != 0) LOG::WARN("Err in denseOakTree");
    if (makeTexture(denseSpruceAdr, textures.denseSpruce) != 0) LOG::WARN("Err in denseSpruceTree");

    if (makeTexture(lightOakAdr, textures.lightOak) != 0) LOG::WARN("Err in lightOakTree");
    if (makeTexture(lightSpruceAdr, textures.lightSpruce) != 0) LOG::WARN("Err in lightSpruceTree");

    LOG::INIT_SUCCESS("Created Textures successfully");
}