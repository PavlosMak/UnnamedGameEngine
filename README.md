# The Unnamed Game Engine
![interactive material editing](/docs/images/banner.gif)

The Unnamed Game Engine, is a 3D game engine build with OpenGL and using [ENTT](https://github.com/skypjack/entt) as a backbone for its entity component system. A complete list of features (and demonstrations) is given below.

Please note that the engine is still developing and subject to big changes.

## Feature List 
At its current state the game engine supports the following features. 

### Physically Based Rendering Materials
The game engine implements various forms of PBR textures, using the [Cook-Torrance BRDF](https://citeseerx.ist.psu.edu/document?repid=rep1&type=pdf&doi=06788323a219caa856a9f9f3e6f2df9084cc9eb5) and the GGX (as demonstrated by [Burley and Brent, 2012](https://media.disneyanimation.com/uploads/production/publication_asset/48/asset/s2012_pbs_disney_brdf_notes_v3.pdf)) material models. 
Concretely the engine allows creating and editing PBR materials from scratch. An example using the interactive inspector is shown below:

![interactive material editing](/docs/images/teapot_interactive.gif)

Additionally, the engine supports textured PBR materials with albedo, normal, roughness, metallic and ambient occlusion maps. These textured materials can even be combined to create animations likes this one:

![oscillating pbr texture](/docs/images/oscillating_dragon.gif)

### Shadow Mapping
The game engine implements real-time shadows using shadow mapping and supports multiple light sources. To mitigate anti-aliasing the shadow map is filtered using *percentage closest filtering* as described by [Reeves et. al, 1987](https://artis.inrialpes.fr/Members/Cyril.Soler/DEA/Ombres/Papers/Reeves.Sig87.pdf). 

![shadow mapping](/docs/images/shadow_mapping.gif)

### Signed Distance Field Glyph Rendering
To render glyphs, the game engine implements the signed distance field introduced by [Green, 2007](https://valvearchive.com/archive/Other%20Files/Publications/SIGGRAPH2007_AlphaTestedMagnification.pdf).
With this technique we can even create effects such as animating the width of fonts as shown in the example below.

![sdf glyphs](/docs/images/sdf_glyphs.gif)

### Animation
For animations the engine supports composite cubic Bézier curves animation:

![example of Bezier curve animation](/docs/images/bunny_hopping.gif)

Furthermore, the engine allows for hand-programmed joint animations to create effects like this one:

![example of hierarchical animation](/docs/images/robot.gif)

## Running Instructions

First clone the repository. After you can either open the project in your favorite IDE or manually build and run. 

To manually build, first navigate inside the project by running: 

```bash
cd UnnamedGameEngine
```

Then make the `build` directory and navigate inside of it:

```bash
mkdir build && cd build
```

Last generate the makefiles and build the project:
```bash
cmake .. && make 
```
You should now be able to find the `Game` executable which you can use to launch the example game.

## Future work
This is a non-exhaustive list of features we would like to add to the game engine. Feel free to get in touch if you want to implement any of them (or some other feature).
- 
- [ ] Physics system
- [ ] Audio system
- [ ] Frustum Culling 
- [ ] (Animated) Skyboxes

## References 
1. [Cook, Robert L., and Kenneth E. Torrance. "A reflectance model for computer graphics." ACM Siggraph Computer Graphics 15.3 (1981): 307-316.](https://citeseerx.ist.psu.edu/document?repid=rep1&type=pdf&doi=06788323a219caa856a9f9f3e6f2df9084cc9eb5)
2. [Burley, Brent, and Walt Disney Animation Studios. "Physically-based shading at disney." ACM Siggraph. Vol. 2012. vol. 2012, 2012.](https://media.disneyanimation.com/uploads/production/publication_asset/48/asset/s2012_pbs_disney_brdf_notes_v3.pdf)
3. [Reeves, William T., David H. Salesin, and Robert L. Cook. "Rendering antialiased shadows with depth maps." Proceedings of the 14th annual conference on Computer graphics and interactive techniques. 1987.](https://artis.inrialpes.fr/Members/Cyril.Soler/DEA/Ombres/Papers/Reeves.Sig87.pdf)
4. [Green, Chris. "Improved alpha-tested magnification for vector textures and special effects." ACM Siggraph 2007 courses. 2007. 9-18.](https://valvearchive.com/archive/Other%20Files/Publications/SIGGRAPH2007_AlphaTestedMagnification.pdf)
