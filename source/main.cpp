#include <iostream>

#include "../include/gameManager.h"

int main(int argc, char* argv[]) {
    GameManager::GetInstance()->run();
    return 0;
}
