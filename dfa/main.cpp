#include <cstdio>
#include <algorithm>
#include <vector>

#include "automaton.h"
#include "makeEpsilonNfa.h"
#include "makeNfa.h"
#include "makeDfa.h"

using namespace std;

int match(char *str, char *pattern) {
  Automaton *ep_nfa = make_epsilon_nfa(pattern);
  Automaton *nfa = make_nfa(ep_nfa);
  Automaton *dfa = make_dfa(nfa);
  
  Automaton *pt = dfa;

  while(*str){
    if(pt == NULL) return -1;
    if(pt->nextNodes[getId(*str)].size() == 0) return -1;
    assert(pt->nextNodes[getId(*str)].size() == 1);
    pt = pt->nextNodes[getId(*str)][0];
    str++;
  }

  if(pt->endState){
    return 0;
  } else {
    return -1;
  }
}

int main(){
  char str[1024], pattern[1024];

  scanf("%s%s", str, pattern);

  int res = match(str, pattern);

  printf("res : %d\n", res);
  
  return 0;
}
