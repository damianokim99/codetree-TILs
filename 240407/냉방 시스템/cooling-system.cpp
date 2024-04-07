// 1320
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <utility>
using namespace std;
int n, m, k, val;
int wx, wy, wd;
int dx[4] = {0, -1, 0, 1};                        // 좌 상 우 하
int dy[4] = {-1, 0, 1, 0};                        //
vector<pair<pair<int, int>, int>> airconditioner; // 0좌 1상 2우 3하
vector<pair<int, int>> room;                      // 사무실 좌표
vector<pair<pair<int, int>, int>> air_info;       // x,y,plus
int mapp[20][20];                                 // 공기 정보
int wall_mapp[20][20][4];                         // 좌상우하 벽 존재
int air_plus[20][20];
int check_room();
void wall_info(int x, int y, int d);
int is_in(int x, int y);
void airconditioner_save(); // 바람적용되는거 저장
void airconditioner_work(); // 바람 적용
void show_mapp();
void wall_decrease();
void wind_flow();
void windchange();

void windchange(int x, int y, int vst[20][20], int mp[20][20])
{
    int now_val = mapp[x][y];
    for (int i = 0; i < 4; i++)
    {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (is_in(nx, ny) && wall_mapp[x][y][i] == 0 && vst[nx][ny] == 0) // 칸안, 벽x
        {
            int new_val = mapp[nx][ny];
            int diff = now_val - new_val;
            if (diff > 0) // 현재 위치가 더 큼
            {
                int add_val = diff / 4;
                mp[x][y] -= add_val;
                mp[nx][ny] += add_val;
            }
            else if (diff < 0) // 옆의 위치가 더 큼
            {
                int add_val = abs(diff) / 4;
                mp[x][y] += add_val;
                mp[nx][ny] -= add_val;
            }
        }
    }
}
void wind_flow()
{
    int mapp_temp[20][20] = {0};
    int visited[20][20] = {0};
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (visited[i][j] == 0)
            {
                visited[i][j] = 1;
                windchange(i, j, visited, mapp_temp);
            }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            mapp[i][j] += mapp_temp[i][j];
}

int main()
{
    // input
    cin >> n >> m >> k;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        {
            cin >> val;
            if (val == 1) // 사무실정보
                room.push_back(make_pair(i, j));
            else if (val > 1)
                airconditioner.push_back(make_pair(make_pair(i, j), val - 2));
        }
    for (int i = 0; i < m; i++)
    {
        cin >> wx >> wy >> wd;
        wall_info(wx - 1, wy - 1, wd);
    }
    //////
    airconditioner_save();
    int turn = 1;
    while (turn <= 100)
    {
        // 에어컨 작동
        airconditioner_work();
        // 확산
        wind_flow();
        // 외벽 감소
        wall_decrease();
        // show_mapp();
        // 종료 확인
        int chk = check_room();
        if (chk == 1)
            break;
        // 턴 증가
        turn++;
    }
    if (turn == 101)
        cout << -1;
    else
        cout << turn;
}
/// @brief //////////////////////////////////////////////////////////
void airconditioner_save()
{
    for (int i = 0; i < airconditioner.size(); i++)
    {
        int mapp_temp[20][20] = {0};
        int visited[20][20] = {0};

        int ax = airconditioner[i].first.first;
        int ay = airconditioner[i].first.second;
        int ad = airconditioner[i].second; // 방향: 0좌 1상 2우 3하
        // cout << "aircon " << ax << "," << ay << " : " << ad << endl;
        int ad1 = (ad + 3) % 4;
        int ad2 = (ad + 1) % 4;

        if (is_in(ax + dx[ad], ay + dy[ad]) == 0) // 에어컨 바로 앞에 벽이면 다음꺼
            continue;

        mapp_temp[ax + dx[ad]][ay + dy[ad]] = 5; // 에어컨 바로 앞 5
        queue<pair<pair<int, int>, int>> q;
        q.push(make_pair(make_pair(ax + dx[ad], ay + dy[ad]), 5));

        while (!q.empty())
        {
            int x = q.front().first.first;
            int y = q.front().first.second;
            int wind = q.front().second;
            q.pop();
            if (wind == 1)
                continue;

            // 1 직진
            if (is_in(x + dx[ad], y + dy[ad]) && wall_mapp[x][y][ad] == 0 && visited[x + dx[ad]][y + dy[ad]] == 0) // 벽없고 칸 안
            {
                mapp_temp[x + dx[ad]][y + dy[ad]] = wind - 1;
                q.push(make_pair(make_pair(x + dx[ad], y + dy[ad]), wind - 1));
                visited[x + dx[ad]][y + dy[ad]] = 1;
            }
            // 2 꺾
            int nx = x + dx[ad1];
            int ny = y + dy[ad1];
            int nnx = nx + dx[ad];
            int nny = ny + dy[ad];
            if (is_in(nx, ny) && wall_mapp[x][y][ad1] == 0 && is_in(nnx, nny) && wall_mapp[nx][ny][ad] == 0 && visited[nnx][nny] == 0)
            {
                mapp_temp[nnx][nny] = wind - 1;
                q.push(make_pair(make_pair(nnx, nny), wind - 1));
                visited[nnx][nny] = 1;
            }
            // 3 꺾
            nx = x + dx[ad2];
            ny = y + dy[ad2];
            nnx = nx + dx[ad];
            nny = ny + dy[ad];
            if (is_in(nx, ny) && wall_mapp[x][y][ad2] == 0 && is_in(nnx, nny) && wall_mapp[nx][ny][ad] == 0 && visited[nnx][nny] == 0)
            {
                mapp_temp[nnx][nny] = wind - 1;
                q.push(make_pair(make_pair(nnx, nny), wind - 1));
                visited[nnx][nny] = 1;
            }
        }
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                air_plus[i][j] += mapp_temp[i][j];
    }
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
void airconditioner_work()
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            mapp[i][j] += air_plus[i][j];
}
int is_in(int x, int y)
{
    if (x >= 0 && y >= 0 && x < n && y < n)
        return 1;
    return 0;
}
void wall_decrease()
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (i == 0 || j == 0 || i == n - 1 || j == n - 1)
                if (mapp[i][j] > 0)
                    mapp[i][j]--;
}
void wall_info(int x, int y, int d) // 0좌 1상 2우 3하
{
    if (d == 0) // 위 벽
    {
        wall_mapp[x][y][1] = 1;     // 위
        wall_mapp[x - 1][y][3] = 1; // 아래
    }
    else // d==1   왼쪽 벽
    {
        wall_mapp[x][y][0] = 1;     // 왼
        wall_mapp[x][y - 1][2] = 1; // 오
    }
}
int check_room()
{
    for (int i = 0; i < room.size(); i++)
    {
        int x = room[i].first;
        int y = room[i].second;
        if (mapp[x][y] < k)
            return 0;
    }
    return 1;
}

///////