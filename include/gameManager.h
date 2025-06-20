#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include <SDL.h>

#include "coin.h"
#include "enemy.h"
#include "levelConfig.h"
#include "map.h"
#include "pathfindingSystem.h"
#include "player.h"
#include "score.h"

enum class GameState {
    LOADING_LEVEL,
    PLAYING,
    LEVEL_COMPLETE,
    GAME_OVER
};

/// Main singleton class that handles core game features and establishes connections between multiple classes.
class GameManager {
public:
    /// Gets singleton instance.
    /// @return Pointer to GameManager singleton object.
    static GameManager* GetInstance();

    // Singleton should not be cloneable.
    GameManager(GameManager& other)=delete;

    // Singleton should not be assignable.
    void operator=(const GameManager&) = delete;

    ///
    /// Starts up the GameManager, setting up core features like the window and begins game loop.
    ///
    void run();

    [[nodiscard]] bool isRunning() const { return running; }
    [[nodiscard]] SDL_Renderer* getRenderer() const { return renderer; }
    [[nodiscard]] const Map& getGameMap() const;
    [[nodiscard]] SDL_Point getPlayerTilePos() const;

    [[nodiscard]] GameState getCurrentState() const { return currentGameState; }
    [[nodiscard]] Score* getScore() {return &score;}
    [[nodiscard]] WavefrontMap* getWavefrontMap() {return &playerWavefront;}

    /// Shows a dialog window.
    /// @param flags SDL dialog window flags.
    /// @param title Title of the window.
    /// @param message Message to be displayed.
    /// @param showScore Whether score/deaths should be shown at the bottom.
    void showDialog(Uint32 flags, const char* title, const char* message, bool showScore);
private:
    GameManager();
    ~GameManager();

    /// Initializes window and renderer.
    /// @param title Window title
    /// @param xpos Window x pos
    /// @param ypos Window y pos
    /// @param width Window width
    /// @param height Window height
    /// @param fullscreen Fullscreen
    /// @return TRUE if successful, FALSE if errored
    bool initSystems(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

    /// Loads level from index.
    /// @param levelIndex Index to load
    void loadLevel(int levelIndex);

    /// Loads the next level.
    void goToNextLevel();

    /// Restarts the current level.
    void restartLevel();

    /// Handles events.
    void handleEvents();

    /// Updates player, enemies and coins.
    void update();

    /// Renders all.
    void render();

    /// Cleans game in preparation for destruction.
    void clean();

    /// Sets game state.
    /// @param newState New state
    void setGameState(GameState newState);

    /// Static singleton instance.
    static GameManager* pinstance_;

    bool running;
    SDL_Window* window;
    SDL_Renderer* renderer;

    Map gameMap;
    Player* player;
    std::vector<Enemy*> enemies;
    std::vector<Coin*> coins;

    PathfindingSystem* pathfinder;
    Path currentFoundPath;
    WavefrontMap playerWavefront;

    std::vector<LevelConfig> allLevels;
    int currentLevelIndex;
    GameState currentGameState;

    Score score;
};

#endif //GAMEMANAGER_H
