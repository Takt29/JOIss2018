#include <cassert>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <bitset>

#include "makeDfa.h"

using namespace std;

struct Bit{
  int n;
  vector<bool> bit;

  Bit(int n):n(n),bit(n,false){}

  bool operator< (const Bit &B) const {
    if(n != B.n) return n < B.n;
    return bit < B.bit;
  }
};

Automaton* make_dfa(Automaton *nfa){
  map<Automaton*, int> dic;
  vector<Automaton*> nodeList;
  int incrementId = 0;

  queue<Automaton*> que;
  que.push(nfa);
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

  const int n = incrementId;
  
  map<Bit,int> bitDic;
  int bitDicIncrementId = 0;
  vector<Automaton*> dfaNodeList;
  Bit Sbit(n); Sbit.bit[0] = true;
  Bit initBit(n);

  queue<Bit> que2;
  que2.push(Sbit);
  bitDic[Sbit] = bitDicIncrementId++;
  dfaNodeList.push_back(new Automaton);

  //printf("S: %lld\n", dfaNodeList[0]);
  
  while(que2.size()){
    auto p = que2.front(); que2.pop();
    int id = bitDic[p];
    
    for(int i=0;i<N;i++){
      if(i == getId(Epsilon)) continue;
      Bit nextBit(n);
      bool endState = false;
      for(int j=0;j<n;j++){
        if(p.bit[j]){
          for(int k=0;k<nodeList[j]->nextNodes[i].size();k++){
            nextBit.bit[dic[nodeList[j]->nextNodes[i][k]]] = true;
            endState |= nodeList[j]->nextNodes[i][k]->endState;
          }
        }
      }

      if(nextBit.bit == initBit.bit) continue;
      
      Automaton* nextNode;
      
      if(bitDic.find(nextBit) == bitDic.end()){
        nextNode = new Automaton;
        //printf("%lld %d 0\n", nextNode, endState);
        nextNode->endState = endState;
        bitDic[nextBit] = bitDicIncrementId++;
        dfaNodeList.push_back(nextNode);
        que2.push(nextBit);
      }else{
        nextNode = dfaNodeList[bitDic[nextBit]];
      }
      dfaNodeList[id]->addEdge(i, nextNode);
    }
  }
  
  for(int i=0;i<incrementId;i++){
    delete nodeList[i];
  }

  return dfaNodeList[0];
}
