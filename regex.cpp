#include "regex.h"

void node::set_n(int _n) { n = _n; }
void node::set_m(int _m) { m = _m; }
void node::set_count() { ++count; }
void node::set_repeat(bool set) { _repeat = set; }
void node::set_terminal(bool set) { isTerminal = set; }
void node::zero_count() { count = 0; }
int node::get_n() { return n; }
int node::get_m() { return m; }
int node::get_count() { return count; }



char node::get_match() //노드 문자 가져오기
{
	return match;
}

bool node::get_terminal() //terminal 노드 확인
{
	return isTerminal;
}

void node::print_eachNode() //노드들 정보 출력
{
	cout << "이름 : " << name << '\n' << "itTerminal : " << isTerminal << '\n' << "match : " << match << '\n' << "repeat : " << _repeat << '\n'
		<< "transited : " << transited << endl;
}

void node::print_linkNode() //노드가 어디에 연결되어 있는지 확인
{
	if (next.empty())
	{
		cout << "다음 노드 정보 : No linked nodes." << endl;
		return;
	}

	for (int i = 0; i < next.size(); ++i)
	{
		if (next[i] != nullptr)
		{
			next[i]->print_eachNode();
		}
		else
		{
			cout << "No information available for this node." << endl;
		}
		cout << endl;
	}
	cout << "----------" << endl;
}

void node::init(const string& _name, bool _isTerminal, char _match)
{
	name = _name;
	_repeat = false;
	isTerminal = _isTerminal;
	match = _match;
	n = m = count = 0;
	state = transited = 0;
}

void node::addNode(node* _next)
{
	next.push_back(_next);
}

void node::cut()
{
	next.pop_back();
}
// for syncronization
node& node::transition()
{
	transited = 1;
	return *this;
}

void node::activation()
{
	state = transited;
	transited = 0;
}


vector<node> buildStateMachine(const string& pattern)
{

	vector<node> s;
	int nameNumber = 0;
	for (int i = 0; i < pattern.length(); ++i) //상태머신 중 노드를 만드는 과정
	{
		node new_node;

		if (pattern[i] == '.')
		{
			new_node.init("S" + to_string(nameNumber++), i == pattern.length(), '.');
			s.push_back(new_node);
		}
		else if ((pattern[i] >= 65 && pattern[i] <= 90) || (pattern[i] >= 97 && pattern[i] <= 122)) //알파벳과 점만 노드를 만든다.
		{
			new_node.init("S" + to_string(nameNumber++), i == pattern.length() - 1, pattern[i]);
			s.push_back(new_node);
		}
	}

	int i = 0, j = 0;
	while (i < s.size() - 1)
	{
		if (s[i].get_match() == pattern[j])
		{
			s[i].addNode(&s[i + 1]);
			i++;
			j++;

			if (i == s.size() - 1 && s[i].get_match() != pattern[j]) // a*b a+b a{n,m}b a|b 해결책 
			{
				if (pattern[j] == '*')
				{
					if (j == 1)
					{
						s[0].addNode(&s[0]);
						_switch = true;
					}
					else
					{
						s[i - 1].addNode(&s[i - 1]);
						s[i - 2].addNode(&s[i]);
					}
				}
				else if (pattern[j] == '+')
				{
					s[i - 1].addNode(&s[i - 1]);
				}
				else if (pattern[j] == '|')
				{
					if (s.size() == 2)
					{
						_switch = true;
						s[0].set_terminal(true);
						s[0].cut();
						return s;
					}
					s[i - 1].cut();
					s[i - 1].set_terminal(true);
					s[i - 2].addNode(&s[i]);
				}
				else if (pattern[j] == '{')
				{
					int k = 0;
					s[i - 1].set_repeat(true);
					s[i - 1].addNode(&s[i - 1]);
					while (pattern[j] != '}')
					{
						if (isdigit(pattern[j]))
						{
							k++;
							s[i - 1].set_m(pattern[j] - '0');
							if (k == 2) break;
						}
						s[i - 1].set_n(s[i - 1].get_m());
						j++;
					}
				}
				j++;
			}
		}
		else
		{
			if (pattern[j] == '*')
			{
				if (j == 1)
				{
					s[0].addNode(&s[0]);
					_switch = true;
				}
				else
				{
					s[i - 1].addNode(&s[i - 1]);
					s[i - 2].addNode(&s[i]);
				}
			}
			else if (pattern[j] == '+')
			{
				s[i - 1].addNode(&s[i - 1]);
			}
			else if (pattern[j] == '|')
			{
				s[i - 1].cut();
				s[i - 1].addNode(&s[i + 1]);
				s[i - 2].addNode(&s[i]);
			}
			else if (pattern[j] == '{')
			{
				int k = 0;
				s[i - 1].set_repeat(true);
				s[i - 1].addNode(&s[i - 1]);
				while (pattern[j] != '}')
				{
					if (isdigit(pattern[j]))
					{
						k++;
						s[i - 1].set_m(pattern[j] - '0');
						if (k == 2) break;
					}
					s[i - 1].set_n(s[i - 1].get_m());
					j++;
				}

			}

			j++;
		}

	}

	return s;
}

void node::input(const char& ch, bool dot)
{

	if (state && (match == ch))
	{
		if (isTerminal)
		{
			cout << "  >>>> accepted by " << name << endl;
		}
		if (_repeat)
		{
			transition();
			set_count();
			return;
		}
		for (int i = 0; i < next.size(); i++)
		{
			next[i]->transition(); //다음 노드를 활성화 시켜준다.
		}
	}
	else if (state && (match != ch) && _repeat)
	{
		if (get_count() >= get_n() && get_count() <= get_m())
		{
			for (int i = 0; i < next.size(); i++)
			{
				next[i]->transition().activation(); //다음 노드를 활성화 시켜준다.
			}
			_repeat = !_repeat;
		}
		zero_count();
	}

	if (dot) //점 노드가 있으면 점 노드 다음 노드는 무조건 활성화
	{
		for (int i = 0; i < next.size(); ++i)
		{
			next[i]->transition();
		}
	}
	state = 0; // deactivate this node after matching, 현재 노드는 다음 노드를 활성화 후 비활성화 상태가 된다.
}

void test(vector<node>& s, const string& str)
{
	cout << "test for ' " << str << "'" << endl;

	for (int i = 0; i < str.length(); i++)
	{
		cout << "  >> input " << str[i] << endl;

		// epsilon activation for s0, 첫 노드에는 입실론 신호에 의해 계속 활성화 상태가 유지된다.
		epsilon_active(s, 0);
		
		if (_switch) // a*b 에서 a가 안나왔을 시 해결해줄 if
		{
			epsilon_active(s, 1);
		}
		// give ch all nodes
		for (int j = 0; j < s.size(); j++)
		{
			if (s[j].get_match() == '.')
			{
				s[j].input(str[i], true);
				continue;
			}

			s[j].input(str[i], false);
		}
		// determine transited state
		for (int j = 0; j < s.size(); j++)
		{
			s[j].activation();
		}
	}
	cout << endl;
}

void epsilon_active(vector<node>& s, int index)
{
	s[index].transition().activation();
}

int main()
{
	vector<node> s = buildStateMachine("abc|de");
	
	cout << endl;
	string test1 = "abce";
	string test2 = "abde";
	test(s, test1);
	test(s, test2);

}
