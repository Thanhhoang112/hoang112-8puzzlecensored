#include <iostream>
#include<time.h>
#include<stdio.h>
#include<stdio.h>
#include<malloc.h>
#include<string.h>
int kiemtra(int a[3][3], int flag[9]);
void zeroinmidle(int i, int j, int b[3][3], int flag[9], int k, int index[9], int& countblind);
void inmang(int a[3][3], int  flag[8]);
void zeroinedge(int i, int j, int b[3][3], int flag[9], int k, int index[9], int& countblind);
using namespace std;
enum class Actions { left, right, up, down };
#define SIDE 3 // Length of the board 
typedef struct Node_array
{
    int state[SIDE][SIDE];
    Actions action;
    struct Node_array* parent; // tree search
    int path_cost;
    int depth;
    struct Node_array* nextNode; // list
} Node;

struct NodeList {
    unsigned int nodeCount;    //the number of nodes in the list
    Node* head;            //pointer to the first node in the list
    Node* tail;            //pointer to the last node in the list
};

Node* Goal;
// cấu trúc dữ liệu 

void set_zero(int a[][SIDE])
{
    int i, j;
    for (i = 0; i < SIDE; i++)
        for (j = 0; j < SIDE; j++)
            a[i][j] = 0;
}

void copy_array(int a[][SIDE], int b[][SIDE])//copy b to a
{
    int i, j;
    for (i = 0; i < SIDE; i++)
        for (j = 0; j < SIDE; j++)
            a[i][j] = b[i][j];
}

int is_equal(int a[][SIDE], int b[][SIDE])
{
    int i, j, flag = 1;
    for (i = 0; i < SIDE; i++)
        for (j = 0; j < SIDE; j++)
            if (a[i][j] != b[i][j])
                flag = 0;
    return flag;
}

void swap(int& a, int& b)
{
    int temp;
    temp = a;
    a = b;
    b = temp;
}

void print_array(int a[][SIDE])
{
    int i, j;
    for (i = 0; i < SIDE; i++)
    {
        for (j = 0; j < SIDE; j++)
            printf("%d  | ", a[i][j]);
        printf("\n");
        for (j = 0; j < SIDE; j++)
            printf("---|-");

        printf("\n");
    }
}

bool isLegalAction(Node* node, Actions action, int& index_i, int& index_j)
{
    for (int i = 0; i < SIDE; i++) {
        for (int j = 0; j < SIDE; j++) {
            if (node->state[i][j] == 0) {
                index_i = i;
                index_j = j;
                i = SIDE;
                break;
            }
        }
    }
    if (action == Actions::left && index_j == 0)  return false;
    if (action == Actions::right && index_j == 2)  return false;

    switch (index_i)
    {
    case 0:
        if (action == Actions::up) return false;
        return true;
    case 2:
        if (action == Actions::down) return false;
        return true;
    default:
        return true;
    }
    return true;
}

Node* Child_node(Node* node, Actions action)
{

    Node* child = new Node();
    int index_i, index_j;
    if (isLegalAction(node, action, index_i, index_j) == true)
    {
        copy_array(child->state, node->state);
        child->action = action;
        child->nextNode = NULL;
        child->parent = node;
        child->depth = node->depth + 1;

        if (action == Actions::down)
        {
            swap(child->state[index_i][index_j], child->state[index_i + 1][index_j]);
        }

        else if (action == Actions::up)
        {
            swap(child->state[index_i][index_j], child->state[index_i - 1][index_j]);
        }

        else if (action == Actions::left)
        {
            swap(child->state[index_i][index_j], child->state[index_i][index_j - 1]);
        }

        else if (action == Actions::right)
        {
            swap(child->state[index_i][index_j], child->state[index_i][index_j + 1]);
        }
        child->path_cost = node->depth + 1;
    }
    else
        return NULL;
    return child;

}

Node* createNode(Node* Curr)
{
    Node* child = new Node();
    copy_array(child->state, Curr->state);
    child->action = Curr->action;
    child->depth = Curr->depth;
    child->nextNode = Curr->nextNode;
    child->parent = Curr->parent;
    child->path_cost = Curr->path_cost;
    return child;
}

