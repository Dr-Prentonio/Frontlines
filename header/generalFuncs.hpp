//
// Created by tobias on 22.10.26.
//

#ifndef STRATEGIE_GAME_GENERALFUNCS_HPP
#define STRATEGIE_GAME_GENERALFUNCS_HPP

// SYSTEM
#include <chrono>
#include <format>

// HEADER
#include "buildingsClasses.hpp"
#include "generalClasses.hpp"
#include "debugLogging.hpp"
#include "UIClasses.hpp"

// GLM
#include "glm/glm.hpp"
#include "glm/gtc/noise.hpp"

// SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Funcs used all over the game
namespace TOOLBOX {



    //
    //
    //      This namespace is for funcs that are used all across the game
    //
    //



    // |============================================|
    // |    Find Mouse Position Relativ to Tiles    |
    // |============================================|

    inline glm::ivec2 findMousePos() {
        // default size of the tiles
        glm::vec2 defaultSize = {
            (static_cast<float>(window.scale.x) / TERRAIN::mapSize.x),
            (static_cast<float>(window.scale.y) / TERRAIN::mapSize.y)
        };
        //        glm::vec2 currentSize = defaultSize * player.zoom;

        glm::ivec2 mousePos;
        SDL_GetMouseState(&mousePos.x, &mousePos.y);

        // mouse pos in world cords
        glm::vec2 mouseWorldPos = {
            mousePos.x - (player.camOffset.x * player.zoom),
            mousePos.y - (player.camOffset.y * player.zoom)
        };

        // tile size with the zoom
        glm::vec2 tileSize = {
            defaultSize.x * player.zoom,
            defaultSize.y * player.zoom
        };

        // tile pos
        glm::ivec2 tilePos = {
            static_cast<int>(mouseWorldPos.x / tileSize.x),
            static_cast<int>(mouseWorldPos.y / tileSize.y)
        };

        // check if tile is inside the world
        if (tilePos.x >= 0 && tilePos.x < TERRAIN::mapSize.x && tilePos.y >= 0 && tilePos.y < TERRAIN::mapSize.y) {
            return tilePos;
        }

        return {0, 0};
    }

    // |==================================================|
    // |    Find the Index of the Tile under the Mouse    |
    // |==================================================|

    inline int findMouseIndex() {
        // default size of the tiles
        glm::vec2 defaultSize = {
            (static_cast<float>(window.scale.x) / TERRAIN::mapSize.x),
            (static_cast<float>(window.scale.y) / TERRAIN::mapSize.y)
        };
        //        glm::vec2 currentSize = defaultSize * player.zoom;

        glm::ivec2 mousePos;
        SDL_GetMouseState(&mousePos.x, &mousePos.y);

        int tileIndex;

        // mouse pos in world cords
        glm::vec2 mouseWorldPos = {
            mousePos.x - (player.camOffset.x * player.zoom),
            mousePos.y - (player.camOffset.y * player.zoom)
        };

        // tile size with the zoom
        glm::vec2 tileSize = {
            defaultSize.x * player.zoom,
            defaultSize.y * player.zoom
        };

        // tile pos
        glm::ivec2 tilePos = {
            static_cast<int>(mouseWorldPos.x / tileSize.x),
            static_cast<int>(mouseWorldPos.y / tileSize.y)
        };

        // check if tile is inside the world
        if (tilePos.x >= 0 && tilePos.x < TERRAIN::mapSize.x && tilePos.y >= 0 && tilePos.y < TERRAIN::mapSize.y) {
            tileIndex = tilePos.x + tilePos.y * TERRAIN::mapSize.x; // find out tile index (unused currently)

            return tileIndex;
        } else {
            return -1;
        }
    }

    // |==============================|
    // |    Fetch Player Resources    |
    // |==============================|

    inline Player::Resources getPlayerResources() {
        Uint32 clothes, wood, stone, steel, oil;

        clothes = player.resources.clothesAmount;
        wood = player.resources.woodAmount;
        stone = player.resources.stoneAmount;
        steel = player.resources.steelAmount;
        oil = player.resources.oilAmount;

        return {wood, stone, clothes, steel, oil};
    }

    // |============================================|
    // |    Check Mouse Collision X Box Collider    |
    // |============================================|

    inline bool checkMouseCollision(
        glm::ivec2 mouse,   // mouse position
        SDL_Rect hitbox     // hitbox of a object
        ) {
        if (mouse.x > hitbox.x &&
            mouse.x < hitbox.x + hitbox.w &&
            mouse.y > hitbox.y &&
            mouse.y < hitbox.y + hitbox.h) {
            return true;
        }

        return false;
    }

