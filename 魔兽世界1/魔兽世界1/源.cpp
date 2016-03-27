#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>
using namespace std;
int mon_cost[5];
string mon_que[5] = {"dragon", "ninja", "iceman", "lion", "wolf"};
int red_order[5] = {2, 3, 4, 1, 0};
int blue_order[5] = {3, 0, 1, 2, 4};
int M;
class head
{
private:
    int life;
    int order[5];   
public:
    int mon_num[5];
    int k;
    int time;
    head(int _life, int *_order)
    {
        life = _life;
        memset(mon_num, 0, sizeof(mon_num));
        for (int i = 0; i < 5; i++)
            order[i] = _order[i];
        k = 0;
        time = 0;
    }
    int cal_cost(int k)
    {
        return (life - mon_cost[order[k % 5]]);
    }
    void create(int k)
    {
        mon_num[order[k % 5]]++;
        life -= mon_cost[order[k % 5]];
    }
};
bool is_all_valid(head p, int k)
{
    for (int i = 0; i < 5; i++)
        if(p.cal_cost(k + i) >= 0)
            return true;
    return false;
}
bool is_valid(head p, int k)
{
    if (p.cal_cost(k) >= 0)
        return true;
    else
        return false;
}
int main()
{
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> M;
        for (int i = 0; i < 5; i++)
            cin >> mon_cost[i];
        cout << "Case:" << i << endl;
        head red(M, red_order), blue(M, blue_order);
        bool end_red = false, end_blue = false;
        while(true)
        {
            if (!end_red)
            {
                if (is_all_valid(red, red.k))
                {
                    for (int i = 0; i < 5; i++)
                        if (is_valid(red, red.k + i))
                        {
                            red.k += i;
                            break;
                        }
                    red.create(red.k);
                    cout << setiosflags(ios::right) << setw(3) << setfill('0') << red.time
                        << " red " << mon_que[red_order[red.k % 5]] << ' '
                        << red.time + 1 << " born with strength "
                        << mon_cost[red_order[red.k % 5]] << ',' << red.mon_num[red_order[red.k % 5]] << ' '
                        << mon_que[red_order[red.k % 5]]
                        << " in red headquarter" << endl;
                    red.time++;
                    red.k++;
                }
                else
                {
                    end_red = true;
                    cout << setiosflags(ios::right) << setw(3) << setfill('0') << red.time
                        << " red headquarter stops making warriors" << endl;
                }
            }
            if (!end_blue)
            {
                if (is_all_valid(blue, blue.time))
                {
                    for (int i = 0; i < 5; i++)
                        if (is_valid(blue, blue.k + i))
                        {
                            blue.k += i;
                            break;
                        }
                    blue.create(blue.k);
                    cout << setiosflags(ios::right) << setw(3) << setfill('0') << blue.time
                        << " blue " << mon_que[blue_order[blue.k % 5]] << ' '
                        << blue.time + 1 << " born with strength "
                        << mon_cost[blue_order[blue.k % 5]] << ',' << blue.mon_num[blue_order[blue.k % 5]]
                        << ' ' << mon_que[blue_order[blue.k % 5]]
                        << " in blue headquarter" << endl;
                    blue.k++;
                    blue.time++;
                }
                else
                {
                    end_blue = true;
                    cout << setiosflags(ios::right) << setw(3) << setfill('0') << blue.time
                        << " blue headquarter stops making warriors" << endl;
                }
            }
            if (end_red && end_blue)
                break;
        }
    }
    system("pause");
    return 0;
}