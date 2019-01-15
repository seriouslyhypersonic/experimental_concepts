/*
 * Copyright Nuno Alves de Sousa 2019
 * nunoalvesdesousa (at) -> me <- (dot) com
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef NON_STD_NUMERICS_H
#define NON_STD_NUMERICS_H

/**
 * This file provides non-standard doubles (a double wrapped in a struct) to
 * test Matrix conversions and operator overloads
 */

#include <iostream>
#include <cmath>

/**
 * When DEBUG is defined, constructor and operator calls are printed to
 * the standard output
 */
inline void debugData(const char* info) { std::cout << info << '\n'; }
#ifdef DEBUG
#define DEBUG_DATA(info) debugData(info)
#else
#define DEBUG_DATA(info)
#endif

// Define common

struct NotBoolean // but EqualityComparable
{
    bool operator!() = delete;

    bool operator==(const NotBoolean&) { return false; }
//    NotBoolean operator==(const NotBoolean&) { return {}; }

    bool operator!=(const NotBoolean&) { return false; }
//    NotBoolean operator!=(const NotBoolean&) { return {}; }
};

struct Double
{
    double* data_;

    Double() : data_{new double{0}}
    { }

    Double(double d) : data_{new double{d}}
    {
        DEBUG_DATA("Double(double)");
    }

    ~Double(){ delete data_; }

    Double(const Double& source)
    {
        DEBUG_DATA("Double(const Double&)");
        data_ = new double{*(source.data_)};
    }

    Double(Double&& source) noexcept
    {
        DEBUG_DATA("Double(Double&&)");
        data_ = source.data_;
        source.data_ = nullptr;
    }

    Double& operator=(const Double& source) {
        if (this == &source) {
            return *this;
        }

        DEBUG_DATA("operator=(const Double&)");
        delete data_;
        data_ = new double{*(source.data_)};

        return *this;
    }

    Double& operator=(Double&& source) noexcept {
        if (this == &source) {
            return *this;
        }

        DEBUG_DATA("operator=(Double&&)");
        delete data_;
        data_ = source.data_;
        source.data_ = nullptr;

        return *this;
    }

    Double& operator+=(const Double source)
    {
        *data_ += *(source.data_);
        return *this;
    }

    Double& operator-=(const Double source)
    {
        *data_ -= *(source.data_);
        return *this;
    }

    Double& operator*=(const Double source)
    {
        *data_ *= *(source.data_);
        return *this;
    }

    Double& operator/=(const Double source)
    {
        *data_ /= *(source.data_);
        return *this;
    }

    Double& operator%=(const Double source)
    {
        *data_ = static_cast<long long>(*data_) % static_cast<long long>(*(source.data_));
        return *this;
    }

    explicit operator double() const { return *data_; }

    operator bool() const { return static_cast<int>(*data_); }

//    bool operator!() = delete;
//    void operator!() {}

    friend std::ostream& operator<<(std::ostream& out, const Double& aDouble)
    {
        return out << ((aDouble.data_)? *(aDouble.data_) : std::nan(""));
    }
};

/* Tests for concept Boolean */

//bool operator&&(const Double&, const bool) = delete;
//void operator&&(const Double&, const bool) { }

//bool operator||(const Double&, const bool) = delete;
//void operator||(const Double&, const bool) { }

//bool operator&&(const Double&, const Double&) = delete;
//void operator&&(const Double&, const Double&) { }

//bool operator&&(const bool, const Double&) = delete;
//void operator&&(const bool, const Double&) { }

//bool operator||(const Double&, const Double&) = delete;
//void operator||(const Double&, const Double&) { }

//bool operator||(const bool, const Double&) = delete;
//void operator||(const bool, const Double&) { }

//bool operator==(const Double&, const Double&) = delete;
//void operator==(const Double&, const Double&) { }

//bool operator==(const Double&, const bool) = delete;
//void operator==(const Double&, const bool) { }

//bool operator==(const bool, const Double&) = delete;
//void operator==(const bool, const Double&) { }

//bool operator!=(const Double&, const Double&) = delete;
//void operator!=(const Double&, const Double&) { }

//bool operator!=(const Double&, const bool) = delete;
//void operator!=(const Double&, const bool) { }

//bool operator!=(const bool, const Double&) = delete;
//void operator!=(const bool, const Double&) { }

/* Tests for concept EqualityComparable */
bool operator==(const Double&, const NotBoolean&) { return true; }
//NotBoolean operator==(const Double&, const NotBoolean&) { return {}; }

bool operator!=(const Double&, const NotBoolean&) { return true; }
//NotBoolean operator!=(const Double&, const NotBoolean&) { return {}; }

bool operator==(const NotBoolean&, const Double&) { return true; }
//NotBoolean operator==(const NotBoolean&, const Double&) { return {}; }

bool operator!=(const NotBoolean&, const Double&) { return true; }
//NotBoolean operator!=(const NotBoolean&, const Double&) { return {}; }

struct AlsoADouble
{
    double data;
    operator Double() const
    {
        DEBUG_DATA("Double(AlsoADouble)");
        return Double{data};
    }
};


template<>
struct std::common_type<double, Double> {
//    using type = double;
};

template<>
struct std::common_type<Double, double> {
//    using type = double;
};


#endif //NON_STD_NUMERICS_H
