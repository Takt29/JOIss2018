#include "automaton.h"

Automaton::Automaton():
  endState(false)
{}

void Automaton::addEdge(int charId, Automaton *next){
  assert(0 <= charId && charId < N);
  nextNodes[charId].push_back(next);
  //printf("%lld %lld %d\n", this, next, charId);
}

int isCharacter(char c){
  return  isalpha(c) || c == '^' || c == '$';
}

int getId(char c){
  if(c == Epsilon) return 0;
  if(islower(c)) return c - 'a' + 1;
  if(isupper(c)) return c - 'A' + 1 + 26;
  if(c == '^') return 1 + 26 * 2;
  if(c == '$') return 2 + 26 * 2;
  
  assert(0);
  return -1;
}
