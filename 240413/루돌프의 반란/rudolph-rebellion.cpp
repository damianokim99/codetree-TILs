#include <iostream>
#include <queue>

using namespace std;

typedef struct
{
    int i_pos;
    int j_pos;
} Rudolph;

typedef struct
{
    int i_pos;
    int j_pos;
    int stun;
    int out;
    int score;
} Santa;

typedef struct
{
    int dy;
    int dx;
} Direction;

Rudolph rudolph;
Santa santa[31];
Direction dir;
queue<int> q;
queue<int> chain;

int board[51][51];

int sizeOfBoard, numberOfTurn, numberOfSanta, rudolph_power, santa_power;
int closest_santa;
int collision_santa;

void set_board();
void show_board();

bool isRange(int y, int x)
{
    return y >= 1 && y <= sizeOfBoard && x >= 1 && x <= sizeOfBoard;
}

int distance(int r1, int c1, int r2, int c2)
{ // 두 칸 사이의 거리
    return (r1 - r2) * (r1 - r2) + (c1 - c2) * (c1 - c2);
}
void find_closest_santa()
{ // 아웃되지 않은 산타 중 가장 가까운 산타 찾기
    int min = 5000;
    for (int i = 1; i <= numberOfSanta; i++)
    {
        if (!santa[i].out)
        {
            int dist = distance(rudolph.i_pos, rudolph.j_pos, santa[i].i_pos, santa[i].j_pos);
            if (dist < min)
            {
                min = dist;
                closest_santa = i;
            }
            else if (dist == min)
            { // 거리가 같다면
                if (santa[i].i_pos > santa[closest_santa].i_pos)
                { // r좌표가 큰 산타에게 돌진
                    closest_santa = i;
                }
                else if (santa[i].i_pos == santa[closest_santa].i_pos)
                { // r좌표도 같다면
                    if (santa[i].j_pos > santa[closest_santa].j_pos)
                    { // c좌표가 큰 산타에게 돌진
                        closest_santa = i;
                    }
                }
            }
        }
    }
}
void move_to_closest()
{
    if (rudolph.i_pos == santa[closest_santa].i_pos && rudolph.j_pos < santa[closest_santa].j_pos)
    { // 산타가 오른쪽에 있다면
        rudolph.j_pos++;
        // direction 설정
        dir.dy = 0;
        dir.dx = 1;
    }
    else if (rudolph.i_pos == santa[closest_santa].i_pos && rudolph.j_pos > santa[closest_santa].j_pos)
    { // 산타가 왼쪽에 있다면
        rudolph.j_pos--;
        // direction 설정
        dir.dy = 0;
        dir.dx = -1;
    }
    else if (rudolph.i_pos < santa[closest_santa].i_pos && rudolph.j_pos == santa[closest_santa].j_pos)
    { // 산타가 아래쪽에 있다면
        rudolph.i_pos++;
        // direction 설정
        dir.dy = 1;
        dir.dx = 0;
    }
    else if (rudolph.i_pos > santa[closest_santa].i_pos && rudolph.j_pos == santa[closest_santa].j_pos)
    { // 산타가 위쪽에 있다면
        rudolph.i_pos--;
        // direction 설정
        dir.dy = -1;
        dir.dx = 0;
    }

    else if (rudolph.i_pos > santa[closest_santa].i_pos && rudolph.j_pos < santa[closest_santa].j_pos)
    { // 산타가 오른쪽 위에 있다면
        rudolph.i_pos--;
        rudolph.j_pos++;
        // direction 설정
        dir.dy = -1;
        dir.dx = 1;
    }
    else if (rudolph.i_pos < santa[closest_santa].i_pos && rudolph.j_pos < santa[closest_santa].j_pos)
    { // 산타가 오른쪽 아래에 있다면
        rudolph.i_pos++;
        rudolph.j_pos++;
        // direction 설정
        dir.dy = 1;
        dir.dx = 1;
    }
    else if (rudolph.i_pos < santa[closest_santa].i_pos && rudolph.j_pos > santa[closest_santa].j_pos)
    { // 산타가 왼쪽 아래에 있다면
        rudolph.i_pos++;
        rudolph.j_pos--;
        // direction 설정
        dir.dy = 1;
        dir.dx = -1;
    }
    else if (rudolph.i_pos > santa[closest_santa].i_pos && rudolph.j_pos > santa[closest_santa].j_pos)
    { // 산타가 왼쪽 위에 있다면
        rudolph.i_pos--;
        rudolph.j_pos--;
        // direction 설정
        dir.dy = -1;
        dir.dx = -1;
    }
}