    // |=========================================|
    // |    Check Box Collider X Box Collider    |
    // |=========================================|

    inline bool checkHitboxCollision(
        SDL_Rect boxA,    // AABB hitbox 1
        SDL_Rect boxB     // AABB hitbox 2
        ) {
        if (boxA.x + boxA.w > boxB.x &&
            boxA.x < boxB.x + boxB.w &&
            boxA.y + boxA.h > boxB.y &&
            boxA.y < boxB.y + boxB.h) {
            return true;
        }

        return false;
    }

    // |====================================================|
    // |    Setup for Buildings Level in the Bottom Left    |
    // |====================================================|

    inline void drawBuildingLevelCounter(
        int         level,                  // level of building
        SDL_Rect    hitbox,                 // hitbox of building
        SDL_Rect    &levelCounterRect,      // drect of the level textbox
        SDL_Texture *&levelCounterTexture   // tex of the level text
        ) {
        SDL_Renderer *rend = window.rend;

        // transform int level -> sdl Texture
        std::string text = std::to_string(level);

        SDL_Surface *textSurface = TTF_RenderUTF8_Solid(
            UI::fonts.RobotoMonoBold,
            text.c_str(),
            {30, 30, 30, 255}
        );

        SDL_Texture *numberTex = SDL_CreateTextureFromSurface(rend, textSurface);

        // check if everything went right
        if (textSurface == nullptr or numberTex == nullptr) {
            LOG::WARN("Couldnt create level Texture in toolbox func");

            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(numberTex);

            return;
        }

        // get double of the width and height of the text
        int textWidth = textSurface->w * 2;
        int textHeight = textSurface->h * 2;

        SDL_Rect drect = {
            hitbox.x + 10,
            hitbox.y + hitbox.h - textHeight,
            textWidth,
            textHeight
        };

        // update tex & rect
        levelCounterTexture = numberTex;
        levelCounterRect = drect;

        // free memory
        SDL_FreeSurface(textSurface);
    }

    // |===========================================|
    // |    Updates the Buildings Level Counter    |
    // |===========================================|

    inline void updateBuildingLevelCounter(
        int             level,                  // level of the building
        SDL_Rect        hitbox,                 // hitbox of building
        SDL_Rect        &levelCounterRect,      // drect of the level textbox
        SDL_Texture     *&levelCounterTexture   // tex of the level text
        ) {

        SDL_Renderer* rend = window.rend;
        SDL_Texture *temp = nullptr;

        // make new tex
        std::string text = std::to_string(level);
        SDL_Surface *textSurface = TTF_RenderUTF8_Solid(
            UI::fonts.RobotoMonoBold,
            text.c_str(),
            {30, 30, 30, 255}
        );

        temp = SDL_CreateTextureFromSurface(rend, textSurface);

        if (temp == nullptr or textSurface == nullptr) {
            LOG::ERR("Couldnt create level Texture in toolbox update func");

            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(temp);

            return;
        }

        int textWidth = textSurface->w * 2;
        int textHeight = textSurface->h * 2;

        SDL_Rect drect = {
            hitbox.x + 10,
            hitbox.y + hitbox.h - textHeight,
            textWidth,
            textHeight
        };

        levelCounterRect = drect;
        levelCounterTexture = temp;

        // Free Memory
        SDL_FreeSurface(textSurface);
    }

    // |====================================================================|
    // |    Translates the raw building position into screen space cords    |
    // |====================================================================|

    inline void screenSpaceMapping(
        const SDL_Rect old,             //  the raw position
        const SDL_Rect currentOffset,   //  the current screen zoom and offset (drect in draw.cpp)
        SDL_Rect& newRect               //  the new rect for the drawing
        ) {

        // normalize cords of building (0.0f - 1.0f)
        glm::vec4 normalized = {
            (float)old.x    / (TERRAIN::mapSize.x * TERRAIN::tileSize),
            (float)old.y    / (TERRAIN::mapSize.y * TERRAIN::tileSize),
            (float)old.w    / (TERRAIN::mapSize.x * TERRAIN::tileSize),
            (float)old.h   / (TERRAIN::mapSize.y * TERRAIN::tileSize)
        };

        // screen position
        SDL_Rect temp = {
            static_cast<int>(currentOffset.x + (normalized.x * currentOffset.w)),
            static_cast<int>(currentOffset.y + (normalized.y * currentOffset.h)),
            static_cast<int>(normalized.z * currentOffset.w),
            static_cast<int>(normalized.w * currentOffset.h)
        };

        newRect = temp;
    }
}

#endif //STRATEGIE_GAME_GENERALFUNCS_HPP
