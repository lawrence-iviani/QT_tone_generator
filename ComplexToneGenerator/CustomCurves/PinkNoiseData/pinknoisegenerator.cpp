#include "pinknoisegenerator.h"

PinkNoiseGenerator::PinkNoiseGenerator()
{
    setup();
}


// The modified algorithm

void PinkNoiseGenerator::setup()
{
    const double pA[] = { 3.80240, 2.96940, 2.59700, 3.08700, 3.40060 };
    double pASum = 15.8564;
    int i;

    lfsr = 1;

    // Build lookup-table
    for (i = 0; i < 32; i++)
    {
        gensum[i] =
            ((((i &  1) != 0) ? pA[0] : 0) +
             (((i &  2) != 0) ? pA[1] : 0) +
             (((i &  4) != 0) ? pA[2] : 0) +
             (((i &  8) != 0) ? pA[3] : 0) +
             (((i & 16) != 0) ? pA[4] : 0));
        gensum[i] = (gensum[i] / pASum) - 0.5;
    }
    gsidx = 0;
}

double PinkNoiseGenerator::generatePinkValue()
{
    // Floating point pSum:
     const double pSum[] = { 0.00198, 0.01478, 0.06378, 0.23378, 0.91578 };
     double ur1 =  rand();

    // 13-bit pSum:
    //const int pSum[] = { 16, 121, 522, 1915, 7501 };
    //int ur1 = Rand2() >> 2; //ur1 < 8192

    // 8-bit pSum:
    // const int pSum[] = { 1, 4, 16, 60, 234 };
    // int ur1 = Rand2() >> 7; //ur1 < 256

    int ur2 = rand1();
    for (uint i = 0; i < 5; i++)
    {
        if (ur1 < pSum[i])
        {
            gsidx &= ~(1 << i);
            gsidx |= ur2 << i;
            break;
        }
    }
    return gensum[gsidx];
}


// Create a vector of noise samples using given noise_fn()
inline void PinkNoiseGenerator::getPinkNoise(short int buf[], uint bufLength)
{
    for (uint i = 0; i < bufLength; i++) {
        buf[i] = (short int)(generatePinkValue() * 32000.0);
    }
}

// Generate a random double value between 0.0 and 1.0 (inclusive)
double PinkNoiseGenerator::rand()
{ return ((double) rand2()) / 32767.0; }

// Generate a pseudo-random 0 or 1.
uint PinkNoiseGenerator::rand1()
{
    uint x = lfsr & 1;
    lfsr = (lfsr >> 1) ^ (-(signed int)(lfsr & 1) & 0xd0000001u);
    return x;
}

// Generate a random value between 0 and 32767 (inclusive)
uint PinkNoiseGenerator::rand2()
{
    int i;
    uint x = 0;

    // You may shorten this loop as needed by the pSum comparison.
    for (i = 0; i < 15; i++) {
        x = (x << 1) | rand1();
    }
    return x;
}
