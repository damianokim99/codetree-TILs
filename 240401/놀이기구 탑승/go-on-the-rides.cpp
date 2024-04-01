#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <climits>
using namespace std;
int n;
int n0, n1, n2, n3, n4;
int mapp[21][21];
int p[400][4]; // 좋아하는 4명
int dx[4] = {1, -1, 0, 0};
int dy[4] = {0, 0, 1, -1};
queue<int> q; // 순서
vector<pair<int, int>> v1;
vector<pair<int, int>> v2;
vector<pair<int, int>> v3;
vector<pair<int, int>> v4;
int x_p;
int y_p;
int points[5] = {0, 1, 10, 100, 1000};

int is_in(int x, int y)
{
    if (x >= 0 && x < n && y >= 0 && y < n)
        return 1;
    return 0;
}
int count_friend(int current, int x, int y)
{
    int sum = 0;
    for (int i = 0; i < 4; i++)
    {
        int f = p[current][i];
        for (int j = 0; j < 4; j++) // 각 상하좌우에 대해 일치 친구 여부 확인
        {
            int nx = x + dx[j];
            int ny = y + dy[j];
            if (is_in(nx, ny) && mapp[nx][ny] == f)
            {
                sum++;
                break;
            }
        }
    }
    return sum;
}
int sel1(int current) // 친구 가장 많은
{
    int max_friend = -1;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (mapp[i][j] == 0)
            {
                int numf = count_friend(current, i, j);
                if (numf > max_friend)
                {
                    v1.clear();
                    v1.push_back(make_pair(i, j));
                    max_friend = numf;
                }
                else if (numf == max_friend)
                    v1.push_back(make_pair(i, j));
            }
    if (v1.size() == 1)
    {
        x_p = v1[0].first;
        y_p = v1[0].second;
        v1.clear();
        return 1;
    }
    return 0;
}
int count_blank(int x, int y)
{
    int sum = 0;
    for (int i = 0; i < 4; i++)
    {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (mapp[nx][ny] == 0 && is_in(nx, ny))
            sum++;
    }
    return sum;
}
int sel2() // 인접한 칸 비어있는
{
    int max_blank = -1;
    for (int i = 0; i < v1.size(); i++) // v1 중에서
    {
        int x = v1[i].first;
        int y = v1[i].second;
        int numb = count_blank(x, y);
        if (numb > max_blank)
        {
            v2.clear();
            v2.push_back(make_pair(x, y));
            max_blank = numb;
        }
        else if (numb == max_blank)
            v2.push_back(make_pair(x, y));
    }
    if (v2.size() == 1)
    {
        x_p = v2[0].first;
        y_p = v2[0].second;
        v2.clear();
        return 1;
    }
    return 0;
}
int sel3() // 행 작
{
    int min_x = 22;
    for (int i = 0; i < v2.size(); i++)
    {
        int x = v2[i].first;
        int y = v2[i].second;
        if (x < min_x)
        {
            v3.clear();
            v3.push_back(make_pair(x, y));
            min_x = x;
        }
        else if (x == min_x)
            v3.push_back(make_pair(x, y));
    }
    if (v3.size() == 1)
    {
        x_p = v3[0].first;
        y_p = v3[0].second;
        v3.clear();
        return 1;
    }
    return 0;
}
int sel4() // 열 작
{
    int min_y = 21;
    for (int i = 0; i < v3.size(); i++)
    {
        int x = v3[i].first;
        int y = v3[i].second;
        if (y < min_y)
        {
            v4.clear();
            v4.push_back(make_pair(x, y));
            min_y = y;
        }
        else if (y == min_y)
            v4.push_back(make_pair(x, y));
    }
    if (v4.size() == 1)
    {
        x_p = v4[0].first;
        y_p = v4[0].second;
        v4.clear();
        return 1;
    }
    return 0;
}
void place(int current)
{
    int s1 = sel1(current);
    if (s1 != 0)
        return;
    int s2 = sel2();
    if (s2 != 0)
        return;
    int s3 = sel3();
    if (s3 != 0)
        return;
    int s4 = sel4();
    if (s4 != 0)
        return;
}
int count_point()
{
    int sum = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            sum += points[count_friend(mapp[i][j], i, j)];

    return sum;
}
void show_mapp()
{
    cout << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            cout << mapp[i][j] << " ";
        cout << endl;
    }
}
int main()
{
    // input
    cin >> n;
    for (int i = 0; i < n * n; i++)
    {
        cin >> n0 >> n1 >> n2 >> n3 >> n4;
        p[n0][0] = n1;
        p[n0][1] = n2;
        p[n0][2] = n3;
        p[n0][3] = n4;
        q.push(n0);
    }

    //
    for (int i = 0; i < n * n; i++)
    {
        // 자리 위치 하기
        int current = q.front();
        q.pop();
        place(current);
        mapp[x_p][y_p] = current;
    }
    // show_mapp();

    // 최종 점수 연산
    cout << count_point();
}