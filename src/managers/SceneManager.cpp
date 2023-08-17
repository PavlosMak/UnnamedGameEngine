#include "SceneManager.h"
#include "fstream"
#include "framework/json.hpp"

SceneManager::SceneManager(const std::string &pathToScenesFile) {

    std::ifstream file(pathToScenesFile + "/scenes.json");
    nlohmann::json data = nlohmann::json::parse(file);
    std::vector sceneNames = data.get<std::vector<std::string>>();
    for(auto const& sceneName : sceneNames) {
        this->scenes.emplace_back(pathToScenesFile + "/scenes/" + sceneName);
    }
    this->currentSceneIndex = 0;
}

Scene& SceneManager::getCurrentScene() {
    return this->scenes[currentSceneIndex];
}