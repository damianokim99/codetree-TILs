// 1350
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
int n, m;
int mapp[16][16];               // 0:길  1:베이스캠프  2:막힘
int dx_move[4] = {-1, 0, 0, 1}; // 상 좌 우 하
int dy_move[4] = {0, -1, 1, 0};
int dx_find[4] = {-1, 0, 0, 1}; // 상 좌 우 하
int dy_find[4] = {0, -1, 1, 0};
vector<pair<int, int>> base; // 베이스캠프 (출발지)
vector<pair<int, int>> dest; // 편의점  (도착지)
int reached[225];            // 0:아직 1: 도착
int timestamp = 0;
int num_reached = 0;

void show()
{
    cout << endl;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
            cout << mapp[i][j];
        cout << endl;
    }
}

int is_in(int x, int y) // 1~n
{
    if (x > 0 && x <= n && y > 0 && y <= n)
        return 1;
    return 0;
}
void find_base(int x, int y) // 편의점 -> 베이스 (경로 거꾸로)
{
    int find_dist; // 찾은 거리
    int visited[16][16] = {0};
    queue<pair<int, int>> q;
    q.push(make_pair(x, y));
    while (!q.empty())
    {
        int a = q.front().first;
        int b = q.front().second;
        if (mapp[a][b] == 1)
        {
            base.push_back(make_pair(a, b));
            mapp[a][b] = 2;
            return;
        }

        visited[a][b] = 1;
        q.pop();

        for (int i = 0; i < 4; i++) // 상 좌 우 하
        {
            int nx = a + dx_find[i];
            int ny = b + dy_find[i];
            if (is_in(nx, ny) && visited[nx][ny] == 0 && mapp[nx][ny] != 2 && mapp[nx][ny] != 3)
                q.push(make_pair(nx, ny));
        }
    }
}
int find_move(int x1, int y1, int x2, int y2) // 첫 경로 찾아야함
{
    queue<pair<int, int>> q;
    int visited[16][16] = {0}; // 상1 좌2 우3 하4
    q.push(make_pair(x1, y1));
    visited[x1][y1] = 5;

    while (!q.empty())
    {
        int a = q.front().first;
        int b = q.front().second;
        q.pop();

        for (int i = 0; i < 4; i++) // 상1 좌2 우3 하4
        {
            int nx = a + dx_move[i];
            int ny = b + dy_move[i];
            if (is_in(nx, ny) && visited[nx][ny] == 0 && mapp[nx][ny] != 2 && mapp[nx][ny] != 3)
            {
                q.push(make_pair(nx, ny));
                visited[nx][ny] = i + 1;
            }
        }
    }
    // 거꾸로 거슬러
    int direc;
    while (visited[x2][y2] != 5)
    {
        // cout << x2 << "," << y2 << "   ";
        direc = visited[x2][y2];
        if (direc == 1)
            x2++;
        else if (direc == 2)
            y2++;
        else if (direc == 3)
            y2--;
        else //(direc==4)
            x2--;
    }
    return direc - 1; // 상1 좌2 우3 하4
}
void take_turn()
{
    /*
    1. 격자에 있는 모두가 동시에 이동
    2. if 도달 : 다음턴 부터, 지날 수 없음
    3. 베이스캠프(출발) 위치하기. 다음부터 지날 수 없음
    */

    // 1. 존재하는 모두가 이동
    queue<pair<int, int>> reached_dest;
    int t = timestamp;
    if (timestamp >= m) // 모두가 출발한 후
        t = m;
    for (int i = 0; i < t; i++)
    {
        if (reached[i] == 1)
            continue;
        int x1 = base[i].first;
        int y1 = base[i].second;
        int x2 = dest[i].first;
        int y2 = dest[i].second;
        // cout << "mv";
        int direc = find_move(x1, y1, x2, y2);
        // cout << "mv";
        // 실제 이동
        // cout << i << ": " << base[i].first << "," << base[i].second;
        base[i].first += dx_move[direc];
        base[i].second += dy_move[direc];
        // cout << " -> " << base[i].first << "," << base[i].second << endl;

        // cout << direc;
    }
    // 2. 도달하면 2로 block
    for (int i = 0; i < t; i++)
    {
        if (reached[i] == 1)
            continue;
        int x1 = base[i].first;
        int y1 = base[i].second;
        int x2 = dest[i].first;
        int y2 = dest[i].second;

        if (x1 == x2 && y1 == y2)
        {
            reached[i] = 1;
            reached_dest.push(make_pair(x1, y1));
            // mapp[x1][y1] = 2; -> 전부 이동하고 움직이기!!
            num_reached++;
        }
    }
    // 2 턴에 도착한 사람들 전부 이동후 변경
    while (!reached_dest.empty())
    {
        int a = reached_dest.front().first;
        int b = reached_dest.front().second;
        reached_dest.pop();
        mapp[a][b] = 3;
    }
    // 3. m이하 시 한명씩 출발
    if (timestamp < m) // 등호 확인
    {
        int a = dest[timestamp].first;
        int b = dest[timestamp].second;
        find_base(a, b);
    }

    // show();
    for (int i = 0; i < t; i++)
    {
        int a = base[i].first;
        int b = base[i].second;
        // cout << i << " : " << a << "," << b << endl;
    }
    // cout << "t: " << timestamp << " n:" << num_reached << endl;
    timestamp++;
}

int main()
{
    // input
    cin >> n >> m;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            cin >> mapp[i][j];
    for (int i = 0; i < m; i++) // m개의 도착 위치
    {
        int a, b;
        cin >> a >> b;
        dest.push_back(make_pair(a, b));
    }
    // test find_base
    // for (int i = 0; i < m; i++)
    // {
    //     int a = dest[i].first;
    //     int b = dest[i].second;
    //     find_base(a, b); // 이게 안됨
    // }

    // 반복
    while (num_reached < m)
        take_turn();

    // int h = 10;
    // while (h--)
    //     take_turn();

    cout << timestamp;
}