#include "Game.h"

#include "TextureManager.h"
#include "Player.h"
#include "Enemy.h"
#include "InputHandler.h"
#include "MenuState.h"
#include "PlayState.h"

#include <iostream>
using std::cout;
using std::endl;

extern const int offsetX = 17;
extern const int offsetY = -17;

//STATIC VAR INIT
Game* Game::pGame = nullptr;

Game* Game::getpGame()
{
    if(pGame == nullptr)
    {
        pGame = new Game();
        return pGame;
    }

    return pGame;
}

Game::Game()
{
    cout << " 1 C Game" << endl;
}

Game::~Game()
{
    cout << " 1 D Game" << endl;

    //release game objects
    for(size_t Index = 0; Index != gameObjects.size(); ++Index)
    {
        delete gameObjects[Index];
    }

    //release SDL-specific objects
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);

    //release game state machine object
    delete pGameStateMachine;
}

bool Game::init()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        cout << "SDL_init success!" << endl;

        pWindow =
        SDL_CreateWindow("CarRacing", SDL_WINDOWPOS_CENTERED, 100, 640, 480, 0);

        if(pWindow != NULL)
        {
            cout << "SDL_CreateWindow success!" << endl;

            pRenderer = SDL_CreateRenderer(pWindow, -1, 0);

            if(pRenderer != NULL)
            {
                cout << "SDL_CreateRenderer success!" << endl;
            }
        }
    }
    else
    {
        cout << "SDL_Init fail!" << endl;
        return false;
    }



    //create textures
    if(! TextureManager::getpTextureManager()->createTexture("images/Renault-Top_1.png",
                                                             "player_car_yellow",
                                                             pRenderer) )
    {
        return false;//don't start the loop
    }
    if(! TextureManager::getpTextureManager()->createTexture("images/Renault-Top_2.png",
                                                             "traffic_car_blue",
                                                             pRenderer) )
    {
        return false;
    }
    if(! TextureManager::getpTextureManager()->createTexture("images/Renault-Top_3.png",
                                                             "traffic_car_orange",
                                                             pRenderer) )
    {
        return false;
    }

    //create objects and push them into container
    gameObjects.push_back(new Player(new SetObjectParams("player_car_yellow", 0 + offsetX, 200 + offsetY, 45, 80, 0, 0, 90)));
    gameObjects.push_back(new Enemy(new SetObjectParams("traffic_car_blue", 0 + offsetX, 100 + offsetY, 45, 80, 0, 0, 90)));
    gameObjects.push_back(new Enemy(new SetObjectParams("traffic_car_orange", 0 + offsetX, 300 + offsetY, 45, 80, 0, 0, 90)));

    //create InputHandler object
    InputHandler::getpInputHandler();

    //create finite state machine
    pGameStateMachine = new GameStateMachine();
    pGameStateMachine->pushState(new MenuState());



    cout << "Starting the game loop!" << endl;
    return true;//start the game loop

}

void Game::getInput()
{
    //poll events
    InputHandler::getpInputHandler()->updateInputStates();

    //change state
    if(InputHandler::getpInputHandler()->getKeyState(SDL_SCANCODE_RETURN))
    {
        //secure against duplicate object creation on every 'Enter' press
        if(! (pGameStateMachine->getGameStates().top()->getStateID() == "PLAY"))
        {
            pGameStateMachine->changeState(new PlayState());
        }
    }

}

void Game::update()
{
    //update game objects destination coordinates and current frame
    for(size_t Index = 0; Index != gameObjects.size(); ++Index)
    {
        gameObjects[Index]->updateObjectParams();
    }

    //update the current game state
    pGameStateMachine->updateCurrentState();

}

void Game::render()
{
    //set the colour used for drawing operations
    SDL_SetRenderDrawColor(pRenderer, 0, 0, 255, 255);
    //clear the current rendering target with the drawing colour
    SDL_RenderClear(pRenderer);


    //draw game objects
    for(size_t Index = 0; Index != gameObjects.size(); ++Index)
    {
        gameObjects[Index]->drawObject();
    }

    //render the current game state
    pGameStateMachine->renderCurrentState();

    //update the screen with rendering performed
    SDL_RenderPresent(pRenderer);

}

void Game::clean()
{
    //ALWAYS REMEMBER TO CLEAN AND DELETE POINTERS BEFORE EXIT!!!

    delete InputHandler::getpInputHandler();//release the InputHandler object memory
    delete TextureManager::getpTextureManager();//release the TextureManager object memory
    delete pGame;//release the Game object memory
    SDL_Delay(60000);
    SDL_Quit();
}
