// 18:40 시작
#include <bits/stdc++.h>
using namespace std;
int N, M, K;
int mapp[11][11];
int attack_time[11][11]; // 0: default
int visited[11][11];
int involved[11][11];
int time_cnt = 1;
int reinforce;
int min_route = numeric_limits<int>::max();
int route_ok = 0;
string route_string = "";
int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
vector<pair<int, int>> v[4];
vector<pair<int, int>> m[4];
vector<pair<int, int>> together;
int ans;

int select_min()
{
    int min = 5000;
    // 공격력 낮은 찾기
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= M; j++)
        {
            if (mapp[i][j] > 0)
            {
                if (mapp[i][j] < min)
                {
                    v[0].clear();
                    v[0].push_back(make_pair(i, j));
                    min = mapp[i][j];
                }
                else if (mapp[i][j] == min)
                {
                    v[0].push_back(make_pair(i, j));
                }
            }
        }
    }

    if (v[0].size() == 1)
        return 0;
    else // 가장 최근 공격한
    {
        int recent_attack = -1;
        for (int i = 0; i < v[0].size(); i++)
        {
            int x = v[0][i].first;
            int y = v[0][i].second;
            if (attack_time[x][y] > recent_attack)
            {
                v[1].clear();
                v[1].push_back(make_pair(x, y));
                recent_attack = attack_time[x][y];
            }
            else if (attack_time[x][y] == recent_attack)
            {
                v[1].push_back(make_pair(x, y));
            }
        }
    }

    if (v[1].size() == 1)
        return 1;
    else // x+y합이 가장 큰
    {
        int sum = 0;
        for (int i = 0; i < v[1].size(); i++)
        {
            int x = v[1][i].first;
            int y = v[1][i].second;
            if (x + y > sum)
            {
                v[2].clear();
                v[2].push_back(make_pair(x, y));
                sum = x + y;
            }
            else if (x + y == sum)
            {
                v[2].push_back(make_pair(x, y));
            }
        }
    }

    if (v[2].size() == 1)
        return 2;
    else // y값이 가장 큰
    {
        int max_y = 0;
        for (int i = 0; i < v[2].size(); i++)
        {
            int x = v[2][i].first;
            int y = v[2][i].second;
            if (y > max_y)
            {
                v[3].clear();
                v[3].push_back(make_pair(x, y));
                max_y = y;
            }
        }
    }
    return 3;
    /*
    1. 공격력 낮은
    2. 가장 최근 공격한
    3. x+y 합이 큰
    4. y값이 큰
    */
}
int select_max()
{
    int max_val = 0;
    // 공격력 높은 찾기
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= M; j++)
        {
            if (mapp[i][j] > 0)
            {
                if (mapp[i][j] > max_val)
                {
                    m[0].clear();
                    m[0].push_back(make_pair(i, j));
                    max_val = mapp[i][j];
                }
                else if (mapp[i][j] == max_val)
                {
                    m[0].push_back(make_pair(i, j));
                }
            }
        }
    }

    if (m[0].size() == 1)
        return 0;
    else // 가장 나중에 공격한
    {
        int oldest_attack = 1000;
        for (int i = 0; i < m[0].size(); i++)
        {
            int x = m[0][i].first;
            int y = m[0][i].second;
            if (attack_time[x][y] < oldest_attack)
            {
                m[1].clear();
                m[1].push_back(make_pair(x, y));
                oldest_attack = attack_time[x][y];
            }
            else if (attack_time[x][y] == oldest_attack)
            {
                m[1].push_back(make_pair(x, y));
            }
        }
    }

    if (m[1].size() == 1)
        return 1;
    else // x+y합이 가장 작은
    {
        int sum = 30;
        for (int i = 0; i < m[1].size(); i++)
        {
            int x = m[1][i].first;
            int y = m[1][i].second;
            if (x + y < sum)
            {
                m[2].clear();
                m[2].push_back(make_pair(x, y));
                sum = x + y;
            }
            else if (x + y == sum)
            {
                m[2].push_back(make_pair(x, y));
            }
        }
    }

    if (m[2].size() == 1)
        return 2;
    else // y값이 가장 작은
    {
        int min_y = 5000;
        for (int i = 0; i < m[2].size(); i++)
        {
            int x = m[2][i].first;
            int y = m[2][i].second;
            if (y < min_y)
            {
                m[3].clear();
                m[3].push_back(make_pair(x, y));
                min_y = y;
            }
        }
    }
    return 3;
    /*
    1. 공격력 높은
    2. 가장 오래전에 공격한
    3. x+y 합이 작은
    4. y값이 작은
    */
}