NodeList* FIFO_initial()
{
    NodeList* L = new NodeList();
    L->head = L->tail = NULL;
    L->nodeCount = 0;
    return L;
}

void FIFO_add(NodeList* list, Node* node)
{
    if (list->head == NULL)
    {
        list->head = list->tail = node;
        list->nodeCount++;
    }
    else
    {
        list->tail->nextNode = node;
        list->tail = node;
        list->tail->nextNode = NULL;
        list->nodeCount++;
    }
}

Node* FIFO_pop(NodeList* list)
{

    Node* pop = NULL;

    if (list->head == NULL)
        return NULL;
    else
    {
        if (list->head->nextNode == NULL)
        {
            pop = list->head;
            list->head = list->head->nextNode;
            list->nodeCount = 0;
            list->head = list->tail = NULL;
        }
        else
        {
            pop = list->head;
            list->head = list->head->nextNode;
            list->nodeCount--;
        }
    }
    return pop;
}

Node* LIFO_pop(NodeList* list)
{

    Node* pop = NULL;

    if (list->head == NULL)
        return NULL;
    else
    {
        if (list->head->nextNode == NULL)
        {
            pop = list->head;
            list->head = list->head->nextNode;
            list->nodeCount = 0;
            list->head = list->tail = NULL;
        }
        else
        {
            pop = list->tail;
            Node* p = list->head;
            while (p->nextNode != list->tail)
            {
                p = p->nextNode;
            }
            list->tail = p;
            list->tail->nextNode = NULL;
            list->nodeCount--;
        }
    }
    return pop;
}

bool Goal_test(Node* node, Node* goal)
{
    return is_equal(node->state, goal->state);
}

bool checkExist(NodeList* list, Node* node)
{
    if (list->head != NULL)
    {
        Node* child = list->head;
        while (child != NULL)
        {
            if (is_equal(child->state, node->state))
            {
                return true;
            }

            child = child->nextNode;
        }
    }
    return false;
}


void Solution(Node* node, int& q)
{
    while (node->parent != NULL)
    {
        print_array(node->state);
        if (node->action == Actions::down)
            printf("\nup:     %d", q++);
        else if (node->action == Actions::up)
            printf("\ndown:   %d", q++);
        else if (node->action == Actions::left)
            printf("\nright:  %d", q++);
        else if (node->action == Actions::right)
            printf("\nleft:   %d", q++);
        cout << endl;
        node = node->parent;
    }
}


Node* FindMinValue(NodeList* list)
{
    Node* min = list->head;
    Node* p = list->head->nextNode;
    while (p != NULL)
    {
        if (p->path_cost < min->path_cost)
            min = p;
        p = p->nextNode;
    }
    if (min == list->head)
        return FIFO_pop(list);
    else if (min == list->tail)
    {
        return LIFO_pop(list);
    }
    else
    {
        Node* prevNode = new Node();
        prevNode = list->head;
        p = list->head->nextNode;
        while (p != min)
        {
            prevNode = p;
            p = p->nextNode;
        }
        prevNode->nextNode = p->nextNode;
        p->nextNode = NULL;
        return p;
    }
}

//A*
unsigned int CountC(Node* Curr, Node* Goal)
{
    unsigned int Count = 0;
    for (int i = 0; i < SIDE; i++)
        for (int j = 0; j < SIDE; j++)
        {
            if (Curr->state[i][j] != Goal->state[i][j])
                Count++;
        }
    return Count;
}


Node* Child_nodeA(Node* node, Actions action)
{

    Node* child = new Node();
    int index_i, index_j;
    if (isLegalAction(node, action, index_i, index_j) == true)
    {
        copy_array(child->state, node->state);
        child->action = action;
        child->nextNode = NULL;
        child->parent = node;
        child->depth = node->depth + 1;

        if (action == Actions::down)
        {
            swap(child->state[index_i][index_j], child->state[index_i + 1][index_j]);
        }

        else if (action == Actions::up)
        {
            swap(child->state[index_i][index_j], child->state[index_i - 1][index_j]);
        }

        else if (action == Actions::left)
        {
            swap(child->state[index_i][index_j], child->state[index_i][index_j - 1]);
        }

        else if (action == Actions::right)
        {
            swap(child->state[index_i][index_j], child->state[index_i][index_j + 1]);
        }
        child->path_cost = CountC(child, Goal); // duong di den o dung + so o khac
    }
    else
        return NULL;
    return child;

}

