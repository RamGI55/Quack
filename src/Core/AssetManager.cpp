//
// Created by Hyunsoo Park on 12/27/25.
//

#include "AssetManager.h"

void AssetManager::CreateFallbackTexture()
{
    const unsigned int Size = 16;
    const unsigned int CellSize = 4;

    std::vector<std::uint8_t> Pixels(Size * Size * 4); // using 0 to 255 bites.

    for (unsigned int y = 0 ; y < Size ; ++y)
    {
        for (unsigned int x = 0 ; x < Size ; ++x)
        {
            bool IsMagenta = ((x/CellSize) + (y/CellSize) % 2) == 0;

            unsigned int Idx = (y * Size + x ) *4;
            Pixels[Idx + 0 ] = IsMagenta ? 255 : 0;
            Pixels[Idx + 1 ] = 0;
            Pixels[Idx + 2] = IsMagenta ? 255 : 0;
            Pixels[Idx + 3] = 255;
        }
    }

    mFallbackTexture = std::make_shared<sf::Texture>(sf::Vector2u {Size, Size});
    mFallbackTexture->update(Pixels.data());
}
