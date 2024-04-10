// 1220
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <utility>
using namespace std;
int N, M, P, C, D;   // M<1000, P<30
int mapp[51][51];    // 1~N 산타 위치 표시
int position[31][2]; // 산타 x,y 좌표
int points[31];
int life[31]; // 죽었으면 -1, 살았으면 0, 잠시 기절 1,2
int Rx, Ry, Rd;
int ddx[8] = {-1, -1, 0, 1, 1, 1, 0, -1}; // 위 부터 시계방향
int ddy[8] = {0, 1, 1, 1, 0, -1, -1, -1};
int dx[4] = {-1, 0, 1, 0}; // 상 우 하 좌
int dy[4] = {0, 1, 0, -1};

int is_in(int x, int y);
void show_mapp();
int find_santa();
int move_rudolf(int id);
void move_santa(int id);
int check_santa();
void show_P();

void crash_rudolf_to_santa(int direc)
{
    int id = mapp[Rx][Ry]; // 루돌프가 이동해온 위치
    if (id == 0)           // 아무것도 없으면
        return;
    // 점수 획득 & C만크 밀려남
    points[id] += C;
    mapp[Rx][Ry] = 0;
    int nx = position[id][0] + ddx[direc] * C;
    int ny = position[id][1] + ddy[direc] * C;

    if (is_in(nx, ny) == 0) // 죽음
    {
        life[id] = -1;
        return;
    }

    // mapp[nx][ny] 최신화 해줘야됨

    // 칸 안이면
    int nid = mapp[nx][ny];
    queue<int> q; // 연쇄 충돌 산타 모임
    while (nid > 0)
    {
        q.push(nid);
        mapp[nx][ny] = 0; // 현재 위치에서 지워줌

        nx += ddx[direc];
        ny += ddy[direc];
        if (is_in(nx, ny) == 0)
            break;
        nid = mapp[nx][ny];
    }

    while (!q.empty()) // 한칸씩 뒤로 밀어줌, 칸 밖이면 죽임
    {
        int id_move = q.front();
        q.pop();
        int x = position[id_move][0] + ddx[direc];
        int y = position[id_move][1] + ddy[direc];
        if (is_in(x, y) == 0)
        {
            life[id_move] = -1;
            continue;
        }
        mapp[x][y] = id;
        position[id_move][0] += ddx[direc];
        position[id_move][1] += ddy[direc];
    }
    // 날라온 산타 처리
    mapp[position[id][0] + ddx[direc] * C][position[id][1] + ddy[direc] * C] = id;
    position[id][0] = position[id][0] + ddx[direc] * C;
    position[id][1] = position[id][1] + ddy[direc] * C;
    life[id] = 2;
    // cout << "b" << endl;
}

void move_santa(int id)
{
    int x = position[id][0];
    int y = position[id][1];

    int dist_min = (Rx - x) * (Rx - x) + (Ry - y) * (Ry - y); // 현재 위치서 루돌프와의 거리
    int direc = -1;
    for (int i = 0; i < 4; i++)
    {
        int nx = x + dx[i];
        int ny = y + dy[i];
        int dist = (Rx - nx) * (Rx - nx) + (Ry - ny) * (Ry - ny);
        if (is_in(nx, ny) && dist < dist_min && mapp[nx][ny] == 0) // 칸 안, 가장 가까움, 산타 없음
        {
            direc = i;
            dist_min = dist;
        }
    }
    if (direc == -1) // 움직일 수 없다면
    {
        // cout << id << ": cannot move" << endl;
        return;
    }

    mapp[x][y] = 0;
    int nx = x + dx[direc];
    int ny = y + dy[direc];
    if (nx != Rx || ny != Ry) // 루돌프와 충돌x
    {
        // cout << id << ": did not crash" << endl;
        mapp[nx][ny] = id;
        position[id][0] = nx;
        position[id][1] = ny;
        return;
    }
    // 루돌프와 충돌 시
    // cout << id << ": crashed" << endl; // 현재 여기 crashed 작업중
    points[id] += D;
    int nd = (direc + 2) % 4;
    // cout << "nd:" << nd << endl;
    nx += dx[nd] * D;
    ny += dy[nd] * D;

    if (is_in(nx, ny) == 0)
    {
        life[id] = -1;
        return;
    }

    int nid = mapp[nx][ny];
    queue<int> q; // 연쇄 충돌 산타 모임
    while (nid > 0)
    {
        q.push(nid);
        mapp[nx][ny] = 0; // 현재 위치에서 지워줌

        nx += dx[nd];
        ny += dy[nd];
        if (is_in(nx, ny) == 0)
            break;

        nid = mapp[nx][ny];
    }
    // cout << "size: " << q.size() << endl;
    while (!q.empty()) // 한칸씩 뒤로 밀어줌, 칸 밖이면 죽임
    {
        int id_move = q.front();
        q.pop();
        int x = position[id_move][0] + dx[nd];
        int y = position[id_move][1] + dy[nd];
        if (is_in(x, y) == 0)
        {
            life[id_move] = -1;
            continue;
        }
        mapp[x][y] = id_move;
        position[id_move][0] += dx[nd];
        position[id_move][1] += dy[nd];
    }
    // 날라온 산타 처리
    mapp[Rx + dx[nd] * D][Ry + dy[nd] * D] = id;
    position[id][0] = Rx + dx[nd] * D;
    position[id][1] = Ry + dy[nd] * D;
    life[id] = 2;
}

