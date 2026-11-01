//
// Created by tobias on 24.10.26.
//

#ifndef STRATEGIE_GAME_SPECIALIZEDFIELDCLASSES_HPP
#define STRATEGIE_GAME_SPECIALIZEDFIELDCLASSES_HPP

#include <SDL2/SDL.h>

#include <iostream>

#include "glm/glm.hpp"

// everything terrain related
namespace TERRAIN {

    // LoDs
    namespace LOD {
        inline SDL_Texture* LoD1; // smoothest
        inline SDL_Texture* LoD2;
        inline SDL_Texture* LoD3; // Crapiest

        void generateLoDs();
    }

    // surface (grass, taiga, stone teiles etc)
    namespace SURFACE {
        enum TerrainType {
            DEEP_SEA,
            SEA,
            SHORE,
            GRASS,
            TAIGA,
            MOUNTAIN,
            SNOW
        };

        struct TerrainTile {
            SDL_Rect rect;
            TerrainType type;
        };

        // corrosion simulation
        struct Droplet {

            glm::vec2 pos;
            glm::vec2 speed = {0,0};
            float amount = 1.0f;
            float sediment = 0.0f;
        };

        const int tileSize = 32;
        const int scale = 14; // whole game relies on THIS one number TwT

        inline std::vector<TerrainTile> map; // map vector

        inline SDL_Texture* mapTexture;

        inline glm::ivec2 mapSize; // size of the map

        void generateTerrain();
        void generateTile(SDL_Renderer* rend);
    }

    // foliage (trees, cotton etc)
    namespace FOLIAGE {
        namespace GENERAL {

            // textures
            struct Textures {
                // trees
                SDL_Texture* oak;
                SDL_Texture* spruce;
                // cotton fields
                SDL_Texture* cotton;
            };

            inline Textures textures;

            inline SDL_Texture* worldTex;

            inline std::vector<bool> isUsed;

            void init();
            void drawIcons();
        }

        namespace FOREST {

            const int spawnRate = 25;

            // longterm save for pos of tiles
            inline std::vector<SDL_Rect> forestTiles;

            // funcs

            void drawTiles (
                int size,                       // size of the base
                glm::ivec2 pos,                 // pos of the center of the base
                SDL_Color normalColor,          // color of the tiles
                std::vector<SDL_Rect>& tiles    // vector to safe tiles to
            );

            void spawnTrees(int treesAmount);
            void generate();
        }

        namespace COTTON {

            const int spawnRate = 25;

            inline std::vector<SDL_Rect> cottonTiles;

            void drawTiles (
                int size,                       // size of the base
                glm::ivec2 pos,                 // pos of the center of the base
                SDL_Color normalColor,          // color of the tiles
                std::vector<SDL_Rect>& tiles    // vector to safe tiles to
            );

            void spawnCotton(int cottonAmount);
            void generate();
        }
    }
}

#endif //STRATEGIE_GAME_SPECIALIZEDFIELDCLASSES_HPP
