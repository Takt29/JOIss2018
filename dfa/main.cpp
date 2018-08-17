#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <vector>

#include "automaton.h"
#include "makeEpsilonNfa.h"
#include "makeNfa.h"
#include "makeDfa.h"

using namespace std;

int match(const char *str, const char *pattern) {
  Automaton *ep_nfa = make_epsilon_nfa(pattern);
  Automaton *nfa = make_nfa(ep_nfa);
  Automaton *dfa = make_dfa(nfa);

  int n = strlen(str);
  char *matchstr = (char*)malloc(sizeof(char) * (n + 3));
  strcpy(matchstr+1, str);
  matchstr[0] = '^';
  matchstr[n+1] = '$';
  matchstr[n+2] = '\0';
  
  for(int i=-1;*matchstr;i++){
    Automaton *pc = dfa;
    const char *sp = matchstr;
    bool flag = true;
    
    while(*sp){
      if(pc == NULL) {flag = false; break;}
      if(pc->endState) break;
      if(pc->nextNodes[getId(*sp)].size() == 0) {flag = false; break;}
      assert(pc->nextNodes[getId(*sp)].size() == 1);
      pc = pc->nextNodes[getId(*sp)][0];
      sp++;
    }
    
    if(flag && pc->endState){
      return max(0,i);
    }
    
    matchstr++;
  }
  
  return -1;
}

int main(){
  char str[1024], pattern[1024];

  scanf("%s%s", str, pattern);

  int res = match(str, pattern);

  printf("res : %d\n", res);
  
  return 0;
}
