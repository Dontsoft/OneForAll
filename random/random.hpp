#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <iterator>

#include <QByteArray>
#include <QChar>

class Random
{
public:
    Random();

    QByteArray randomize(QByteArray& ba, int rounds = 1) const;

    template <class Iterator>
    constexpr typename std::iterator_traits<Iterator>::value_type selectRandom(
        Iterator first, Iterator last, int* index = nullptr) const
    {
        const auto currentDistance = std::distance(first, last);
        const quint32 randomAdvance = getRandom(currentDistance);
        std::advance(first, randomAdvance);
        if (index != nullptr)
        {
            *index = randomAdvance;
        }
        return *first;
    }

    template <class Container>
    auto selectRandom(Container container, int* index = nullptr) const
    {
        return selectRandom(std::begin(container), std::end(container), index);
    }

private:
    quint32 getRandom(int highest = -1) const;
};

#endif // RANDOM_HPP
