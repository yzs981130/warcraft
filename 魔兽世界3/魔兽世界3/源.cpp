#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;
#define nullptr NULL
enum e_mon_name { dragon, ninja, iceman, lion, wolf };
enum e_wea_name { sword, bomb, arrow };
enum e_hd_color { red, blue };
string wea_name[3] = {"sword", "bomb", "arrow"};
string mon_name[5] = {"dragon", "ninja", "iceman", "lion", "wolf"};
e_mon_name mon_order[2][5] = {{iceman, lion, wolf, ninja, dragon},{lion, dragon, ninja, iceman, wolf}};
string hd_color[2] = {"red", "blue"};
int mon_att[5];
int mon_cost[5];
int wea_attack[5];
int wea_cost[5];
int hour, minute;
int total_minute;
int M, N, arrow_attack, loyal_reduce;

void print_time()
{
    cout << setw(3) << setfill('0') << hour << ':' << setw(2) << minute << ' ';
}
class city;
class hd;
class wea
{
private:
    int name;
    int sword_attack;
    int arrow_left_time;
public:
    int &use_time() { return arrow_left_time; }
    int sw_atk() { return sword_attack; }
    wea(int _name, int _sword_attack):name(_name), sword_attack(_sword_attack), arrow_left_time(3) {}
    bool sword_dull()
    {
        sword_attack = int(sword_attack * 0.8);
        return sword_attack;
    }
};
class mon
{
private:
    e_mon_name name;
    int num, life, step, l_life;
    double morale;
    int att_power;
    int loyal;
    bool if_win, if_kill;
    city* city_belong_to;
    hd* hd_belong_to;
    wea* weapon[3];
public:
    mon(int _num, e_mon_name &_name, int &_life, int &t_life, hd*_hd)
    {
        name = _name;
        num = _num;
        life = _life;
        l_life = 0;
        att_power = mon_att[_name];
        step = 0;
        city_belong_to = nullptr;
        hd_belong_to = _hd;
        weapon[0] = nullptr;
        weapon[1] = nullptr;
        weapon[2] = nullptr;
        switch (name)
        {
            case dragon:
            case iceman:weapon[num % 3] = new wea(num % 3, int(mon_att[name] * 0.2)); break;
            case ninja:weapon[num % 3] = new wea(num % 3, int(mon_att[name] * 0.2));
                weapon[(num + 1) % 3] = new wea((num + 1) % 3, int(mon_att[name] * 0.2)); break;
            default:break;
        }
        if (weapon[sword] && (!weapon[sword]->sw_atk()))
            weapon[sword] = nullptr;
        morale = double(t_life) / double(life);
        if_kill = false;
        if_win = false;
        loyal = t_life;
        cout << mon_name[name] << ' ' << num << " born" << endl;
        if (name == dragon)
            cout << "Its morale is " << fixed << setprecision(2) << morale << endl;
        if (name == lion)
            cout << "Its loyalty is " << loyal << endl;
    }
    bool &IF_win() { return if_win; }
    int cal_life() { return life; }
    int fight(mon *p);
    bool bomb(mon *, int);
    void reward();
    void report();
    void dead();
    void account(mon *p)
    {
        if_win = true;
        if (if_kill)
        {
            p->dead();
            if_kill = false;
        }
        if (p->name == lion)
            life += p->l_life;
        if (name == wolf)
            for (int i = 0; i < 3; i++)
                if (p->weapon[i] && !weapon[i])
                {
                    weapon[i] = p->weapon[i];
                    p->weapon[i] = nullptr;
                }
        morale_change();
    }
    void add_life(int);
    void arrow_shot();
    void be_shoted();
    void change_city(city *p)
    {
        city_belong_to = p;
    }
    void Loyal_reduce()
    {
        loyal -= loyal_reduce;
    }
    bool weapon_(e_wea_name name) { return weapon[name]; }
    void use_bomb(mon *);
    void Fight(mon *p);
    bool minute_5();
    void minute_10(bool);
    void minute_30(int);
    void morale_change()
    {
        if (if_win)
            morale += 0.2;
        else
            morale -= 0.2;
    }
    
};
class city
{
private:
    int pos;
    int flag;//-1нч
    int flag_g[2];
    int life;
    int enemy_enter;
    mon* mon_in_city[2];
public:
    city(int n)
    {
        pos = n;
        life = 0;
        flag = -1;
        enemy_enter = 0;
        flag_g[0] = 0;
        flag_g[1] = 0;
        mon_in_city[0] = nullptr;
        mon_in_city[1] = nullptr;
    }
    int cal_pos() { return pos; }
    int cal_life() { return life; }
    void minute_5()
    {
        for (int i = 0; i < 2; i++)
            if (mon_in_city[i] && mon_in_city[i]->minute_5())
            {
                delete mon_in_city[i];
                mon_in_city[i] = nullptr;
            }
    }
    void move(city *p, int i)
    {
        mon_in_city[i] = p->mon_in_city[i];
        if (mon_in_city[i])
            mon_in_city[i]->change_city(this);
    }
    void move(mon *p, int i)
    {
        mon_in_city[i] = p;
        if (mon_in_city[i])
            mon_in_city[i]->change_city(this);
    }
    void move(hd *, int);
    bool if_exist(int i) { return mon_in_city[i]; }
    void minute_10()
    {
        for (int i = 0; i < 2; i++)
            if (mon_in_city[i])
                mon_in_city[i]->minute_10(false);
    }
    void add_life() { life += 10; }
    void minute_30()
    {
        for (int i = 0; i < 2; i++)
            if ((mon_in_city[i]) && (!(mon_in_city[1 - i])))
            {
                mon_in_city[i]->minute_30(life);
                life = 0;
                break;
            }
    }
    void minute_35(city *p[])
    {
        for (int i = 0; i < 2; i++)
            if (mon_in_city[i] && mon_in_city[i]->weapon_(arrow) && p[1 - i] && (p[1 - i]->mon_in_city[1 - i]))
            {
                mon_in_city[i]->arrow_shot();
                p[1 - i]->mon_in_city[1 - i]->be_shoted();
            }
    }
    void minute_38()
    {
        if (mon_in_city[0] && mon_in_city[1])
        {
            bool check = false;
            for (int i = 0; i < 2; i++)
                if (mon_in_city[i]->weapon_(bomb))
                {
                    if (flag == -1)
                        check = mon_in_city[i]->bomb(mon_in_city[1 - i], pos);
                    else
                        check = mon_in_city[i]->bomb(mon_in_city[i - 1], flag + 1);
                    if (check)
                    {
                        delete mon_in_city[0];
                        mon_in_city[0] = nullptr;
                        delete mon_in_city[1];
                        mon_in_city[1] = nullptr;
                        break;
                    }
                }
            if (!check && mon_in_city[0]->cal_life() <= 0 && mon_in_city[1]->cal_life() <= 0)
            {
                delete mon_in_city[0];
                mon_in_city[0] = nullptr;
                delete mon_in_city[1];
                mon_in_city[1] = nullptr;
            }
        }
    }
    void battle()
    {
        if (mon_in_city[0] && mon_in_city[1])
        {
            if (flag == -1)
            {
                if (pos % 2)
                    mon_in_city[0]->Fight(mon_in_city[1]);
                else
                    mon_in_city[1]->Fight(mon_in_city[0]);
            }
            else
                mon_in_city[flag]->Fight(mon_in_city[1 - flag]);
        }
    }
    void flag_reset(int i)
    {
        if (i == -1)
        {
            flag_g[0] = 0;
            flag_g[1] = 0;
        }
        else
        {
            flag_g[i]++;
            flag_g[1 - i] = 0;
            if (flag_g[i] == 2 && flag != i)
            {
                flag = i;
                print_time();
                cout << hd_color[i] << " flag raised in city " << pos << endl;
            }
        }
    }
    void reward(int i)
    {
        if (mon_in_city[i])
            mon_in_city[i]->reward();
    }
    void mon_report(int i)
    {
        if (mon_in_city[i])
            mon_in_city[i]->report();
    }
    void win_get()
    {
        for (int i = 0; i < 2; i++)
            if (mon_in_city[i])
            {
                if (mon_in_city[i]->IF_win())
                {
                    mon_in_city[i]->add_life(life);
                    life = 0;
                    mon_in_city[i]->IF_win() = false;
                }
                else if (mon_in_city[i]->cal_life() <= 0)
                {
                    delete mon_in_city[i];
                    mon_in_city[i] = nullptr;
                }
            }
    }
};
class hd
{
private:
    int life;
    int color;
    int pos;
    int mon_num;
    int enemy_enter;
    bool danger;
    mon* enemys[2];
    mon* warrior;
public:
    hd(int _life, int _color):life(_life), color(_color), mon_num(0), danger(false), enemy_enter(0) { enemys[1] = nullptr; enemys[2] = nullptr; }
    int &cal_life() { return life; }
    int cal_pos() { return pos; }
    int cal_color() { return color; }
    void enemy_move_in(mon *enemy)
    {
        danger = true;
        enemys[enemy_enter] = enemy;
        enemy_enter++;
    }
    string colour(bool p) { return p ? hd_color[color] : hd_color[1 - color]; }
    void move(city *p, int i)
    {
        p->move(warrior, i);
        warrior = nullptr;
    }
    bool is_occupied()
    {
        if (enemy_enter == 2)
        {
            print_time();
            cout << hd_color[color] << " headquarter was taken" << endl;
            return true;
        }
        return false;
    }
    void minute_0()
    {
        if (life >= mon_cost[mon_order[color][mon_num % 5]])
        {
            life -= mon_cost[mon_order[color][mon_num % 5]];
            print_time();
            cout << hd_color[color] << ' ';
            warrior = new mon(mon_num + 1, mon_order[color][mon_num % 5], mon_cost[mon_order[color][mon_num % 5]], life, this);
            mon_num++;
        }
    }
    void minute_5()
    {
        if (warrior && (warrior->minute_5()))
        {
            delete warrior;
            warrior = nullptr;
        }
    }
    void minute_10()
    {
        if (danger)
        {
            enemys[enemy_enter - 1]->minute_10(true);
            danger = false;
        }
    }
    void minute_30(int a_life)
    {
        life += a_life;
    }
    void report_life()
    {
        print_time();
        cout << life << " elements in " << hd_color[color] << " headquarter" << endl;
    }
    void report()
    {
        if (enemy_enter)
            enemys[enemy_enter - 1]->report();
    }
    ~hd()
    {
        if (warrior)
            delete warrior;
        if (enemys[0])
            delete enemys[0];
        if (enemys[1])
            delete enemys[1];
    }
};


