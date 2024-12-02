#include <stdio.h>
#include <stdlib.h> 
#include <memory.h>
#include <stdbool.h>

typedef struct node 
{
    int rk;  //rank
    int t_num;  //toast number
    // int x; //colomn
    // int y; //row
    struct node *up;
    struct node *down;
    struct node *right;
    struct node *left;
} node;

typedef struct coordinate
{
    int x;
    int y;
    node *adrs;
} coordinate;

node *alloc(int rk, int t_num) 
{
    node *tmp = (node *) malloc(sizeof(node));
    tmp ->rk = rk;
    tmp ->t_num = t_num;
    tmp ->up = NULL;
    tmp ->down = NULL;
    tmp ->right = NULL;
    tmp ->left = NULL;
    return tmp;
}

void swap (node **a, node **b)
{
    node *copy = *a;
    *a = *b;
    *b = copy;
}

node *insert_right(node **current, int rk, int t_num, node *up)
{
    node *new = alloc(rk, t_num);
    // new ->right = NULL;
    (*current) ->right = new;
    new ->left = (*current);
    // new ->down = NULL;
    new ->up = up;
    return new;
}

node *insert_right_R(node **current, int rk, int t_num, node *up)  //反向用(第三張圖)
{
    node *new = alloc(rk, t_num);
    // new ->right = NULL;
    (*current) ->left = new;
    new ->right = (*current);
    // new ->down = NULL;
    new ->down = up;
    return new;
}

void insert_down(node **current, int rk, int t_num)
{
    node *new = alloc(rk, t_num);
    // new ->down = NULL;
    (*current) ->down = new;
    new ->up = (*current);
    // new ->left = NULL;
    // new ->right = NULL;
}

void insert_down_R(node **current, int rk, int t_num)  //反向用(第三張圖)
{
    node *new = alloc(rk, t_num);
    // new ->down = NULL;
    (*current) ->up = new;
    new ->down = (*current);
    // new ->left = NULL;
    // new ->right = NULL;
}

// node *search_rank(coordinate *rk_ary, node ***cor_ary, int rank)
// {
//     int row = rk_ary[rank].y;
//     int col = rk_ary[rank].x;
//     return cor_ary[row][col];
// }

