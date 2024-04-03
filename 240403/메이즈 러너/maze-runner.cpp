#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <climits>
using namespace std;

int N, M, K;
int dest_x, dest_y;
int mapp[11][11];
int dx[4] = {-1, 0, 1, 0}; // 상 우 하 좌
int dy[4] = {0, 1, 0, -1};
vector<pair<int, int>> p; // 각 참가자들의 x,y 좌표 -> 완료 시 죽여
vector<int> direc_to[8];
int move_cnt = 0;
int row_length;
int fx, fy;

int is_in(int x, int y);
int move_to(int x, int y);
void init_direc_to();
void reached_p();
void show_p();
void show_cube();
void show_mapp();

void move_p()
{
    for (int i = 0; i < p.size(); i++) // 각 사람들 움직이기
    {
        int x = p[i].first;
        int y = p[i].second;
        int direc = move_to(x, y);
        for (int j = 0; j < direc_to[direc].size(); j++)
        {
            int nx = x + dx[direc_to[direc][j]];
            int ny = y + dy[direc_to[direc][j]];
            if (is_in(nx, ny) && mapp[nx][ny] == 0) // 칸안, 벽x
            {
                p[i].first = nx;
                p[i].second = ny;
                move_cnt++;
                break; // 성공시 다음으로 넘어감
            }
        }
    }
    // cout << "aftermove" << endl;
    // for (int i = 0; i < p.size(); i++)
    // {
    //     int x = p[i].first;
    //     int y = p[i].second;
    //     // cout << i << "..." << x << " " << y << endl;
    // }
}
vector<int> v1; // 정사각형 만들수 있는 가까운 사람 저장
int find_cube_length()
{
    int min_cube = INT_MAX;
    for (int i = 0; i < p.size(); i++)
    {
        int x = p[i].first;
        int y = p[i].second;
        int row = max(abs(dest_x - x), abs(dest_y - y)); // 가로 와 세로중 더 긴거
        // cout << x << "," << y << " row:" << row << endl;
        if (row < min_cube)
        {
            row_length = row + 1; // 전역변수에 저장
            v1.clear();
            v1.push_back(i);
            min_cube = row;
        }
        else if (row == min_cube)
            v1.push_back(i);
    }
    if (v1.size() == 1)
        return 1;
    return 0;
    // 행 작
}
vector<pair<int, int>> v2; // 각 정사각형 좌측 꼭짓점 좌표
void make_cube()
{
    for (int i = 0; i < v1.size(); i++)
    {
        int x = p[v1[i]].first;
        int y = p[v1[i]].second;
        int ax = min(x, dest_x); // 좌측 상단 좌표
        int ay = min(y, dest_y);
        int width = abs(dest_y - y);
        int height = abs(dest_x - x);
        int leng_diff = abs(width - height);
        if (width > height) // 위아래로 늘려야함
        {
            while (leng_diff > 0)
            {
                if (ax > 1)
                    ax--;
                leng_diff--;
            }
        }
        else if (width < height) // 좌우로 늘려야함
        {
            while (leng_diff > 0)
            {
                if (ay > 1)
                    ay--;
                leng_diff--;
            }
        }
        // 정사각형은 그대로
        v2.push_back(make_pair(ax, ay));
    }
}
vector<int> pcube;
void find_in_p()
{
    for (int i = 0; i < p.size(); i++)
    {
        int x = p[i].first;
        int y = p[i].second;
        if (x >= fx && x < fx + row_length && y >= fy && y < fy + row_length)
        {
            pcube.push_back(i);
        }
    }
}
vector<pair<int, int>> v3;
void rotate_cube() // v2 는 현재 동일한 크기의 정사각형에 대해 좌측 상단 x,y 저장된 상태
{
    int min_x = INT_MAX;
    int min_y = INT_MAX;
    for (int i = 0; i < v2.size(); i++)
    {
        int x = v2[i].first;
        int y = v2[i].second;
        if (x < min_x) // x값이 작으면 투입
        {
            min_x = x;
            min_y = y;
        }
        else if (x == min_x && y < min_y) // x값 동일 하면 y 값비교
        {
            min_y = y;
        }
    }
    fx = min_x;
    fy = min_y;
    // cout << "!!!!!!!!!!!!!!!!!!!!" << fx << " " << fy << " " << row_length << endl;
    // row_length
    find_in_p();
    // 전체 지도 회전
    int mapp_temp1[11][11] = {0};
    int mapp_temp2[11][11] = {0};
    for (int i = 0; i < row_length; i++)
        for (int j = 0; j < row_length; j++)
            mapp_temp1[i][j] = mapp[fx + i][fy + j];

    for (int i = 0; i < row_length; i++)
        for (int j = 0; j < row_length; j++)
            mapp_temp2[j][row_length - 1 - i] = mapp_temp1[i][j];

    for (int i = 0; i < row_length; i++)
        for (int j = 0; j < row_length; j++)
        {
            int temp = mapp_temp2[i][j] - 1;
            if (temp == -1)
                temp = 0;
            mapp[fx + i][fy + j] = temp;
        }
    // 안에 있는 사람과, 도착지 회전
    for (int i = 0; i < pcube.size(); i++)
    {
        int x = p[pcube[i]].first;
        int y = p[pcube[i]].second;
        // 0,0 기준으로 옮김
        int nx = x - fx;
        int ny = y - fy;

        int nnx = ny;
        int nny = row_length - 1 - nx;
        p[pcube[i]].first = nnx + fx;
        p[pcube[i]].second = nny + fy;
        // cout << "Res:" << nnx + fx << " " << nny + fy << endl;
    }
    int temp_dx = dest_x - fx;
    int temp_dy = dest_y - fy;
    int nfx = temp_dy;
    int nfy = row_length - 1 - temp_dx;
    dest_x = nfx + fx;
    dest_y = nfy + fy;
    // cout << "dest" << dest_x << dest_y << endl;
}
int main()
{
    // input
    cin >> N >> M >> K;
    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= N; j++)
            cin >> mapp[i][j];
    for (int i = 0; i < M; i++)
    {
        int x, y;
        cin >> x >> y;
        p.push_back(make_pair(x, y));
    }
    cin >> dest_x >> dest_y;
    init_direc_to();

    while (K--)
    {
        // 1. 각 참가자들 조건에 맞춰 이동
        move_p();
        // 도달 시 제거
        reached_p();
        // 2. 정사각형 선정
        find_cube_length();
        make_cube();
        // 3. 정사각형 회전
        rotate_cube();
        // show_mapp();

        // for (int i = 0; i < p.size(); i++)
        // {
        //     cout << i << ": " << p[i].first << "," << p[i].second << endl;
        // }
        // cout << "dest: " << dest_x << "," << dest_y << endl;

        // 다 돌았는지 확인
        if (p.size() == 0)
            break;
        // 변수 초기화
        v1.clear();
        v2.clear();
        v3.clear();
        pcube.clear();
        // cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    }

    // 출력 : 모든참가자 이동거리 합 & 출구 좌표
    cout << move_cnt << endl;
    cout << dest_x << " " << dest_y;
}

