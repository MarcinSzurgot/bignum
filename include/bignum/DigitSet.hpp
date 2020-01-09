#pragma once

#include <iosfwd>
#include <type_traits>
#include <vector>

namespace bignum
{

template<typename DigitType>
struct DigitSet
{
    static_assert(std::is_integral_v<DigitType>);

    using digit_type = DigitType;
    using size_type = typename std::vector<digit_type>::size_type;

    DigitSet()
    : digits_(size_type(1), digit_type())
    {

    }

    DigitSet(const std::vector<digit_type>& digits)
    : digits_(digits.empty() ? std::vector<digit_type>{digit_type()} : digits)
    {

    }

    DigitSet(std::vector<digit_type>&& digits)
    : digits_(digits.empty() ? std::vector<digit_type>{digit_type()} : std::move(digits))
    {

    }

    DigitSet(std::initializer_list<digit_type> list)
    : digits_(list.size() ? list : std::initializer_list<digit_type>{digit_type()})
    {

    }

    DigitSet(size_type size, const digit_type& sample = {})
    : digits_(size ? size : size_type(1), sample)
    {

    }

    void trim()
    {
        auto lastNonZeroIndex = size_type();
        for(auto digit = digits_.size(); digit > 0u; --digit)
        {
            if (digits_[digit - 1])
            {
                lastNonZeroIndex = digit - 1;
                break;
            }
        }
        digits_.resize(lastNonZeroIndex + 1);
    }

    void push_front(digit_type digit)
    {
        digits_.insert(begin(digits_), digit);
    }

    void push_back(digit_type digit)
    {
        digits_.push_back(digit);
    }

    void pop_back()
    {
        if (digits_.size() > size_type(1))
        {
            digits_.pop_back();
        }
    }

    void pop_front()
    {
        if (digits_.size() > size_type(1))
        {
            digits_.erase(begin(digits_));
        }
    }

    void clear()
    {
        *this = {};
    }

    void resize(size_type size)
    {
        digits_.resize(size ? size : size_type(1));
    }

    template<typename
    void edit()
    {

    }

    size_type size() const
    {
        return digits_.size();
    }

    digit_type operator[](size_type index) const
    {
        return digits_[index];
    }

    friend bool operator==(const DigitSet& lhs, const DigitSet& rhs)
    {
        return lhs.digits_ == rhs.digits_;
    }

    friend bool operator!=(const DigitSet& lhs, const DigitSet& rhs)
    {
        return !(lhs == rhs);
    }

    friend auto begin(      DigitSet& digitSet) { return begin(digitSet.digits_); }
    friend auto begin(const DigitSet& digitSet) { return begin(digitSet.digits_); }
    friend auto end(      DigitSet& digitSet) { return end(digitSet.digits_); }
    friend auto end(const DigitSet& digitSet) { return end(digitSet.digits_); }

private:
    std::vector<digit_type> digits_;
};

}
