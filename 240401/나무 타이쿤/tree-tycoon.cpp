#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <climits>
using namespace std;
int n, m;
int d, p;
int mapp[15][15];
int ins[100][2]; // 방향, 칸수
int move_cnt = 0;
int dx[8] = {0, -1, -1, -1, 0, 1, 1, 1};
int dy[8] = {1, 1, 0, -1, -1, -1, 0, 1};
int ddx[4] = {1, 1, -1, -1};
int ddy[4] = {1, -1, 1, -1};
vector<pair<int, int>> ribrose;

void show_mapp()
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            cout << mapp[i][j] << " ";
        cout << endl;
    }
    cout << endl;
}
void show_rib()
{
    int visited[15][15] = {0};
    for (int i = 0; i < ribrose.size(); i++)
        visited[ribrose[i].first][ribrose[i].second] = 1;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            cout << visited[i][j] << " ";
        cout << endl;
    }
    cout << endl;
}

int is_in(int x, int y)
{
    if (x >= 0 && x < n && y >= 0 && y < n)
        return 1;
    return 0;
}
void init_rib()
{
    ribrose.push_back(make_pair(n - 2, 0));
    ribrose.push_back(make_pair(n - 2, 1));
    ribrose.push_back(make_pair(n - 1, 0));
    ribrose.push_back(make_pair(n - 1, 1));
}
void move_rib()
{
    for (int i = 0; i < ribrose.size(); i++)
    {
        int direc = ins[move_cnt][0];
        int pp = ins[move_cnt][1];
        ribrose[i].first = (ribrose[i].first + dx[direc] * pp + n) % n;
        ribrose[i].second = (ribrose[i].second + dy[direc] * pp + n) % n;
    }
    move_cnt++;
}
void grow_rib1()
{
    for (int i = 0; i < ribrose.size(); i++)
    {
        int x = ribrose[i].first;
        int y = ribrose[i].second;
        mapp[x][y]++;
    }
}
int count_cross(int x, int y)
{
    int sum = 0;
    for (int i = 0; i < 4; i++)
    {
        int nx = x + ddx[i];
        int ny = y + ddy[i];
        if (is_in(nx, ny) && mapp[nx][ny] != 0)
            sum++;
    }
    return sum;
}
void grow_rib2()
{
    for (int i = 0; i < ribrose.size(); i++)
    {
        int x = ribrose[i].first;
        int y = ribrose[i].second;
        mapp[x][y] += count_cross(x, y);
    }
}
void next_rib()
{
    int visited[15][15] = {0};
    for (int i = 0; i < ribrose.size(); i++)
    {
        int x = ribrose[i].first;
        int y = ribrose[i].second;
        visited[x][y] = 1;
    }
    ribrose.clear();

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (mapp[i][j] >= 2 && visited[i][j] == 0)
            {
                mapp[i][j] -= 2;
                ribrose.push_back(make_pair(i, j));
            }
}
int count_ans()
{
    int sum = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            sum += mapp[i][j];
    return sum;
}

int main()
{
    // input
    cin >> n >> m;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> mapp[i][j];
    for (int i = 0; i < m; i++)
    {
        cin >> d >> p;
        ins[i][0] = d - 1;
        ins[i][1] = p;
    }
    init_rib();

    while (m--)
    {
        // 각 리브로수 이동
        move_rib();
        // 리브로수 +1 성장
        grow_rib1();
        // 각 리브로수 마다 인접 대각선 리브로수 갯수 count 및 증가
        grow_rib2();
        // 해당년도 제외 2이상 잘라내고 위치 저장
        next_rib();
    }

    // print
    cout << count_ans();
}