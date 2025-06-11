#include <iostream>
#include "../include/gameManager.h"

#include "../include/movement/randomMovement.h"
#include "../include/movement/wavefrontChase.h"

GameManager* GameManager::pinstance_{nullptr};

GameManager* GameManager::GetInstance() {
    if (pinstance_ == nullptr) {
        pinstance_ = new GameManager();
    }
    return pinstance_;
}

GameManager::GameManager() {
    this->running = false;
    this->window = nullptr;
    this->renderer = nullptr;
    this->player = nullptr;
    this->pathfinder = new PathfindingSystem();
    this->currentLevelIndex = 0;
    this->currentGameState = GameState::LOADING_LEVEL;

    LevelConfig level1; level1.filePath = "maps/level1.txt"; level1.startPos = {3,0}; level1.exitPos = {3,6}; level1.enemies.push_back({3,3, EnemyAI::Behavior::RANDOM, 60}); allLevels.push_back(level1);
    LevelConfig level2; level2.filePath = "maps/level2.txt"; level2.startPos = {3,0}; level2.exitPos = {3,6}; level2.enemies.push_back({5,1, EnemyAI::Behavior::WAVEFRONT, 60}); allLevels.push_back(level2);
    LevelConfig level3; level3.filePath = "maps/level3.txt"; level3.startPos = {3,0}; level3.exitPos = {4,8}; level3.enemies.push_back({6,1, EnemyAI::Behavior::WAVEFRONT, 30}); allLevels.push_back(level3);
    LevelConfig level4; level4.filePath = "maps/level4.txt"; level4.startPos = {3,0}; level4.exitPos = {4,8}; level4.enemies.push_back({5,1, EnemyAI::Behavior::WAVEFRONT, 35}); level4.enemies.push_back({2,5, EnemyAI::Behavior::WAVEFRONT, 40}); allLevels.push_back(level4);
    LevelConfig level5; level5.filePath = "maps/level5.txt"; level5.startPos = {6,1}; level5.exitPos = {4,9}; level5.enemies.push_back({5,0, EnemyAI::Behavior::WAVEFRONT, 15}); level5.enemies.push_back({6,6, EnemyAI::Behavior::WAVEFRONT, 10}); level5.enemies.push_back({9,8, EnemyAI::Behavior::RANDOM, 45}); allLevels.push_back(level5);
}

