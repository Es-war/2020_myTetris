#include <graphics.h>
#include <conio.h>
#include <cmath>
#include <ctime>
#include <windows.h>

const double pi = acos(-1);

DWORD time_now[2];
bool flag_next[2];
bool flag_over[2];
int next_c_idx[2];
int next_s_idx[2];
int next_d_idx[2];
int now_c_idx[2];
int now_s_idx[2];
int now_d_idx[2];
int now_mp_x[2];
int now_mp_y[2];
int score[2];

COLORREF tmp_line_color;
LINESTYLE tmp_line_style;
COLORREF tmp_text_color;
LOGFONT tmp_text_style;
COLORREF tmp_fill_color;

COLORREF colors[7] = { RGB(255, 0, 0), RGB(255, 165, 0), RGB(255, 255, 0), RGB(0, 128, 0), RGB(0, 255, 255), RGB(0, 0, 255), RGB(128, 0, 128) };
int mp[2][10][15];
int mp_tmp_one[10][15];
int mp_tmp_two[10][15];
COLORREF mp_c[2][10][15];
COLORREF mp_c_tmp_one[10][15];
COLORREF mp_c_tmp_two[10][15];
wchar_t score_tips[2][30];
wchar_t over_tips[50];

struct Square
{
    int dir[4][4][2];
};

enum Cmd
{
    Cmd_rotate, // ������ת
    Cmd_left,   // ��������
    Cmd_right,  // ��������
    Cmd_down    // ��������
};

Square squares[7] = {
    {0, -2, 0, -1, 0, 0, 1, 0, 0, -1, 1, -1, 2, -1, 0, 0, 0, -2, 1, -2, 1, -1, 1, 0, 0, 0, 1, 0, 2, 0, 2, -1},      //  L ��
    {0, 0, 1, 0, 1, -1, 1, -2, 0, -1, 0, 0, 1, 0, 2, 0, 0, -2, 0, -1, 0, 0, 1, -2, 0, -1, 1, -1, 2, -1, 2, 0},      //  L �ͣ�����
    {0, -1, 0, 0, 1, -1, 1, 0, 0, -1, 0, 0, 1, -1, 1, 0, 0, -1, 0, 0, 1, -1, 1, 0, 0, -1, 0, 0, 1, -1, 1, 0},       // �� ��
    {0, 0, 1, -1, 1, 0, 2, 0, 1, -2, 1, -1, 1, 0, 2, -1, 0, -1, 1, -1, 1, 0, 2, -1, 0, -1, 1, -2, 1, -1, 1, 0},     // ɽ ��
    {0, -3, 0, -2, 0, -1, 0, 0, 0, -3, 1, -3, 2, -3, 3, -3, 0, -3, 0, -2, 0, -1, 0, 0, 0, -3, 1, -3, 2, -3, 3, -3}, //  | ��
    {0, -1, 1, -1, 1, 0, 2, 0, 0, -1, 0, 0, 1, -2, 1, -1, 0, -1, 1, -1, 1, 0, 2, 0, 0, -1, 0, 0, 1, -2, 1, -1},     //  Z ��
    {0, 0, 1, -1, 1, 0, 2, -1, 1, -2, 1, -1, 2, -1, 2, 0, 0, 0, 1, -1, 1, 0, 2, -1, 1, -2, 1, -1, 2, -1, 2, 0}      //  Z �ͣ�����
};

Cmd getTwoCmd()
{
    while (true)
    {
        //ָ�ʱ
        DWORD time_tmp = GetTickCount();
        if (time_tmp - time_now[1] >= 1000)
        {
            time_now[1] = time_tmp;
            return Cmd_down;
        }

        //����ָ��
        if (_kbhit())
        {
            //��˴�Сд
            switch (_getch())
            {
            case 'W':
            case 'w':
                return Cmd_rotate;
            case 'A':
            case 'a':
                return Cmd_left;
            case 'D':
            case 'd':
                return Cmd_right;
            case 'S':
            case 's':
                return Cmd_down;
            }
        }

        // ����CPUռ��
        Sleep(20);
    }
}
Cmd getOneCmd()
{
    while (true)
    {
        //ָ�ʱ
        DWORD time_tmp = GetTickCount();
        if (time_tmp - time_now[0] >= 1000)
        {
            time_now[0] = time_tmp;
            return Cmd_down;
        }

        //����ָ��
        if (_kbhit())
        {
            switch (_getch())
            {
            case 72:
                return Cmd_rotate;
            case 75:
                return Cmd_left;
            case 77:
                return Cmd_right;
            case 80:
                return Cmd_down;
            }
        }

        // ����CPUռ��
        Sleep(20);
    }
}

