#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <cstdlib>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
using namespace std;
/* Head ends here */

class Median
{
public:
    void add(int v)
    {
        if (!m_top.empty() && v < *m_top.begin())
        {
            m_bottom.insert(v);
        }
        else
        {
            m_top.insert(v);
        }

        rebalance();
    }

    bool remove(int v)
    {
        bool is_removed = [&]()
        {
            if (!m_top.empty() && v < *m_top.begin())
            {
                auto it = m_bottom.find(v);
                if (it != m_bottom.end())
                {
                    m_bottom.erase(it);
                    return true;
                }
            }
            else
            {
                auto it = m_top.find(v);
                if (it != m_top.end())
                {
                    m_top.erase(it);
                    return true;
                }
            }

            return false;
        }();

        if (is_removed)
        {
            rebalance();
        }

        return is_removed;
    }

    double getMedian() const
    {
        if (m_top.size() == m_bottom.size())
        {
            return (static_cast<double>(*m_top.begin()) + static_cast<double>(*std::prev(m_bottom.end()))) / 2.;
        }
        else
        {
            return *m_top.begin();
        }
    }

    bool empty() const
    {
        return m_top.empty();
    }

private:
    void rebalance()
    {
        while (m_top.size() < m_bottom.size())
        {
            m_top.insert(*std::prev(m_bottom.end()));
            m_bottom.erase(std::prev(m_bottom.end()));
        }
        while (m_bottom.size() + 1 < m_top.size())
        {
            m_bottom.insert(*m_top.begin());
            m_top.erase(m_top.begin());
        }
    }

private:
    std::multiset<int> m_bottom;
    std::multiset<int> m_top;
};

void printMedian(double median)
{
    //double intpart;
    //auto fractpart = std::modf(median, &intpart);
    //std::cout << static_cast<int>(intpart);
    //if (std::abs(fractpart) > std::numeric_limits<double>::epsilon() * std::abs(intpart))
    //{
        //std::cout << ".5";
    //}
    //std::cout << std::endl;

    long m = median * 10.;
    auto s = std::to_string(m);
    if (m % 10 == 0)
    {
        s.pop_back();
    }
    else
    {
        auto it = s.insert(std::prev(s.end()), '.');
        if (std::abs(m) == 5)
        {
            s.insert(it, '0');
        }
    }

    if (m == 0)
    {
        s.push_back('0');
    }

    std::cout << s << std::endl;
}

void median(vector<char> s,vector<int> X) {
    Median m;

    for (int i = 0; i < s.size(); ++i)
    {
        auto c = s[i];
        auto v = X[i];

        if (c == 'r')
        {
            if (!m.empty() && m.remove(v) && !m.empty())
            {
                printMedian(m.getMedian());
            }
            else
            {
                std::cout << "Wrong!\n";
            }
        }
        else
        {
            m.add(v);
            printMedian(m.getMedian());
        }

    }
}

int main(void){

//Helpers for input and output

	int N;
	cin >> N;

	vector<char> s;
    vector<int> X;
	char temp;
    int tempint;
	for(int i = 0; i < N; i++){
		cin >> temp >> tempint;
        s.push_back(temp);
        X.push_back(tempint);
	}

	median(s,X);
	return 0;
}