void reinforce_attacker(int x, int y)
{
    mapp[x][y] += reinforce;
}

int attack_laser(int x_1, int y_1, int x_2, int y_2, string route, int route_cnt, int arr[11][11])
{
    if (arr[x_1][y_1] == 1 || mapp[x_1][y_1] == 0) // 방문 되었거나 값이 0이면 return
        return 0;

    int arr_cpy[11][11]; // copy

    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= M; j++)
        {
            arr_cpy[i][j] = arr[i][j];
        }
    }
    arr_cpy[x_1][y_1] = 1;

    if (x_1 == x_2 && y_1 == y_2) // 도착 완료
    {
        route_ok = 1;              // 전역변수
        if (route_cnt < min_route) // 최솟값 갱신
        {
            route_string = route; // 전역변수 설정
            min_route = route_cnt;
        }
        return 0;
    }
    // 우
    if (y_1 != M)
        attack_laser(x_1, y_1 + 1, x_2, y_2, route + "1", route_cnt + 1, arr_cpy);
    else
        attack_laser(x_1, 1, x_2, y_2, route + "1", route_cnt + 1, arr_cpy);
    // 하
    if (x_1 != N)
        attack_laser(x_1 + 1, y_1, x_2, y_2, route + "2", route_cnt + 1, arr_cpy);
    else
        attack_laser(1, y_1, x_2, y_2, route + "2", route_cnt + 1, arr_cpy);
    // 좌
    if (y_1 != 1)
        attack_laser(x_1, y_1 - 1, x_2, y_2, route + "3", route_cnt + 1, arr_cpy);
    else
        attack_laser(x_1, M, x_2, y_2, route + "3", route_cnt + 1, arr_cpy);
    // 상
    if (x_1 != 1)
        attack_laser(x_1 - 1, y_1, x_2, y_2, route + "4", route_cnt + 1, arr_cpy);
    else // 1이면
        attack_laser(N, y_1, x_2, y_2, route + "4", route_cnt + 1, arr_cpy);

    return 0;
}

int laser_bfs(int x_1, int y_1, int x_2, int y_2)
{
    queue<tuple<int, int, string>> q;
    q.push(make_tuple(x_1, y_1, ""));

    while (!q.empty())
    {
        int x, y;
        string s;
        tuple<int, int, string> item = q.front();
        tie(x, y, s) = item;

        if (x == x_2 && y == y_2) // 경로 찾으면 리턴
        {
            route_ok = 1;
            route_string = s;
            return 0;
        }

        q.pop();
        if (visited[x][y] == 0 && mapp[x][y] != 0)
        {
            visited[x][y] = 1;
            // 우
            if (y != M)
                q.push(make_tuple(x, y + 1, s + "1"));
            else
                q.push(make_tuple(x, 1, s + "1"));
            // 하
            if (x != N)
                q.push(make_tuple(x + 1, y, s + "2"));
            else
                q.push(make_tuple(1, y, s + "2"));
            // 좌
            if (y != 1)
                q.push(make_tuple(x, y - 1, s + "3"));
            else
                q.push(make_tuple(x, M, s + "3"));
            // 상
            if (x != 1)
                q.push(make_tuple(x - 1, y, s + "4"));
            else
                q.push(make_tuple(N, y, s + "4"));
        }
    }
    return 0;
}

