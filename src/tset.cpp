// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : BitField(mp)
{
  MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField)
{
  MaxPower = s.MaxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf)
{
  MaxPower = bf.GetLength();
}

TSet::operator TBitField()
{
    return this->BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
  BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
  BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
  BitField = s.BitField;
  MaxPower = s.MaxPower;
  return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
  return MaxPower == s.MaxPower && BitField == s.BitField;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
  return !(*this == s);
}

TSet TSet::operator+(const TSet &s) // объединение
{
  TSet result(max(MaxPower, s.MaxPower));
  result.BitField = BitField | s.BitField;
  return result;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
  TSet result(*this);
  result.BitField.SetBit(Elem);
  return result;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
  TSet result(*this);
  result.BitField.ClrBit(Elem);
  return result;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
  TSet result(max(MaxPower, s.MaxPower));
  result.BitField = BitField & s.BitField;
  return result;
}

TSet TSet::operator~(void) // дополнение
{
  TSet result(~this->BitField);
  return result;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
  ostr << "{";
  for (int i = 0; i < s.MaxPower; i++) {
    if (s.IsMember(i))
      ostr << i << " ";
  }
  ostr << "}";
  return ostr;
}
