#include <cassert>
#include <cctype>

#include "makeEpsilonNfa.h"

using namespace std;

struct Data{
  Automaton *start;
  Automaton *end;

  Data():start(NULL), end(NULL){}
};

/* 再帰下降法 */
// char *p = str; res = expr(p);

Data term(char* &s);
Data character(char* &s);
Data factor(char* &s);
Data expr(char* &s);

Data star(Data data){
  Data newData;

  newData.start = new Automaton;
  newData.end = new Automaton;

  newData.start->addEdge(getId(Epsilon), newData.end);
  newData.start->addEdge(getId(Epsilon), data.start);
  data.end->addEdge(getId(Epsilon), data.start);
  
  return newData;
}

//文字
Data character(char* &s){
  assert(isalpha(s[0]));
  Data res;

  res.start = res.end = new Automaton;

  while(isalpha(s[0])){
    Data data;
    data.start = new Automaton;
    data.end = new Automaton;
    data.start->addEdge(getId(s[0]), data.end);
    s++;
    
    if(s[1] == '*'){
      data = star(data);
      s++;
    }
    
    res.end->addEdge(getId(Epsilon), data.start);
    res.end = data.end;
  }
  
  return res;
}

//乗算除算(優先順位:高)
Data term(char* &s){
  Data res, data;

  res.start = new Automaton;
  res.end = new Automaton;
  
  data = factor(s);
  res.start->addEdge(getId(Epsilon), data.start);
  data.end->addEdge(getId(Epsilon), res.end);
  
  while(1){
    if(*s == '|'){
      data = factor(++s);
      res.start->addEdge(getId(Epsilon), data.start);
      data.end->addEdge(getId(Epsilon), res.end);
    } else {
      break;
    }
  }
  
  return res;
}

//括弧か数
Data factor(char* &s){
  if(*s != '(') return character(s);
  Data res = expr(++s); s++;

  if(s[0] == '*'){
    res = star(res);
    s++;
  }
  
  return res;
}

//式(優先順位: 低)
Data expr(char* &s){
  Data res = term(s);
  return res;
}

Automaton *make_epsilon_nfa(char *pattern) {
  Data data = expr(pattern);
  data.end->endState = true;

  return data.start;
}
