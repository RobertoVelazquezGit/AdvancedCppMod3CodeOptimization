/*
In the code below, create an optimized Structure of Arrays mplementation :
Design separate vectors for each particle property(x, y, z, vx, vy, vz, mass, id)
Implement methods to initialize particles and calculate kinetic energy
Measure performance difference between AoS and SoA approaches using at least 10, 000 particles
Run multiple iterations and calculate average execution times
*/

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <random>

// Current implementation - Array of Structures
struct Particle {
    double x, y, z;      // Position
    double vx, vy, vz;   // Velocity
    double mass;
    int id;
};

class ParticleSystemAoS {
private:
    std::vector<Particle> particles;

public:
    void resize(size_t count) {
        particles.resize(count);
    }

    void initializeParticles() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> pos(-100.0, 100.0);
        std::uniform_real_distribution<double> vel(-10.0, 10.0);
        std::uniform_real_distribution<double> mass(0.1, 10.0);

        for (size_t i = 0; i < particles.size(); ++i) {
            particles[i] = { pos(gen), pos(gen), pos(gen),
                           vel(gen), vel(gen), vel(gen),
                           mass(gen), static_cast<int>(i) };
        }
    }

    double calculateTotalKineticEnergy() const {
        double totalEnergy = 0.0;
        for (const auto& p : particles) {
            double vel2 = p.vx * p.vx + p.vy * p.vy + p.vz * p.vz;
            totalEnergy += 0.5 * p.mass * vel2;
        }
        return totalEnergy;
    }
};

class ParticleSystemSoA {
private:
    std::vector<double> x, y, z, vx, vy, vz, mass;
    std::vector<int> id;

public:
    void resize(size_t count) {
        x.resize(count); y.resize(count); z.resize(count);
        vx.resize(count); vy.resize(count); vz.resize(count);
        mass.resize(count); id.resize(count);
    }

    void initializeParticles() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> pos(-100.0, 100.0);
        std::uniform_real_distribution<double> vel(-10.0, 10.0);
        std::uniform_real_distribution<double> massDistribution(0.1, 10.0);

        for (size_t i = 0; i < x.size(); ++i) {
            x[i] = pos(gen); y[i] = pos(gen); z[i] = pos(gen);
            vx[i] = vel(gen); vy[i] = vel(gen); vz[i] = vel(gen);
            mass[i] = massDistribution(gen);
            id[i] = static_cast<int>(i);
        }
    }

    double calculateTotalKineticEnergy() const {
        double totalEnergy = 0.0;
        for (size_t i = 0; i < x.size(); ++i) {
            double vel2 = vx[i] * vx[i] + vy[i] * vy[i] + vz[i] * vz[i];
            totalEnergy += 0.5 * mass[i] * vel2;
        }
        return totalEnergy;
    }
};

int main() {
    constexpr size_t particleCount = 100000;
    constexpr int iterations = 1000;

    // SoA is faster in this benchmark because the energy calculation reads only
    // velocity and mass. Each vector is its own contiguous block of doubles,
    // improving cache locality and avoiding unused position and id fields from AoS.
    ParticleSystemAoS aos;
    ParticleSystemSoA soa;
    aos.resize(particleCount);
    soa.resize(particleCount);
    aos.initializeParticles();
    soa.initializeParticles();

    // Calentamiento: evita incluir costes de primera ejecución en la medida.
    volatile double warmup = aos.calculateTotalKineticEnergy()
                           + soa.calculateTotalKineticEnergy();
    (void)warmup;

    auto start = std::chrono::high_resolution_clock::now();
    double aosEnergy = 0.0;
    for (int i = 0; i < iterations; ++i) {
        aosEnergy += aos.calculateTotalKineticEnergy();
    }
    auto end = std::chrono::high_resolution_clock::now();
    const auto aosTime = std::chrono::duration<double, std::micro>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    double soaEnergy = 0.0;
    for (int i = 0; i < iterations; ++i) {
        soaEnergy += soa.calculateTotalKineticEnergy();
    }
    end = std::chrono::high_resolution_clock::now();
    const auto soaTime = std::chrono::duration<double, std::micro>(end - start).count();

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Particulas: " << particleCount << ", iteraciones: " << iterations << '\n';
    std::cout << "AoS - energia acumulada: " << aosEnergy
              << ", tiempo medio: " << aosTime / iterations << " us\n";
    std::cout << "SoA - energia acumulada: " << soaEnergy
              << ", tiempo medio: " << soaTime / iterations << " us\n";
    std::cout << "Aceleracion SoA/AoS: " << aosTime / soaTime << "x\n";

    return 0;
}