void MoveaStar(Node* Curr)
{
    NodeList* Now = FIFO_initial();
    Node* move = Child_nodeA(Curr, Actions::down);
    if (move != NULL)
    {
        FIFO_add(Now, move);
    }
    move = Child_nodeA(Curr, Actions::up);
    if (move != NULL)
        FIFO_add(Now, move);

    move = Child_nodeA(Curr, Actions::left);
    if (move != NULL)
        FIFO_add(Now, move);

    move = Child_nodeA(Curr, Actions::right);
    if (move != NULL)
        FIFO_add(Now, move);
    Curr->nextNode = Now->head;
}


int aStarSearch(Node* root, Node* goal, int countblind, int& countsolve)
{
    int q = countblind;
    NodeList* Ftier = FIFO_initial();
    NodeList* Explorer = FIFO_initial();
    FIFO_add(Ftier, root);
    bool isGoal = false;
    int s = 0;
    while (Ftier->nodeCount > 0 && !isGoal)
    {

        Node* Curr = FindMinValue(Ftier); // tim chi phi nho nhat
        FIFO_add(Explorer, createNode(Curr)); // khoi tao buoc di tiep theo
        MoveaStar(Curr);
        Node* CurrChild = Curr->nextNode;
        while (CurrChild != NULL)
        {

            if (Goal_test(CurrChild, goal))
            {
                isGoal = true;
                cout << "loi giai :" << endl;
                Solution(CurrChild, q);
            }
            bool x = checkExist(Ftier, CurrChild);
            bool y = checkExist(Explorer, CurrChild);
            if (!x && !y)
            {
                Node* Temp = createNode(CurrChild);
                Temp->nextNode = NULL;
                FIFO_add(Ftier, Temp);
            }
            CurrChild = CurrChild->nextNode;

            if (s == 6000)   /// dung de kiem tra co giai duoc hay khong ?
            {
                printf("khong giai duoc\n");
                return 0;
            }
            s++;
        }
    }
    countsolve = q - countblind;
    return 1;
}

//findblindpuzzle
int findpuzzle(Node* root, int a[3][3], int& countblind, int countsolve)
{
    int k, hang, cot, hang0, cot0, s = 1;
    int index[9], flag[9];
    printf("\n chon o trong nhap tu 1-9\n");
    std::cin >> k;
    hang = (k - 1) / 3;
    cot = (k - 1) % 3;
    printf("\n puzzle bi che \n");

    int i, j;
    for (i = 0; i < SIDE; i++)
    {
        for (j = 0; j < SIDE; j++)
            if (i == hang && j == cot)
            {
                printf("%d  | ", a[i][j]);
            }
            else if (a[i][j] != 0)
            {
                printf("X  | ");
            }
            else
            {
                hang0 = i;
                cot0 = j;
                printf("%d  | ", a[i][j]);
            }
        printf("\n");
        for (j = 0; j < SIDE; j++)
            printf("---|-");

        printf("\n");
    }
    for (i = 0; i < SIDE; i++)                      /// thiet lap vi tri va trang thai
    {
        for (j = 0; j < SIDE; j++)
        {

            index[a[i][j]] = s;
            if (a[i][j] == 0 || s == k)
            {
                flag[a[i][j]] = 1;
            }
            else
            {
                flag[a[i][j]] = 0;
            }
            s++;
        }
    }


    printf("\n tim so 0 : \n");
    //tim so 0 tai vi tri da cho thay
    while (index[0] != k)
    {
        while (hang > hang0)
        {
            swap(a[hang0 + 1][cot0], a[hang0][cot0]);
            swap(index[a[hang0 + 1][cot0]], index[a[hang0][cot0]]);
            printf("\ndown:   %d", countblind++);
            inmang(a, flag);
            hang0++;
        }
        while (hang < hang0)
        {
            swap(a[hang0 - 1][cot0], a[hang0][cot0]);
            swap(index[a[hang0 - 1][cot0]], index[a[hang0][cot0]]);
            printf("\nup:     %d", countblind++);
            inmang(a, flag);
            hang0--;
        }
        while (cot > cot0)
        {
            swap(a[hang0][cot0 + 1], a[hang0][cot0]);
            swap(index[a[hang0][cot0 + 1]], index[a[hang0][cot0]]);
            printf("\nright:  %d", countblind++);
            inmang(a, flag);
            cot0++;
        }
        while (cot < cot0)
        {
            swap(a[hang0][cot0 - 1], a[hang0][cot0]);
            swap(index[a[hang0][cot0 - 1]], index[a[hang0][cot0]]);
            printf("\nleft:   %d", countblind++);
            inmang(a, flag);
            cot0--;
        }
    }
    printf("\ntim cac so con lai:\n\n");
    if (k != 5)
        zeroinedge(hang, cot, a, flag, k, index, countblind); /// so 0 nam ngoai bien
    else
        zeroinmidle(hang, cot, a, flag, k, index, countblind); /// so 0 o trungtam
    printf("\n");

    return k;

}

