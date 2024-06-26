#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <utility>
using namespace std;
int n, m, k;      // 격자, 팀, 라운드
int mapp[20][20]; // 1머리, 2중간, 3꼬리, 4경로, 0빈칸
int points = 0;

int dx[4] = {0, -1, 0, 1};
int dy[4] = {1, 0, -1, 0};
int tx = 0, ty = 0;
int direc = 0;
int ball_move_direc = 3;
int head_x, head_y, tail_x, tail_y;
int next_direc_change = 0;

int is_in(int x, int y);
void show_mapp();
void move_group(int x, int y, int vst[20][20]);
void move_people();
void throw_ball();

int bfs(int x, int y)
{
    int return_val = 0;
    int is_one = 0;
    if (mapp[x][y] == 1)
    {
        head_x = x;
        head_y = y;
        is_one = 1;
    }
    else if (mapp[x][y] == 3)
    {
        tail_x = x;
        tail_y = y;
    }
    int visited[20][20] = {0};
    queue<pair<pair<int, int>, int>> q;
    q.push(make_pair(make_pair(x, y), 1));
    visited[x][y] = 1;
    while (!q.empty())
    {
        int a = q.front().first.first;
        int b = q.front().first.second;
        int dist = q.front().second;
        q.pop();
        for (int i = 0; i < 4; i++)
        {
            int nx = a + dx[i];
            int ny = b + dy[i];
            if (is_in(nx, ny) && visited[nx][ny] == 0 && (mapp[nx][ny] == 1 || mapp[nx][ny] == 2 || mapp[nx][ny] == 3))
            {
                visited[nx][ny] = 1;
                q.push(make_pair(make_pair(nx, ny), dist + 1));
                if (mapp[nx][ny] == 1)
                {
                    return_val = dist + 1;
                    head_x = nx;
                    head_y = ny;
                }
                else if (mapp[nx][ny] == 3)
                {
                    tail_x = nx;
                    tail_y = ny;
                }
            }
        }
    }
    if (is_one == 1)
        return 1;
    return return_val;
}
void throw_ball()
{
    // tx ty 에서 direc 방향으로 공을 던지다가 1 2 3찾기
    int x = tx;
    int y = ty;
    int seq;
    while (is_in(x, y))
    {
        if (mapp[x][y] == 1 || mapp[x][y] == 2 || mapp[x][y] == 3)
        {
            seq = bfs(x, y); // 1로부터 몇번째 선수인지 리턴
            break;
        }
        x += dx[direc];
        y += dy[direc];
    }
    points += seq * seq;
    if (seq > 0)
    {
        int temp_val = mapp[head_x][head_y];
        mapp[head_x][head_y] = mapp[tail_x][tail_y];
        mapp[tail_x][tail_y] = temp_val;
    }
}
void change_ball()
{
    if (next_direc_change == 1)
    {
        ball_move_direc = (ball_move_direc + 1) % 4;
        direc = (direc + 1) % 4;
        next_direc_change = 0;
        return;
    }
    tx += dx[ball_move_direc];
    ty += dy[ball_move_direc];
    if ((tx == 0 && ty == 0) || (tx == n - 1 && ty == 0) || (tx == n - 1 && ty == n - 1) || (tx == 0 && ty == n - 1))
        next_direc_change = 1;
}
int main()
{
    // input
    cin >> n >> m >> k;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> mapp[i][j];

    while (k--)
    {
        // cout << "ball info: " << tx << " " << ty << " " << direc << endl;
        // 머리사람을 따라 한칸씩 이동
        move_people();
        // cout << "after move" << endl;
        // show_mapp();

        // 공던지기 & 공 잡은 사람 점수 획득 & 방향 바꾸기
        throw_ball();
        // cout << "after ball" << endl;
        // show_mapp();
        // 다음 공 방향 및 위치 설정
        change_ball();
    }
    cout << points;
}
///////////////////////////////////////////////////
int is_in(int x, int y)
{
    if (x >= 0 && x < n && y >= 0 && y < n)
        return 1;
    return 0;
}
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
void move_group(int x, int y, int vst[20][20])
{
    int visited[20][20] = {0};
    int a = x;
    int b = y;
    int stop = 0;
    while (stop == 0)
    {
        int now_num = mapp[a][b];
        if (now_num == 1) // 4 찾아 교환
        {
            for (int i = 0; i < 4; i++)
            {
                int nx = a + dx[i];
                int ny = b + dy[i];
                if (is_in(nx, ny) && mapp[nx][ny] == 4)
                {
                    vst[nx][ny] = 1;
                    mapp[nx][ny] = 1;
                    mapp[a][b] = 4; // 현재 위치에 4
                    break;
                }
            }
        }
        else if (now_num == 4)
        {
            visited[a][b] = 1;
            for (int i = 0; i < 4; i++)
            {
                int nx = a + dx[i];
                int ny = b + dy[i];
                if (is_in(nx, ny) && visited[nx][ny] == 0 && (mapp[nx][ny] == 2 || mapp[nx][ny] == 3))
                {
                    mapp[a][b] = mapp[nx][ny]; // 현재 위치에 2 또는 3위치하기
                    mapp[nx][ny] = 4;
                    if (mapp[a][b] == 3)
                        stop = 1;
                    a = nx;
                    b = ny;
                    break;
                }
            }
        }
    }
}
void move_people()
{
    int visited[20][20] = {0};
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (mapp[i][j] == 1 && visited[i][j] == 0)
                move_group(i, j, visited);
}