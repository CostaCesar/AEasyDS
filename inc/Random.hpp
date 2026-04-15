#ifndef AEASYDS_RANDOM_HPP
#define AEASYDS_RANDOM_HPP

#include <cstdint>

namespace Aeasyds::Random
{
class LCG_64
{
private:
    uint64_t state;
    uint64_t seed;

public:
    LCG_64(uint64_t _seed = 1);

    void Seed(uint64_t new_seed);
    uint64_t Seed();
    void Reset();
    uint64_t Rand();
};

class LFSR_64
{
private:
    uint64_t state;
    uint64_t seed;

public:
    LFSR_64(uint64_t _seed = 1);

    void Seed(uint64_t new_seed);
    uint64_t Seed();
    void Reset();
    uint64_t Rand();
};

} // namespace Aeasyds :: Random

#endif