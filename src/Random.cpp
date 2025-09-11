#include "Random.hpp"

namespace Aeasyds::Random
{

/* LCG_64 implementation */

#define MULT ((uint64_t) 1103515245)
#define ADDC ((uint64_t) 12345)
#define MODL ((uint64_t) 0xffffffff)

LCG_64::LCG_64(uint64_t _seed)
{
    this->seed = _seed;
    this->state = seed;
}
void LCG_64::Seed(uint64_t new_seed)
{
    seed = new_seed;
    Reset();
}
uint64_t LCG_64::Seed()
{
    return seed;
}
void LCG_64::Reset()
{
    this->state = seed;
}
uint64_t LCG_64::Rand()
{
    state = (MULT * state + ADDC);
    return state;
}

/* LFSR_64 implementation */

LFSR_64::LFSR_64(uint64_t _seed)
{
    this->seed = _seed;
    this->state = this->seed;
}
void LFSR_64::Seed(uint64_t new_seed)
{
    this->seed = new_seed;
    Reset();
}
uint64_t LFSR_64::Seed()
{
    return seed;
}
void LFSR_64::Reset()
{
    this->state = seed;
}
uint64_t LFSR_64::Rand()
{
    state ^= (state << 21);
    state ^= (state >> 30);
    state ^= (state << 4);
    return state;
}

} // namespace Aeasyds::Random
