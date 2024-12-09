#pragma once
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class node
{
private:
    string name; // state node name

    bool transited; // for syncronization
  
    bool isTerminal; // 0: terminal, 1: non-terminal,  최종 매치 판단 여부를 나타낸다.
    bool state; // 0: deactivated, 1: activated 

    bool _repeat;

    char match; // target character for matching

    int n, m, count; //구간 변수
    vector<node*> next; // next link 

public:
    void init(const string& _name, bool _isTerminal, char _match);

    void addNode(node* _next);  //다음 노드를 이어준다.
   
   
    // for syncronization
    node& transition(); //다음 노드를 활성화 시키도록 준비한다.
  
    void activation();  //다음 노드를 활성화 시킨다.
  
    void input(const char& ch, bool dot); //isTerminal 이 활성화 되어있다면 문자열 찾았다는 로그 띄워준다. 아니라면 다음 노드를 활성화 시키는 방향으로

    //추가 함수들
    void cut(); //마지막 노드 짜르기
    void print_eachNode(); //생성된 상태머신 노드의 상태
    void print_linkNode(); //상태머신의 연결상태 확인
    
    char get_match(); //노드 match 정보 가져오기
    bool get_terminal(); //노드 terminal 정보 가져오기
    void set_terminal(bool set); //노드 terminal 정보 설정하기 
    void set_repeat(bool set); //구간 확인시 트리거

    void set_n(int _n); //구간 n
    void set_m(int _m); //구간 m
    void set_count(); //내부 카운터 세는 함수
    void zero_count();
    int get_n();
    int get_m();
    int get_count();
};

vector<node> buildStateMachine(const string& pattern); //상태머신 만드는 함수
void test(vector<node>& s, const string& str); 
void epsilon_active(vector<node>& s, int index); //노드를 계속해서 활성화시켜주는 함수
bool _switch = false;

