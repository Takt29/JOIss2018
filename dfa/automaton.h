#ifndef __AUTOMATON_H__
#define __AUTOMATON_H__

#include <cctype>
#include <cassert>
#include <vector>

#define N 55 //a-zA-Zep^$
#define Epsilon -1

struct Automaton{
  std::vector<Automaton*> nextNodes[N]; // a-zA-Zep
  bool endState;

  Automaton();

  void addEdge(int charId, Automaton *next);
};

int isCharacter(char c);
int getId(char c);

#endif //__AUTOMATON_H__