void zeroinmidle(int i, int j, int b[3][3], int flag[9], int k, int index[9], int& countblind)
{
    int hang, cot;
    hang = (k - 1) / 3;
    cot = (k - 1) % 3;

    swap(b[i + 1][j], b[i][j]);//xuong
    if (flag[b[hang][cot]] == 0)
    {
        flag[b[i][j]] = 1;
        printf("\ndown:   %d", countblind++);
        inmang(b, flag);

    }
    swap(b[i + 1][j], b[i][j]);
    printf("\nup:     %d", countblind++);
    inmang(b, flag);


    swap(b[i - 1][j], b[i][j]);//len
    if (flag[b[hang][cot]] == 0)
    {
        flag[b[i][j]] = 1;
        printf("\nup:     %d", countblind++);
        inmang(b, flag);
    }
    swap(b[i - 1][j], b[i][j]);
    printf("\ndown     %d", countblind++);
    inmang(b, flag);


    swap(b[i][j - 1], b[i][j]);//qua trai
    if (flag[b[hang][cot]] == 0)
    {
        flag[b[i][j]] = 1;
        printf("\nleft:   %d", countblind++);
        inmang(b, flag);
    }
    swap(b[i][j - 1], b[i][j]);
    printf("\nright   %d", countblind++);
    inmang(b, flag);
    swap(b[i][j + 1], b[i][j]);//qua phai
    if (flag[b[hang][cot]] == 0)
    {
        flag[b[i][j]] = 1;
        printf("\nright:  %d", countblind++);
        inmang(b, flag);
    }
    swap(b[i][j + 1], b[i][j]);
    printf("\nleft:  %d", countblind++);
    inmang(b, flag);


    ///////////////////////////////////////////////

    swap(b[i + 1][j], b[i][j]); i++;//xuong
    printf("\ndown:   %d", countblind++);
    inmang(b, flag);

    swap(b[i][j + 1], b[i][j]); j++;//qua phai
    printf("\nright:  %d", countblind++);
    inmang(b, flag);

    swap(b[i - 1][j], b[i][j]); i--;//len
    printf("\nup:     %d", countblind++);
    inmang(b, flag);

    swap(b[i][j - 1], b[i][j]); j--;//qua trai
    printf("\nleft:   %d", countblind++);
    inmang(b, flag);

    /////////////////////////////////////////////////
    swap(b[i + 1][j], b[i][j]); //xuong va hien X
    if (flag[b[hang][cot]] == 0)
    {
        flag[b[i][j]] = 1;
        printf("\ndown:   %d", countblind++);
        inmang(b, flag);
    }i++;
    swap(b[i][j - 1], b[i][j]); j--;//qua trai
    printf("\nleft:   %d", countblind++);
    inmang(b, flag);

    swap(b[i - 1][j], b[i][j]); i--;//len
    printf("\nup:     %d", countblind++);
    inmang(b, flag);

    swap(b[i][j + 1], b[i][j]); j++;//qua phai
    printf("\nright:  %d", countblind++);
    inmang(b, flag);

    /////////////////////////////////////////////////
    swap(b[i + 1][j], b[i][j]); //xuong va hien X
    if (flag[b[hang][cot]] == 0)
    {
        flag[b[i][j]] = 1;
        printf("\ndown:   %d", countblind++);
        inmang(b, flag);

    }i++;
    //////////////////////////////////////////////////
    swap(b[i - 1][j], b[i][j]); i--;//len
    printf("\nup:     %d", countblind++);
    inmang(b, flag);

    swap(b[i - 1][j], b[i][j]); i--;//len
    printf("\nup:     %d", countblind++);
    inmang(b, flag);

    swap(b[i][j - 1], b[i][j]); j--;//qua trai
    printf("\nleft:   %d", countblind++);
    inmang(b, flag);

    swap(b[i + 1][j], b[i][j]); i++;//xuong
    printf("\ndown:   %d", countblind++);
    inmang(b, flag);

    swap(b[i][j + 1], b[i][j]); j++;//qua phai
    printf("\nright:  %d", countblind++);
    inmang(b, flag);

    swap(b[i - 1][j], b[i][j]); //len va hien thi
    if (flag[b[hang][cot]] == 0)
    {
        flag[b[i][j]] = 1;
        printf("\nup:     %d", countblind++);
        inmang(b, flag);

    }i--;
    ///////////////////////////////////////////////
    swap(b[i][j + 1], b[i][j]); j++;//qua phai
    printf("\nright:  %d", countblind++);
    inmang(b, flag);

    swap(b[i + 1][j], b[i][j]); i++;//xuong
    printf("\ndown:   %d", countblind++);
    inmang(b, flag);

    swap(b[i][j - 1], b[i][j]); j--;//qua trai
    printf("\nleft:   %d", countblind++);
    inmang(b, flag);

    swap(b[i - 1][j], b[i][j]); //len va hien thi
    if (flag[b[hang][cot]] == 0)
    {
        flag[b[i][j]] = 1;
        printf("\nup:     %d", countblind++);
        inmang(b, flag);

    }i--;
}

