#pragma once

#include "string"
#include "vector"
#include "Scene.h"

class SceneManager {
private:
    std::vector<Scene> scenes;
    int currentSceneIndex;
public:
    SceneManager(const std::string& pathToScenesFile);

    Scene& getCurrentScene();
};