void train(coordinate *rk_ary, int day, int row, int col, int *c_total, int *c_tot)  //node ***cor_ary
{
    node *start;
    node *up_p, *down_p, *left_p, *right_p;
    node **dir = (node**)malloc(sizeof(node*) * 4);;
    node *founded;
    int r, l, s;  // r-rank, l-riding length, s-車隊人數
    int x, y, dis;
    int dis_min, d;
    int c_day;  //當天倒的店數
    int v_day;  //當天實際拜訪的店數

    for(int i = 0; i < day; i++)
    {    
        scanf("%d %d %d", &r, &l, &s);      
        node **visited = (node**)malloc(sizeof(node*) * (l+5));
        int *closed = (int*)malloc(sizeof(int) * l);
        start = rk_ary[r].adrs;
        founded = start;
        c_day = 0;
        v_day = 0;
        for(int j = 0; j < l; j++)  //找店吃麵包 
        { 
            up_p = start;
            down_p = start;
            left_p = start;
            right_p = start;
            if((start ->up) != NULL && (start ->up) ->t_num != 0)
                up_p = start ->up;
            if((start ->down) != NULL && (start ->down) ->t_num != 0)
                down_p = start ->down;
            if((start ->left) != NULL && (start ->left) ->t_num != 0)
                left_p = start ->left;
            if((start ->right) != NULL && (start ->right) ->t_num != 0)
                right_p = start ->right;

            dir[0] = up_p;
            dir[1] = right_p; 
            dir[2] = down_p;
            dir[3] = left_p;
            dis_min = 1001;
            d = 0;
            for(int k = 0; k < 4; k++)  //找四個方向最近的距離
            {
                x = abs(rk_ary[dir[k] ->rk].x - rk_ary[start ->rk].x);
                y = abs(rk_ary[dir[k] ->rk].y - rk_ary[start ->rk].y);
                if(x >= y)
                    dis = x;
                else
                    dis = y;          
                if((dis > 0) && (dis < dis_min))
                {
                    d = 0;
                    dis_min = dis;
                    dir[d] = dir[k];
                    d++;
                }
                else if((dis > 0) && (dis == dis_min))
                {
                    dir[d] = dir[k];
                    d++;
                }
            }
            if(d == 0)  //四周都是NULL
                break;

            founded = dir[0];  //比rank小
            for(int k = 0; k < d; k++)
            {   
                if((dir[k] ->rk) < (founded ->rk))
                    founded = dir[k];                
            }
       
            // if(founded == start)  //四周都是NULL
            //     break;

            visited[j] = start;
            v_day++;

            (founded ->t_num) -= s;
            if((founded ->t_num) <= 0)
            {
                founded ->t_num = 0;
                closed[c_day] = founded ->rk;
                c_day++;
            }
            //---------拔點(w/o最後一間)----------
            if((start ->left) != NULL)
                (start ->left) ->right = start ->right;
            if((start ->right) != NULL)
                (start ->right) ->left = start ->left;
            if((start ->up) != NULL)
                (start ->up) ->down = start ->down;
            if((start ->down) != NULL)
                (start ->down) ->up = start ->up;

            start = founded;          
        }

        //---------拔點(最後一間)----------
        visited[v_day] = start;
        v_day++;
        if((start ->left) != NULL)
            (start ->left) ->right = start ->right;
        if((start ->right) != NULL)
            (start ->right) ->left = start ->left;
        if((start ->up) != NULL)
            (start ->up) ->down = start ->down;
        if((start ->down) != NULL)
            (start ->down) ->up = start ->up;

        //-----------接拔掉的點(visited)---------
        for(int j = 0; j < v_day; j++)
        {
            if((visited[j] ->left) != NULL)
                (visited[j] ->left) ->right = visited[j];
            if((visited[j] ->right) != NULL)
                (visited[j] ->right) ->left = visited[j];
            if((visited[j] ->up) != NULL)
                (visited[j] ->up) ->down = visited[j];
            if((visited[j] ->down) != NULL)
                (visited[j] ->down) ->up = visited[j];
        } 

        //-----------把當天倒掉的店拔掉&加到已倒陣列(c_total)
        node *c_adrs;
        for(int j = 0; j < c_day; j++)
        {
            c_total[*c_tot + j] = closed[j];
            c_adrs = rk_ary[closed[j]].adrs;

            if((c_adrs ->left) != NULL)
                (c_adrs ->left) ->right = c_adrs ->right;
            if((c_adrs ->right) != NULL)
                (c_adrs ->right) ->left = c_adrs ->left;
            if((c_adrs ->up) != NULL)
                (c_adrs ->up) ->down = c_adrs ->down;
            if((c_adrs ->down) != NULL)
                (c_adrs ->down) ->up = c_adrs ->up;
        }
        
        *c_tot += c_day;

        free(visited);        
        free(closed);   
    }   

    // //------------把所有倒掉的店(c_total)接回去
    // for(int i = 0; i < c_tot; i++)
    // {
    //     if((c_total[i] ->left) != NULL)
    //         (c_total[i] ->left) ->right = c_total[i];
    //     if((c_total[i]->right) != NULL)
    //         (c_total[i] ->right) ->left = c_total[i];
    //     if((c_total[i] ->up) != NULL)
    //         (c_total[i] ->up) ->down = c_total[i];
    //     if((c_total[i] ->down) != NULL)
    //         (c_total[i] ->down) ->up = c_total[i];
    // }

    // free(c_total);
    free(dir);
}

