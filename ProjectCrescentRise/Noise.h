// this file has been taken from an external sourcce, and swhould not be included in any marking for my final project

#pragma once
#include "Library.h"
#include <vector>
#include <cmath>
#include <array>

class Noise
{
private:
    // Initial permutation table
    static const std::array<int, 256> Source;
    
    static constexpr int RandomSize = 256;
    static constexpr double Sqrt3 = 1.7320508075688772935;
    static constexpr double Sqrt5 = 2.2360679774997896964;
    
    std::vector<int> _random;

    // Skewing and unskewing factors for 2D, 3D and 4D, 
    // some of them pre-multiplied.
    static constexpr double F2 = 0.5 * (Sqrt3 - 1.0);
    static constexpr double G2 = (3.0 - Sqrt3) / 6.0;
    static constexpr double G22 = G2 * 2.0 - 1;

    static constexpr double F3 = 1.0 / 3.0;
    static constexpr double G3 = 1.0 / 6.0;

    static constexpr double F4 = (Sqrt5 - 1.0) / 4.0;
    static constexpr double G4 = (5.0 - Sqrt5) / 20.0;
    static constexpr double G42 = G4 * 2.0;
    static constexpr double G43 = G4 * 3.0;
    static constexpr double G44 = G4 * 4.0 - 1.0;

    // Gradient vectors for 3D (pointing to mid points of all edges of a unit cube)
    static const std::array<std::array<int, 3>, 12> Grad3;

    // Private helper methods
    void Randomize(int seed);
    static double Dot(const std::array<int, 3>& g, double x, double y, double z);
    static double Dot(const std::array<int, 4>& g, double x, double y, double z, double t);
    static double Dot(const std::array<int, 2>& g, double x, double y);
    static int FastFloor(double x);
    static void UnpackLittleUint32(int value, std::array<unsigned char, 4>& buffer);

public:
    // Constructors
    Noise();
    Noise(int seed);

    // Main noise evaluation function
    // Generates value, typically in range [-1, 1]
    float Evaluate(const glm::vec3& point);
    
    // Alternative interface for different vector types
    float Evaluate(float x, float y, float z);
    
    // Reseed the noise generator
    void SetSeed(int seed);
};