class paint
{
public:
    void initEnviroment();
    void drawGameBG();
    void drawLeftSide();
    void drawRightSide();
    void drawLeftMap();
    void drawRightMap();
    void drawSquareNext(int num);
    void drawItem(int x, int y, COLORREF c);
    void drawSquareNow(int num);
};
void paint::initEnviroment()
{
    // ��������
    initgraph(1210, 540);
    HWND hwnd = GetHWnd();
    SetWindowText(hwnd, L"����˹");
    SetWindowPos(hwnd, HWND_TOP, 0, 20, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);

    // ��ͼģʽ����
    setbkmode(TRANSPARENT);

    // ���������
    srand(time(NULL));
}
void paint::drawGameBG()
{
    COLORREF tmp = getlinecolor();

    for (int i = 0; i < 541; ++i)
    {
        setlinecolor(RGB(135, 206, 250 - i / 5));
        // line(0, 540 - i, 360, 540 - i);
        line(241, 540 - i, 600, 540 - i);
        line(611, 540 - i, 970, 540 - i);
    }

    setlinecolor(tmp);
}
void paint::drawLeftSide()
{
    tmp_line_color = getlinecolor();
    getlinestyle(&tmp_line_style);
    tmp_text_color = getlinecolor();
    gettextstyle(&tmp_text_style);

    for (int i = 0; i < 541; ++i)
    {
        setlinecolor(RGB(224, 178, 220 - i / 15));
        line(0, i, 240, i);
    }
    settextcolor(RGB(65, 105, 225));
    settextstyle(30, 0, L"���Ŀ���");
    outtextxy(48, 300, L"���ƣ�A");
    outtextxy(48, 330, L"���ƣ�D");
    outtextxy(48, 360, L"���Σ�W");
    outtextxy(48, 390, L"���䣺S");

    setlinecolor(WHITE);
    // rectangle(360 + 48, 36, 600 - 48, 36 + 144);
    rectangle(48, 36, 240 - 48, 36 + 144);
    //�������δ����������Ⱦ��ʾ��
    drawSquareNext(0);
    setlinecolor(0x7FFFAA);
    // rectangle(360 + 48, 36, 600 - 48, 36 + 144);
    rectangle(48, 36, 240 - 48, 36 + 144);
    swprintf(score_tips[0], 29, L"�÷֣�%d", score[0] * 100);
    outtextxy(48, 200, score_tips[0]);

    setlinecolor(tmp_line_color);
    setlinestyle(&tmp_line_style);
    settextcolor(tmp_text_color);
    settextstyle(&tmp_text_style);
}
void paint::drawRightSide()
{
    tmp_line_color = getlinecolor();
    getlinestyle(&tmp_line_style);
    tmp_text_color = getlinecolor();
    gettextstyle(&tmp_text_style);

    for (int i = 0; i < 541; ++i)
    {
        setlinecolor(RGB(224, 178, 220 - i / 15));
        line(971, i, 1210, i);
    }
    settextcolor(RGB(65, 105, 225));
    settextstyle(30, 0, L"���Ŀ���");
    outtextxy(970 + 48, 300, L"���ƣ���");
    outtextxy(970 + 48, 330, L"���ƣ���");
    outtextxy(970 + 48, 360, L"���Σ���");
    outtextxy(970 + 48, 390, L"���䣺��");

    setlinecolor(WHITE);
    // rectangle(360 + 48, 36, 600 - 48, 36 + 144);
    rectangle(970 + 48, 36, 1210 - 48, 36 + 144);
    //�������δ����������Ⱦ��ʾ��
    drawSquareNext(1);
    setlinecolor(0x7FFFAA);
    // rectangle(360 + 48, 36, 600 - 48, 36 + 144);
    rectangle(970 + 48, 36, 1210 - 48, 36 + 144);

    swprintf(score_tips[1], 29, L"�÷֣�%d", score[1] * 100);
    outtextxy(970 + 48, 200, score_tips[1]);

    setlinecolor(tmp_line_color);
    setlinestyle(&tmp_line_style);
    settextcolor(tmp_text_color);
    settextstyle(&tmp_text_style);
}
void paint::drawLeftMap()
{
    //һ�����
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 15; j++)
            if (mp[0][i][j])
                drawItem(240 + i * 36, j * 36, mp_c[0][i][j]);
}
void paint::drawRightMap()
{
    //�������
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 15; j++)
            if (mp[1][i][j])
                drawItem(610 + i * 36, j * 36, mp_c[1][i][j]);
}
void paint::drawItem(int x, int y, COLORREF c)
{
    // �������
    // ʵ�ʳߴ磺32 * 32����λ�����أ�
    // �߿���ɫ����ɫ
    // Բ�ǰ뾶��4����λ�����أ�
    // �ڲ���ࣺ2����λ�����أ�
    tmp_fill_color = getfillcolor();

    const int r = 6;
    const int p = 2;

    int up_l_x = x + p + r;
    int up_r_x = x + 36 - p - r;
    int up___y = y + p;

    int down_l_x = x + p + r;
    int down_r_x = x + 36 - p - r;
    int down___y = y + 36 - p;

    int left_u_y = y + p + r;
    int left_d_y = y + 36 - p - r;
    int left___x = x + p;

    int right_u_y = y + p + r;
    int right_d_y = y + 36 - p - r;
    int right___x = x + 36 - p;

    line(up_l_x, up___y, up_r_x, up___y);
    line(down_l_x, down___y, down_r_x, down___y);
    line(left___x, left_u_y, left___x, left_d_y);
    line(right___x, right_u_y, right___x, right_d_y);
    arc(x + p, y + p, x + p + 2 * r, y + p + 2 * r, pi / 2, pi);
    arc(x + 36 - p, y + p, x + 36 - p - 2 * r, y + p + 2 * r, 0, pi / 2);
    arc(x + p, y + 36 - p, x + p + 2 * r, y + 36 - p - 2 * r, -pi, -pi / 2);
    arc(x + 36 - p, y + 36 - p, x + 36 - p - 2 * r, y + 36 - p - 2 * r, -pi / 2, 0);

    setfillcolor(c);
    floodfill(x + p + r + 1, y + p + r + 1, WHITE);

    setfillcolor(tmp_fill_color);
}
void paint::drawSquareNext(int num)
{
    int tmp_x = 48, tmp_y = 36 + 108;
    if (num)
    {
        tmp_x += 970;
    }
    COLORREF c = colors[next_c_idx[num]];
    drawItem(tmp_x + squares[next_s_idx[num]].dir[next_d_idx[num]][0][0] * 36, tmp_y + squares[next_s_idx[num]].dir[next_d_idx[num]][0][1] * 36, c);
    drawItem(tmp_x + squares[next_s_idx[num]].dir[next_d_idx[num]][1][0] * 36, tmp_y + squares[next_s_idx[num]].dir[next_d_idx[num]][1][1] * 36, c);
    drawItem(tmp_x + squares[next_s_idx[num]].dir[next_d_idx[num]][2][0] * 36, tmp_y + squares[next_s_idx[num]].dir[next_d_idx[num]][2][1] * 36, c);
    drawItem(tmp_x + squares[next_s_idx[num]].dir[next_d_idx[num]][3][0] * 36, tmp_y + squares[next_s_idx[num]].dir[next_d_idx[num]][3][1] * 36, c);
}
void paint::drawSquareNow(int num)
{
    drawItem((now_mp_x[num] + squares[now_s_idx[num]].dir[now_d_idx[num]][0][0]) * 36, (now_mp_y[num] + squares[now_s_idx[num]].dir[now_d_idx[num]][0][1]) * 36, colors[now_c_idx[num]]);
    drawItem((now_mp_x[num] + squares[now_s_idx[num]].dir[now_d_idx[num]][1][0]) * 36, (now_mp_y[num] + squares[now_s_idx[num]].dir[now_d_idx[num]][1][1]) * 36, colors[now_c_idx[num]]);
    drawItem((now_mp_x[num] + squares[now_s_idx[num]].dir[now_d_idx[num]][2][0]) * 36, (now_mp_y[num] + squares[now_s_idx[num]].dir[now_d_idx[num]][2][1]) * 36, colors[now_c_idx[num]]);
    drawItem((now_mp_x[num] + squares[now_s_idx[num]].dir[now_d_idx[num]][3][0]) * 36, (now_mp_y[num] + squares[now_s_idx[num]].dir[now_d_idx[num]][3][1]) * 36, colors[now_c_idx[num]]);
}

