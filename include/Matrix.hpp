#pragma once

#include <iomanip>
#include <iostream>

#include "Array.hpp"

typedef unsigned int uint;

template <typename ValT>
class Matrix
{
public:
    Matrix() = default;
    Matrix(uint rows, uint columns) : rows(rows), columns(columns)
    {
        m_rows = Array<Array<ValT>>(rows);

        for (uint i = 0; i < rows; i++)
        {
            m_rows[i] = Array<ValT>(columns);
        }
    }

    Matrix(uint rows, uint columns, ValT *data)
        : rows(rows), columns(columns)
    {
        m_rows = Array<Array<ValT>>(rows);

        for (uint i = 0; i < rows; i++)
        {
            m_rows[i] = Array<ValT>(columns);
        }

        for (uint r = 0; r < rows; r++)
        {
            for (uint c = 0; c < columns; c++)
            {
                m_rows[r][c] = data[c + r * columns];
            }
        }
    }

    bool isQuad() const { return rows == columns; }

    // Copies matrix piece of dimensions [startRow, endRow) [startColumn, endColumn)
    Matrix copy(int startRow, int startColumn, int endRow, int endColumn) const
    {
        const int copyRows = endRow - startRow;
        const int copyCols = endColumn - startColumn;
        Matrix<ValT> copied(copyRows, copyCols);

        for (uint r = startRow; r < endRow; r++)
        {
            for (uint c = startColumn; c < endColumn; c++)
            {
                copied[r - startRow][c - startColumn] = this->operator[](r)[c];
            }
        }

        return copied;
    }

    auto operator[](uint row) -> Array<ValT> &
    {
        return m_rows[row];
    }

    auto operator[](uint row) const -> const Array<ValT> &
    {
        return m_rows[row];
    }

    template <typename Scalar = ValT>
    Matrix operator*(Scalar val)
    {
        for (uint r = 0; r < rows; r++)
        {
            for (uint c = 0; c < columns; c++)
            {
                m_rows[r][c] *= val;
            }
        }

        return *this;
    }

    template <typename Scalar = ValT>
    Matrix operator/(Scalar val)
    {
        for (uint r = 0; r < rows; r++)
        {
            for (uint c = 0; c < columns; c++)
            {
                m_rows[r][c] /= val;
            }
        }

        return *this;
    }

    template <typename Scalar = ValT>
    Matrix operator+(Scalar val)
    {
        for (uint r = 0; r < rows; r++)
        {
            for (uint c = 0; c < columns; c++)
            {
                m_rows[r][c] += val;
            }
        }

        return *this;
    }

    template <typename Scalar = ValT>
    Matrix operator-(Scalar val)
    {
        for (uint r = 0; r < rows; r++)
        {
            for (uint c = 0; c < columns; c++)
            {
                m_rows[r][c] -= val;
            }
        }

        return *this;
    }

    template <typename Value>
    friend std::ostream& operator<<(std::ostream& out, const Matrix<Value>& m);

    uint rows = 0;
    uint columns = 0;

private:
    Array<Array<ValT>> m_rows;
};

template <typename ValT>
std::ostream& operator<<(std::ostream& out, const Matrix<ValT>& m)
{
    for (uint r = 0; r < m.rows; r++)
    {
        out << "| ";
        for (uint c = 0; c < m.columns; c++)
        {
            out << std::right << std::setw(7) << m[r][c];
        }
        out << " |\n";
    }

    return out;
}