#include <vector>
#include <map>
#include <queue>
#include <stack>
#include <cstring>

#include "makeNfa.h"

using namespace std;

Automaton *make_nfa(Automaton* ep_nfa){
  map<Automaton*, int> dic;
  vector<Automaton*> nodeList;
  int incrementId = 0;

  queue<Automaton*> que;
  que.push(ep_nfa);
  while(que.size()){
    auto p = que.front(); que.pop();
    if(p && dic.find(p) == dic.end()){

      dic[p] = incrementId++;
      nodeList.push_back(p);
      
      for(int i=0;i<N;i++)
        for(int j=0;j<p->nextNodes[i].size();j++)
          que.push(p->nextNodes[i][j]);
    }
  }

  bool dp[incrementId][incrementId][N];
  memset(dp, 0, sizeof(dp));

  for(int i=0;i<incrementId;i++) dp[i][i][getId(Epsilon)] = true;
  
  que.push(ep_nfa);
  while(que.size()){
    auto p = que.front(); que.pop();
    if(p && dic.find(p) == dic.end()){
      dic[p] = incrementId++;
      for(int i=0;i<N;i++){
        for(int j=0;j<p->nextNodes[i].size();j++){
          if(p->nextNodes[i][j] != NULL) {
            que.push(p->nextNodes[i][j]);
            dp[dic[p]][dic[p->nextNodes[i][j]]][i] = true;
          }
        }
      }
    }
  }


  for(int k=0;k<incrementId;k++){
    for(int i=0;i<incrementId;i++){
      for(int j=0;j<incrementId;j++){
        for(int c=0;c<N;c++){
          dp[i][j][c] |= dp[i][k][getId(Epsilon)] && dp[k][j][c];
          dp[i][j][c] |= dp[i][k][c] && dp[k][j][getId(Epsilon)];
        }
      }
    }
  }

  for(int i=0;i<incrementId;i++){
    for(int j=0;j<N;j++){
      nodeList[i]->nextNodes[i].resize(0);

      if(i != getId(Epsilon)){
        for(int k=0;k<incrementId;k++){
          if(dp[i][k][j]) nodeList[i]->addEdge(j, nodeList[k]);
        }
      }
    }
  }

  fprintf(stderr, "N = %d\n", incrementId);
  
  return ep_nfa;
}