void reached_p()
{
    for (int i = 0; i < p.size(); i++)
    {
        int x = p[i].first;
        int y = p[i].second;
        if (x == dest_x && y == dest_y)
        {
            p.erase(p.begin() + i);
            i--;
        }
    }
}
int move_to(int x, int y) // dest_x dest_y 와 비교
{
    if (dest_x < x && dest_y > y) // 상 우
        return 0;
    if (dest_x < x && dest_y < y) // 상 좌
        return 1;
    if (dest_x > x && dest_y < y) // 하 좌
        return 2;
    if (dest_x > x && dest_y > y) // 하 우
        return 3;
    if (dest_x < x && dest_y == y) // 상
        return 4;
    if (dest_x > x && dest_y == y) // 하
        return 5;
    if (dest_x == x && dest_y < y) // 좌
        return 6;
    if (dest_x == x && dest_y > y) // 우
        return 7;
    return -1; // 에러
}
void init_direc_to() // 상0 우1 하2 좌3
{
    direc_to[0].push_back(0);
    direc_to[0].push_back(1);

    direc_to[1].push_back(0);
    direc_to[1].push_back(3);

    direc_to[2].push_back(2);
    direc_to[2].push_back(3);

    direc_to[3].push_back(2);
    direc_to[3].push_back(1);

    direc_to[4].push_back(0);
    direc_to[5].push_back(2);
    direc_to[6].push_back(3);
    direc_to[7].push_back(1);
}
int is_in(int x, int y)
{
    if (x > 0 && x <= N && y > 0 && y <= N)
        return 1;
    return 0;
}
void show_p()
{
    for (int i = 0; i < p.size(); i++)
    {
        int x = p[i].first;
        int y = p[i].second;
        cout << i << " : " << x << "," << y << endl;
    }
}
void show_cube()
{
    for (int i = 0; i < v1.size(); i++)
    {
        cout << v1[i] << endl;
    }
    for (int i = 0; i < v2.size(); i++)
    {
        int x = v2[i].first;
        int y = v2[i].second;
        cout << i << " : " << x << "," << y << endl;
    }
}
void show_mapp()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
            cout << mapp[i][j] << " ";
        cout << endl;
    }
    cout << endl;
}