bool GameManager::initSystems(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;
    if (fullscreen) flags = SDL_WINDOW_FULLSCREEN;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow(title, xpos, ypos, width, height, SDL_WINDOW_SHOWN | flags);
    if (window == nullptr) {
        std::cerr << "Window could not be created! Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    std::cout << "Game Systems Initialized" << std::endl;
    return true;
}

GameManager::~GameManager() {
    std::cout << "GameManager destructor called" << std::endl;
    delete this->player;
    delete this->pathfinder;
    this->player = nullptr;
    this->pathfinder = nullptr;
    for (Enemy* enemy : this->enemies) { delete enemy; } this->enemies.clear();
}

void GameManager::showDialog(Uint32 flags, const char *title, const char *message, bool showScore) {
    std::string msg = message;

    if (showScore) {
        msg = msg + "\n\nScore: " + std::to_string(score.getScore());
    }

    int result = SDL_ShowSimpleMessageBox(flags, title, msg.c_str(), window);
    if (result != 0) {
        // Error!
        std::cerr << "Error when trying to show dialog: " << SDL_GetError() << std::endl;
    }
}


void GameManager::loadLevel(int levelIndex) {
    if (levelIndex < 0 || static_cast<size_t>(levelIndex) >= allLevels.size()) {
        if (static_cast<size_t>(levelIndex) >= allLevels.size() && !allLevels.empty()) {
             std::cout << "CONGRATULATIONS! You completed all levels!" << std::endl;
             showDialog(SDL_MESSAGEBOX_INFORMATION, "Congratulations!", "You completed all the levels!", true);
        } else {
             std::cerr << "Error: Invalid level index or no levels configured." << std::endl;
        }
        setGameState(GameState::GAME_OVER); return;
    }
    currentLevelIndex = levelIndex;
    const LevelConfig& config = allLevels[currentLevelIndex];
    std::cout << "Loading Level " << currentLevelIndex + 1 << ": " << config.filePath << std::endl;
    if (!gameMap.loadMap(config.filePath, 32)) {
        std::cerr << "Failed to load map for level " << currentLevelIndex + 1 << ": " << config.filePath << std::endl;
        setGameState(GameState::GAME_OVER); return;
    }
    delete player; player = nullptr;
    for (Enemy* e : enemies) { delete e; } enemies.clear();
    currentFoundPath.clear();
    if (config.startPos.x >= 0 && config.startPos.x < gameMap.getWidth() && config.startPos.y >= 0 && config.startPos.y < gameMap.getHeight() && gameMap.getTile(config.startPos.x, config.startPos.y).isTraversable()) {
        player = new Player(config.startPos.x, config.startPos.y, gameMap.getTileSize());
    } else { std::cerr << "CRITICAL: Cannot place player on level " << levelIndex+1 << ". Check map/config." << std::endl; setGameState(GameState::GAME_OVER); return; }
    for (const auto& spawnInfo : config.enemies) {
        if (spawnInfo.startX >=0 && spawnInfo.startX < gameMap.getWidth() && spawnInfo.startY >=0 && spawnInfo.startY < gameMap.getHeight() && gameMap.getTile(spawnInfo.startX, spawnInfo.startY).isTraversable()) {
            Enemy* newEnemy = new Enemy(spawnInfo.startX, spawnInfo.startY, gameMap.getTileSize(), spawnInfo.moveDelay);
            switch (spawnInfo.behavior) {
                case EnemyAI::Behavior::WAVEFRONT: newEnemy->setMovementStrategy(new WavefrontChaseMovementStrategy()); break;
                case EnemyAI::Behavior::RANDOM:    newEnemy->setMovementStrategy(new RandomMovementStrategy());    break;
                case EnemyAI::Behavior::ASTAR_PURSUE: newEnemy->setMovementStrategy(new RandomMovementStrategy()); break; // Fallback
            }
            enemies.push_back(newEnemy);
        } else { std::cerr << "Could not place enemy on level " << levelIndex+1 << ". Check map/config." << std::endl; }
    }
    if (player) { playerWavefront = pathfinder->generateWavefrontMap(getPlayerTilePos(), gameMap); }
    setGameState(GameState::PLAYING);
    std::cout << "Level " << currentLevelIndex + 1 << " loaded. Player: (" << player->getTileX() << "," << player->getTileY() << "), Exit: (" << gameMap.getExitPosition().x << "," << gameMap.getExitPosition().y << ")" << std::endl;
}

void GameManager::run() {
    if (!initSystems("Pathfinder Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false)) { return; }
    setGameState(GameState::LOADING_LEVEL);
    running = true;
    while(running) {
        if(currentGameState == GameState::LOADING_LEVEL) {
            loadLevel(currentLevelIndex);
        }
        handleEvents();
        if(currentGameState == GameState::PLAYING) {
            update();
        }
        render();
        SDL_Delay(30);
    }
    clean();
}

void GameManager::setGameState(GameState newState) {
    if (currentGameState == newState && newState != GameState::LOADING_LEVEL) return;
    currentGameState = newState;
    currentFoundPath.clear();
    switch (currentGameState) {
        case GameState::LEVEL_COMPLETE: std::cout << "LEVEL_COMPLETE! Next..." << std::endl; goToNextLevel(); break;
        case GameState::GAME_OVER:      std::cout << "GAME_OVER!" << std::endl; running = false; break;
        case GameState::LOADING_LEVEL:  std::cout << "State: LOADING_LEVEL for level " << currentLevelIndex +1 << std::endl; break;
        case GameState::PLAYING:        std::cout << "State: PLAYING Level " << currentLevelIndex + 1 << std::endl; break;
    }
}

void GameManager::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        if (currentGameState == GameState::PLAYING && player) {
            player->handleEvent(event);
        }

        // Pathfinding with mouse.
        if (currentGameState == GameState::PLAYING && event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT && player) {
                int mouseX, mouseY; SDL_GetMouseState(&mouseX, &mouseY);
                int targetTileX = mouseX / gameMap.getTileSize(); int targetTileY = mouseY / gameMap.getTileSize();
                if (targetTileX >= 0 && targetTileX < gameMap.getWidth() && targetTileY >= 0 && targetTileY < gameMap.getHeight() && gameMap.getTile(targetTileX, targetTileY).isTraversable()) {
                    SDL_Point startPos = getPlayerTilePos(); SDL_Point endPos = {targetTileX, targetTileY};
                    currentFoundPath = pathfinder->findPath(startPos, endPos, gameMap, AlgorithmType::ASTAR);
                    score.changeScore(-20); // score penalty for showing path.
                }
            }
        }
    }
}

