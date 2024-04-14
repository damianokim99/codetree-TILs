// 1413
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <utility>
using namespace std;
int n, m, cx, cy;                  // m: 사람 수
int mapp[16][16];                  // 1~N      0: 빈칸, 1베이스, 2막힘
vector<pair<int, int>> con;        // i번째 사람이 가고 싶어 하는 편의점 위치
queue<pair<int, int>> reached_con; // 이번턴에 도달된 -> 벽으로 변신 예정
int p[226][2];                     // i번째 사람 위치
int reached_p;                     // 도달한 사람 수
bool reached[226];
int dfx[4] = {-1, 0, 0, 1}; // 상 좌 우 하
int dfy[4] = {0, -1, 1, 0};
int dx[4] = {-1, 0, 0, 1}; // 상 좌 우 하
int dy[4] = {0, -1, 1, 0};

int is_in(int x, int y);
void start_base(int id);
void move_people(int id);
void close_con();
int oppose_direc(int d);
void show_mapp();

void move_people(int id)
{
    int x = p[id][0]; // 현재 위치
    int y = p[id][1];

    int dest_x = con[id].first; // 목적지
    int dest_y = con[id].second;

    int visited[16][16] = {0};
    queue<pair<int, int>> q;
    q.push(make_pair(x, y));
    visited[x][y] = -1; // 출발지 기록
    while (!q.empty())
    {
        int a = q.front().first;
        int b = q.front().second;
        q.pop();
        for (int i = 0; i < 4; i++)
        {
            int nx = a + dx[i];
            int ny = b + dy[i];
            if (is_in(nx, ny) && visited[nx][ny] == 0 && mapp[nx][ny] != 2) // 칸 안, 방문x, 벽x
            {
                q.push(make_pair(nx, ny));
                visited[nx][ny] = i + 1;
            }
        }
    }

    int a = dest_x;
    int b = dest_y;
    int direc = oppose_direc(visited[a][b] - 1);

    // 한칸 바로 옆이라면
    if (a + dx[direc] == x && b + dy[direc] == y)
    {
        reached[id] = 1;
        reached_p++;
        reached_con.push(make_pair(dest_x, dest_y));
        return;
    }

    while (1)
    {
        direc = oppose_direc(visited[a][b] - 1);
        if (a + dx[direc] == x && b + dy[direc] == y)
            break;

        a += dx[direc];
        b += dy[direc];
    }
    // 위치 옮기기
    p[id][0] = a;
    p[id][1] = b;
}
void start_base(int id)
{
    int dest_x = con[id].first;
    int dest_y = con[id].second;
    // cout << "dest: " << dest_x << ", " << dest_y << endl;

    int visited[16][16] = {0};
    queue<pair<int, int>> q;
    q.push(make_pair(dest_x, dest_y));
    visited[dest_x][dest_y] = 1;
    int is_stop = 0;

    while (!q.empty())
    {
        int a = q.front().first;
        int b = q.front().second;
        q.pop();
        for (int i = 0; i < 4; i++)
        {
            int nx = a + dfx[i];
            int ny = b + dfy[i];
            if (is_in(nx, ny) && visited[nx][ny] == 0 && mapp[nx][ny] != 2) // 칸안, 방문x, 벽x
            {
                if (mapp[nx][ny] == 1)
                {
                    p[id][0] = nx;
                    p[id][1] = ny;
                    mapp[nx][ny] = 2; // 이제부터 이동 불가
                    is_stop = 1;      // 그만탐색
                    break;
                }
                else if (mapp[nx][ny] == 0)
                {
                    q.push(make_pair(nx, ny));
                    visited[nx][ny] = 1;
                }
            }
        }
        if (is_stop == 1)
            break;
    }
}

int main()
{
    // input
    cin >> n >> m;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            cin >> mapp[i][j]; // 1: 시작 위치
    for (int i = 0; i < m; i++)
    {
        cin >> cx >> cy;
        con.push_back(make_pair(cx, cy)); // 도착 편의점
    }
    // cout << endl;
    int turn = 0;
    while (reached_p != m)
    {
        // 격자에 존재하는 사람들 전부 원하는 방향으로 이동
        int move_p = turn;
        if (turn >= m)
            move_p = m;

        // cout << "M: " << move_p << endl;///

        for (int i = 0; i < move_p; i++)
            if (reached[i] == 0)
                move_people(i);

        // 전부 도달 시 종료
        // if (reached_p == m)
        //     break;

        // 닫아야 하는 편의점들 전부 닫기
        close_con();

        // cout << "after_move" << endl;
        // show_mapp();

        // 자신의 목적 편의점과 가장 가까운 베이스에서 사람 생성 (동일하면 x작은, y작은 순으로)
        // 이때 부터 이 위치 이동 불가
        if (turn < m) // ex) m=3
            start_base(turn);
        turn++;

        // cout << "after_create" << endl;
        // show_mapp();

        // cout << "--------------------------" << endl;
    }
    cout << turn;
}
//```````````````````````````````````````````````````````````````````````````//
void close_con()
{
    while (!reached_con.empty())
    {
        int x = reached_con.front().first;
        int y = reached_con.front().second;
        reached_con.pop();
        mapp[x][y] = 2;
    }
}
int oppose_direc(int d)
{
    if (d == 0)
        return 3;
    if (d == 1)
        return 2;
    if (d == 2)
        return 1;
    return 0; // 3
}
int is_in(int x, int y)
{
    if (x > 0 && y > 0 && x <= n && y <= n)
        return 1;
    return 0;
}
void show_mapp()
{
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
            cout << mapp[i][j] << " ";
        cout << endl;
    }
}