node *race(node *hd_2, coordinate *rk_ary_2, coordinate *rk_ary_3, int day)  //2是順向 3是反向
{
    int center, len;
    node *c_2;
    node *c_3;
    node *tmp_2;
    node *tmp_3;
    int jg = hd_2 ->rk;

    for(int i = 0; i < day; i++)
    {
        scanf("%d %d", &center, &len);
        c_2 = rk_ary_2[center].adrs;
        c_3 = rk_ary_3[center].adrs;
        tmp_2 = c_2;
        tmp_3 = c_3;

        for(int j = 0; j < (len - 1)/2; j++)
        {
            tmp_2 = (tmp_2 ->left) ->up;
            tmp_3 = (tmp_3 ->left) ->up;
        }

        if(tmp_2 ->rk == jg)
            jg = tmp_3 ->rk;
        else if(tmp_3 ->rk == jg)
            jg = tmp_2 ->rk;

        //-------左上->左下-----------
        for(int j = 0; j < (len - 1); j++)
        {
            swap(&(tmp_2 ->left), &(tmp_3 ->left));
            if((tmp_2 ->left) != NULL)
                (tmp_2 ->left) ->right = tmp_2;
            if((tmp_3 ->left) != NULL)
                (tmp_3 ->left) ->right = tmp_3;
    
            tmp_2 = tmp_2 ->down;
            tmp_3 = tmp_3 ->down;
        }
        swap(&(tmp_2 ->left), &(tmp_3 ->left));
        if((tmp_2 ->left) != NULL)
            (tmp_2 ->left) ->right = tmp_2;
        if((tmp_3 ->left) != NULL)
            (tmp_3 ->left) ->right = tmp_3;        
        //-------左下->右下-----------
        for(int j = 0; j < (len - 1); j++)
        {
            swap(&(tmp_2 ->down), &(tmp_3 ->down));  
            if((tmp_2 ->down) != NULL)   
                (tmp_2 ->down) ->up = tmp_2;    
            if((tmp_3 ->down) != NULL)
                (tmp_3 ->down) ->up = tmp_3;    
    
            tmp_2 = tmp_2 ->right;
            tmp_3 = tmp_3 ->right;  
        }       
        swap(&(tmp_2 ->down), &(tmp_3 ->down));    
        if((tmp_2 ->down) != NULL)
            (tmp_2 ->down) ->up = tmp_2;
        if((tmp_3 ->down) != NULL)
            (tmp_3 ->down) ->up = tmp_3;    
        //-------右下->右上-----------
        for(int j = 0; j < (len - 1); j++)
        {
            swap(&(tmp_2 ->right), &(tmp_3 ->right));
            if((tmp_2 ->right) != NULL)
                (tmp_2 ->right) ->left = tmp_2;
            if((tmp_3 ->right) != NULL)
                (tmp_3 ->right) ->left = tmp_3;
    
            tmp_2 = tmp_2 ->up;
            tmp_3 = tmp_3 ->up;
        }
        swap(&(tmp_2 ->right), &(tmp_3 ->right));
        if((tmp_2 ->right) != NULL)
            (tmp_2 ->right) ->left = tmp_2;
        if((tmp_3 ->right) != NULL)
            (tmp_3 ->right) ->left = tmp_3;
        //-------右上->左上-----------
        for(int j = 0; j < (len - 1); j++)
        {
            swap(&(tmp_2 ->up), &(tmp_3 ->up));
            if((tmp_2 ->up) != NULL)
                (tmp_2 ->up) ->down = tmp_2;
            if((tmp_3 ->up) != NULL)
                (tmp_3 ->up) ->down = tmp_3;
    
            tmp_2 = tmp_2 ->left;
            tmp_3 = tmp_3 ->left;
        }
        swap(&(tmp_2 ->up), &(tmp_3 ->up));
        if((tmp_2 ->up) != NULL)
            (tmp_2 ->up) ->down = tmp_2;
        if((tmp_3 ->up) != NULL)
            (tmp_3 ->up) ->down = tmp_3;
    }

    tmp_2 = rk_ary_2[1].adrs;
    tmp_3 = rk_ary_3[1].adrs;

    while(tmp_2 ->left != NULL)
    {
        tmp_2 = tmp_2 ->left;    
    }
    while(tmp_2 ->up != NULL)
    {
        tmp_2 = tmp_2 ->up;    
    }

    while(tmp_3 ->left != NULL)
    {
        tmp_3 = tmp_3 ->left;    
    }
    while(tmp_3 ->up != NULL)
    {
        tmp_3 = tmp_3 ->up;    
    }

    if((tmp_2 ->rk) == jg)
        return tmp_2;
    else if((tmp_3 ->rk) == jg)
        return tmp_3;

}

