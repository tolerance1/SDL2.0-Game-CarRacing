#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <unordered_map>
#include <string>

#include "SDL.h"

class TextureManager
{
    public:
        ~TextureManager();

        bool createTexture(std::string fileName, std::string textureID,
                         SDL_Renderer* pRenderer);
        void drawStatic(std::string id, int destX, int destY,
                        int width, int height,
                        SDL_Renderer* pRenderer,
                        SDL_RendererFlip flip = SDL_FLIP_NONE);
        void drawAnimated(std::string id, int destX, int destY,
                          int width, int height,
                          int currentRow, int currentFrame,
                          SDL_Renderer* pRenderer,
                          double rotationAngle,
                          SDL_RendererFlip flip);
        void DestroyTextures();


        static TextureManager* getpTextureManager();//create a single instance

    private:
        static TextureManager* pTextureManager;//pointer to the instance

        //container for textures
        std::unordered_map<std::string, SDL_Texture*> textureUMap;

        TextureManager();
        TextureManager(const TextureManager& rhs);
        TextureManager& operator=(const TextureManager& rhs);
};

#endif // TEXTUREMANAGER_H
