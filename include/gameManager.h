#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include <SDL.h>

#include "enemy.h"
#include "levelConfig.h"
#include "map.h"
#include "pathfindingSystem.h"
#include "player.h"

enum class GameState {
    LOADING_LEVEL,
    PLAYING,
    LEVEL_COMPLETE,
    GAME_OVER
};

class GameManager {
public:
    static GameManager* GetInstance();

    // Singleton should not be cloneable.
    GameManager(GameManager& other)=delete;

    // Singleton should not be assignable.
    void operator=(const GameManager&) = delete;

    void run();
    bool initSystems(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void loadLevel(int levelIndex);
    void goToNextLevel();

    void handleEvents();
    void update();
    void render();
    void clean();

    [[nodiscard]] bool isRunning() const { return running; }
    [[nodiscard]] SDL_Renderer* getRenderer() const { return renderer; }
    [[nodiscard]] const Map& getGameMap() const;
    [[nodiscard]] SDL_Point getPlayerTilePos() const;

    void setGameState(GameState newState);
    [[nodiscard]] GameState getCurrentState() const { return currentGameState; }
private:
    GameManager();
    ~GameManager();

    static GameManager* pinstance_;

    bool running;
    SDL_Window* window;
    SDL_Renderer* renderer;

    Map gameMap;
    Player* player;
    std::vector<Enemy*> enemies;

    PathfindingSystem* pathfinder;
    Path currentFoundPath;
    WavefrontMap playerWavefront;

    std::vector<LevelConfig> allLevels;
    int currentLevelIndex;
    GameState currentGameState;
};

#endif //GAMEMANAGER_H
