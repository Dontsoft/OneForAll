#include "random.hpp"

#include <QRandomGenerator>

Random::Random()
{
}

// Implementing Fisher-Yates-Shuffle
// https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle

void fisher_yates(QByteArray& ba)
{
    for (int i = ba.length() - 1; i >= 1; --i)
    {
        auto j = QRandomGenerator::system()->bounded(i + 1);
        auto c = ba[i];
        ba[i] = ba[j];
        ba[j] = c;
    }
}

QByteArray Random::randomize(QByteArray& ba, int rounds) const
{
    rounds = qMax(1, rounds);
    for (int round = 0; round < rounds; ++round)
    {
        fisher_yates(ba);
    }
    return ba;
}

quint32 Random::getRandom(int highest) const
{
    if (highest == -1)
    {
        return QRandomGenerator::system()->generate();
    }
    return QRandomGenerator::system()->bounded(highest);
}
