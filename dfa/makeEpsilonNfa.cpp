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

Data term(const char* &s);
Data character(const char* &s);
Data factor(const char* &s);
Data expr(const char* &s);
Data bracket(const char* &s);

Data star(Data data){
  Data newData;
  
  newData.start = new Automaton;
  newData.end = new Automaton;

  newData.start->addEdge(getId(Epsilon), newData.end);
  newData.start->addEdge(getId(Epsilon), data.start);
  data.end->addEdge(getId(Epsilon), data.start);
  data.end->addEdge(getId(Epsilon), newData.end);
  
  return newData;
}

Data plus_(Data data){
  Data newData;
  
  newData.start = new Automaton;
  newData.end = new Automaton;

  //newData.start->addEdge(getId(Epsilon), newData.end);
  newData.start->addEdge(getId(Epsilon), data.start);
  data.end->addEdge(getId(Epsilon), data.start);
  data.end->addEdge(getId(Epsilon), newData.end);
  
  return newData;
}

Data question(Data data){
  data.start->addEdge(getId(Epsilon), data.end);
  return data;
}

//文字
Data character(const char* &s){
  Data res;

  if(s[0] == '['){
    res = bracket(++s);
    s++;
  }else{
    assert(isCharacter(s[0]));
  
    res.start = new Automaton;
    res.end = new Automaton;
    res.start->addEdge(getId(s[0]), res.end);
    s++;
  }
  
  if(s[0] == '*'){
    res = star(res);
    s++;
  }

  if(s[0] == '+'){
    res = plus_(res);
    s++;
  }

  if(s[0] == '?'){
    res = question(res);
    s++;
  }
  
  return res;
}

//角括弧[a-z]
Data bracket(const char* &s){
  Data res, data;

  res.start = new Automaton;
  res.end = new Automaton;

  while(isCharacter(s[0])){
    char l = s[0], r;
    if(s[1] == '-' && s[2] != ']'){
      r = s[2];
      s += 3;
    }else{
      r = s[0];
      s++;
    }

    for(char i=l;i<=r;i++){
      res.start->addEdge(getId(i), res.end);
    }
  }

  assert(s[0] == ']');
  return res;
}

//乗算除算(優先順位:高)
Data term(const char* &s){
  Data res = factor(s);

  while(1){
    if(isCharacter(*s) || *s == '(' || *s == '['){
      Data data = factor(s);
      res.end->addEdge(getId(Epsilon), data.start);
      res.end = data.end;
    }else{
      break;
    }
  }
  return res;
}

//括弧か数
Data factor(const char* &s){
  if(*s != '(') return character(s);
  Data res = expr(++s); s++;

  if(s[0] == '*'){
    res = star(res);
    s++;
  }

  if(s[0] == '+'){
    res = plus_(res);
    s++;
  }
  
  if(s[0] == '?'){
    res = question(res);
    s++;
  }
  
  return res;
}

//式(優先順位: 低)
Data expr(const char* &s){
  Data res, data;

  res.start = new Automaton;
  res.end = new Automaton;
  
  data = term(s);
  res.start->addEdge(getId(Epsilon), data.start);
  data.end->addEdge(getId(Epsilon), res.end);
  
  while(1){
    if(*s == '|') {
      data = term(++s);
      res.start->addEdge(getId(Epsilon), data.start);
      data.end->addEdge(getId(Epsilon), res.end);
    }else{
      break;
    }
  }
  return res;
}

Automaton *make_epsilon_nfa(const char *pattern) {
  Data data = expr(pattern);
  data.end->endState = true;

  return data.start;
}
