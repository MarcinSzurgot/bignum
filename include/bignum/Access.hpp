#pragma once 

#include <concepts>

#include <bignum/BigUnsigned.hpp>

namespace bignum {

struct BigUnsigned::Access {
    friend BigUnsigned;

    auto size() const { return base_.digits_.size(); }
    auto capacity() const { return base_.digits_.capacity(); }
    auto resize(std::size_t newSize) { base_.digits_.resize(newSize); }
    auto reserve(std::size_t newCapacity) { base_.digits_.reserve(newCapacity); }
    auto push_back(BigUnsigned::NativeDigit digit) { 
        if (digit) {
            base_.digits_.push_back(digit);
        }
    }

    void swap(std::vector<BigUnsigned::NativeDigit>& other) {
        std::swap(base_.digits_, other);
        base_.trim();
    }

    template<std::unsigned_integral U = BigUnsigned::NativeDigit>
    requires (sizeof(U) <= sizeof(BigUnsigned::NativeDigit))
    std::span<U> digits() { 
        auto first = reinterpret_cast<U*>(begin(base_.digits_).base());
        auto last  = reinterpret_cast<U*>(  end(base_.digits_).base());
        return {first, last};
    }

    ~Access() { base_.trim(); }

private:
    Access(BigUnsigned& base) : base_(base) { }

    BigUnsigned& base_;

};

}