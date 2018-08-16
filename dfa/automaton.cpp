#include "automaton.h"

Automaton::Automaton():
  endState(false)
{}

void Automaton::addEdge(int charId, Automaton *next){
  assert(0 <= charId && charId < N);
  nextNodes[charId].push_back(next);
}

int getId(char c){
  assert(c == Epsilon || isalpha(c));

  if(c == Epsilon) return 0;
  if(islower(c)) return c - 'a' + 1;
  if(isupper(c)) return c - 'A' + 1;

  assert(0);
  return -1;
}