int main()
{
    // input
    cin >> N >> M >> P >> C >> D >> Rx >> Ry;
    for (int i = 0; i < P; i++)
    {
        int ai, ax, ay;
        cin >> ai >> ax >> ay;
        mapp[ax][ay] = ai;
        position[ai][0] = ax;
        position[ai][1] = ay;
    }
    // show_mapp();
    // cout << "--------------------" << endl;
    int turn = 1;
    while (M--)
    {
        // cout << "turn: " << turn++ << endl;
        // cout << Rx << "," << Ry << " " << endl;
        // 가장 가까운 산타 찾기
        int santa_id = find_santa();
        // cout << "select santa." << santa_id << ": " << position[santa_id][0] << "," << position[santa_id][1] << endl;
        // 루돌프 움직이기
        int rd = move_rudolf(santa_id); // 루돌프가 움직인 방향 리턴

        // cout << Rx << "," << Ry << " " << rd << endl;
        // 상호작용 확인
        // show_mapp();
        // cout << "before" << endl;
        crash_rudolf_to_santa(rd);
        // cout << "after_rudolf" << endl;
        // 남은 산타 수 확인
        int live_sant = check_santa();
        if (live_sant == 0)
            break;
        // show_mapp();
        // cout << "after_santa" << endl;
        // 1~P 산타 움직이기
        for (int i = 1; i <= P; i++)
        {
            if (life[i] != 0)
                continue;
            move_santa(i);
        }

        // show_mapp();
        // return 0;

        // 살아있는 넘들 +1
        for (int i = 1; i <= P; i++)
            if (life[i] != -1)
                points[i]++;
        /// 정보 출력//------------------------------------------------------------------------
        // cout << "life" << endl;
        // for (int i = 1; i <= P; i++)
        //     cout << life[i] << " ";
        // 기절 산타들 1턴씩 깨우기
        for (int i = 1; i <= P; i++)
            if (life[i] > 0)
                life[i]--;
        // cout << endl;
        // cout << "points" << endl;
        // for (int i = 1; i <= P; i++)
        //     cout << points[i] << " ";
        // cout << endl;
        // cout << "pos" << endl;
        // show_P();
        // cout << "-----------------------" << endl;
    }

    // 정답 출력
    for (int i = 1; i <= P; i++)
        cout << points[i] << " ";
}
///----------------------------------------------------------------------------//
int check_santa()
{
    int cnt = 0;
    for (int i = 1; i <= P; i++)
        if (life[i] >= 0)
            cnt++;
    return cnt;
}
int move_rudolf(int id) // id 산타를 향해 이동, 그 방향 리턴
{
    int s_x = position[id][0];
    int s_y = position[id][1];

    int min_dist = INT_MAX;
    int pos_x = 0;
    int pos_y = 0;
    int direc;
    for (int i = 0; i < 8; i++)
    {
        int nx = Rx + ddx[i];
        int ny = Ry + ddy[i];
        int dist = (s_x - nx) * (s_x - nx) + (s_y - ny) * (s_y - ny);
        if (is_in(nx, ny) && dist < min_dist)
        {
            direc = i;
            min_dist = dist;
            pos_x = nx;
            pos_y = ny;
        }
    }
    Rx = pos_x;
    Ry = pos_y;
    return direc;
}
int find_santa()
{
    int close_dist = INT_MAX;
    int close_santa = 0;
    int close_santa_max_x = 0;
    int close_santa_max_y = 0;
    for (int i = 1; i <= P; i++)
    {
        if (life[i] == -1) // 죽었으면 무시
            continue;
        int x = position[i][0];
        int y = position[i][1];
        int dist = (x - Rx) * (x - Rx) + (y - Ry) * (y - Ry);
        if (dist < close_dist)
        {
            close_dist = dist;
            close_santa_max_x = x;
            close_santa_max_y = y;
            close_santa = i;
        }
        else if (dist == close_dist && x > close_santa_max_x)
        {
            close_santa_max_x = x;
            close_santa_max_y = y;
            close_santa = i;
        }
        else if (dist == close_dist && x == close_santa_max_x && y > close_santa_max_y)
        {
            close_santa_max_y = y;
            close_santa = i;
        }
    }
    return close_santa;
}
int is_in(int x, int y)
{
    if (x > 0 && y > 0 && x <= N && y <= N)
        return 1;
    return 0;
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
void show_P()
{
    for (int i = 1; i <= P; i++)
    {
        if (life[i] == -1)
            continue;
        cout << i << ":" << position[i][0] << "," << position[i][1] << endl;
        ;
    }
}