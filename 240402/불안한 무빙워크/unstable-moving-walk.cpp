#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <climits>
using namespace std;
bool plate[200]; // i번 발판위에 사람 존재 유무
int life[200];   // life[i]=d     -> i번 발판의 남은 수명
vector<int> v;   // 현재 투입된 사람이 있는 발판 번호
int n, k;
int cnt_0 = 0;
int t = 0;
int ans = 1;

int find_plate(int x) // x번 발판의 위치
{
    return (x + t) % (2 * n);
}
void show_plate()
{
    for (int i = 0; i < 2 * n; i++)
    {
        cout << i << " where: " << find_plate(i);
        cout << " life: " << life[i];
        cout << " bool: " << plate[i] << endl;
    }
    cout << endl;
}
void pop_n() // n-1 번째 위치 pop
{
    // 현재 n-1 번쨰 위치의 발판 번호
    int p = (((2 * n - t) % (2 * n)) + (n - 1)) % (2 * n);
    if (plate[p] == 1)
    {
        plate[p] = 0;
        cnt_0++;
        for (int i = 0; i < v.size(); i++)
            if (v[i] == p)
            {
                v.erase(v.begin() + i);
                break;
            }
    }
}
void move_people()
{
    for (int i = 0; i < v.size(); i++)
    {
        int p = v[i]; // i번째 사람이 존재한 발판번호
        int np = (p + 1) % (2 * n);
        // 앞의 발판으로 갈 수 있는지 확인
        if (plate[np] == 0 && life[np] != 0) // 옮길수 있으면
        {
            plate[p] = 0;
            v[i] = np;
            plate[np] = 1;
            life[np]--;
            if (life[np] == 0)
                cnt_0++;

            if (find_plate(np) == n - 1) // 이 발판이 n-1 번쨰이면 pop
            {
                plate[np] = 0;
                v.erase(v.begin());
            }
        }
    }
}
void push_people()
{
    int p = (2 * n - t) % (2 * n);     // 0번 위치에 있는 발판 번호
    if (plate[p] == 0 && life[p] != 0) // 이 칸에 사람x, 목숨0 x
    {
        plate[p] = 1;
        life[p]--;
        v.push_back(p);
        if (life[p] == 0)
            cnt_0++;
    }
}
int main()
{
    // input
    cin >> n >> k;
    for (int i = 0; i < 2 * n; i++)
        cin >> life[i];

    while (1)
    {
        // 1. 회전 (t 로 알아서 찾아감)
        t = (t + 1) % (2 * n);
        // n-1 번째 위치의 사람 있으면 내보내기
        pop_n();
        // 2. 가장 먼저 올라간 사람부터 이동할 수 있으면 이동 (n번칸에 위치시 즉시 내림)
        move_people();
        // 3. 1번칸에 올릴수 있으면 올리기
        push_people();
        // 4. 안정성 0 k개 이상이면 종료
        if (cnt_0 >= k)
            break;

        // show_plate();
        // if (ans == 15)
        //     break;

        // 횟수 카운트
        ans++;
    }
    // cout << cnt_0;
    cout << ans;
}