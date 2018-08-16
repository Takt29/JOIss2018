#ifndef __AUTOMATON_H__
#define __AUTOMATON_H__

#include <cctype>
#include <cassert>
#include <vector>

#define N 53 //a-zA-Zep
#define Epsilon -1

struct Automaton{
  std::vector<Automaton*> nextNodes[26*2 + 1]; // a-zA-Zep
  bool endState;

  Automaton();

  void addEdge(int charId, Automaton *next);
};

int getId(char c);

#endif //__AUTOMATON_H__
