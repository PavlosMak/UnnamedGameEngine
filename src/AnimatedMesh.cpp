#include "AnimatedMesh.h"

void AnimatedMesh::draw() {
    frames[lastFrame].draw();
    lastFrame = (lastFrame + 1) % totalFrames;
}