void add_together(int x, int y) // 경로 추가 x,y에서 출발
{
    int l = route_string.size();

    for (int i = 0; i < l - 1; i++)
    {
        int direction = route_string[i];
        if (direction == '1')
            y++;
        else if (direction == '2')
            x++;
        else if (direction == '3')
            y--;
        else // direction=='4'
            x--;

        if (x == 0)
            x = N;
        else if (x == N + 1)
            x = 1;
        if (y == 0)
            y = M;
        else if (y == M + 1)
            y = 1;
        together.push_back(make_pair(x, y));
        involved[x][y] = 1;
    }
}
void attack_bomb(int x_1, int y_1, int x_2, int y_2) // 경로 추가
{
    for (int i = 0; i < 8; i++)
    {
        int x = x_2 + dx[i];
        int y = y_2 + dy[i];
        if (x == N + 1)
            x = 1;
        else if (x == 0)
            x = N;
        if (y == M + 1)
            y = 1;
        else if (y == 0)
            y = M;

        if (x != x_1 || y != y_1)
        {
            together.push_back(make_pair(x, y));
            involved[x][y] = 1;
        }
    }
}

int count_remain() // 2명 이상이면 return 1, 1 명이면 return 0-> 게임 종료
{
    int cnt = 0;
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= M; j++)
        {
            if (mapp[i][j] > 0)
            {
                cnt++;
                ans = mapp[i][j];
            }
            if (cnt > 1)
                return 1;
        }
    }
    return 0;
}

int main()
{
    cin >> N >> M >> K;
    reinforce = N + M;
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= M; j++)
        {
            cin >> mapp[i][j];
        }
    }

    while (K--)
    {
        // 공격 피격 대상 찾기.
        int find_min = select_min();
        int find_max = select_max();

        int x_attack = v[find_min][0].first;
        int y_attack = v[find_min][0].second;

        int x_hit = m[find_max][0].first;
        int y_hit = m[find_max][0].second;

        // 공격자 강화
        reinforce_attacker(x_attack, y_attack);

        // 공격
        laser_bfs(x_attack, y_attack, x_hit, y_hit);

        //  경로 추가
        if (route_ok == 1)
            add_together(x_attack, y_attack);
        else
            attack_bomb(x_attack, y_attack, x_hit, y_hit);

        // 공격 (대상과 +a 데미지)
        involved[x_attack][y_attack] = 1;
        involved[x_hit][y_hit] = 1;

        int attack_force = mapp[x_attack][y_attack];
        mapp[x_hit][y_hit] -= attack_force;

        if (mapp[x_hit][y_hit] < 0)
            mapp[x_hit][y_hit] = 0;
        for (int i = 0; i < together.size(); i++)
        {
            mapp[together[i].first][together[i].second] -= attack_force / 2;
            if (mapp[together[i].first][together[i].second] < 0)
                mapp[together[i].first][together[i].second] = 0;
        }

        // 한명 남았는지 확인 -> 맞으면 즉시 break
        if (count_remain() == 0)
        {
            cout << ans;
            return 0;
        }

        // // 공격 안받은 포탑들 +1
        for (int i = 1; i <= N; i++)
        {
            for (int j = 1; j <= M; j++)
            {
                if (involved[i][j] == 0 && mapp[i][j] > 0)
                    mapp[i][j]++;
            }
        }

        // 공격카운트 증가 및  및 변수 초기화&최신화
        attack_time[x_attack][y_attack] = time_cnt;
        time_cnt++;
        v[find_min].clear();
        m[find_max].clear();
        together.clear();
        for (int i = 1; i <= N; i++)
        {
            for (int j = 1; j <= M; j++)
            {
                visited[i][j] = 0;
                involved[i][j] = 0;
            }
        }
        min_route = numeric_limits<int>::max();
        route_string = "";
        route_ok = 0;
    }
    // 결과 출력
    int ans_max = 0;
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= M; j++)
        {
            if (mapp[i][j] > ans_max)
                ans_max = mapp[i][j];
        }
    }
    cout << ans_max;
}