int main()
{
    int t;
    cin >> t;
    for (int i = 0; i < t; i++)
    {
        hour = 0; 
        minute = 0;
        cout << "Case " << t + 1 << ' ' << endl;
        cin >> M >> N >> arrow_attack >> loyal_reduce >> total_minute;
        hd headquarter[2] = {hd(M, 0), hd(M, 1)};
        vector<city> citys;
        for (int i = 0; i < N; ++i)
        {
            city temp(i + 1);
            citys.push_back(temp);
        }
        for (int i = 0; i < 5; ++i)
            cin >> mon_cost[i];
        for (int i = 0; i < 5; ++i)
            cin >> mon_att[i];        
        while (hour * 60 <= total_minute)
        {
            headquarter[0].minute_0();
            headquarter[1].minute_0();
            minute = 5;

            if (hour * 60 + minute > total_minute)
                break;
            else
            {
                headquarter[0].minute_5();
                for (vector<city>::iterator i = citys.begin(); i != citys.end(); ++i)
                    (*i).minute_5();
                headquarter[1].minute_5();
                minute = 10;
            }

            if (hour * 60 + minute > total_minute)
                break;
            else
            {
                if (citys[0].if_exist(1))
                    citys[0].move(&headquarter[0], 1);
                for (int i = 0; i < N - 1; ++i)
                    citys[i].move(&citys[i + 1], 1);
                headquarter[1].move(&citys[N - 1], 1);
                if (citys[N - 1].if_exist(0)) 
                    citys[N - 1].move(&headquarter[1], 0);
                for (int i = N - 1; i > 0; i--)
                    citys[i].move(&citys[i - 1], 0);
                headquarter[0].move(&citys[0], 0);
                headquarter[0].minute_10();
                bool p = headquarter[0].is_occupied();
                for (int i = 0; i < N; ++i) 
                    citys[i].minute_10();
                headquarter[1].minute_10();
                if (p + headquarter[1].is_occupied()) 
                    break;                
            }
            minute = 20;

            if (hour * 60 + minute > total_minute) 
                break;
            else
                for (int i = 0; i < N; ++i) 
                    citys[i].add_life();
            minute = 30;

            if (hour * 60 + minute > total_minute) 
                break;
            else
                for (int i = 0; i < N; ++i) 
                    citys[i].minute_30();
            minute = 35;

            if (hour * 60 + minute > total_minute)
                break;
            else if (N > 1)
            {
                city *p[2] = {};
                p[0] = nullptr;
                p[1] = &citys[1];
                citys[0].minute_35(p);
                for (int i = 1; i < N - 1; i++)
                {
                    p[0] = &citys[i - 1];
                    p[1] = &citys[i + 1];
                    citys[i].minute_35(p);
                }
                p[0] = &citys[N - 2];
                p[1] = nullptr;
                citys[N - 1].minute_35(p);
                
            }
            minute = 38;

            if (hour * 60 + minute > total_minute)
                break;
            else
                for (vector<city>::iterator i = citys.begin(); i != citys.end(); ++i)
                    (*i).minute_38();
            minute = 40;
            
            if (hour * 60 + minute > total_minute)
                break;
            else
            {
                for (vector<city>::iterator i = citys.begin(); i != citys.end(); ++i)
                    (*i).battle();
                for (vector<city>::iterator i = citys.begin(); i != citys.end(); ++i)
                    (*i).reward(0);
                for (vector<city>::reverse_iterator i = citys.rbegin(); i != citys.rend(); ++i)
                    (*i).reward(1);
                for (vector<city>::iterator i = citys.begin(); i != citys.end(); ++i)
                    (*i).win_get();
            }
            minute = 50;
            
            if (hour * 60 + minute > total_minute)
                break;
            else
            {
                headquarter[0].report_life();
                headquarter[1].report_life();
            }
            minute = 55;

            if (hour * 60 + minute > total_minute)
                break;
            else
            {
                for (vector<city>::iterator i = citys.begin(); i != citys.end(); ++i)
                    (*i).mon_report(0);
                headquarter[1].report();
                headquarter[0].report();
                for (vector<city>::iterator i = citys.begin(); i != citys.end(); ++i)
                    (*i).mon_report(1);
            }
            minute = 0;
            hour++;
        }
    }
    system("pause");
    return 0;
}
int mon::fight(mon *p)
{
    if (life > 0 && p->life > 0)
    {
        int sw = 0;
        if (weapon[sword])
            sw = weapon[sword]->sw_atk();
        if (p->life - att_power - sw > 0)
        {
            if (p->name == ninja)
                return 0;
            else
            {
                int e_sw = 0;
                if (p->weapon[sword])
                    e_sw = (p->weapon[sword])->sw_atk();
                if (life - (p->att_power / 2) - e_sw > 0)
                    return 0;
                else
                    return -1;
            }
        }
        else
            return 1;
    }
    return 0;
}
bool mon::bomb(mon *cp, int p)
{
    if ((p + hd_belong_to->cal_color()) % 2)
    {
        if (fight(cp) == -1)
        {
            use_bomb(cp);
            return true;
        }
    }
    else
        if (cp->fight(this) == 1)
        {
            use_bomb(cp);
            return true;
        }
    return false;
}
void mon::reward()
{
    if (if_win && hd_belong_to->cal_life() > 7)
    {
        hd_belong_to->cal_life() -= 8;
        life += 8;
    }
}
void mon::report()
{
    bool check = false;
    print_time();
    cout << hd_belong_to->colour(true) << ' ' << mon_name[name] << ' ' << num << " has ";
    for (int i = 2; i >= 0; i--)
        if (weapon[i])
        {
            if (check)
                cout << ',';
            else
                check = true;
            cout << wea_name[i];
            if (i == 2)
                cout << '(' << weapon[arrow]->use_time() << ')';
            if (i == 0)
                cout << '(' << weapon[sword]->sw_atk() << ')';
        }
    if (!check)
        cout << "no weapon";
    cout << endl;
}
void mon::dead()
{
    print_time();
    cout << hd_belong_to->colour(true) << ' ' << mon_name[name] << ' ' << num << " was killed in city " << city_belong_to->cal_pos() << endl;
}
void mon::add_life(int life)
{
    hd_belong_to->cal_life() += life;
}
void mon::arrow_shot()
{
    print_time();
    cout << hd_belong_to->colour(true) << ' ' << mon_name[name] << ' ' << num << " shot";
    weapon[arrow]->use_time()--;
    if (!weapon[arrow]->use_time())
    {
        delete weapon[arrow];
        weapon[arrow] = nullptr;
    }
}
void mon::be_shoted()
{
    life -= arrow_attack;
    if (life <= 0)
        cout << " and killed " << hd_belong_to->colour(true) << ' ' << mon_name[name] << ' ' << num;
    cout << endl;
}
void mon::use_bomb(mon *p)
{
    print_time();
    cout << hd_belong_to->colour(true) << ' ' << mon_name[name] << ' ' << num << " used a bomb and killed " << (p->hd_belong_to)->colour(true) << ' ' << mon_name[p->name] << ' ' << p->num << endl;
}
void mon::Fight(mon *p)
{
    mon*winner = nullptr;
    mon*loser = nullptr;
    l_life = life;
    p->l_life = p->life;
    if (life > 0 && (p->life) > 0)
    {
        int sw = 0;
        if (weapon[sword])
            sw = weapon[sword]->sw_atk();
        print_time();
        cout << hd_belong_to->colour(true) << ' ' << mon_name[name] << ' ' << num << " attacked " << (p->hd_belong_to)->colour(true) << ' ' << mon_name[p->name] << ' ' << p->num << " in city " << city_belong_to->cal_pos() << " with " << life << " elements and force " << att_power << endl;
        p->life = p->life - att_power - sw;
        if (sw && !weapon[sword]->sword_dull())
        {
            delete weapon[sword];
            weapon[sword] = nullptr;
        }
        if (p->life > 0)
        {
            if (p->name == ninja)
            {
                city_belong_to->flag_reset(-1);
                morale_change();
                Loyal_reduce();
            }
            else
            {
                int e_sw = 0;
                if (p->weapon[sword])
                    e_sw = (p->weapon[sword])->sw_atk();
                print_time();
                cout << (p->hd_belong_to)->colour(true) << ' ' << mon_name[p->name] << ' ' << p->num << " fought back against " << hd_belong_to->colour(true) << ' ' << mon_name[name] << ' ' << num << " in city " << city_belong_to->cal_pos() << endl;
                life = life - (p->att_power / 2) - e_sw;
                if (e_sw && !(p->weapon[sword]->sword_dull()))
                {
                    delete p->weapon[sword];
                    p->weapon[sword] = nullptr;
                }
                if (life > 0)
                {
                    city_belong_to->flag_reset(-1);
                    morale_change();
                    p->morale_change();
                    Loyal_reduce();
                    p->Loyal_reduce();
                }
                else
                {
                    p->if_kill = true; 
                    loser = this; 
                    winner = p;
                }
            }
        }
        else
        {
            if_kill = true;
            winner = this;
            loser = p;
        }
    }
    else
    {
        if (life > 0)
        {
            p->l_life = 0;
            winner = this;
            loser = p;
        }
        else
        {
            l_life = 0;
            winner = p;
            loser = this;
        }
    }
    if (winner)
        winner->account(loser);
    if (name == dragon && life > 0 && morale > 0.8)
    {
        print_time();
        cout << (winner->hd_belong_to)->colour(true) << ' ' << mon_name[winner->name] << ' ' << winner->num << " earned " << city_belong_to->cal_life() << " elements for his headquarter" << endl;
        (winner->city_belong_to)->flag_reset((winner->hd_belong_to)->cal_color());
    }
}
bool mon::minute_5()
{
    if (name == lion && loyal <= 0)
    {
        print_time();
        cout << hd_belong_to->colour(true) << " lion " << num << " ran away" << endl;
        return true;
    }
    return false;
}
void mon::minute_10(bool if_arrive)
{
    step++;
    if (name == iceman &&!(step % 2))
    {
        if (life < 10)
            life = 1;
        else
            life -= 9;
        att_power += 20;
    }
    print_time();
    cout << hd_belong_to->colour(true) << ' ' << mon_name[name] << ' ' << num;
    if (if_arrive)
        cout << " reached " << hd_belong_to->colour(false) << " headquarter";
    else
        cout << " marched to city " << city_belong_to->cal_pos();
    cout << " with " << life << " elements and force " << att_power << endl;
}
void mon::minute_30(int add_life)
{
    print_time();
    cout << hd_belong_to->colour(true) << ' ' << num << " earned " << add_life << " elements for his headquarter" << endl;
    hd_belong_to->minute_30(add_life);
}
void city::move(hd *p, int i)
{
    p->enemy_move_in(mon_in_city[i]);
    mon_in_city[i] = nullptr;
}