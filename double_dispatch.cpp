#include<iostream>
using namespace std;

class Monster;
class Warrior;

class Creature{
public:
   virtual void fightwho(Creature&) { std::cout << "Unknown creature" << std::endl;}
    virtual void fightwho(Warrior& w) = 0;
    virtual void fightwho(Monster& m) = 0;
    virtual void fight(Creature&) =0;
};

class Monster: public Creature{
    void fightwho(Warrior& w) {cout<<"Monster versus Warrior"<<endl; }
    void fightwho(Monster& m) {cout<<"Monster versus Monster"<<endl; }
public:
    void fight(Creature& c)  {c.fightwho(*this);}
};

class Warrior: public Creature{
    void fightwho(Warrior& w) {cout<<"Warrior versus Warrior"<<endl; }
    void fightwho(Monster& m) {cout<<"Monster versus Warrior"<<endl; }
public:
    void fight(Creature& c) {c.fightwho(*this);}
};

int main()
{
   Warrior w;
   Monster m;
   Creature& c1 = w;
   Creature& c2 = m;
   c1.fight(c2);
}
