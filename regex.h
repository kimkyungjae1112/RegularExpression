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
  
    bool isTerminal; // 0: terminal, 1: non-terminal,  ���� ��ġ �Ǵ� ���θ� ��Ÿ����.
    bool state; // 0: deactivated, 1: activated 

    bool _repeat;

    char match; // target character for matching

    int n, m, count; //���� ����
    vector<node*> next; // next link 

public:
    void init(const string& _name, bool _isTerminal, char _match);

    void addNode(node* _next);  //���� ��带 �̾��ش�.
   
   
    // for syncronization
    node& transition(); //���� ��带 Ȱ��ȭ ��Ű���� �غ��Ѵ�.
  
    void activation();  //���� ��带 Ȱ��ȭ ��Ų��.
  
    void input(const char& ch, bool dot); //isTerminal �� Ȱ��ȭ �Ǿ��ִٸ� ���ڿ� ã�Ҵٴ� �α� ����ش�. �ƴ϶�� ���� ��带 Ȱ��ȭ ��Ű�� ��������

    //�߰� �Լ���
    void cut(); //������ ��� ¥����
    void print_eachNode(); //������ ���¸ӽ� ����� ����
    void print_linkNode(); //���¸ӽ��� ������� Ȯ��
    
    char get_match(); //��� match ���� ��������
    bool get_terminal(); //��� terminal ���� ��������
    void set_terminal(bool set); //��� terminal ���� �����ϱ� 
    void set_repeat(bool set); //���� Ȯ�ν� Ʈ����

    void set_n(int _n); //���� n
    void set_m(int _m); //���� m
    void set_count(); //���� ī���� ���� �Լ�
    void zero_count();
    int get_n();
    int get_m();
    int get_count();
};

vector<node> buildStateMachine(const string& pattern); //���¸ӽ� ����� �Լ�
void test(vector<node>& s, const string& str); 
void epsilon_active(vector<node>& s, int index); //��带 ����ؼ� Ȱ��ȭ�����ִ� �Լ�
bool _switch = false;

