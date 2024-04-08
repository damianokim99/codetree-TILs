#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <utility>
using namespace std;

int n, m;
int mapp[20][20]; // -1:돌, 0: red 1~: color
int point = 0;
int dx[4] = {1, -1, 0, 0};
int dy[4] = {0, 0, 1, -1};
int selected_x;
int selected_y;

int is_in(int x, int y);
void show_mapp();
int find_group();
void bfs(int x, int y, int vst[20][20], int current[4]);
void delete_group();
void push_down();
void rotate();
void rotate()
{
    int mapp_temp[20][20];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            mapp_temp[n - 1 - j][i] = mapp[i][j];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            mapp[i][j] = mapp_temp[i][j];
}

int main()
{
    // input
    cin >> n >> m;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> mapp[i][j];

    int turn = 1;
    while (1)
    {
        // cout << turn << endl;
        // 1. 폭탄 묶음 찾기
        int num_of_group = find_group();
        // cout << "num_of_group: " << num_of_group << endl;
        // @@ 묶음 폭탄 존재 x 시 break
        if (num_of_group <= 1)
            break;
        point += num_of_group * num_of_group;
        // 2. 폭탄 제거 & 점수 획득
        delete_group();
        // 3. 중력 작용
        push_down();
        // 4. 반시계 90 회전
        rotate();
        // 5. 중력 작용
        push_down();
        // show_mapp();
        // return 0;
        // 변수 초기화
        turn++;
    }
    cout << point;
}
//---------------------------------------------------------//
void push_down()
{
    queue<int> q;
    for (int j = 0; j < n; j++) // 각 열에 대해 아래로 내리기
    {
        int before_start = n - 1;
        for (int i = n - 1; i >= 0; i--) // 아래서부터
        {
            if (mapp[i][j] >= 0) // 색깔 폭탄 저장하고 그자리 -2
            {
                q.push(mapp[i][j]);
                mapp[i][j] = -2;
            }

            if (mapp[i][j] == -1 || i == 0) // 돌
            {
                while (!q.empty())
                {
                    mapp[before_start][j] = q.front();
                    q.pop();
                    before_start--;
                }

                before_start = i - 1;
            }
        }
    }
}
void delete_group()
{
    int visited[20][20] = {0};
    queue<pair<int, int>> q;
    int col = mapp[selected_x][selected_y];
    mapp[selected_x][selected_y] = -2;
    q.push(make_pair(selected_x, selected_y));
    visited[selected_x][selected_y] = 1;
    while (!q.empty())
    {
        int x = q.front().first;
        int y = q.front().second;
        q.pop();
        for (int i = 0; i < 4; i++)
        {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (is_in(nx, ny) && visited[nx][ny] == 0 && (mapp[nx][ny] == col || mapp[nx][ny] == 0))
            {
                visited[nx][ny] = 1;
                mapp[nx][ny] = -2;
                q.push(make_pair(nx, ny));
            }
        }
    }
}
void bfs(int x, int y, int vst[20][20], int current[4]) // 갯수 리턴. 기준점(x,y), red갯수, 기준점.
{
    int cnt = 1;
    int max_x = x;
    int min_y = y;
    int red_use = 0;
    queue<pair<int, int>> q;
    queue<pair<int, int>> red;
    q.push(make_pair(x, y));
    int col = mapp[x][y];
    vst[x][y] = 1;
    while (!q.empty())
    {
        int a = q.front().first;
        int b = q.front().second;
        q.pop();
        for (int i = 0; i < 4; i++)
        {
            int nx = a + dx[i];
            int ny = b + dy[i];
            if (is_in(nx, ny) && (mapp[nx][ny] == col || mapp[nx][ny] == 0) && vst[nx][ny] == 0) // 칸 안, 같은 색, 방문 x  + 붉은색
            {
                cnt++;
                q.push(make_pair(nx, ny));
                vst[nx][ny] = 1;
                if (mapp[nx][ny] == 0) // 붉은 폭탄
                {
                    red_use++;
                    red.push(make_pair(nx, ny));
                }
                else // 기준점 비교
                {
                    if (nx > max_x)
                    {
                        max_x = nx;
                        min_y = ny;
                    }
                    else if (nx == max_x && ny < min_y)
                        min_y = ny;
                }
            }
        }
    }
    // cnt, red_use, (x,y) 비교
    if (cnt > current[0])
    {
        current[0] = cnt;
        current[1] = red_use;
        current[2] = max_x;
        current[3] = min_y;
    }
    else if (cnt == current[0] && red_use < current[1])
    {
        current[0] = cnt;
        current[1] = red_use;
        current[2] = max_x;
        current[3] = min_y;
    }
    else if (cnt == current[0] && red_use == current[1] && max_x > current[2])
    {
        current[0] = cnt;
        current[1] = red_use;
        current[2] = max_x;
        current[3] = min_y;
    }
    else if (cnt == current[0] && red_use == current[1] && max_x == current[2] && min_y < current[3])
    {
        current[0] = cnt;
        current[1] = red_use;
        current[2] = max_x;
        current[3] = min_y;
    }
    while (!red.empty()) // 붉은색은 visited 다시 복구
    {
        vst[red.front().first][red.front().second] = 0;
        red.pop();
    }
}
int find_group()
{
    int selected[4] = {INT_MIN, INT_MAX, INT_MIN, INT_MAX}; // 갯수큰,red적은,기준x큰,기준y작은
    int visited[20][20] = {0};
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (visited[i][j] == 0 && mapp[i][j] > 0) // 방문x, 색 폭탄일시
                bfs(i, j, visited, selected);

    selected_x = selected[2];
    selected_y = selected[3];

    return selected[0];
}
void show_mapp()
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            cout << mapp[i][j] << " ";
        cout << endl;
    }
}
int is_in(int x, int y)
{
    if (x >= 0 && y >= 0 && x < n && y < n)
        return 1;
    return 0;
}