class player
{
private:
    int num;

public:
    player(int n = 0)
    {
        num = n;
    }
    void execCmd(Cmd cmd);                          //ִ������
    void moveRotate();                              //������ת
    void moveLeft();                                //��������
    void moveRight();                               //��������
    void moveDown();                                //�����½�
    bool checkPut(int mp_x, int mp_y, int dir_idx); //���ı��Ƿ����
    void recordSquareNow();
    void execClear();          //������
    bool checkOver();          //����Ƿ����
    void initDatasPerSquare(); //��ʼ���µķ���
    void initDatasPerRound();  //��ʼ��ÿ����Ϸ
};
void player::execCmd(Cmd cmd)
{
    switch (cmd)
    {
    case Cmd_down:
        moveDown();
        break;
    case Cmd_left:
        moveLeft();
        break;
    case Cmd_right:
        moveRight();
        break;
    case Cmd_rotate:
        moveRotate();
        break;
    }
}
void player::moveRotate()
{
    // ����ʣ��������״��������ת��������ת״̬
    for (int i = 1; i <= 3; ++i)
        if (checkPut(now_mp_x[num], now_mp_y[num], (now_d_idx[num] + i) % 4))
        {
            now_d_idx[num] = (now_d_idx[num] + i) % 4;
            break;
        }
}
void player::moveLeft()
{
    // �����ܷ�����
    if (checkPut(now_mp_x[num] - 1, now_mp_y[num], now_d_idx[num]))
        --now_mp_x[num];
}
void player::moveRight()
{
    // �����ܷ�����
    if (checkPut(now_mp_x[num] + 1, now_mp_y[num], now_d_idx[num]))
        ++now_mp_x[num];
}
void player::moveDown()
{
    // �����ܷ�����
    if (checkPut(now_mp_x[num], now_mp_y[num] + 1, now_d_idx[num]))
    {
        ++now_mp_y[num];
        return;
    }
    // ����������˵����鷽�顰�������ˣ�ִ�����²���
    // 1����ʾ���Կ�ʼ��һ������
    // 2���������¼��map��ͼ��
    // 3���ж��Ƿ��������
    // 4���ж����к���Ϸ�Ƿ����
    flag_next[num] = 1;
    recordSquareNow();
    execClear();
    if (checkOver())
        flag_over[num] = 1;
}
void player::recordSquareNow()
{
    int sq_x[4], sq_y[4];
    for (int i = 0; i < 4; i++)
    {
        sq_x[i] = now_mp_x[num] + squares[now_s_idx[num]].dir[now_d_idx[num]][i][0];
        sq_y[i] = now_mp_y[num] + squares[now_s_idx[num]].dir[now_d_idx[num]][i][1];
    }
    for (int i = 0; i < 4; i++)
    {
        if (sq_y[i] >= 0)
        {
            mp[num][sq_x[i]][sq_y[i]] = 1;
            mp_c[num][sq_x[i]][sq_y[i]] = colors[now_c_idx[num]];
        }
    }
}
void player::execClear()
{
    //�����ʱ��ͼ�е�����
    if (num)
    {
        memset(mp_tmp_two, 0, sizeof(mp_tmp_two));
        memset(mp_c_tmp_two, 0, sizeof(mp_c_tmp_two));
        int cnt_j = 14;
        for (int j = 14; j >= 0; j--)
        {
            int cnt_i = 0;
            for (int i = 0; i < 10; i++)
                if (mp[1][i][j])
                    cnt_i++;
            if (cnt_i != 10)
            {
                for (int i = 0; i < 10; i++)
                {
                    mp_tmp_two[i][cnt_j] = mp[1][i][j];
                    mp_c_tmp_two[i][cnt_j] = mp_c[1][i][j];
                }
                cnt_j--;
            }
            else
            {
                score[1]++;
            }
        }
    }
    else
    {
        memset(mp_tmp_one, 0, sizeof(mp_tmp_one));
        memset(mp_c_tmp_one, 0, sizeof(mp_c_tmp_one));
        int cnt_j = 14;
        for (int j = 14; j >= 0; j--)
        {
            int cnt_i = 0;
            for (int i = 0; i < 10; i++)
                if (mp[0][i][j])
                    cnt_i++;
            if (cnt_i != 10)
            {
                for (int i = 0; i < 10; i++)
                {
                    mp_tmp_one[i][cnt_j] = mp[0][i][j];
                    mp_c_tmp_one[i][cnt_j] = mp_c[0][i][j];
                }
                cnt_j--;
            }
            else
            {
                score[0]++;
            }
        }
    }
}
bool player::checkOver()
{
    if (num)
    {
        for (int i = 0; i < 10; i++)
            if (mp[1][i][0])
                return true;
        return false;
    }
    else
    {
        for (int i = 0; i < 10; i++)
            if (mp[0][i][0])
                return true;
        return false;
    }
}
void player::initDatasPerSquare()
{
    now_mp_x[num] = 5;
    now_mp_y[num] = -1;
    flag_next[num] = false;
    now_c_idx[num] = next_c_idx[num];
    now_s_idx[num] = next_s_idx[num];
    now_d_idx[num] = next_d_idx[num];
    next_c_idx[num] = rand() % 7;
    next_s_idx[num] = rand() % 7;
    next_d_idx[num] = rand() % 4;
}
void player::initDatasPerRound()
{
    memset(mp, 0, sizeof(mp));
    memset(mp, 0, sizeof(mp_c));
    time_now[num] = GetTickCount();
    next_c_idx[num] = rand() % 7;
    next_s_idx[num] = rand() % 7;
    next_d_idx[num] = rand() % 4;
    flag_over[num] = false;
    score[num] = 0;
}
bool player::checkPut(int mp_x, int mp_y, int dir_idx)
{
    int sq_x[4];
    int sq_y[4];
    for (int i = 0; i < 4; ++i)
    {
        sq_x[i] = mp_x + squares[now_s_idx[num]].dir[dir_idx][i][0];
        sq_y[i] = mp_y + squares[now_s_idx[num]].dir[dir_idx][i][1];
    }

    // ������Խ�硢�·�Խ�硢�ظ�ռ��
    for (int i = 0; i < 4; ++i)
    {
        if (sq_x[i] < 0 || sq_x[i] > 9 || sq_y[i] > 14)
            return false;
        if (sq_y[i] < 0) // �������Ϸ���
            continue;
        if (mp[sq_x[i]][sq_y[i]])
            return false;
    }
    return true;
}
int main()
{
    paint starpaint;
    player playerOne(0), playerTwo(1);
    //��ʼ��Ϸ
    //���ƴ���
    starpaint.initEnviroment();
    while (true)
    {
        playerOne.initDatasPerRound();
        playerOne.initDatasPerRound();
        while ((!flag_over[0]) || (!flag_over[1]))
        {
            if (flag_next[0])
                playerOne.initDatasPerSquare();
            if (flag_next[1])
                playerTwo.initDatasPerSquare();
            while ((!flag_next[0]) && (!flag_next[1]))
            {
                BeginBatchDraw();
                //����������ڿ�ʼ������ͼ��ִ�к��κλ�ͼ����������ʱ���������Ļ�ϣ�
                //ֱ��ִ�� FlushBatchDraw �� EndBatchDraw �Ž�֮ǰ�Ļ�ͼ�����
                starpaint.drawGameBG();

                starpaint.drawRightSide();
                starpaint.drawLeftSide();

                starpaint.drawRightMap();
                starpaint.drawLeftMap();

                starpaint.drawSquareNow(0);
                starpaint.drawSquareNow(1);
                EndBatchDraw();

                Cmd cmd1 = getOneCmd();
                Cmd cmd2 = getTwoCmd();
                playerOne.execCmd(cmd1);
                playerTwo.execCmd(cmd2);
            }
        }

        //һ�ֽ���
        swprintf(over_tips, 39, L"��Ϸ����\n���1�����յ÷֣�%d\n���2�����յ÷֣�%d\n�Ƿ������ھ֣�", score[0] * 100, score[1] * 100);
        if (MessageBox(GetHWnd(), over_tips, L"�����ھ֣�", MB_ICONQUESTION | MB_YESNO) == IDNO)
            break;
    }
    return 0;
} 