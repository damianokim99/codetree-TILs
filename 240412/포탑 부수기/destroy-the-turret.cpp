#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <utility>
using namespace std;

int N, M, K, Ax, Ay, Bx, By, turn = 1;
int mapp[10][10]; // 0 ~ N-1
int last_attack[10][10];
int dx[4] = {0, 1, -1, 0}; // 우 하 상 좌 로 탐색
int dy[4] = {1, 0, 0, -1};
int ddx[8] = {1, 1, 1, 0, 0, -1, -1, -1};
int ddy[8] = {1, 0, -1, 1, -1, 1, 0, -1};
int participated[10][10] = {0};

int check_left();
int x_change(int x);
int y_change(int y);
int oppose_direc(int d);

void show_mapp();
void print_ans();
void find_attack();
void find_hit();
int find_laser();
void find_bomb();

int find_laser() // 동일하면 우 하 상 좌 순서로
{
    int laser_ok = 0;
    int visited[10][10] = {0};
    queue<pair<int, int>> q;
    q.push(make_pair(Ax, Ay));
    visited[Ax][Ay] = -1; // 시작위치 저장
    while (!q.empty())
    {
        int x = q.front().first;
        int y = q.front().second;
        q.pop();
        for (int i = 0; i < 4; i++) // 우 하 상 좌
        {
            int nx = x + dx[i];
            int ny = y + dy[i];
            nx = x_change(nx);
            ny = y_change(ny);
            if (mapp[nx][ny] > 0 && visited[nx][ny] == 0)
            {
                q.push(make_pair(nx, ny));
                visited[nx][ny] = i + 1; // 방향 기억하기
                if (nx == Bx && ny == By)
                    laser_ok = 1;
            }
        }
    }
    if (laser_ok == 0)
        return -1;

    participated[Ax][Ay] = 1;
    participated[Bx][By] = 1;

    int hit_half = mapp[Ax][Ay] / 2;
    mapp[Bx][By] -= mapp[Ax][Ay];
    int direc = oppose_direc(visited[Bx][By] - 1);
    Bx = x_change(Bx + dx[direc]);
    By = y_change(By + dy[direc]);

    while (visited[Bx][By] != -1) // 출발지 도착
    {
        participated[Bx][By] = 1;
        mapp[Bx][By] -= hit_half;
        direc = oppose_direc(visited[Bx][By] - 1);
        Bx = x_change(Bx + dx[direc]);
        By = y_change(By + dy[direc]);
    }
    return 0;
}
void find_bomb()
{
    int damage = mapp[Ax][Ay];
    int half_damage = damage / 2;
    mapp[Bx][By] -= damage;
    participated[Ax][Ay] = 1;
    participated[Bx][By] = 1;

    for (int i = 0; i < 8; i++)
    {
        int nx = Bx + ddx[i];
        int ny = By + ddy[i];
        nx = x_change(nx);
        ny = y_change(ny);
        if (mapp[nx][ny] > 0 && (nx != Ax || ny != Ay)) // 0 아니고 공격자 아니면
        {
            mapp[nx][ny] -= half_damage;
            participated[nx][ny] = 1;
        }
    }
}

int main()
{
    // input
    cin >> N >> M >> K;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            cin >> mapp[i][j];

    while (K--)
    {
        // cout << turn << endl;
        // 공격자 선정 (가장 약한 포탑)
        find_attack();
        // 피격자 선정 (가장 강한 포탑)
        find_hit();
        // 공격자 강화
        mapp[Ax][Ay] += (M + N);
        // 레이저 공격 & 포탑정비
        int laser_ok = find_laser();
        // 포탑 공격
        if (laser_ok == -1)
            find_bomb();
        // 하나 남으면 중지
        int check = check_left();
        if (check == 1)
            break;
        // show_mapp();
        // 포탑정비
        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++)
                if (mapp[i][j] > 0 && participated[i][j] == 0)
                    mapp[i][j]++;
        // 턴 증가
        turn++;
        // 참여자 초기화
        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++)
                participated[i][j] = 0;
    }
    print_ans();
}
//------------------------------------------------------//

void find_hit()
{
    /*
    1. 공격력 높은
    2. 가장 오래전에 공격
    3. x+y작은
    4. y작은
    */
    int max_attack = INT_MIN;
    int late_attack = INT_MIN;
    int xy_min = INT_MAX;
    int y_min = INT_MAX;
    int x;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            if (mapp[i][j] > 0)
            {
                if (mapp[i][j] > max_attack)
                {
                    max_attack = mapp[i][j];
                    late_attack = last_attack[i][j];
                    xy_min = i + j;
                    y_min = j;
                    x = i;
                }
                else if (mapp[i][j] == max_attack && last_attack[i][j] < late_attack)
                {
                    late_attack = last_attack[i][j];
                    xy_min = i + j;
                    y_min = j;
                    x = i;
                }
                else if (mapp[i][j] == max_attack && last_attack[i][j] == late_attack && i + j < xy_min)
                {
                    xy_min = i + j;
                    y_min = j;
                    x = i;
                }
                else if (mapp[i][j] == max_attack && last_attack[i][j] == late_attack && i + j == xy_min && j < y_min)
                {
                    y_min = j;
                    x = i;
                }
            }
    Bx = x;
    By = y_min;
}
void find_attack()
{
    /*
    1. 공격력 낮은
    2. 가장 최근 공격
    3. x+y큰
    4. y큰
    */
    int min_attack = INT_MAX;
    int recent_attack = INT_MIN;
    int xy_max = INT_MIN;
    int y_max = INT_MIN;
    int x;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            if (mapp[i][j] > 0)
            {
                if (mapp[i][j] < min_attack)
                {
                    min_attack = mapp[i][j];
                    recent_attack = last_attack[i][j];
                    xy_max = i + j;
                    y_max = j;
                    x = i;
                }
                else if (mapp[i][j] == min_attack && last_attack[i][j] > recent_attack)
                {
                    recent_attack = last_attack[i][j];
                    xy_max = i + j;
                    y_max = j;
                    x = i;
                }
                else if (mapp[i][j] == min_attack && last_attack[i][j] == recent_attack && i + j > xy_max)
                {
                    xy_max = i + j;
                    y_max = j;
                    x = i;
                }
                else if (mapp[i][j] == min_attack && last_attack[i][j] == recent_attack && i + j == xy_max && j > y_max)
                {
                    y_max = j;
                    x = i;
                }
            }
    Ax = x;
    Ay = y_max;
    last_attack[Ax][Ay] = turn;
}
void print_ans()
{
    int max_attack = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            if (mapp[i][j] > max_attack)
                max_attack = mapp[i][j];
    cout << max_attack;
}
void show_mapp()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
            cout << mapp[i][j] << " ";
        cout << endl;
    }
}
int check_left()
{
    int cnt = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            if (mapp[i][j] > 0)
                cnt++;
    return cnt;
}
int x_change(int x)
{
    if (x == -1)
        return N - 1;
    if (x == N)
        return 0;
    return x;
}
int y_change(int y)
{
    if (y == -1)
        return M - 1;
    if (y == M)
        return 0;
    return y;
}
int oppose_direc(int d)
{
    if (d == 0)
        return 3;
    if (d == 1)
        return 2;
    if (d == 2)
        return 1;
    if (d == 3)
        return 0;
    return -1;
}