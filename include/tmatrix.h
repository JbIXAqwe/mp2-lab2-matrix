// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
// Copyright (c) Сысоев А.В.
//
//
#include <cmath>
#ifndef __TDynamicMatrix_H__
#define __TDynamicMatrix_H__

#include <iostream>
#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <utility>

using namespace std;

const int MAX_VECTOR_SIZE = 100000000;
const int MAX_MATRIX_SIZE = 10000;

// Динамический вектор - 
// шаблонный вектор на динамической памяти
template<typename T>
class TDynamicVector
{
protected:
  size_t sz;
  T* pMem;
public:
  TDynamicVector(size_t size = 1) : sz(size)
  {
    if (sz == 0 || sz > MAX_VECTOR_SIZE)
      throw out_of_range("Vector size is out of range");
    pMem = new T[sz]();// {}; // У типа T д.б. констуктор по умолчанию
  }
  TDynamicVector(T* arr, size_t s) : TDynamicVector(s)
  {
    if (arr == nullptr)
      throw invalid_argument("Array must not be null");
    std::copy(arr, arr + sz, pMem);
  }
  TDynamicVector(const TDynamicVector& v)
  {
    sz = v.sz;
    pMem = new T[sz]();
    try
    {
      for (size_t i = 0; i < sz; i++)
        pMem[i] = v.pMem[i];
    }
    catch (...)
    {
      delete[] pMem;
      throw;
    }
  }
  TDynamicVector(TDynamicVector&& v) noexcept
  {
    sz = v.sz;
    pMem = v.pMem;
    v.sz = 0;
    v.pMem = nullptr;
  }
  ~TDynamicVector()
  {
    delete[] pMem;
  }
  TDynamicVector& operator=(const TDynamicVector& v)
  {
    if (this != &v)
    {
      TDynamicVector copy(v);
      swap(*this, copy);
    }
    return *this;
  }
  TDynamicVector& operator=(TDynamicVector&& v) noexcept
  {
    if (this != &v)
    {
      delete[] pMem;
      sz = v.sz;
      pMem = v.pMem;
      v.sz = 0;
      v.pMem = nullptr;
    }
    return *this;
  }

  size_t size() const noexcept { return sz; }

  // индексация
  T& operator[](size_t ind)
  {
    return pMem[ind];
  }
  const T& operator[](size_t ind) const
  {
    return pMem[ind];
  }
  // индексация с контролем
  T& at(size_t ind)
  {
    if (ind >= sz)
      throw out_of_range("Index out of range");
    return pMem[ind];
  }
  const T& at(size_t ind) const
  {
    if (ind >= sz)
      throw out_of_range("Index out of range");
    return pMem[ind];
  }

  // сравнение
  bool operator==(const TDynamicVector& v) const noexcept
  {
      int k = 1;
      if (sz != v.sz)
      {
          return 0;
      }
      for (size_t i = 0; i < v.sz; i++)
          k *= (pMem[i] == v.pMem[i]);
      if (k == 1)
          return 1;
      else
          return 0;
      
  }
  bool operator!=(const TDynamicVector& v) const noexcept
  {
      int k = 1;
      if (sz != v.sz)
      {
          return 1;
      }
      for (size_t i = 0; i < v.sz; i++)
          k *= (pMem[i] == v.pMem[i]);
      if (k == 0)
          return 1;
      else
          return 0;
  }

  // скалярные операции
  TDynamicVector operator+(T val)
  {
      TDynamicVector res(*this);
      for (size_t i = 0; i < sz; i++)
      {
          res.pMem[i] = pMem[i] + val;
      }
      return res;
  }
  TDynamicVector operator-(T val)
  {
      TDynamicVector res(*this);
      for (size_t i = 0; i < sz; i++)
      {
          res.pMem[i] = pMem[i] - val;
      }
      return res;
  }
  TDynamicVector operator*(T val)
  {
      TDynamicVector res(*this);
      for (size_t i = 0; i < sz; i++)
      {
          res.pMem[i] = pMem[i] * val;
      }
      return res;
  }

  // векторные операции
  TDynamicVector operator+(const TDynamicVector& v)
  {
    if (sz != v.sz)
      throw out_of_range("Vector sizes must be equal");
    TDynamicVector res(*this);
    for (size_t i = 0; i < sz; i++)
      res.pMem[i] = pMem[i] + v.pMem[i];
    return res;
  }
  TDynamicVector operator-(const TDynamicVector& v)
  {
    if (sz != v.sz)
      throw out_of_range("Vector sizes must be equal");
    TDynamicVector res(*this);
    for (size_t i = 0; i < sz; i++)
      res.pMem[i] = pMem[i] - v.pMem[i];
    return res;
  }
  T operator*(const TDynamicVector& v)
  {
    if (sz != v.sz)
      throw out_of_range("Vector sizes must be equal");
    T res = T();
    for (size_t i = 0; i < sz; i++)
      res += pMem[i] * v.pMem[i];
    return res;
  }

