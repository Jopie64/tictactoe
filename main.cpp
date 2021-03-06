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
#include <climits>

using namespace std;

const int WINVAL = 1000000000;

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

  bool get(int pos) const
  {
    return !!(v & 1 << pos);
  }
  
  void set(int pos)
  {
    v |= 1 << pos;
    if (pos < 9)
      setWin();
  }

  void reset(int pos)
  {
    v &= ~(1<<pos);
    if (pos < 9)
      setWin();
  }

  void setWin()
  {
    if (calcWin())
      set(9);
    else
      reset(9);
  }

  void clear()
  {
    v = 0;
  }

  bool isWin() const
  {
    return get(9);
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
  
private:
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

  int v;
};

class BigField
{
public:
  bool isSetAllowed(int pos) const
  {
    return isSetAllowed(pos/9, pos%9);
  }

  bool isSetAllowed(int field, int pos) const
  {
    const Field& f = v[field];
    return !f.isWin() && !f.get(pos);
  }

  bool set(int field, int pos)
  {
    Field& f = v[field];
    if (f.isWin() || f.get(pos))
      return false;
    f.set(pos);
    return true;
  }

  bool set(int pos)
  {
    return set(pos/9, pos%9);
  }

  void reset(int field, int pos)
  {
    v[field].reset(pos);
  }

  void reset(int pos)
  {
    reset(pos/9, pos%9);
  }

  void print(ostream& os)
  {
    for(int row=0; row<5; ++row)
    {
      for(int field = 0; field < 3; ++field)
      {
	if (field != 0)
	  os << " ";
	v[field].printrow(os, row);
      }
      os << endl;
    }
  }

  bool isWin() const
  {
    for(auto i : v)
      if(!i.isWin())
	return false;
    return true;
  }

  void clear()
  {
    for(auto& i : v)
      i.clear();
  }

  Field v[3];
};

int getScore(Field f, int a, int b, int* bestPos = NULL)
{
    if (f.isWin())
      return WINVAL; //Dont want to win. But other already won. So its good!
    int score = -WINVAL;
    for(int pos = 0; pos < 9; ++pos)
    {
      if (f.get(pos))
	continue;
      Field nf = f;
      nf.set(pos);
      int newScore = -getScore(nf, -b, -a);
      // How much further away, how weaker the score.
      // This causes the AI to delay its doom as long as possible
      // or to get its victory as quick as possible.
      if (newScore > 0)
	newScore -= 1;
      else if (newScore < 0)
	newScore += 1;
      if (newScore > a)
	a = newScore;
      if (a >= b)
	return a; //Beta cutoff
      if(newScore > score || (bestPos && *bestPos < 0))
      {
	score = newScore;
	if (bestPos)
	  *bestPos = pos;
      }
    }
    return score;
}

int getMove(Field f, int& score)
{
    int move = -1;
    score = getScore(f, -WINVAL, WINVAL, &move);
    return move;
}


int getScore(const BigField& f, int a, int b, int* bestPos = NULL)
{
    if (f.isWin())
      return WINVAL; //Dont want to win. But other already won. So its good!
    int score = -WINVAL;
    BigField nf = f;
    for(int pos = 0; pos < 27; ++pos)
    {
      if (!f.isSetAllowed(pos))
	continue;
      nf.set(pos);
      int newScore = -getScore(nf,-b,-a);
      // How much further away, how weaker the score.
      // This causes the AI to delay its doom as long as possible
      // or to get its victory as quick as possible.
      if (newScore > 0)
	newScore -= 1;
      else if (newScore < 0)
	newScore += 1;
      if (newScore > a)
	a = newScore;
      if (a >= b)
	return a; //Beta cutoff
      if(newScore > score || (bestPos && *bestPos < 0))
      {
	score = newScore;
	if (bestPos)
	  *bestPos = pos;
      }
      nf.reset(pos);
    }
    return score;
}

int getMove(BigField f, int& score)
{
    int move = -1;
    score = getScore(f, -WINVAL, WINVAL, &move);
    return move;
}

int main(int argc, char* argv[])
{
  BigField f;
  char c;
  bool reset = false;
  do
  {
    if (f.isWin())
      cout << "Win!" << endl;
    f.print(cout);
    cin >> c;
    int fieldIx = -1;
    if (c >= 'a' && c <= 'c')
    {
      fieldIx = c - 'a';
      cin >> c;
    }
    if (c >= '1' && c <= '9')
    {
      char p = c - '1';
      if (reset)
	f.reset(fieldIx, p);
      else
	if (!f.set(fieldIx, p))
	  cout << "Can't do that right now..." << endl;
      reset = false;
    }
    else switch(tolower(c))
    {
      case 't':
      {
	int score = 0;
	int move = getMove(f, score);
	if (move < 0)
	  cout << "Cant move..." << endl;
	else
	{
	  cout << "Move: " << move << " score: " << score << endl;
	  f.set(move);
	}
      }
      break;
      case 'r':
	reset = true;
      break;
      case 'a':
	if (reset)
	  f.clear();
	else
	  cout << "All what?" << endl;
	reset = false;
      break;
    }
  } while(c != 'q' && c != '0');
  return 0;
}
