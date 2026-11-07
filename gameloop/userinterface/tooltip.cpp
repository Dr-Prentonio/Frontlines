//
// Created by tobias on 04.11.26.
//

#include "../../header/UIClasses.hpp"
#include "../../header/generalClasses.hpp"
#include "../../header/generalFuncs.hpp"
#include "glm/glm.hpp"

// |===============|
// |    Tooltip    |
// |===============|

void UI::TOOLTIP::surfaceMode(SDL_Renderer* rend) {

    glm::ivec2 pos;
    SDL_GetMouseState(&pos.x, &pos.y);
    SDL_Rect drect = {pos.x + 30, pos.y + 30, 0,0};

    SDL_SetRenderDrawColor(rend, 25, 25, 25, 200);

    Uint32 wrapLength = 0;

    // title surface
    SDL_Surface* boldTextSurface = TTF_RenderUTF8_Blended_Wrapped(
        fonts.terminessBold,
        title.c_str(),
        {255,255,255, 255},
        wrapLength
    );
    if (!boldTextSurface) return;

    // text surface
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended_Wrapped(
        fonts.terminessItalic,
        text.c_str(),
        {255,255,255,255},
        wrapLength
        );
    if (!textSurface) return;

    // get width and height of title and text
    SDL_Texture* textTexture =  SDL_CreateTextureFromSurface(rend, textSurface);
    SDL_Texture* boldTextTexture = SDL_CreateTextureFromSurface(rend, boldTextSurface);

    int boldTextWidth = boldTextSurface->w;
    int boldTextHeight = boldTextSurface->h;

    int textWidth   = textSurface->w;
    int textHeight  = textSurface->h;

    SDL_FreeSurface(textSurface);
    SDL_FreeSurface(boldTextSurface);

    //
    // design
    //

    glm::ivec2 padding = {5, 4};

    // background drect
    SDL_Rect bgRect = {
        drect.x,
        drect.y,
        textWidth + (padding.x * 2),
        textHeight + (padding.y * 2) + 28
    };

    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);

    // background rect
    SDL_SetRenderDrawColor(rend, 25,25,25,200);
    SDL_RenderFillRect(rend, &bgRect);

    // Frame
    SDL_SetRenderDrawColor(rend, 100, 100, 100, 255);
    SDL_RenderDrawRect(rend, &bgRect);

    SDL_Rect textDrect = {
        drect.x + padding.x,
        drect.y + padding.y + 28,
        textWidth,
        textHeight
    };
    SDL_Rect boldTextDrect = {
        drect.x + padding.x,
        drect.y + padding.y,
        boldTextWidth,
        boldTextHeight
    };

    SDL_RenderCopy(rend, textTexture, NULL, &textDrect);
    SDL_RenderCopy(rend, boldTextTexture, NULL, &boldTextDrect);
    SDL_DestroyTexture(textTexture);
    SDL_DestroyTexture(boldTextTexture);
}

void UI::TOOLTIP::manager() {
    // we need to figure out if mouse is on terrain surface, flora, or buildings

    glm::ivec2 mouseGridPos = TOOLBOX::findMousePos() * TERRAIN::SURFACE::tileSize;

    // global vars
    std::string name;
    std::string troopCost;
    std::string ownerID;

    // surface terrain layer (its on top cuz its always going to be true)
    {
        // tooltip
        glm::ivec2 pos;
        SDL_GetMouseState(&pos.x, &pos.y);

        // get index & type
        int index = TOOLBOX::findMouseIndex();
        if (index == -1) return;

        int type = TERRAIN::SURFACE::map.at(index).type;

        // choose type
        switch (type) {
            case 0:
                name = "Deep Sea Tile";
                troopCost = "-";
                break;
            case 1:
                name = "Sea Tile";
                troopCost = "-";
                break;
            case 2:
                name = "Shore Tile";
                troopCost = "100";
                break;
            case 3:
                name = "Grass Tile";
                troopCost = "75";
                break;
            case 4:
                name = "Taiga Tile";
                troopCost = "100";
                break;
            case 5:
                name = "Mountain Tile";
                troopCost = "125";
                break;
            case 6:
                name = "Snow Tile";
                troopCost = "250";
                break;
            default:
                name = "err: unknown";
                break;
        }

        ownerID = std::to_string(TERRITORY::layer.at(index).ownerID);

        title = name;
        text = "Troop cost: " + troopCost + " | Owner: " + ownerID;
        enable = true;
    }

    // foliage
    {
        // lambda to check what tile is uses
        auto kniga = [mouseGridPos, name, troopCost, ownerID](const std::vector<SDL_Rect>& tiles, const std::string& title) {
            for (auto& check : tiles) {
                if (TOOLBOX::checkMouseCollision(mouseGridPos, check)) {
                    TOOLTIP::title = title;
                    text = name + " | Troop cost: " + troopCost + " | Owner: " + ownerID;
                    break;
                }
            }
        };

        kniga(TERRAIN::FOLIAGE::FOREST::forestTiles, "Forest"      );
        kniga(TERRAIN::FOLIAGE::COTTON::cottonTiles, "Cotton Field");
        kniga(TERRAIN::FOLIAGE::OIL::oilTiles,       "Oil Field"   );
        kniga(TERRAIN::FOLIAGE::STONE::stoneTiles,   "Rocky Hill"  );
    }

    // factorys
    for (auto& factory : FACTORY::factorys) {
        // if click is inside an already existing building
        if (TOOLBOX::checkMouseCollision(mouseGridPos, factory.hitbox)) {

            std::string type;

            switch (factory.facType) {
                case 1: type = "Clothing";  break;
                case 2: type = "Wood";      break;
                case 3: type = "Stone";     break;
                case 4: type = "Steel";     break;
                case 5: type = "Oil";       break;
                default: type = "Unknown"; break;
            }

            glm::ivec2 pos;
            SDL_GetMouseState(&pos.x, &pos.y);

            title = "Factory";
            text = "Level: " + std::to_string(factory.level) + " | Type: " + type;
            enable = true;

            break;
        }
    }

    // citys
    for (auto& factory : CITY::citys) {
        // if click is inside an already existing building
        if (TOOLBOX::checkMouseCollision(mouseGridPos, factory.hitbox)) {
            glm::ivec2 pos;
            SDL_GetMouseState(&pos.x, &pos.y);

            title = "City";
            text = "Level: " + std::to_string(factory.level);
            enable = true;

            break;
        }
    }

    // military camps
    for (auto& camp : MILITARYCAMP::camps) {
        if (TOOLBOX::checkMouseCollision(mouseGridPos, camp.hitbox)) {
            glm::ivec2 pos;
            SDL_GetMouseState(&pos.x, &pos.y);

            title = "Military camp";
            text = "Level: " + std::to_string(camp.level) + "        ";
            enable = true;

            break;
        }
    }
}