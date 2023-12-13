# Minecraft terrain generation notes

[//]: # (https://www.youtube.com/watch?v=CSa5O6knuwI)

## Generation of chunks steps

1. Terrain shaping: which blocks are solid and which aren't. It places stone or air in each position.
2. We call which of the air blocks are beneath ocean, river or lakes and fill those with water.
3. We apply a surface layer, replacing the top blocks with things like dirt, grass, sand, etc depending of the biome
4. Finally we added features, structures such as trees, and ores and villagers

## Terrain shaping

When we generate a chunk we call `getBlock(int x, int y, int z)` for each position:
``` c++
#include <iostream>
#include <math.h> 

using BlockState = uint16_t;

enum : BlockState {
    AIR = 0,
    STONE = 1,
    WATER = 2,
};

BlockState getBlock(int x, int y, int z) {
    /*
    float frecuency = 0.1f;
    int amplitude = 10;
    
    float xOffset = sin(x * frecuency) * amplitude;
    float zOffset = sin(z * frecuency) * amplitude;
    float surfaceY = 100 + xOffset + zOffset;
    std::cout << "debug: surfaceY " << surfaceY << std::endl;
    */
    
    /*
    int octaves = 4;
    int amplitude = 20;
    
    PerlinNoise noise = new PerlinNoise(octaves)
    var surfaceY = 100 + noise.sample2d(x, z) * amplitude
    */
    
    int seaLevel = 102;
    
    if (y < surfaceY) {
        return STONE;
    } else if (y < seaLevel) {
        return WATER;
    } else {
        return AIR;
    }
}

int main() {
    BlockState air = getBlock(0, 120, 0);
    BlockState stone = getBlock(1, 90, 0);
    BlockState water = getBlock(30, 101, 40);
    
    std::cout << "air " << air << std::endl;
    std::cout << "stone " << stone << std::endl;
    std::cout << "water " << water << std::endl;

    return 0;
}
```
[//]: # (https://github.com/Auburn/FastNoise2/wiki/2:-Including-the-library-in-your-project)

Adding one octave is adding a second surfaceY calculation for example sin(x) and sin(4*x)/4 or two surfaceY calculated by PerlinNoise or whatever noise generator you use. Octaves are more cpu demanding but can be optimized (investigate).

We skipped performance optimization, such as interpolation and caching

## Spline interpolation

[//]: # (https://github.com/Hopson97/open-builder/issues/67)

[//]: # (https://github.com/KdotJPG/Simply-Improved-Terrain/blob/af61d86022d48fd32fddbe05e87aabaffdc591b8/src/main/java/jpg/k/simplyimprovedterrain/world/gen/NoiseChunkGeneratorImproved.java#L403)

We create different noise files to control height with values -1 to 1:
- continentalness
- erosion
- peaks & valleys PV

Each with their own configuration of amplitude and octaves.

![noise_files](pictures\noise_files.png "Noise files")

We can define different spline points to interpolate height in the terrain. We can easily start shaping the terrain by simply adding or moving spline points.

2d thus is only used for height.

## 3D Noise

In practice we use 3D noise for the detail shape of the terrain, it offers more variability than 2D.

3 inputs (x, y, z)
1 output ("density") from -1 to 1

Each block position has a density value, it is used to calculated the terrain shape.

The perlin noise is gradually so if we take positive as solid and negative as air we would get a cheese-like world.

Thus we apply a high bias to the density. Density is decreased the higher we go and increased the lower we go.

There is a center point let's say 100 where density is left unchanged. This squashed the terrain, when we are high enough density will always be negative and hence always be air and vice versa.

![3d_generation](pictures\3d_generation.png "3d generation")

It's controlled by two parameters squash factor and high offset. High squash factor will result in flat terrain, the opposite will result in high mountains and floating terrain, high offset will move the whole terrain up or down so that's the main terrain elevation.

Both parameters are configured via terrain shapping where spline points are configured by the three noise variables mentioned earlier.

## Cave types

Cheese caves in a 2D noise field we define white is air and black is stone.

`if noise(x, y, z) < 0 then cave`

Spaghetti caves in a 2D noise field we define the border between black and white air, white and black would be stone.

`if abs(noise(x, y, z)) < r then cave`

For spaghetti & noodle caves we take two ridged 3d noises and intersect them. I visually think of it like this: imagine two hollow oranges with a thick peel. The two oranges slightly overlap in 3d space. Keep only the part where the two peels overlap. That gives you a thick ring. Make the ring hollow, and apply some randomness to it. Result: long squiggly tunnels. Was a bit tricky to explain in the video without making it too long, so I simplified it by just showing the 2d equivalent.

## Biomes

Think of a biome as a thin layer of material on top of the terrain shape. Desert add sand, Jungle add jungle vegetation. Biomes also control which type of creatures and features appear on them.

Biome placement is based on the three noise files already mention plus two more temperature and humidity.

For that we have configuration tables. We take the different noises values in each noise file and make indexes.

![biome_explanation](pictures\biome_1.png "Biomes configuration table")
![erosion_index](pictures\erosion_index.png "Erosion index example")
![temperature_index](pictures\temperature_index.png "Temperature index example")

Other examples of noise files could be weiderness or of biome configurations parameters such as rarity, size, which biomes make sense close each other desert and badland for example.

## References

https://github.com/MovingBlocks/Terasology/wiki/Developing-Modules#understanding-terasologys-git-setup

https://github.com/Terasology/TutorialAssetSystem/wiki/Block-Attributes

https://github.com/MovingBlocks/Terasology/wiki/Entity-System-Architecture