void GameManager::update() {
    if (!running || currentGameState != GameState::PLAYING) return;
    SDL_Point oldPlayerPos = {-1,-1}; if(player) oldPlayerPos = getPlayerTilePos();
    if (player) player->update(gameMap);
    bool playerActuallyMoved = false; if (player && (oldPlayerPos.x != player->getTileX() || oldPlayerPos.y != player->getTileY())) playerActuallyMoved = true;
    if (playerActuallyMoved) { score.changeScore(-1); playerWavefront = pathfinder->generateWavefrontMap(getPlayerTilePos(), gameMap); }
    else if (!playerActuallyMoved && player && playerWavefront.empty() && gameMap.getWidth() > 0) { playerWavefront = pathfinder->generateWavefrontMap(getPlayerTilePos(), gameMap); }
    for (Enemy* enemy : enemies) { if (enemy && player && !playerWavefront.empty()) { enemy->update(gameMap, getPlayerTilePos(), &playerWavefront); if (enemy->getTileX() == player->getTileX() && enemy->getTileY() == player->getTileY()) { score.changeScore(-30); showDialog(SDL_MESSAGEBOX_ERROR, "Game Over!", "You bumped with an enemy. You lost 30 points.", false); restartLevel(); return; } } }
    if (player) { SDL_Point exitPos = gameMap.getExitPosition(); if (exitPos.x != -1 && player->getTileX() == exitPos.x && player->getTileY() == exitPos.y) { score.changeScore(100); setGameState(GameState::LEVEL_COMPLETE); } }
}

void GameManager::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); SDL_RenderClear(renderer);
    gameMap.draw(renderer);
    if (!currentFoundPath.empty() && currentGameState == GameState::PLAYING) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 150);
        for (const auto& point : currentFoundPath) {
            SDL_Rect pathRect = { point.x * gameMap.getTileSize(), point.y * gameMap.getTileSize(), gameMap.getTileSize(), gameMap.getTileSize() };
            SDL_RenderFillRect(renderer, &pathRect);
        }
    }
    for (Enemy* enemy : enemies) { if (enemy) { enemy->draw(renderer); } }
    if (player) { player->draw(renderer); }
    SDL_RenderPresent(renderer);
}

void GameManager::clean() {
    std::cout << "Cleaning up GameManager..." << std::endl;
    delete player; player = nullptr;
    for (Enemy* enemy : enemies) { delete enemy; } enemies.clear();
    if (renderer) { SDL_DestroyRenderer(renderer); renderer = nullptr; }
    if (window) { SDL_DestroyWindow(window); window = nullptr; }
    SDL_Quit();
    std::cout << "GameManager cleaned up." << std::endl;
}

const Map& GameManager::getGameMap() const { return gameMap; }
SDL_Point GameManager::getPlayerTilePos() const { if (player) { return {player->getTileX(), player->getTileY()}; } return {-1, -1}; }
void GameManager::goToNextLevel() { currentLevelIndex++; this->restartLevel(); }
void GameManager::restartLevel() { currentGameState = GameState::LOADING_LEVEL; currentFoundPath.clear(); }