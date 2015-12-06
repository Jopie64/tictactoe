/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  TicTacToe main
 *
 *        Version:  1.0
 *        Created:  03-12-15 21:27:00
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Johan 't Hart (jh), johanthart@gmail.com
 *        Company:  HartWare
 *
 * =====================================================================================
 */

#include <iostream>

using namespace std;

struct Field
{
  Field():v(0){}

  static inline int bit(char x, char y)
  {
    return y * 3 + x;
  }

  bool get(char x, char y) const
  {
    return !!(v & 1 << bit(x,y));
  }
  
  void set(char x, char y)
  {
    v |= 1 << bit(x,y);
  }
  
  bool calcWin() const
  {
    for(int i = 0; i < 3; ++i)
    {
      {
	int n = 7 << i * 3;
	if ((v & n) == n)
	  return true;
      }
      {
	int n = (1 + 8 + 64) << i;
	if ((v & n) == n)
	  return true;
      }
    }
    {
      int n = 1 + 16 + 256;
      if ((v & n) == n)
	return true;
    }
    {
      int n = 4 + 16 + 64;
      if ((v & n) == n)
	return true;
    }
    return false;
  }

  void printrow(ostream& os, int row)
  {
    char c[] = {' ', 'x'};
    if (row % 2) os << "-+-+-";
    else
    {
      row /= 2;
      os << c[get(0,row)] << '|' << c[get(1,row)] << '|' << c[get(2,row)];
    }
  }

  void print(ostream& os)
  {
    for(int row=0; row<5; ++row)
    {
      printrow(os, row);
      os << endl;
    }
  }

  int v;
};

int main(int argc, char* argv[])
{
  Field f;
  char c;
  do
  {
    if (f.calcWin())
      cout << "Win!" << endl;
    f.print(cout);
    cin >> c;
    if (c >= '1' && c <= '9')
    {
      char p = c - '1';
      f.set(p % 3, p / 3);
    }
  } while(c != 'q' && c != '0');
  return 0;
}
