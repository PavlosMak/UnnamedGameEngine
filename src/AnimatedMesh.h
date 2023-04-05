#include "vector"
#include "mesh.h"

class AnimatedMesh {
public:
    AnimatedMesh(std::vector<std::filesystem::path> paths) {
        totalFrames = paths.size();
        for (const auto &path: paths) {
            frames.emplace_back(path);
        }
    }

    void draw();

private:
    std::vector<GPUMesh> frames;
    int lastFrame{0};
    int totalFrames{0};
};
