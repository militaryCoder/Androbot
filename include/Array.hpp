#pragma once

#include <iostream>

template <typename T>
class Array
{
public:
    Array() = default;
    Array(size_t s) : size(s)
    {
        m_data = new T[size];
    }
    explicit Array(size_t s, T *dat) : size(s)
    {
        m_data = new T[size];
        for (unsigned int i = 0; i < size; i++)
        {
            m_data[i] = dat[i];
        }
    }

    Array(const Array &other)
    {
        this->size = other.size;
        this->m_data = new T[size];

        for (unsigned int i = 0; i < size; i++)
        {
            m_data[i] = other[i];
        }
    }

    auto rawData() const -> T *
    {
        return m_data;
    }

    T &operator[](size_t index)
    {
        if (index >= size || index < 0) throw std::invalid_argument("Attempt to access out-of-range array element.");

        return m_data[index];
    }

    const T &operator[](size_t index) const
    {
        if (index >= size || index < 0) throw std::invalid_argument("Attempt to access out-of-range array element.");

        return m_data[index];
    }

    Array &operator=(const Array &other)
    {
        if (nullptr != m_data)
        {
            delete[] m_data;
            m_data = nullptr;
        }

        this->size = other.size;
        this->m_data = new T[size];

        for (unsigned int i = 0; i < size; i++)
        {
            m_data[i] = other[i];
        }

        return *this;
    }

    ~Array()
    {
        delete[] m_data;
        m_data = nullptr;
    }

    unsigned int size = 0;

private:
    T *m_data = nullptr;
};

template <typename T>
std::ostream &operator<<(std::ostream &out, const Array<T> &arr)
{
    out << "[ ";
    for (unsigned int i = 0; i < arr.size - 1; i++)
    {
        out << arr[i] << ", ";
    }
    out << arr[arr.size - 1] << " ]";

    return out;
}