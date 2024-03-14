#pragma once 

#include <concepts>

#include <bignum/BigUnsigned.hpp>

namespace bignum {

template<std::unsigned_integral Native>
struct BigUnsigned<Native>::Access {
    friend BigUnsigned;

    auto size() const { return base_.digits_.size(); }
    auto capacity() const { return base_.digits_.capacity(); }
    auto resize(std::size_t newSize) { base_.digits_.resize(newSize); }
    auto reserve(std::size_t newCapacity) { base_.digits_.reserve(newCapacity); }
    auto push_back(Native digit) { 
        if (digit) {
            base_.digits_.push_back(digit);
        }
    }

    void swap(std::vector<Native>& other) {
        std::swap(base_.digits_, other);
        base_.trim();
    }

    template<std::unsigned_integral U = Native>
    requires (sizeof(U) <= sizeof(Native))
    std::span<U> digits() { 
        auto first = reinterpret_cast<U*>(begin(base_.digits_).base());
        auto last  = reinterpret_cast<U*>(  end(base_.digits_).base());
        return {first, last};
    }

    ~Access() { base_.trim(); }

private:
    Access(BigUnsigned<Native>& base) : base_(base) { }

    BigUnsigned<Native>& base_;

};

}