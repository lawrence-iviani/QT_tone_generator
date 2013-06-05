#ifndef PINKNOISEGENERATOR_H
#define PINKNOISEGENERATOR_H

#include <QObject>
//Free inspired from http://home.earthlink.net/~ltrammell/tech/pinkgen.c
//ref: http://home.earthlink.net/~ltrammell/tech/newpink.htm

class PinkNoiseGenerator
{
public:
    PinkNoiseGenerator();
    void getPinkNoise(short int buf[], uint bufLength);
    double generatePinkValue();

private:
    double gensum[32];
    int gsidx;
    uint lfsr;
    void setup();

    /**
     * @brief rand Generate a random double value between 0.0 and 1.0 (inclusive)
     * @return
     */
    double rand();

    /**
     * @brief rand1 Generate a pseudo-random 0 or 1.
     * @return
     */
    uint rand1();

    /**
     * @brief rand2 Generate a random value between 0 and 32767 (inclusive)
     * @return
     */
    uint rand2();
};

#endif // PINKNOISEGENERATOR_H
