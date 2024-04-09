#include <iostream>
#include <queue>
#include <vector>
#include <utility>
#include <climits>
#include <map>
using namespace std;
struct Rabbit
{
    int x, y, j, id;
    //(현재까지의 총 점프 횟수가 적은 토끼,
    // 현재 서있는 행 번호 + 열 번호가 작은 토끼,
    // 행 번호가 작은 토끼,
    // 열 번호가 작은 토끼,
    // 고유번호가 작은 토끼)
    bool operator<(const Rabbit &b) const
    {
        if (j != b.j)
            return j > b.j;
        if (x + y != b.x + b.y)
            return x + y > b.x + b.y;
        if (x != b.x)
            return x > b.x;
        if (y != b.y)
            return y > b.y;
        return id > b.id;
    }
};

int N, M, Q, ins, num_of_rabbit; // 격자는 1,1 부터
priority_queue<Rabbit> Rb;
map<int, int> id_to_idx; // i번째 토끼의
int rb_id[2000];
int dist[2000];
int position[2000][2]; // x y 좌표
int last_jump[2000];
long long point[2000];
void ready();
void run_rabbit();
void change_dist();
void show_best();
void show_rb();

void run_rabbit()
{
    bool runned[2000] = {0};
    int K, S;
    cin >> K >> S;
    while (K--)
    {
        Rabbit now_r = Rb.top(); // 우선순위 제일 높은 애
        Rb.pop();
        int idx = id_to_idx[now_r.id];
        int x0 = position[idx][0];
        int y0 = position[idx][1];
        int d = dist[idx];
        runned[idx] = 1;
        // cout << "selected: " << idx << endl;
        // 상하좌우 중 선정
        int x1, x2, y1, y2; // 상 하 좌 우  ---> 한번 확인
        // 하
        int res_x2 = (d + (x0 - 1)) % (2 * (N - 1));
        if (res_x2 < N - 1)
            x2 = 1 + res_x2;
        else
            x2 = 1 + (2 * (N - 1) - res_x2);
        // 우
        int res_y2 = (d + (y0 - 1)) % (2 * (M - 1));
        if (res_y2 < M - 1)
            y2 = 1 + res_y2;
        else
            y2 = 1 + (2 * (M - 1) - res_y2);
        ///////////////////////////////////////////////
        // 상
        int res_x1 = (d + (N - x0)) % (2 * (N - 1));
        if (res_x1 < N - 1)
            x1 = N - res_x1;
        else
            x1 = N - (2 * (N - 1) - res_x1);
        // 좌
        int res_y1 = (d + (M - y0)) % (2 * (M - 1));
        if (res_y1 < M - 1)
            y1 = M - res_y1;
        else
            y1 = M - (2 * (M - 1) - res_y1);
        vector<pair<int, int>> v;
        v.push_back(make_pair(x0, y1));
        v.push_back(make_pair(x0, y2));
        v.push_back(make_pair(x1, y0));
        v.push_back(make_pair(x2, y0));
        /////////////////////////////////////////////
        int xy_max = 0;
        int x_max = 0;
        int y_max = 0;
        for (int i = 0; i < 4; i++)
        {
            int nx = v[i].first;
            int ny = v[i].second;
            if (nx + ny > xy_max)
            {
                xy_max = nx + ny;
                x_max = nx;
                y_max = ny;
            }
            else if (nx + ny == xy_max && nx > x_max)
            {
                x_max = nx;
                y_max = ny;
            }
        }
        // 적용
        for (int i = 0; i < num_of_rabbit; i++)
        {
            if (i == idx) // 자기 자신 제외
                continue;
            point[i] += xy_max;
        }
        // 최신화 & 다시 push
        position[idx][0] = x_max;
        position[idx][1] = y_max;
        last_jump[idx]++;
        Rabbit nr;
        nr.x = position[idx][0];
        nr.y = position[idx][1];
        nr.id = now_r.id;
        nr.j = last_jump[idx];
        Rb.push(nr);
        // show_rb();
    }
    // 뛰었던 놈들중 우선순위 파악해서 S점수 부여
    // 현재 서있는 행 번호 + 열 번호가 큰 토끼, 행 번호가 큰 토끼, 열 번호가 큰 토끼, 고유번호가 큰 토끼
    int xy_max = 0;
    int x_max = 0;
    int id_max = 0;
    for (int i = 0; i < num_of_rabbit; i++)
    {
        if (runned[i] == 0)
            continue;
        int x = position[i][0];
        int y = position[i][1];
        int id = rb_id[i];
        if (x + y > xy_max)
        {
            xy_max = x + y;
            x_max = x;
            id_max = id;
        }
        else if (x + y == xy_max && x > x_max)
        {
            x_max = x;
            id_max = id;
        }
        else if (x + y == xy_max && x == x_max && id > id_max)
        {
            id_max = id;
        }
    }
    int a = id_to_idx[id_max];
    point[a] += S;

    // ///
    // cout << "-----" << endl;
    // cout << "end of this turn" << endl;
    // show_rb();
    // cout << "-----" << endl;
}

int main()
{
    cin >> Q;
    while (Q--)
    {
        cin >> ins;
        if (ins == 100)
            ready();
        else if (ins == 200)
            run_rabbit();
        else if (ins == 300)
            change_dist();
        else // ins ==400
            show_best();
    }
}
/// -----------------------------------------------------------------/////
void show_rb()
{
    for (int i = 0; i < num_of_rabbit; i++)
        cout << rb_id[i] << ": " << position[i][0] << "," << position[i][1] << " " << last_jump[i] << "   " << point[i] << "|d. " << dist[i] << endl;
}
void show_best()
{
    int max_point = 0;
    for (int i = 0; i < num_of_rabbit; i++)
        if (point[i] > max_point)
            max_point = point[i];
    cout << max_point;
}
void change_dist()
{
    int pid, L;
    cin >> pid >> L;
    dist[id_to_idx[pid]] *= L;
}
void ready()
{
    cin >> N >> M >> num_of_rabbit;
    for (int i = 0; i < num_of_rabbit; i++)
    {
        int pid, dst;
        cin >> pid >> dst;
        Rabbit new_rb;
        new_rb.x = 1;
        new_rb.y = 1;
        new_rb.j = 0;
        new_rb.id = pid;

        rb_id[i] = pid;
        id_to_idx[pid] = i;
        dist[i] = dst;
        position[i][0] = 1;
        position[i][1] = 1;
        Rb.push(new_rb);
    }
}