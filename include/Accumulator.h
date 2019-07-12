#ifndef __ACCUMULATOR_H__
#define __ACCUMULATOR_H__

#include <cassert>
#include <cmath>
#include <limits>

class Accumulator
{
public:
    Accumulator() : m_count(), m_total(), m_totalSquare() {}

    Accumulator& operator<<(double data)
    {
        ++ m_count;
        m_total += data;
        m_totalSquare += data * data;
        return *this;
    }

    double mean() const
    {
        if (m_count == 0) return std::numeric_limits<double>::quiet_NaN();
        return m_total/m_count;
    }

    double error() const
    {
        if (m_count == 0) return std::numeric_limits<double>::quiet_NaN();
        // Use sigma instead of rms for an overestimate
        return sigma()/sqrt(m_count);
    }

    double rms() const
    {
        if (m_count == 0) return std::numeric_limits<double>::quiet_NaN();
        const auto meanValue(mean());
        const auto meanSquare(m_totalSquare/m_count - meanValue * meanValue);
        assert(meanSquare >= 0);
        return sqrt(meanSquare);
    }

    double sigma() const
    {
        if (m_count <= 1) return std::numeric_limits<double>::quiet_NaN();
        const auto sdRatio(sqrt(1. * m_count/(m_count - 1)));
        return sdRatio * rms();
    }

    double total() const
    {
        return m_total;
    }

    unsigned int count() const
    {
        return m_count;
    }

    Accumulator& reset()
    {
        m_count = 0;
        m_total = 0;
        m_totalSquare = 0;
        return *this;
    }

private:
    unsigned int m_count;
    double m_total;
    double m_totalSquare;
};

#endif // __ACCUMULATOR_H__
