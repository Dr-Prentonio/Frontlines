#include "../header/generalClasses.hpp"
#include "../header/UIClasses.hpp"
#include "../header/buildingsClasses.hpp"
#include "../header/generalFuncs.hpp"
#include "../header/specializedFieldClasses.hpp"

void SDL::Gameloop::draw() {
    static SDL_Renderer* rend;
    static float playerZoom;

    playerZoom = player.zoom;
    rend = window.rend;

    SDL_RenderClear(rend);

    //======================0
    //       LAYER 1
    //======================0

    SDL_Rect drect = {
        static_cast<int>(player.camOffset.x * playerZoom),
        static_cast<int>(player.camOffset.y * playerZoom),
        static_cast<int>(window.scale.x * playerZoom),
        static_cast<int>(window.scale.y * playerZoom)
    };

    // player cant escape the map
    if (player.camOffset.x > 0) { drect.x = 0; player.touchesBorderLeft = true; }
    else player.touchesBorderLeft = false;

    if (player.camOffset.y > 0) { drect.y = 0; player.touchesBorderTop = true; }
    else player.touchesBorderTop = false;

    if (drect.x < window.scale.x - drect.w) { drect.x = window.scale.x - drect.w; player.touchesBorderRight = true; }
    else player.touchesBorderRight = false;

    if (drect.y < window.scale.y - drect.h) { drect.y = window.scale.y - drect.h; player.touchesBorderBottom = true; }
    else player.touchesBorderBottom = false;

    // |===================|
    // |    Terrain LoD    |
    // |===================|

    // player zoom 1.0f - 7.5f (level of detail)
    if (playerZoom <= 2.5) {
        SDL_RenderCopy(rend, TERRAIN::LOD.LoD3, nullptr, &drect);
    } else if (playerZoom <= 5) {
        SDL_RenderCopy(rend, TERRAIN::LOD.LoD2, nullptr, &drect);
    } else {
        SDL_RenderCopy(rend, TERRAIN::LOD.LoD1, nullptr, &drect);
    }

    for (auto& trees : FORREST::trees) {

        SDL_Rect newRect;
        TOOLBOX::screenSpaceMapping(trees.rect, drect, newRect);

        switch (trees.type) {
            case FORREST::LIGHT_OAK:
                SDL_RenderCopy(rend, FORREST::textures.lightOak, nullptr, &newRect);
                break;
            case FORREST::DENSE_OAK:
                SDL_RenderCopy(rend, FORREST::textures.denseOak, nullptr, &newRect);
                break;
            case FORREST::LIGHT_SPRUCE:
                SDL_RenderCopy(rend, FORREST::textures.lightSpruce, nullptr, &newRect);
                break;
            case FORREST::DENSE_SPRUCE:
                SDL_RenderCopy(rend, FORREST::textures.denseSpruce, nullptr, &newRect);
                break;
            default: break;
        }

        TOOLBOX::screenSpaceMapping(trees.hitbox, drect, newRect);

        SDL_SetRenderDrawColor(rend, 225,225,225,80);
        SDL_RenderFillRect(rend, &newRect);
    }

    for (auto& forestTiles : FORREST::forestTiles) {
        SDL_SetRenderDrawColor(rend, 50, 50, 100, 180);
        SDL_RenderFillRect(rend, &forestTiles);
    }

    //===========================0
    //          LAYER 2
    //===========================0

    // |================================|
    // |    Player Territory Texture    |
    // |================================|

    SDL_RenderCopy(rend, TERRITORY::layerTexture,   nullptr, &drect);

    // |==============================|
    // |    Player Factory Texture    |
    // |==============================|

    for (auto& factory : FACTORY::factorys) {

        // calc screen space mapping
        SDL_Rect buildingCords = {0,0,0,0};
        TOOLBOX::screenSpaceMapping(factory.facRect, drect, buildingCords);

        SDL_RenderCopy(rend, factory.facTex, nullptr, &buildingCords);

        // Calc the screen space mapping
        SDL_Rect textCords = {0,0,0,0};
        TOOLBOX::screenSpaceMapping(factory.levelRect, drect, textCords);

        SDL_RenderCopy(rend, factory.levelTex, nullptr, &textCords);
    }

    // |===========================|
    // |    Player City Texture    |
    // |===========================|

    for (auto& city : CITY::citys) {

        SDL_Rect buildingCords = {0,0,0,0};
        TOOLBOX::screenSpaceMapping(city.cityRect, drect, buildingCords);

        SDL_RenderCopy(rend, CITY::cityTex, nullptr, &buildingCords);

        SDL_Rect textCords = {0,0,0,0};
        TOOLBOX::screenSpaceMapping(city.levelRect, drect, textCords);

        SDL_RenderCopy(rend, city.levelTex, nullptr, &textCords);
    }


    //
    // LAYER 3
    //

    UI::UIManager(rend);

    SDL_RenderPresent(rend);
}