bool isCollision()
{ // 충돌이 일어났는지 확인
    for (int i = 1; i <= numberOfSanta; i++)
    {
        if (santa[i].i_pos == rudolph.i_pos && santa[i].j_pos == rudolph.j_pos)
        {
            collision_santa = i;
            return true;
        }
    }
    return false;
}
void collision_by_rudolph()
{ // 루돌프에 의한 충돌

    // 루돌프의 힘만큼 밀려남
    santa[collision_santa].i_pos += rudolph_power * dir.dy;
    santa[collision_santa].j_pos += rudolph_power * dir.dx;
    // 산타 점수 추가
    santa[collision_santa].score += rudolph_power;
    santa[collision_santa].stun = 2; // 2턴 동안 기절 처리

    if (!isRange(santa[collision_santa].i_pos, santa[collision_santa].j_pos))
    { // 보드판 밖이라면 아웃처리
        santa[collision_santa].out = true;
    }
    else if (board[santa[collision_santa].i_pos][santa[collision_santa].j_pos] > 0)
    { // 착지한 곳에 다른 산타가 있다면

        q.push(board[santa[collision_santa].i_pos][santa[collision_santa].j_pos]);
        chain.push(board[santa[collision_santa].i_pos][santa[collision_santa].j_pos]);

        while (!q.empty())
        {
            int cy = santa[q.front()].i_pos;
            int cx = santa[q.front()].j_pos;
            q.pop();

            if (isRange(cy + dir.dy, cx + dir.dx && board[cy + dir.dy][cx + dir.dx] > 0))
            { // 범위 안에 있고 다른 산타가 있다면
                q.push(board[cy + dir.dy][cx + dir.dx]);
                chain.push(board[cy + dir.dy][cx + dir.dx]);
            }
        }
    }

    while (!chain.empty())
    { // 연쇄작용이 일어난 모든 산타들에게
        int x = chain.front();
        chain.pop();

        santa[x].i_pos += dir.dy;
        santa[x].j_pos += dir.dx;

        if (!isRange(santa[x].i_pos, santa[x].j_pos))
        {
            santa[x].out = 1;
        }
    }
}
void move_rudolph()
{                         // 루돌프 움직임
    find_closest_santa(); // 가장 가까운 산타를 찾고
    move_to_closest();
    if (isCollision())
    {
        collision_by_rudolph();
    }
}
void move_to_rudolph(int i)
{ // 루돌프에게 이동

    if (santa[i].i_pos > rudolph.i_pos && santa[i].j_pos == rudolph.j_pos)
    { // 루돌프가 위에 있다면
        if (board[santa[i].i_pos - 1][santa[i].j_pos] <= 0)
        {                     // 위에 다른 산타가 없다면
            santa[i].i_pos--; // 위로 이동
            // direction 설정
            dir.dy = -1;
            dir.dx = 0;
        }
    }
    else if (santa[i].i_pos == rudolph.i_pos && santa[i].j_pos < rudolph.j_pos)
    { // 루돌프가 오른쪽에 있다면
        if (board[santa[i].i_pos][santa[i].j_pos + 1] <= 0)
        {                     // 오른쪽에 다른 산타가 없다면
            santa[i].j_pos++; // 오른쪽으로 이동
            // direction 설정
            dir.dy = 0;
            dir.dx = 1;
        }
    }
    else if (santa[i].i_pos < rudolph.i_pos && santa[i].j_pos == rudolph.j_pos)
    { // 루돌프가 아래에 있다면
        if (board[santa[i].i_pos + 1][santa[i].j_pos] <= 0)
        {                     // 아래에 다른 산타가 없다면
            santa[i].i_pos++; // 아래로 이동
            // direction 설정
            dir.dy = 1;
            dir.dx = 0;
        }
    }
    else if (santa[i].i_pos == rudolph.i_pos && santa[i].j_pos > rudolph.j_pos)
    { // 루돌프가 왼쪽에 있다면
        if (board[santa[i].i_pos][santa[i].j_pos - 1] <= 0)
        {                     // 왼쪽에 다른 산타가 없다면
            santa[i].j_pos--; // 왼쪽으로 이동
            // direction 설정
            dir.dy = 0;
            dir.dx = -1;
        }
    }

    // 상우하좌 우선순위
    else if (santa[i].i_pos > rudolph.i_pos && santa[i].j_pos > rudolph.j_pos)
    { // 루돌프가 왼쪽 위에 있다면
        // 위로 가는게 왼쪽으로 가는것보다 루돌프에게 가까워진다면
        if (distance(santa[i].i_pos - 1, santa[i].j_pos, rudolph.i_pos, rudolph.j_pos) <= distance(santa[i].i_pos, santa[i].j_pos - 1, rudolph.i_pos, rudolph.j_pos))
        {
            if (board[santa[i].i_pos - 1][santa[i].j_pos] <= 0)
            {                     // 위에 다른 산타가 없다면
                santa[i].i_pos--; // 위로 이동
                // direction 설정
                dir.dy = -1;
                dir.dx = 0;
            }
            else
            {
                if (board[santa[i].i_pos][santa[i].j_pos - 1] <= 0)
                {                     // 왼쪽에 다른 산타가 없다면
                    santa[i].j_pos--; // 왼쪽으로 이동
                    // direction 설정
                    dir.dy = 0;
                    dir.dx = -1;
                }
            }
        }
        else
        { // 왼쪽으로 가는게 더 가깝다면
            if (board[santa[i].i_pos][santa[i].j_pos - 1] <= 0)
            {                     // 왼쪽에 다른 산타가 없다면
                santa[i].j_pos--; // 왼쪽으로 이동
                // direction 설정
                dir.dy = 0;
                dir.dx = -1;
            }
            else
            {
                if (board[santa[i].i_pos - 1][santa[i].j_pos] <= 0)
                {                     // 위에 다른 산타가 없다면
                    santa[i].i_pos--; // 위로 이동
                    // direction 설정
                    dir.dy = -1;
                    dir.dx = 0;
                }
            }
        }
    }
    else if (santa[i].i_pos > rudolph.i_pos && santa[i].j_pos < rudolph.j_pos)
    { // 루돌프가 오른쪽 위에 있다면
        // 위로가는게 오른쪽으로 가는것보다 가깝다면
        if (distance(santa[i].i_pos - 1, santa[i].j_pos, rudolph.i_pos, rudolph.j_pos) <= distance(santa[i].i_pos, santa[i].j_pos + 1, rudolph.i_pos, rudolph.j_pos))
        {
            if (board[santa[i].i_pos - 1][santa[i].j_pos] <= 0)
            {                     // 위에 다른 산타가 없다면
                santa[i].i_pos--; // 위로 이동
                // direction 설정
                dir.dy = -1;
                dir.dx = 0;
            }
            else
            {
                if (board[santa[i].i_pos][santa[i].j_pos + 1] <= 0)
                {                     // 오른쪽에 다른 산타가 없다면
                    santa[i].j_pos++; // 오른쪽으로 이동
                    // direction 설정
                    dir.dy = 0;
                    dir.dx = 1;
                }
            }
        }
        else
        { // 오른쪽이 더 가깝다면
            if (board[santa[i].i_pos][santa[i].j_pos + 1] <= 0)
            {                     // 오른쪽에 다른 산타가 없다면
                santa[i].j_pos++; // 오른쪽으로 이동
                // direction 설정
                dir.dy = 0;
                dir.dx = 1;
            }
            else
            {
                if (board[santa[i].i_pos - 1][santa[i].j_pos] <= 0)
                {                     // 위에 다른 산타가 없다면
                    santa[i].i_pos--; // 위로 이동
                    // direction 설정
                    dir.dy = -1;
                    dir.dx = 0;
                }
            }
        }
    }
    else if (santa[i].i_pos < rudolph.i_pos && santa[i].j_pos < rudolph.j_pos)
    { // 루돌프가 오른쪽 아래에 있다면
        // 오른쪽으로 가는게 아래로 가는것보다 가깝다면
        if (distance(santa[i].i_pos, santa[i].j_pos + 1, rudolph.i_pos, rudolph.j_pos) <= distance(santa[i].i_pos + 1, santa[i].j_pos, rudolph.i_pos, rudolph.j_pos))
        {
            if (board[santa[i].i_pos][santa[i].j_pos + 1] <= 0)
            {                     // 오른쪽에 다른 산타가 없다면
                santa[i].j_pos++; // 오른쪽으로 이동
                // direction 설정
                dir.dy = 0;
                dir.dx = 1;
            }
            else
            {
                if (board[santa[i].i_pos + 1][santa[i].j_pos] <= 0)
                {                     // 아래에 다른 산타가 없다면
                    santa[i].i_pos++; // 아래로 이동
                    // direction 설정
                    dir.dy = 1;
                    dir.dx = 0;
                }
            }
        }
        else
        {
            if (board[santa[i].i_pos + 1][santa[i].j_pos] <= 0)
            {                     // 아래에 다른 산타가 없다면
                santa[i].i_pos++; // 아래로 이동
                // direction 설정
                dir.dy = 1;
                dir.dx = 0;
            }
            else
            {
                if (board[santa[i].i_pos][santa[i].j_pos + 1] <= 0)
                {                     // 오른쪽에 다른 산타가 없다면
                    santa[i].j_pos++; // 오른쪽으로 이동
                    // direction 설정
                    dir.dy = 0;
                    dir.dx = 1;
                }
            }
        }
    }
    else if (santa[i].i_pos < rudolph.i_pos && santa[i].j_pos > rudolph.j_pos)
    { // 루돌프가 왼쪽 아래에 있다면
        // 아래로 가는게 더 가깝다면
        if (distance(santa[i].i_pos + 1, santa[i].j_pos, rudolph.i_pos, rudolph.j_pos) <= distance(santa[i].i_pos, santa[i].j_pos - 1, rudolph.i_pos, rudolph.j_pos))
        {
            if (board[santa[i].i_pos + 1][santa[i].j_pos] <= 0)
            {                     // 아래에 다른 산타가 없다면
                santa[i].i_pos++; // 아래로 이동
                // direction 설정
                dir.dy = 1;
                dir.dx = 0;
            }
            else
            {
                if (board[santa[i].i_pos][santa[i].j_pos - 1] <= 0)
                {                     // 왼쪽에 다른 산타가 없다면
                    santa[i].j_pos--; // 왼쪽으로 이동
                    // direction 설정
                    dir.dy = 0;
                    dir.dx = -1;
                }
            }
        }
        else
        {
            if (board[santa[i].i_pos][santa[i].j_pos - 1] <= 0)
            {                     // 왼쪽에 다른 산타가 없다면
                santa[i].j_pos--; // 왼쪽으로 이동
                // direction 설정
                dir.dy = 0;
                dir.dx = -1;
            }
            else
            {
                if (board[santa[i].i_pos + 1][santa[i].j_pos] <= 0)
                {                     // 아래에 다른 산타가 없다면
                    santa[i].i_pos++; // 아래로 이동
                    // direction 설정
                    dir.dy = 1;
                    dir.dx = 0;
                }
            }
        }
    }
}
bool isCollision(int i)
{
    if (santa[i].i_pos == rudolph.i_pos && santa[i].j_pos == rudolph.j_pos)
    {
        return true;
    }
    else
    {
        return false;
    }
}
void collision_by_santa(int i)
{
    // 산타의 힘만큼 밀려남
    // cout << santa[i].i_pos << santa[i].j_pos << endl;
    santa[i].i_pos -= santa_power * dir.dy;
    santa[i].j_pos -= santa_power * dir.dx;
    // cout << santa[i].i_pos << santa[i].j_pos << endl;
    // 산타 점수 추가
    santa[i].score += santa_power;
    santa[i].stun = 2; // 2턴 동안 기절 처리
    // cout << santa[i].score << endl;
    if (!isRange(santa[i].i_pos, santa[i].j_pos))
    { // 보드판 밖이라면 아웃처리
        santa[i].out = true;
    }
    else if (board[santa[i].i_pos][santa[i].j_pos] > 0 && board[santa[i].i_pos][santa[i].j_pos] != i)
    { // 착지한 곳에 다른 산타가 있다면

        q.push(board[santa[i].i_pos][santa[i].j_pos]);
        chain.push(board[santa[i].i_pos][santa[i].j_pos]);

        while (!q.empty())
        {
            int cy = santa[q.front()].i_pos;
            int cx = santa[q.front()].j_pos;
            q.pop();

            if (isRange(cy - dir.dy, cx - dir.dx) && board[cy - dir.dy][cx - dir.dx] > 0)
            { // 범위 안에 있고 다른 산타가 있다면
                q.push(board[cy - dir.dy][cx - dir.dx]);
                chain.push(board[cy - dir.dy][cx - dir.dx]);
            }
        }

        while (!chain.empty())
        { // 연쇄작용이 일어난 모든 산타들에게
            int x = chain.front();
            chain.pop();

            santa[x].i_pos -= dir.dy;
            santa[x].j_pos -= dir.dx;

            if (!isRange(santa[x].i_pos, santa[x].j_pos))
            {
                santa[x].out = 1;
            }
        }
    }
}