void zeroinedge(int i, int j, int b[3][3], int flag[9], int k, int index[9], int& countblind)
{
    int hang, cot, s = 7, x, t, p, l;
    hang = (k - 1) / 3;
    cot = (k - 1) % 3;
    while (s != 0)
    {
        x = 1, t = 1, p = 1, l = 1, k = 0;
        while (x != 0 && t != 0 && l != 0 && p != 0)
        {
            if (x == 1)
            {
                while (i <= 1)
                {
                    swap(b[i + 1][j], b[i][j]);
                    swap(index[b[i + 1][j]], index[b[i][j]]);

                    if (flag[b[hang][cot]] == 0)
                        flag[b[i][j]] = 1;
                    printf("\ndown:   %d", countblind++);
                    inmang(b, flag);

                    k = kiemtra(b, flag);
                    if (k == 1)
                        return;
                    i++;
                }
                x--;
            }
            if (p == 1)
            {
                while (j <= 1)
                {
                    swap(b[i][j + 1], b[i][j]);
                    swap(index[b[i][j + 1]], index[b[i][j]]);

                    if (flag[b[hang][cot]] == 0)
                        flag[b[i][j]] = 1;
                    printf("\nright:  %d", countblind++);
                    inmang(b, flag);

                    k = kiemtra(b, flag);
                    if (k == 1)
                        return;
                    j++;
                }
                p--;
            }
            if (l == 1)
            {
                while (i >= 1)
                {
                    swap(b[i - 1][j], b[i][j]);
                    swap(index[b[i - 1][j]], index[b[i][j]]);

                    if (flag[b[hang][cot]] == 0)
                        flag[b[i][j]] = 1;
                    printf("\nup:     %d", countblind++);
                    inmang(b, flag);

                    k = kiemtra(b, flag);
                    if (k == 1)
                        return;
                    i--;
                }
                l--;
            }
            if (t == 1)
            {
                while (j >= 1)
                {
                    swap(b[i][j - 1], b[i][j]);
                    swap(index[b[i][j - 1]], index[b[i][j]]);

                    if (flag[b[hang][cot]] == 0)
                        flag[b[i][j]] = 1;
                    printf("\nleft:   %d", countblind++);
                    inmang(b, flag);

                    k = kiemtra(b, flag);
                    if (k == 1)
                        return;
                    j--;
                }
                t--;
            }
        }
        s--;
    }
}
int kiemtra(int a[3][3], int flag[9])
{
    int d = 0;
    for (int i = 0; i < 9; i++)
        if (flag[i] == 0) d++;

    if (d == 1)
        for (int i = 1; i < 9; i++)
            if (flag[i] == 0)
            {
                printf("\nso con lai la : %d\n", i);
                flag[i] = 1;
                inmang(a, flag);
            }
    return d;
}

