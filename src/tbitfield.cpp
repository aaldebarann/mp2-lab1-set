// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
  if (len < 0)
    throw invalid_argument("index is out of range");
   
  BitLen = len;
  MemLen = (len - 1) / bitsInElem + 1;
  pMem = new TELEM[MemLen];
  for (int i = 0; i < MemLen; i++) {
    pMem[i] = 0;
  }
}

TBitField::TBitField(const TBitField &bf) : TBitField(bf.BitLen)
{
  memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));
}// конструктор копирования

TBitField::~TBitField()
{
  delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / bitsInElem;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << (n % bitsInElem);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
  if (n < 0 || n >= BitLen)
    throw out_of_range("index is out of range");
  pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
  if (n < 0 || n >= BitLen)
    throw out_of_range("index is out of range");
  pMem[GetMemIndex(n)] &= (~GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  if (n < 0 || n >= BitLen)
    throw out_of_range("index is out of range");
  return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
  
  if (&bf == this) // если один и тот же объект
    return *this;
  BitLen = bf.BitLen;
  if (MemLen != bf.MemLen) { // если выделено разное кол-во эл-ов
    delete[] pMem;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
  }
  memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));
  return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
  if (BitLen != bf.BitLen) // TODO: верно ли?
    return false;
  for (int i = 0; i < MemLen - 1; i++) { 
    // сравниваем биты целыми элементами
    if (pMem[i] != bf.pMem[i])
      return false;
  }
  for (int j = bitsInElem * (MemLen - 1); j < BitLen; j++) {
    // сравниваем биты последнего элемента
    if (GetBit(j) != bf.GetBit(j))
      return false;
  }
  return true;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
  TBitField result(max(BitLen, bf.BitLen));
  // при создании все биты - нули
  for (int i = 0; i < result.BitLen; i++) {
    if (i < BitLen && GetBit(i) || i < bf.BitLen && bf.GetBit(i))
      result.SetBit(i);
  }
  return result;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
  TBitField result(max(BitLen, bf.BitLen));
  // при создании все биты - нули
  for (int i = 0; i < result.BitLen; i++) {
    result.SetBit(i);
    if (i >= BitLen || i >= bf.BitLen || (!GetBit(i)) || (!bf.GetBit(i)))
      result.ClrBit(i);
  }
  return result;
}

TBitField TBitField::operator~(void) const // отрицание
{
  TBitField result(this->BitLen);
  for (int i = 0; i < MemLen; i++)
    result.pMem[i] = ~pMem[i];
  return result;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
  string iString;
  istr >> iString;
  TBitField tmp(iString.size());
  for (int i = 0; i < iString.size(); i++) {
    if (iString[i] - '0')
      bf.SetBit(i);
    else
      bf.ClrBit(i);
  }
  return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
  for (int i = 0; i < bf.BitLen; i++) {
    ostr << bf.GetBit(i);
  }
  return ostr;
}