void move_santa()
{ // 산타 이동
    for (int i = 1; i <= numberOfSanta; i++)
    {
        if (santa[i].stun <= 0 && !santa[i].out)
        { // 기절, 아웃되지 않은 산타라면
            move_to_rudolph(i);
            if (isCollision(i))
            {
                collision_by_santa(i);
                set_board();
                // cout << santa[i].i_pos << santa[i].j_pos << endl;
            }
            else
            {
                board[santa[i].i_pos][santa[i].j_pos] = i;
                board[santa[i].i_pos - dir.dy][santa[i].j_pos - dir.dx] = 0;
            }
        }
    }
}

bool isEnd()
{
    int count = 0;
    for (int i = 1; i <= numberOfSanta; i++)
    {
        if (santa[i].out)
        {
            count++;
        }
    }

    if (count == numberOfSanta)
    {
        return true;
    }
    else
    {
        return false;
    }
}
void result()
{
    for (int i = 1; i <= numberOfSanta; i++)
    {
        cout << santa[i].score << " ";
    }
}
void plus_score()
{
    for (int i = 1; i <= numberOfSanta; i++)
    {
        if (!santa[i].out)
        {
            santa[i].score++;
        }
    }
}
void minus_stun()
{
    for (int i = 1; i <= numberOfSanta; i++)
    {
        if (!santa[i].out)
        {
            if (santa[i].stun > 0)
            {
                santa[i].stun--;
            }
        }
    }
}
void show_santa()
{
    for (int i = 1; i <= numberOfSanta; i++)
    {
        printf("%d santa : (%d,%d), out : %d, stun : %d, score : %d\n", i, santa[i].i_pos, santa[i].j_pos, santa[i].out, santa[i].stun, santa[i].score);
    }
}
int main()
{
    cin >> sizeOfBoard >> numberOfTurn >> numberOfSanta >> rudolph_power >> santa_power;

    cin >> rudolph.i_pos >> rudolph.j_pos;

    for (int i = 1; i <= numberOfSanta; i++)
    {

        int num, i_pos, j_pos;
        cin >> num >> i_pos >> j_pos;

        santa[num].i_pos = i_pos;
        santa[num].j_pos = j_pos;
    }

    for (int i = 1; i <= numberOfTurn; i++)
    {
        // cout << "turn : " << i << endl;
        // cout << santa[1].stun << "," << santa[1].out << endl;
        set_board();
        // show_board();
        move_rudolph();
        if (isEnd())
        {
            break;
        }
        // cout << "rudolph moved " << endl;
        set_board();
        // show_board();
        move_santa();
        // cout << "santa moved " << endl;
        // show_board();
        if (isEnd())
        {
            break;
        }
        plus_score();
        minus_stun();
        // show_santa();
    }
    result();

    return 0;
}

void set_board()
{
    for (int i = 1; i <= sizeOfBoard; i++)
    {
        for (int j = 1; j <= sizeOfBoard; j++)
        {
            board[i][j] = 0;
        }
    }
    for (int i = 1; i <= numberOfSanta; i++)
    {
        board[santa[i].i_pos][santa[i].j_pos] = i;
    }
    board[rudolph.i_pos][rudolph.j_pos] = -1;
}
void show_board()
{
    for (int i = 1; i <= sizeOfBoard; i++)
    {
        for (int j = 1; j <= sizeOfBoard; j++)
        {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}