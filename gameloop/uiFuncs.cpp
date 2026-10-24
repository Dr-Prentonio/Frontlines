//
// Created by tobias on 13.10.26.
//

#include "../header/UIClasses.hpp"
#include "../header/generalClasses.hpp"
#include "glm/glm.hpp"

void UI::drawToolTip(SDL_Renderer* rend) {

    SDL_Rect drect = toolTip.toolTipRect;

    SDL_SetRenderDrawColor(rend, 25, 25, 25, 200);

    Uint32 wrapLength = 0;
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended_Wrapped(
        fonts.terminessItalic,
        toolTip.toolTipText.c_str(),
        {255,255,255,255},
        wrapLength
        );
    if (!textSurface) return;

    SDL_Surface* boldTextSurface = TTF_RenderUTF8_Blended_Wrapped(
        fonts.terminessBold,
        toolTip.toolTipTitle.c_str(),
        {255,255,255, 255},
        wrapLength
        );
    if (!boldTextSurface) return;



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

void UI::topUiBar(SDL_Renderer *rend) {
    SDL_SetRenderDrawColor(rend, 25,25,25,200);

    std::string troopOverview = "Troops: " + std::to_string(player.troopAmount);
    std::string tilesOverview = "Tiles Owned: " + std::to_string(player.playerTiles.size());
    std::string citizenOverview = "Citizens: " + std::to_string(player.citizen.citizen);

    std::string text = troopOverview + " | " + tilesOverview + " | " + citizenOverview;

    int wrapLength = 0;
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended_Wrapped(
    fonts.terminessNormal,
    text.c_str(),
    {255,255,255,255},
    wrapLength
    );

    SDL_Texture* textTexture =  SDL_CreateTextureFromSurface(rend, textSurface);

    int textWidth   = textSurface->w;
    int textHeight  = textSurface->h;

    SDL_FreeSurface(textSurface);

    glm::ivec2 padding = {5, 4};

    // background drect
    SDL_Rect bgRect = {
        topUIBar.pos.x,
        topUIBar.pos.y,
        textWidth + padding.x,
        textHeight + padding.y
    };

    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);

    // background rect
    SDL_SetRenderDrawColor(rend, 25,25,25,200);
    SDL_RenderFillRect(rend, &bgRect);

    // Frame
    SDL_SetRenderDrawColor(rend, 100, 100, 100, 255);
    SDL_RenderDrawRect(rend, &bgRect);

    SDL_Rect textDrect = {
        topUIBar.pos.x + padding.x / 2,
        topUIBar.pos.y + padding.y / 2,
        textWidth,
        textHeight
    };

    SDL_RenderCopy(rend, textTexture, NULL, &textDrect);
    SDL_DestroyTexture(textTexture);
}

void UI::rightUiBar(SDL_Renderer *rend) {
    SDL_SetRenderDrawColor(rend, 25,25,25,200);

    std::string wood = "Wood: " + std::to_string(player.resources.woodAmount);
    std::string stone = "Stone: " + std::to_string(player.resources.stoneAmount);
    std::string steel = "Steel: " + std::to_string(player.resources.steelAmount);
    std::string oil = "Oil: " + std::to_string(player.resources.oilAmount);
    std::string clothing = "Clothing: " + std::to_string(player.resources.clothesAmount);

    std::string text = wood + " | " + stone + " | " + clothing + " | " + steel + " | " + oil;

    int wrapLength = 0;
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended_Wrapped(
    fonts.terminessNormal,
    text.c_str(),
    {255,255,255,255},
    wrapLength
    );

    SDL_Texture* textTexture =  SDL_CreateTextureFromSurface(rend, textSurface);

    int textWidth   = textSurface->w;
    int textHeight  = textSurface->h;

    SDL_FreeSurface(textSurface);

    glm::ivec2 padding = {5, 4};

    glm::ivec2 barPos = {
        window.scale.x - textWidth - 10,
        textHeight - 20
    };

    // background drect
    SDL_Rect bgRect = {
        barPos.x,
        barPos.y,
        textWidth + padding.x,
        textHeight + padding.y
    };

    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);

    // background rect
    SDL_SetRenderDrawColor(rend, 25,25,25,200);
    SDL_RenderFillRect(rend, &bgRect);

    // Frame
    SDL_SetRenderDrawColor(rend, 100, 100, 100, 255);
    SDL_RenderDrawRect(rend, &bgRect);

    SDL_Rect textDrect = {
        barPos.x + padding.x / 2,
        barPos.y + padding.y / 2,
        textWidth,
        textHeight
    };

    SDL_RenderCopy(rend, textTexture, NULL, &textDrect);
    SDL_DestroyTexture(textTexture);
}

void UI::bottomLeftUI(SDL_Renderer *rend) {
    SDL_SetRenderDrawColor(rend, 25,25,25,200);

    std::string text =  "––––––––––––––––––––FACTORY–––––––––––––––––––––– \n"
                        "1 Clothing Factory | 20 Wood, 50 Steel, 20 Stone \n"
                        "2 Wood Factory     | 20 Wood, 25 Steel, 50 Stone \n"
                        "3 Stone Factory    | 30 Wood, 50 Steel, 20 Stone \n"
                        "4 Steel Factory    | 30 Wood, 50 Steel, 50 Stone \n"
                        "5 Oil Factory      | 30 Wood, 100 Steel, 200 Stone \n"
                        "–––––––––––––––––––––CITY––––––––––––––––––––––––– \n"
                        "6 City             | 50 Wood, 75 Steel, 75 Stone \n";

    int wrapLength = 0;
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended_Wrapped(
    fonts.terminessNormal,
    text.c_str(),
    {255,255,255,255},
    wrapLength
    );

    SDL_Texture* textTexture =  SDL_CreateTextureFromSurface(rend, textSurface);

    int textWidth   = textSurface->w;
    int textHeight  = textSurface->h;

    SDL_FreeSurface(textSurface);

    glm::ivec2 padding = {5, 4};

    glm::ivec2 barPos = {
        10,
        window.scale.y - textHeight - 10
    };

    // background drect
    SDL_Rect bgRect = {
        barPos.x,
        barPos.y,
        textWidth + padding.x,
        textHeight + padding.y + 2
    };

    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);

    // background rect
    SDL_SetRenderDrawColor(rend, 25,25,25,200);
    SDL_RenderFillRect(rend, &bgRect);

    // Frame
    SDL_SetRenderDrawColor(rend, 100, 100, 100, 255);
    SDL_RenderDrawRect(rend, &bgRect);

    SDL_Rect textDrect = {
        barPos.x + padding.x / 2 +5,
        barPos.y + padding.y / 2,
        textWidth,
        textHeight
    };

    SDL_RenderCopy(rend, textTexture, NULL, &textDrect);
    SDL_DestroyTexture(textTexture);

    int offset = 26 + (27 * (bottomLeftUi.index - 1));

    if (bottomLeftUi.index > 5) offset += 26;

    SDL_Rect pickyDrect = {
        barPos.x + 5,
        barPos.y + 5 + offset,
        textWidth - 5,
        textHeight / 8
    };

    SDL_SetRenderDrawColor(rend, 255,255,255,100);
    SDL_RenderFillRect(rend, &pickyDrect);
}

//
//
//

// part of the game loop (child of main/draw)
void UI::UIManager(SDL_Renderer* rend) {

    if (toolTip.drawToolTip) drawToolTip(rend);

    topUiBar(rend);
    rightUiBar(rend);
    bottomLeftUI(rend);
}