void main()
{
    int row, col, rank, t;
    scanf("%d %d", &row, &col);

    coordinate *rnk_1 = (coordinate*)malloc(sizeof(coordinate) * (row*col + 1));  //rank由小排到大的一維陣列(原圖)
    coordinate *rnk_2 = (coordinate*)malloc(sizeof(coordinate) * (row*col + 1));  //rank由小排到大的一維陣列(第二張圖 順向)
    coordinate *rnk_3 = (coordinate*)malloc(sizeof(coordinate) * (row*col + 1));  //rank由小排到大的一維陣列(第三張圖 反向)
    // node ***cor = (node***)malloc(sizeof(node**) * row);  //link list座標對應的二維陣列
    // for(int i = 0; i < row; i++)
    // {
    //     cor[i] = (node**)malloc(sizeof(node*) * col);
    // }
    int *c_total = (int*)malloc(sizeof(int) * (row*col+5)); //記錄倒掉的店
    int c_tot = 0;
    
    scanf("%d", &rank);
    node *head_1 = alloc(rank, 0);
    node *head_2 = alloc(rank, 0);
    node *head_3 = alloc(rank, 0);
    node *tmp_col_1 = head_1;
    node *tmp_row_1 = head_1;
    node *upper_p_1 = head_1;
    node *current_1;
    node *tmp_col_2 = head_2;
    node *tmp_row_2 = head_2;
    node *upper_p_2 = head_2;
    node *current_2;
    node *tmp_col_3 = head_3;
    node *tmp_row_3 = head_3;
    node *upper_p_3 = head_3;
    node *current_3;
    //------------------存rank-----------------
    //alloc第一row
    // scanf(" %d", &rank);
    // insert_down(&tmp_row, rank, 0);
    // tmp_row = tmp_row ->down;
    // head = NULL;
    // cor[0][0] = tmp_row;
    rnk_1[rank].adrs = tmp_col_1;
    rnk_2[rank].adrs = tmp_col_2;
    rnk_3[rank].adrs = tmp_col_3;

    rnk_1[rank].x = 0;
    rnk_1[rank].y = 0;
    // tmp_col = tmp_row;
    for(int i = 2 ; i <= col ; i++)
    {
        scanf(" %d", &rank);
        insert_right(&tmp_col_1, rank, 0, NULL);
        tmp_col_1 = tmp_col_1 ->right;
        // cor[0][i-1] = tmp_col;
        rnk_1[rank].adrs = tmp_col_1;

        insert_right(&tmp_col_2, rank, 0, NULL);
        tmp_col_2 = tmp_col_2 ->right;
        rnk_2[rank].adrs = tmp_col_2;

        insert_right_R(&tmp_col_3, rank, 0, NULL);
        tmp_col_3 = tmp_col_3 ->left;
        rnk_3[rank].adrs = tmp_col_3;  

        rnk_1[rank].x = i-1;
        rnk_1[rank].y = 0;      
    }
    
    // alloc第二row到最後一row
    for(int i = 2 ; i <= row ; i++)
    {
        scanf("%d", &rank);
        insert_down(&tmp_row_1, rank, 0);
        tmp_row_1 = (tmp_row_1 ->down);
        // cor[i-1][0] = tmp_row;
        rnk_1[rank].adrs = tmp_row_1;
        tmp_col_1 = tmp_row_1;
        upper_p_1 = (tmp_row_1 ->up) ->right;
        rnk_1[rank].x = 0;
        rnk_1[rank].y = i-1;

        insert_down(&tmp_row_2, rank, 0);
        tmp_row_2 = (tmp_row_2 ->down);
        rnk_2[rank].adrs = tmp_row_2;
        tmp_col_2 = tmp_row_2;
        upper_p_2 = (tmp_row_2 ->up) ->right;

        insert_down_R(&tmp_row_3, rank, 0);
        tmp_row_3 = (tmp_row_3 ->up);
        rnk_3[rank].adrs = tmp_row_3;
        tmp_col_3 = tmp_row_3;
        upper_p_3 = (tmp_row_3 ->down) ->left;
        
        for(int j = 2 ; j <= col ; j++)
        {
            scanf(" %d", &rank);
            current_1 = insert_right(&tmp_col_1, rank, 0, upper_p_1);
            upper_p_1 ->down = current_1;
            tmp_col_1 = tmp_col_1 ->right;
            // cor[i-1][j-1] = tmp_col;
            rnk_1[rank].adrs = tmp_col_1;
            upper_p_1 = upper_p_1 ->right;

            current_2 = insert_right(&tmp_col_2, rank, 0, upper_p_2);
            upper_p_2 ->down = current_2;
            tmp_col_2 = tmp_col_2 ->right;
            rnk_2[rank].adrs = tmp_col_2;
            upper_p_2 = upper_p_2 ->right;
            
            current_3 = insert_right_R(&tmp_col_3, rank, 0, upper_p_3);
            upper_p_3 ->up = current_3;
            tmp_col_3 = tmp_col_3 ->left;
            rnk_3[rank].adrs = tmp_col_3;
            upper_p_3 = upper_p_3 ->left;

            rnk_1[rank].x = j-1;
            rnk_1[rank].y = i-1;
        }    
    }

    rnk_1[0].x = -1;
    rnk_1[0].y = -1;
    rnk_1[0].adrs = NULL;
    // for(int i = 0; i < row; i++)
    // {
    //     for(int j = 0; j < col; j++)
    //     {
    //         rnk[(cor[i][j] ->rk)].x = j;
    //         rnk[(cor[i][j] ->rk)].y = i; 
    //     }
    // }

    //------------------存toast(只存進第一張)-----------------
    tmp_row_1 = head_1;
    tmp_col_1 = tmp_row_1;
    scanf("%d", &t);
    tmp_col_1 ->t_num = t;
    
    // first row
    for(int i = 2 ; i <= col ; i++)
    {
        tmp_col_1 = tmp_col_1 ->right;
        scanf(" %d", &t);
        tmp_col_1 ->t_num = t;
    }

    //second row to the last
    for(int i = 2 ; i <= row ; i++)
    {
        tmp_row_1 = (tmp_row_1 ->down);
        tmp_col_1 = tmp_row_1;
        scanf("%d", &t);
        tmp_col_1 ->t_num = t;
        for(int j = 2 ; j <= col ; j++)
        {
            tmp_col_1 = tmp_col_1 ->right;
            scanf(" %d", &t);
            tmp_col_1 ->t_num = t;
        }    
    }

    //--------------先遍歷一次把麵包是0的店記錄下來&拔掉
    tmp_row_1 = head_1;
    for(int i = 0; i < row; i++)
    {
        tmp_col_1 = tmp_row_1; 
        for(int j = 0; j < col; j++)
        {
            if(tmp_col_1 ->t_num == 0)
            {
                c_total[c_tot] = tmp_col_1 ->rk;
                c_tot++; 
                if((tmp_col_1 ->left) != NULL)
                (tmp_col_1 ->left) ->right = tmp_col_1 ->right;
                if((tmp_col_1 ->right) != NULL)
                    (tmp_col_1 ->right) ->left = tmp_col_1 ->left;
                if((tmp_col_1 ->up) != NULL)
                    (tmp_col_1 ->up) ->down = tmp_col_1 ->down;
                if((tmp_col_1 ->down) != NULL)
                    (tmp_col_1 ->down) ->up = tmp_col_1 ->up;
            }
            tmp_col_1 = tmp_col_1 ->right;
        }
        tmp_row_1 = tmp_row_1 ->down;
    }

    int T1, R, T2;
    scanf("%d %d %d", &T1, &R, &T2);

    train(rnk_1, T1, row, col, c_total, &c_tot);
 
    node *c_adrs;

    if(R != 0)
    {
        //------------把所有倒掉的店(c_total)接回去       
        for(int i = 0; i < c_tot; i++)
        {
            c_adrs = rnk_1[c_total[i]].adrs;

            if((c_adrs ->left) != NULL)
                (c_adrs ->left) ->right = c_adrs;
            if((c_adrs->right) != NULL)
                (c_adrs ->right) ->left = c_adrs;
            if((c_adrs ->up) != NULL)
                (c_adrs ->up) ->down = c_adrs;
            if((c_adrs ->down) != NULL)
                (c_adrs ->down) ->up = c_adrs;
        }

        //-----------遍歷一次把所有店的吐司數依rank記下來
        int *toast = (int*)malloc(sizeof(int) * (row*col + 1));
        toast[0] = -1;
        tmp_row_1 = head_1;
        for(int i = 0; i < row; i++)
        {
            tmp_col_1 = tmp_row_1; 
            for(int j = 0; j < col; j++)
            {
                toast[tmp_col_1 ->rk] = tmp_col_1 ->t_num;
                tmp_col_1 = tmp_col_1 ->right;
            }
            tmp_row_1 = tmp_row_1 ->down;
        }


        tmp_row_2 = race(head_2, rnk_2, rnk_3, R);

        //-----------遍歷一次把所選地圖的rank更新到第一張圖 & 把土司數重新賦值 & 更新rnk_1
        tmp_row_1 = head_1;
        for(int i = 0; i < row; i++)
        {
            tmp_col_1 = tmp_row_1;  
            tmp_col_2 = tmp_row_2;  
            for(int j = 0; j < col; j++)
            {
                tmp_col_1 ->rk = tmp_col_2 ->rk;    
                tmp_col_1 ->t_num = toast[tmp_col_1 ->rk];  
                // if((tmp_col_1 ->t_num) == 0)
                // {
                //     if((tmp_col_1 ->left) != NULL)
                //         (tmp_col_1 ->left) ->right = tmp_col_1 ->right;
                //     if((tmp_col_1 ->right) != NULL)
                //         (tmp_col_1 ->right) ->left = tmp_col_1 ->left;
                //     if((tmp_col_1 ->up) != NULL)
                //         (tmp_col_1 ->up) ->down = tmp_col_1 ->down;
                //     if((tmp_col_1 ->down) != NULL)
                //         (tmp_col_1 ->down) ->up = tmp_col_1 ->up;
                // }
                rnk_1[tmp_col_1 ->rk].x = j;
                rnk_1[tmp_col_1 ->rk].y = i;
                rnk_1[tmp_col_1 ->rk].adrs = tmp_col_1;

                tmp_col_1 = tmp_col_1 ->right;
                tmp_col_2 = tmp_col_2 ->right;
            }
            tmp_row_1 = tmp_row_1 ->down;
            tmp_row_2 = tmp_row_2 ->down;
        }
        //--------------& 把土司數為0的店拔掉
        for(int i = 0; i < c_tot; i++)
        {
            c_adrs = rnk_1[c_total[i]].adrs;       
            if((c_adrs ->left) != NULL)
                (c_adrs ->left) ->right = c_adrs ->right;
            if((c_adrs->right) != NULL)
                (c_adrs ->right) ->left = c_adrs ->left;
            if((c_adrs ->up) != NULL)
                (c_adrs ->up) ->down = c_adrs ->down;
            if((c_adrs ->down) != NULL)
                (c_adrs ->down) ->up = c_adrs ->up;
        }

    }
    


    // //把更新後的第一張圖重接一次
    // tmp_row_1 = head_1;
    // while(tmp_row_1 != NULL)
    // {
    //     tmp_col_1 = tmp_row_1;
    //     while(tmp_col_1 != NULL)
    //     {
    //         if(rnk_1[tmp_col_1 ->rk].y == 0)
    //         tmp_col_1 ->up = NULL;
    //         if(rnk_1[tmp_col_1 ->rk].y == (row - 1))
    //         tmp_col_1 ->down = NULL;
    //         if(rnk_1[tmp_col_1 ->rk].x == 0)
    //         tmp_col_1 ->left = NULL;
    //         if(rnk_1[tmp_col_1 ->rk].x == (col - 1))
    //         tmp_col_1 ->right = NULL;

    //         tmp_col_1 = tmp_col_1 ->right;
    //     }
    //     tmp_row_1 = tmp_row_1 ->down;
    // }

    // tmp_row_1 = head_1;
    // while(tmp_row_1 != NULL)
    // {
    //     tmp_col_1 = tmp_row_1;
    //     while(tmp_col_1 != NULL)
    //     {
    //         if((tmp_col_1 ->left) != NULL)
    //             (tmp_col_1 ->left) ->right = tmp_col_1;
    //         if((tmp_col_1 ->right) != NULL)
    //             (tmp_col_1 ->right) ->left = tmp_col_1;
    //         if((tmp_col_1 ->up) != NULL)
    //             (tmp_col_1 ->up) ->down = tmp_col_1;
    //         if((tmp_col_1 ->down) != NULL)
    //             (tmp_col_1 ->down) ->up = tmp_col_1;
            
    //         tmp_col_1 = tmp_col_1 ->right;
    //     }
    //     tmp_row_1 = tmp_row_1 ->down;
    // }

    train(rnk_1, T2, row, col, c_total, &c_tot);

    //------------把所有倒掉的店(c_total)接回去
    for(int i = 0; i < c_tot; i++)
    {
        c_adrs = rnk_1[c_total[i]].adrs;       
        if((c_adrs ->left) != NULL)
            (c_adrs ->left) ->right = c_adrs;
        if((c_adrs->right) != NULL)
            (c_adrs ->right) ->left = c_adrs;
        if((c_adrs ->up) != NULL)
            (c_adrs ->up) ->down = c_adrs;
        if((c_adrs ->down) != NULL)
            (c_adrs ->down) ->up = c_adrs;
    }

    // tmp_row_1 = head_1;
    // while(tmp_row_1 != NULL)
    // {
    //     tmp_col_1 = tmp_row_1;
    //     while(tmp_col_1 != NULL)
    //     {
    //         printf("%d ", tmp_col_1 ->t_num);
    //         tmp_col_1 = tmp_col_1 ->right;
    //     }
    //     printf("\n");
    //     tmp_row_1 = tmp_row_1 ->down;
    // }

    tmp_row_1 = head_1;
    for(int i = 0; i < row; i++)
    {
        tmp_col_1 = tmp_row_1; 
        for(int j = 0; j < col; j++)
        {
            printf("%d ", tmp_col_1 ->t_num);
            tmp_col_1 = tmp_col_1 ->right;
        }
        printf("\n");
        tmp_row_1 = tmp_row_1 ->down;
    }

    //---------------free memories-------------
    free(c_total);

    tmp_row_1 = head_1;
    node *killed;
    while(tmp_row_1 != NULL)
    {
        tmp_col_1 = tmp_row_1;
        while(tmp_col_1 != NULL)
        {
            killed = tmp_col_1;
            tmp_col_1 = tmp_col_1 ->right;
            free(killed);
        }
        tmp_row_1 = tmp_row_1 ->down;
    }

    free(rnk_1);
    // for(int i = 0; i < row; i++)
    // {
    //     free(cor[i]);
    // }
    // free(cor);    
}

    


// founded_rk = start ->rk;
// up_p = start;
// down_p = start;
// left_p = start;
// right_p = start;
// while (founded_rk == (start ->rk))  //外擴
// {
//     if(up_p != NULL || up_p != head)
//         up_p = up_p ->up;
//     if(down_p != NULL || down_p != head)
//         down_p = down_p ->down;
//     if(left_p != NULL || left_p != head)
//         left_p = left_p ->left;
//     if(right_p != NULL || right_p != head)
//         right_p = right_p ->right;
    
//     dir[0] = up_p;
//     dir[1] = right_p; 
//     dir[2] = down_p;
//     dir[3] = left_p;

//     for(int k = 0; k < 4; k++)
//     {
//         if(dir[k] == NULL || (dir[k] ->t_num) == 0)
//             continue;
//         else
//         {
//             if(founded_rk == (start ->rk))
//             {
//                 founded_rk = dir[k] ->rk;
//                 start = dir[k];
//             }
//             else
//             {
//                 if((dir[k] ->rk) < founded_rk)
//                 founded_rk = dir[k] ->rk;
//                 start = dir[k];
//             }
//         }
//     }
// }

// (start ->t_num) -= s;  //吃麵包
// if((start ->t_num) < 0)
// start ->t_num = 0;