  friend void swap(TDynamicVector& lhs, TDynamicVector& rhs) noexcept
  {
    std::swap(lhs.sz, rhs.sz);
    std::swap(lhs.pMem, rhs.pMem);
  }

  // ввод/вывод
  friend istream& operator>>(istream& istr, TDynamicVector& v)
  {
    for (size_t i = 0; i < v.sz; i++)
      istr >> v.pMem[i]; // требуется оператор>> для типа T
    return istr;
  }
  friend ostream& operator<<(ostream& ostr, const TDynamicVector& v)
  {
    for (size_t i = 0; i < v.sz; i++)
      ostr << v.pMem[i] << ' '; // требуется оператор<< для типа T
    return ostr;
  }
};


// Динамическая матрица - 
// шаблонная матрица на динамической памяти
template<typename T>
class TDynamicMatrix : private TDynamicVector<TDynamicVector<T>>
{
  using TDynamicVector<TDynamicVector<T>>::pMem;
  using TDynamicVector<TDynamicVector<T>>::sz;

  static size_t checkSize(size_t s)
  {
    if (s == 0 || s > MAX_MATRIX_SIZE)
      throw out_of_range("Matrix size is out of range");
    return s;
  }
public:
  TDynamicMatrix(size_t s = 1) : TDynamicVector<TDynamicVector<T>>(checkSize(s))
  {
    for (size_t i = 0; i < sz; i++)
      pMem[i] = TDynamicVector<T>(sz);
  }

  using TDynamicVector<TDynamicVector<T>>::size;
  using TDynamicVector<TDynamicVector<T>>::operator[];
  using TDynamicVector<TDynamicVector<T>>::at;

  // сравнение
  bool operator==(const TDynamicMatrix& m) const noexcept
  {
    if (sz != m.sz)
      return false;
    for (size_t i = 0; i < sz; i++)
      for (size_t u = 0; u < sz; u++)
        if (!(pMem[i][u] == m.pMem[i][u]))
          return false;
    return true;
  }
  bool operator!=(const TDynamicMatrix& m) const noexcept
  {
    return !(*this == m);
  }

  // матрично-скалярные операции
  TDynamicMatrix operator*(const T& val)
  {
    TDynamicMatrix res(*this);
    for (size_t i = 0; i < sz; i++)
      for (size_t u = 0; u < sz; u++)
        res.pMem[i][u] = pMem[i][u] * val;
    return res;
  }

  // матрично-векторные операции
  TDynamicVector<T> operator*(const TDynamicVector<T>& v)
  {
    if (sz != v.size())
      throw out_of_range("Matrix and vector sizes must be equal");
    TDynamicVector<T> res(sz);
    for (size_t i = 0; i < sz; i++)
      for (size_t u = 0; u < sz; u++)
        res[i] += pMem[i][u] * v[u];
    return res;
  }

  // матрично-матричные операции
  TDynamicMatrix operator+(const TDynamicMatrix& m)
  {
    if (sz != m.sz)
      throw out_of_range("Matrix sizes must be equal");
    TDynamicMatrix res(*this);
    for (size_t i = 0; i < sz; i++)
      for (size_t u = 0; u < sz; u++)
        res.pMem[i][u] = pMem[i][u] + m.pMem[i][u];
    return res;
  }
  TDynamicMatrix operator-(const TDynamicMatrix& m)
  {
    if (sz != m.sz)
      throw out_of_range("Matrix sizes must be equal");
    TDynamicMatrix res(*this);
    for (size_t i = 0; i < sz; i++)
      for (size_t u = 0; u < sz; u++)
        res.pMem[i][u] = pMem[i][u] - m.pMem[i][u];
    return res;
  }
  TDynamicMatrix operator*(const TDynamicMatrix& m)
  {
    if (sz != m.sz)
      throw out_of_range("Matrix sizes must be equal");
    TDynamicMatrix res(sz);
    for (size_t i = 0; i < sz; i++)
      for (size_t u = 0; u < sz; u++)
        for (size_t k = 0; k < sz; k++)
          res.pMem[i][u] += pMem[i][k] * m.pMem[k][u];
    return res;
  }

  // ввод/вывод
  friend istream& operator>>(istream& istr, TDynamicMatrix& v)
  {
    for (size_t i = 0; i < v.sz; i++)
      istr >> v.pMem[i];
    return istr;
  }
  friend ostream& operator<<(ostream& ostr, const TDynamicMatrix& v)
  {
    for (size_t i = 0; i < v.sz; i++)
      ostr << v.pMem[i] << '\n';
    return ostr;
  }
};

#endif
