#pragma once

#include <vector>

namespace bignum
{

template<typename T>
struct NonEmptyVector
{
    using value_type             = T;
    using reference              = typename std::vector<value_type>::reference;
    using const_reference        = typename std::vector<value_type>::const_reference;
    using size_type              = typename std::vector<value_type>::size_type;
    using iterator               = typename std::vector<value_type>::iterator;
    using const_iterator         = typename std::vector<value_type>::const_iterator;
    using reverse_iterator       = typename std::vector<value_type>::reverse_iterator;
    using const_reverse_iterator = typename std::vector<value_type>::const_reverse_iterator;

    NonEmptyVector()
    : NonEmptyVector(size_type(1))
    {

    }

    NonEmptyVector(size_type size, const_reference sample = {})
    : vector_(size ? size : size_type(1), sample)
    {

    }

    NonEmptyVector(std::initializer_list<value_type> list)
    : vector_(std::empty(list) ? std::initializer_list<value_type>{value_type()} : list)
    {

    }

    void resize(size_type size__, const_reference sample = {})
    {
        if (size__)
        {
            vector_.resize(size__, sample);
        }
    }

    void pop_back()
    {
        if (vector_.size())
        {
            vector_.pop_back();
        }
    }

    void erase(const_iterator position)
    {
        if (!vector_.empty())
        {
            vector_.erase(position);
        }
    }

    void erase(const_iterator first, const_iterator last)
    {
        const auto numberOfElementsToRemove = std::distance(first, last);
        if (numberOfElementsToRemove < vector_.size())
        {
            vector_.erase(first, last);
        }
    }

    template<typename ValueType>
    void push_back(ValueType&& value)
    {
        vector_.push_back(std::forward<ValueType>(value));

    }

    reference       operator[](size_type index)       { return vector_[index]; }
    const_reference operator[](size_type index) const { return vector_[index]; }

    bool empty() const { return std::empty(vector_); }
    size_type  size() const { return  std::size(vector_); }

    friend bool empty(const NonEmptyVector& vector) { return vector.empty(); }
    friend size_type size(const NonEmptyVector& vector) { return  vector.size(); }

    friend iterator begin(NonEmptyVector& vector) { return begin(vector.vector_); }
    friend iterator   end(NonEmptyVector& vector) { return end(vector.vector_); }

    friend const_iterator begin(const NonEmptyVector& vector) { return begin(vector.vector_); }
    friend const_iterator   end(const NonEmptyVector& vector) { return end(vector.vector_); }

    friend reverse_iterator rbegin(NonEmptyVector& vector) { return rbegin(vector.vector_); }
    friend reverse_iterator   rend(NonEmptyVector& vector) { return rend(vector.vector_); }

    friend const_reverse_iterator rbegin(const NonEmptyVector& vector) { return rbegin(vector.vector_); }
    friend const_reverse_iterator   rend(const NonEmptyVector& vector) { return rend(vector.vector_); }

private:
    std::vector<value_type> vector_;
};

}
