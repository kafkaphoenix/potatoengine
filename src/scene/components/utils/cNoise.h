#pragma once

#include "FastNoiseLite.h"
#include "pch.h"
#include "utils/numericComparator.h"

namespace potatoengine {

struct CNoise {
    std::string _type{"simplex"};
    int seed{0};
    int octaves{
      1}; // more octaves result in more detailed and complex noise patterns
    float frequency{1.f}; // scale of the noise features
    float persistence{
      0.5f}; // how much each octave contributes to the overall shape
    float lacunarity{2.f}; // how much detail is added or removed at each octave
    int amplitude{1};      // amplitude of the noise values
    bool positive{
      false}; // whether to clamp the noise values to positive values
    FastNoiseLite noise;

    CNoise() = default;
    CNoise(std::string type, int seed, int octaves, float frequency,
           float persistence, float lacunarity, int amplitude, bool positive)
      : _type(std::move(type)), seed(seed), octaves(octaves),
        frequency(frequency), persistence(persistence), lacunarity(lacunarity),
        amplitude(amplitude), positive(positive) {}

    void print() const {
      ENGINE_BACKTRACE("\t\ttype: {0}\n\t\t\t\tseed: {1}\n\t\t\t\toctaves: "
                       "{2}\n\t\t\t\tfrequency: {3}\n\t\t\t\tpersistence: "
                       "{4}\n\t\t\t\tlacunarity: {5}\n\t\t\t\tamplitude: "
                       "{6}\n\t\t\t\tpositive: {7}",
                       _type, seed, octaves, frequency, persistence, lacunarity,
                       amplitude, positive);
    }

    std::map<std::string, std::string, NumericComparator> getInfo() const {
      std::map<std::string, std::string, NumericComparator> info;
      info["type"] = _type;
      info["seed"] = std::to_string(seed);
      info["octaves"] = std::to_string(octaves);
      info["frequency"] = std::to_string(frequency);
      info["persistence"] = std::to_string(persistence);
      info["lacunarity"] = std::to_string(lacunarity);
      info["amplitude"] = std::to_string(amplitude);
      info["positive"] = std::to_string(positive);

      return info;
    }

    float getNoise(float x, float y) const {
      float sample = noise.GetNoise(x, y);

      if (positive) {
        sample = (sample + 1.f) / 2.f;
      }

      return sample * amplitude;
    }

    void setNoiseType() {
      if (_type == "simplex") {
        noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
      } else if (_type == "perlin") {
        noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
      } else if (_type == "value") {
        noise.SetNoiseType(FastNoiseLite::NoiseType_Value);
      } else if (_type == "valueCubic") {
        noise.SetNoiseType(FastNoiseLite::NoiseType_ValueCubic);
      } else if (_type == "cellular") {
        noise.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
      } else {
        ENGINE_ASSERT(false, "Unknown noise type {}", _type);
      }
    }

    void setSeed() { noise.SetSeed(seed); }

    void setOctaves() { noise.SetFractalOctaves(octaves); }

    void setFrequency() { noise.SetFrequency(frequency); }

    void setPersistence() { noise.SetFractalGain(persistence); }

    void setLacunarity() { noise.SetFractalLacunarity(lacunarity); }
};
}
