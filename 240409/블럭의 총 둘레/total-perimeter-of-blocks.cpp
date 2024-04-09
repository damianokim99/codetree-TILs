#include <bits/stdc++.h>
using namespace std;
int n, ans;
int ix, iy;
int dx[4] = {1, -1, 0, 0};
int dy[4] = {0, 0, 1, -1};
int mapp[101][101]; // 0~101
int visited[101][101];

int is_in(int x, int y)
{
    if (x >= 0 && y >= 0 && x <= 100 && y <= 100)
        return 1;
    return 0;
}
void bfs()
{
    queue<pair<int, int>> q;
    q.push(make_pair(0, 0));
    visited[0][0] = 1;
    while (!q.empty())
    {
        int a = q.front().first;
        int b = q.front().second;
        q.pop();
        for (int i = 0; i < 4; i++)
        {
            int nx = a + dx[i];
            int ny = b + dy[i];
            if (is_in(nx, ny) && mapp[nx][ny] == 0 && visited[nx][ny] == 0)
            {
                q.push(make_pair(nx, ny));
                visited[nx][ny] = 1;
            }
            else if (is_in(nx, ny) && mapp[nx][ny] == 1)
            {
                ans++;
            }
        }
    }
}
int main()
{
    // input
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        cin >> ix >> iy;
        mapp[ix][iy] = 1;
    }
    bfs();
    //
    cout << ans;
}