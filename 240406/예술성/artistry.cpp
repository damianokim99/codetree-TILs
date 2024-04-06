#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <climits>
using namespace std;
int dx[4] = {1, -1, 0, 0};
int dy[4] = {0, 0, 1, -1};
int mapp[29][29];
int group_mapp[29][29]; // 인덱싱 1부터
int wall[900][900];     // wall[i][j]: i 번 그룹이 j 그룹과 맞다아 있는 벽의 수
int visited2[29][29];
int num_of_group[900];   // 각 그룹별 갯수
int xy_of_group[900][2]; // 각 그룹의 첫 x,y 좌표
int n;
int group_num = 0; // 그룹 갯수
int points = 0;    // 정답출력

int is_in(int x, int y);
void show_groupmapp();
void show_mapp();
void init_var();
void bfs1(int x, int y, int group_n);
void bfs2(int x, int y);
int count_points();

void rotate_small(int x, int y, int size)
{
    // 현재 배열 복사해오기
    int mapp_copy[14][14];
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            mapp_copy[i][j] = mapp[i + x][j + y];

    // for (int i = 0; i < size; i++)
    // {
    //     for (int j = 0; j < size; j++)
    //         cout << mapp_copy[i][j] << " ";
    //     cout << endl;
    // }

    // 배열 회전
    int mapp_res[14][14];
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            mapp_res[j][size - 1 - i] = mapp_copy[i][j];

    // for (int i = 0; i < size; i++)
    // {
    //     for (int j = 0; j < size; j++)
    //         cout << mapp_res[i][j] << " ";
    //     cout << endl;
    // }
    // 원본에 옮기기
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            mapp[x + i][y + j] = mapp_res[i][j];
}
void rotate()
{
    int mapp_copy[29][29];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            mapp_copy[i][j] = mapp[i][j];
    // 중심 회전
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            mapp_copy[n - 1 - j][i] = mapp[i][j];

    // for (int i = 0; i < n; i++)
    // {
    //     for (int j = 0; j < n; j++)
    //         cout << mapp_copy[i][j] << " ";
    //     cout << endl;
    // }
    // cout << endl;

    // 각 네 부분 회전
    rotate_small(0, 0, (n - 1) / 2);
    rotate_small(n / 2 + 1, 0, (n - 1) / 2);
    rotate_small(0, n / 2 + 1, (n - 1) / 2);
    rotate_small(n / 2 + 1, n / 2 + 1, (n - 1) / 2);

    // 중심 회전한거 옮겨오기
    // 마지막으로 mapp[i][j]=mapp_copy[i][j];
    for (int i = 0; i < n; i++)
    {
        mapp[n / 2][i] = mapp_copy[n / 2][i];
        mapp[i][n / 2] = mapp_copy[i][n / 2];
    }
}
int main()
{
    // input
    cin >> n;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> mapp[i][j];
    // cout << endl;
    points += count_points();
    // show_mapp();

    int turn = 3;
    while (turn--)
    {
        // 회전
        rotate();
        init_var();
        points += count_points();
    }
    cout << points;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void init_var()
{
    /*
    int group_mapp[29][29]; // 인덱싱 1부터
    */
    for (int i = 0; i < 30; i++)
        for (int j = 0; j < 30; j++)
            group_mapp[i][j] = 0;
    for (int i = 0; i < 900; i++)
        num_of_group[i] = 0;
    for (int i = 0; i < 900; i++)
        for (int j = 0; j < 900; j++)
            wall[i][j] = 0;
    for (int i = 0; i < 900; i++)
        for (int j = 0; j < 2; j++)
            xy_of_group[i][j] = 0;
    for (int i = 0; i < 30; i++)
        for (int j = 0; j < 30; j++)
            visited2[i][j] = 0;
    group_num = 0;
}
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
void show_groupmapp()
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            cout << group_mapp[i][j] << " ";
        cout << endl;
    }
    cout << endl;
}
void bfs1(int x, int y, int group_n)
{
    xy_of_group[group_n][0] = x;
    xy_of_group[group_n][1] = y;
    int cnt = 1;
    int num_now = mapp[x][y];
    int visited[29][29] = {0};
    queue<pair<int, int>> q;
    q.push(make_pair(x, y));
    visited[x][y] = 1;
    group_mapp[x][y] = group_n;
    while (!q.empty())
    {
        int a = q.front().first;
        int b = q.front().second;
        q.pop();
        for (int i = 0; i < 4; i++)
        {
            int nx = a + dx[i];
            int ny = b + dy[i];
            if (is_in(nx, ny) && visited[nx][ny] == 0 && mapp[nx][ny] == num_now)
            {
                q.push(make_pair(nx, ny));
                visited[nx][ny] = 1;
                group_mapp[nx][ny] = group_n;
                cnt++;
            }
        }
    }
    num_of_group[group_n] = cnt;
}
void bfs2(int x, int y)
{
    int now_group = group_mapp[x][y];
    queue<pair<int, int>> q;
    q.push(make_pair(x, y));
    visited2[x][y] = 0;
    while (!q.empty())
    {
        int a = q.front().first;
        int b = q.front().second;
        q.pop();
        for (int i = 0; i < 4; i++)
        {
            int nx = a + dx[i];
            int ny = b + dy[i];
            if (is_in(nx, ny) && visited2[nx][ny] == 0 && group_mapp[nx][ny] == now_group)
                visited2[x][y] = 1;
            else if (is_in(nx, ny) && group_mapp[nx][ny] != now_group)
                wall[now_group][group_mapp[nx][ny]]++;
        }
    }
}
int count_points()
{
    int sum = 0;
    // 구역을 먼저 나눔
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (group_mapp[i][j] == 0)
            {
                group_num++;
                bfs1(i, j, group_num);
            }
    // 구역별 인접한 벽 갯수 카운팅
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (visited2[i][j] == 0)
                bfs2(i, j);
    // 점수 계산
    int next_ok = 1;

    for (int idx = 1; idx <= group_num; idx++) // (그룹 수 + 그룹 수) x 숫자 x 숫자 x 벽 수
        for (int i = 1; i <= group_num; i++)
            if (wall[idx][i] > 0)
            {
                next_ok = 1;
                int idx_x = xy_of_group[idx][0];
                int idx_y = xy_of_group[idx][1];
                int i_x = xy_of_group[i][0];
                int i_y = xy_of_group[i][1];
                int temp = (num_of_group[idx] + num_of_group[i]) * mapp[idx_x][idx_y] * mapp[i_x][i_y] * wall[idx][i];
                // cout << idx << " , " << i << " " << temp << endl;
                sum += temp;
            }

    return (sum / 2);
}