int checkarray(int b[3][3])
{
    int k[9], x = 0;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            if (b[i][j] < 9)
            {
                k[x] = b[i][j];
                x++;
            }
            else return 0;
        }
    int t = 0;
    for (int i = 0; i < 9; i++)
    {
        t = 0;
        for (int j = 0; j < 9; j++)
        {
            if (k[i] == k[j])
                t++;
        }
        if (t > 1)
            return 0;
    }
    return 1;
}
void inmang(int a[3][3], int  flag[8])
{
    int i, j;
    printf("\n");
    for (i = 0; i < SIDE; i++)
    {
        for (j = 0; j < SIDE; j++)
            if (flag[a[i][j]] == 1)
            {
                printf("%d  | ", a[i][j]);
            }
            else
            {
                printf("X  | ");
            }
        printf("\n");
        for (j = 0; j < SIDE; j++)
            printf("---|-");

        printf("\n");
    }
}
int main()
{
    Node* Start;
    int i, j, a[3][3], countblind = 1, countsolve = 0, checkinput = 0, checkoutput = 0;
    int b[3][3] = { 0 };
    Goal = (Node*)malloc(sizeof(Node));
    Start = (Node*)malloc(sizeof(Node));
    printf("nhap trang thai dich\n");

    while (checkinput == 0)
    {
        for (i = 0; i < 3; i++)
            for (j = 0; j < 3; j++)
            {
                std::cin >> Start->state[i][j];
                b[i][j] = Start->state[i][j];
            }
        checkinput = checkarray(b);
        if (checkinput == 0)
            printf("Trang thai khong hop le moi nhap lai :\n");
    }
    printf("Goal state:\n");
    print_array(Start->state);
    Goal->parent = NULL;
    Goal->depth = 0;
    printf("nhap trang thai hien tai\n");
    while (checkoutput == 0)
    {
        for (i = 0; i < 3; i++)
            for (j = 0; j < 3; j++)
            {
                std::cin >> Goal->state[i][j];
                a[i][j] = Goal->state[i][j];
            }
        checkoutput = checkarray(a);
        if (checkoutput == 0)
            printf("Trang thai khong hop le moi nhap lai :\n");
    }
    printf("Start state:\n");
    print_array(Goal->state);


    clock_t begin = clock();

    int k = findpuzzle(Goal, a, countblind, countsolve);

    for (i = 0; i < 3; i++)                      // gan lai trang thai sau khi tim ra duoc cac so trong puzzle
        for (j = 0; j < 3; j++)
        {
            Goal->state[i][j] = a[i][j];
        }

    Start->parent = NULL;
    Start->path_cost = CountC(Start, Goal);// so o khac nhau trong mang 2 chieu
    Start->nextNode = NULL;

    int m = aStarSearch(Start, Goal, countblind, countsolve);
    if (m == 0)
    {
        printf("\nvi tri o nhin thay luc dau :          %d\n", k);
        printf("\nso buoc de giai puzzle che :          %d\n", countblind - 1);
        printf("khong giai duoc puzzle sau khi tim ra puzzle che\n");
    }
    else
    {
        print_array(Start->state);
        printf("\nvi tri o nhin thay luc dau :          %d\n", k);
        printf_s("\nso buoc de giai puzzle che :          %d\n", countblind - 1); // phai -1 vi o buoc giai puzzle che o cuoi buoc co them 1
        printf("so buoc de giai puzzle sau khi che:   %d\n", countsolve);
        printf("tong so buoc :                        %d\n\n", countsolve + countblind - 1);
    }

    clock_t end = clock();
    printf("\n thoi gian chay \n");
    cout << "Time run: " << (float)(end - begin) / CLOCKS_PER_SEC << " s" << endl;
    return 0;



}




