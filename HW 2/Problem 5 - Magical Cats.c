#include <stdio.h>
#include <stdlib.h> 
#include <memory.h>
#include <stdbool.h>

int N, M;    //number of cats, number of steps

typedef struct cat
{
    int apt;    //cat's appetite
    int clr;    //cat's color
    struct apt_node *apt_ptr;
    int color_arr_idx;
    int cir_arr_idx;
} cat;
 
typedef struct apt_node
{
    int apt;
    struct cat *cat_ptr;
    struct apt_node *pre;
    struct apt_node *nxt;
} apt_node;

apt_node *head;    //head ->min apt
apt_node *tail;    //tail ->max apt

typedef struct clr_node
{
    int clr;    //color index
    int tail_idx;
    int size;
    struct cat **cir_arr;
} clr_node;

cat *alloc_cat(int ap, apt_node *ptr) 
{
    cat *tmp = (cat *) malloc(sizeof(cat));
    tmp ->apt = ap;
    tmp ->clr = -1;
    tmp ->apt_ptr = ptr;     
    tmp ->color_arr_idx = -1;
    tmp ->cir_arr_idx = -1;   
    return tmp;
}

// clr_node *alloc_clr_node() 
// {
//     clr_node *tmp = (clr_node *) malloc(sizeof(clr_node));
//     tmp ->clr = -1;    //color index
//     tmp ->tail_idx = -1;
//     tmp ->size = -1;
//     tmp ->cir_arr = NULL; 
//     return tmp;
// }

void merge(cat **arr, int left, int middle, int right)
{
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 = right - middle;

    // 創建暫時的陣列
    cat **left_arr = (cat**)malloc(sizeof(cat*) * n1);
    cat **right_arr = (cat**)malloc(sizeof(cat*) * n2);

    // 複製資料到暫時的陣列 leftArr[] 與 rightArr[]
    for (i = 0; i < n1; i++)
        left_arr[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        right_arr[j] = arr[middle + 1 + j];

    // 將暫時的陣列 leftArr[] 與 rightArr[] 合併回原陣列 arr[]
    i = 0; // 初始化第一個子陣列的索引
    j = 0; // 初始化第二個子陣列的索引
    k = left; // 初始化合併後子陣列的索引
    while (i < n1 && j < n2)
    {
        if ((left_arr[i] ->clr) < (right_arr[j] ->clr))
        {
            arr[k] = left_arr[i];
            i++;
        }
        else if((left_arr[i] ->clr) == (right_arr[j] ->clr))
        {
            if ((left_arr[i] ->apt) < (right_arr[j] ->apt))
            {
                arr[k] = left_arr[i];
                i++;
            }
            else
            {
                arr[k] = right_arr[j];
                j++;
            }
        }
        else
        {
            arr[k] = right_arr[j];
            j++;
        }
        k++;
    }

    // 將剩餘的元素複製回原陣列 arr[]
    while (i < n1)
    {
        arr[k] = left_arr[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = right_arr[j];
        j++;
        k++;
    }
    free(left_arr);
    free(right_arr);
}

void mergeSort(cat **arr, int left, int right)
{
    if (left < right) {
        int middle = left + (right - left) / 2;

        // 分割左邊的子陣列
        mergeSort(arr, left, middle);

        // 分割右邊的子陣列
        mergeSort(arr, middle + 1, right);

        // 合併子陣列
        merge(arr, left, middle, right);
    }
}

apt_node* merge_List(apt_node* l1, apt_node* l2) {
    apt_node dummy = {0};
    apt_node* curr = &dummy;

    while (l1 != NULL && l2 != NULL) {
        if (l1 ->apt < l2 ->apt) {
            curr ->nxt = l1;
            l1 ->pre = curr;
            l1 = l1 ->nxt;
        } else {
            curr ->nxt = l2;
            l2 ->pre = curr;
            l2 = l2 ->nxt;
        }
        curr = curr ->nxt;
    }

    if (l1 != NULL)
    {
        curr ->nxt = l1;
        l1 ->pre = curr;
    }
    else
    {
        curr ->nxt = l2;
        l2 ->pre = curr;
    }
        
    // curr->next->prev = curr;
    return dummy.nxt;
}

apt_node* sort_List(apt_node* hd) {
    if (hd == NULL || hd ->nxt == NULL) {
        return hd; // base case
    }

    /* Step 1: Split the list into two halves */
    apt_node* slow = hd;
    apt_node* fast = hd ->nxt;
    while (fast != NULL && fast ->nxt != NULL) {
        slow = slow ->nxt;
        fast = fast ->nxt ->nxt;
    }

    apt_node* right = slow ->nxt;
    right ->pre = NULL;
    slow ->nxt = NULL;        

    /* Step 2: Sort each half recursively */
    apt_node* left_sorted = sort_List(hd);  
    apt_node* right_sorted = sort_List(right);  

    /* Step 3: Merge the sorted halves */
    return merge_List(left_sorted, right_sorted);
}

// int binarySearch_max(cat **arr, int k, int left, int right)     //找有多少比上限小
// {
//     int mid = (left + right) / 2;
//     int index = -1;
//     while(left <= right)     
//     {
//        mid = (left + right) / 2;
//        if(arr[mid] ->apt <= k)
//        {
//          index = mid;
//          left = mid + 1;
//        }
//        else
//          right = mid - 1;
//     }
//     return (index + 1);
// }

// int binarySearch_min(cat **arr, int k, int left, int right)      //找有多少比下限大
// {
//     // int l = 0;
//     // int r = n - 1;
//     int len = right - left + 1;     
//     int index = len;
//     while (left <= right)
//     {
//         int mid = left + (right - left) / 2;
//         if (arr[mid] ->apt >= k)
//         {
//             index = mid;
//             right = mid - 1;
//         }
//         else
//             left = mid + 1;
//     }
//     return (len - index);
// }

int binarySearch_max(cat **arr, int k, int left, int right)     //找有多少比上限小
{
    int org_left = left;
    int mid = (left + right) / 2;
    int index = -1;
    while(left <= right)     
    {
       mid = (left + right) / 2;
       if(arr[mid] ->apt <= k)
       {
         index = mid - org_left;
         left = mid + 1;
       }
       else
         right = mid - 1;
    }
    return (index + 1);
}

int binarySearch_min(cat **arr, int k, int left, int right)      //找有多少比下限大
{
    // int l = 0;
    // int r = n - 1;
    int org_left = left;
    int len = right - left + 1;     
    int index = len;
    while (left <= right)
    {
        // int mid = left + (right - left) / 2;
        int mid = (left + right) / 2;
        if (arr[mid] ->apt >= k)
        {
            index = mid - org_left;
            right = mid - 1;
        }
        else
            left = mid + 1;
    }
    return (len - index);
}

int binary_search_color(clr_node *arr, int left, int right, int key)      //for color array
{
    while (left <= right) 
    {
        int mid = left + (right - left) / 2;
        if (arr[mid].clr == key) 
            return mid;
        else if (arr[mid].clr < key) 
            left = mid + 1;
        else 
            right = mid - 1;
    }
    return -1;
}

void swap_int(int *a, int *b)
{
    int copy = *a;
    *a = *b;
    *b = copy;
}

void swap_cat_ptr(cat **a, cat **b)
{
    cat *copy = *a;
    *a = *b;
    *b = copy;
}

// void swap_apt_ptr(apt_node **a, apt_node **b)
// {
//     apt_node *copy = *a;
//     *a = *b;
//     *b = copy;
// }

// void swapNodes(apt_node *node_A, apt_node *node_B) 
// {
//     if (node_A == node_B) {
//         printf("swap node err");
//     }

//     apt_node *pre_A = node_A ->pre;
//     apt_node *pre_B = node_B ->pre;
//     apt_node *next_A = node_A ->nxt;
//     apt_node *next_B = node_B ->nxt;

//     if (pre_A)
//         pre_A ->nxt = node_B;
//     else
//     {
//         node_A ->pre = node_B;
//     }
        

//     // if (next_A)
//         next_A ->pre = node_B;
//     // else
//         // node_A ->nxt = node_B;
    
//     node_B ->pre = pre_A;
//     node_B ->nxt = next_A;

//     // if (pre_B)
//         pre_B ->nxt = node_A;
//     // else
//         // node_B ->pre = node_A;
    
//     if (next_B)
//         next_B ->pre = node_A;
//     else
//         node_B ->nxt = node_A;
    
//     node_A ->pre = pre_B;
//     node_A ->nxt = next_B;
// }

void swapNodes(apt_node *node_l, apt_node *node_r) 
{
    // if (node_l == node_r) {
    //     printf("swap node err");
    // }

    apt_node *pre_l = node_l ->pre;
    apt_node *next_r = node_r ->nxt;

    if(pre_l == NULL && next_r == NULL)     //only two cats
    {
        node_r ->pre = pre_l;
        node_r ->nxt = node_l;
        node_l ->pre = node_r;
        node_l ->nxt = next_r;
        head = node_r;
        tail = node_l;
    }
    else if(pre_l == NULL)      //node_l is head
    {
        node_r ->pre = pre_l;
        node_r ->nxt = node_l;
        node_l ->pre = node_r;
        node_l ->nxt = next_r;
        next_r ->pre = node_l;
        head = node_r;
    }
    else if(next_r == NULL)     //node_r is tail 
    {
        node_r ->pre = pre_l;
        node_r ->nxt = node_l;
        node_l ->pre = node_r;
        node_l ->nxt = next_r;
        pre_l ->nxt = node_r;
        tail = node_l;
    }
    else
    {
        node_r ->pre = pre_l;
        node_r ->nxt = node_l;
        node_l ->pre = node_r;
        node_l ->nxt = next_r;
        next_r ->pre = node_l;
        pre_l ->nxt = node_r;
    }
}

void question(clr_node *color_arr, int color_arr_sz)
{
    int c, min, max;
    int color_arr_idx;
    int left_num = 0;
    int right_num = 0;
    scanf(" %d %d %d", &c, &min, &max);     
    color_arr_idx = binary_search_color(color_arr, 0, color_arr_sz - 1, c); 
    if(color_arr_idx == -1)
        printf("0\n");
    else
    {
        left_num += binarySearch_min(color_arr[color_arr_idx].cir_arr, min, 0, color_arr[color_arr_idx].tail_idx);  
        left_num += binarySearch_max(color_arr[color_arr_idx].cir_arr, max, 0, color_arr[color_arr_idx].tail_idx); 
        left_num = left_num - (color_arr[color_arr_idx].tail_idx - 0 + 1);
        right_num += binarySearch_min(color_arr[color_arr_idx].cir_arr, min, color_arr[color_arr_idx].tail_idx + 1, color_arr[color_arr_idx].size - 1);
        right_num += binarySearch_max(color_arr[color_arr_idx].cir_arr, max, color_arr[color_arr_idx].tail_idx + 1, color_arr[color_arr_idx].size - 1);
        right_num = right_num - (color_arr[color_arr_idx].size - color_arr[color_arr_idx].tail_idx - 1);
        printf("%d\n", left_num + right_num);
    }
}

void greedy(cat **cat_arr, clr_node *color_arr)
{
    int cat_idx;
    scanf("%d", &cat_idx);  
    swap_int(&(cat_arr[cat_idx] ->apt), &(cat_arr[cat_idx] ->apt_ptr ->nxt ->cat_ptr ->apt));   
    if(cat_arr[cat_idx]->clr == cat_arr[cat_idx] ->apt_ptr ->nxt ->cat_ptr ->clr)
    {/*printf("ggggggggggg %d %d\n", cat_arr[cat_idx]->apt, cat_arr[cat_idx] ->apt_ptr ->nxt ->cat_ptr ->apt);*/
        swap_cat_ptr(&(color_arr[cat_arr[cat_idx] ->color_arr_idx].cir_arr[cat_arr[cat_idx] ->cir_arr_idx]), &(color_arr[cat_arr[cat_idx] ->color_arr_idx].cir_arr[cat_arr[cat_idx] ->apt_ptr ->nxt ->cat_ptr ->cir_arr_idx]));
        swap_int(&(cat_arr[cat_idx] ->cir_arr_idx), &(cat_arr[cat_idx] ->apt_ptr ->nxt ->cat_ptr ->cir_arr_idx)); /*printf("ggggggggggg");*/
    }
    // swap_cat_ptr(&(cat_arr[cat_idx] ->apt_ptr ->cat_ptr), &(cat_arr[cat_idx] ->apt_ptr ->nxt ->cat_ptr)); 
    // swap_apt_ptr(&(cat_arr[cat_idx] ->apt_ptr), &(cat_arr[cat_idx] ->apt_ptr ->nxt ->cat_ptr ->apt_ptr));
    swap_int(&(cat_arr[cat_idx] ->apt_ptr ->apt), &(cat_arr[cat_idx] ->apt_ptr ->nxt ->apt));
    swapNodes(cat_arr[cat_idx] ->apt_ptr, cat_arr[cat_idx] ->apt_ptr ->nxt); 
}

void magic(clr_node *color_arr, int color_arr_sz)
{
    int c, sel_cat, sf;      //color
    scanf(" %d %d %d", &c, &sel_cat, &sf);
    int color_arr_idx;
    int idx;    //記錄作用魔法的貓在cir_arr中的index
    color_arr_idx = binary_search_color(color_arr, 0, color_arr_sz - 1, c); 
    // if(color_arr_idx == -1)
    //     printf("wrong magic color");
    if(sel_cat == 0)    //選到該顏色食慾最小的貓
    {
        if(sf == 0)     //魔法失敗
        {
            idx = (color_arr[color_arr_idx].tail_idx + 1) % (color_arr[color_arr_idx].size);
            //------------維護cat struct--------------------
            color_arr[color_arr_idx].cir_arr[idx] ->apt = head ->cat_ptr ->apt - 1;    //更新cat struct中的apt
            //------------維護apt_link list--------------------
            if(N == 2)
            {
                if(color_arr[color_arr_idx].cir_arr[idx] == head ->cat_ptr)
                    color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->apt = color_arr[color_arr_idx].cir_arr[idx] ->apt;
                else    //sel_cat == tail
                {
                    color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->apt = color_arr[color_arr_idx].cir_arr[idx] ->apt;
                    swapNodes(head, tail);
                }           
            }
            else if(color_arr[color_arr_idx].cir_arr[idx] == head ->cat_ptr)
            {
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->apt = color_arr[color_arr_idx].cir_arr[idx] ->apt;
            }
            else if(color_arr[color_arr_idx].cir_arr[idx] == tail ->cat_ptr)
            {
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->apt = color_arr[color_arr_idx].cir_arr[idx] ->apt;
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->pre ->nxt = NULL;
                tail = color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->pre;
                head ->pre = color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr;
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->pre = NULL;
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->nxt = head;
                head = color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr;
            }
            else
            {
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->apt = color_arr[color_arr_idx].cir_arr[idx] ->apt;
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->pre ->nxt = color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->nxt;
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->nxt ->pre = color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->pre;
                head ->pre = color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr;
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->pre = NULL;
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->nxt = head;
                head = color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr;
            }
            //------------維護cir_arr(tail_idx)--------------------
                //不用做
        }

        else            //sf == 1; 魔法成功
        {
            idx = (color_arr[color_arr_idx].tail_idx + 1) % (color_arr[color_arr_idx].size);
            //------------維護cat struct--------------------
            color_arr[color_arr_idx].cir_arr[idx] ->apt = tail ->cat_ptr ->apt + 1;    //更新cat struct中的apt
            //------------維護apt_link list--------------------
            if(N == 2)
            {
                if(color_arr[color_arr_idx].cir_arr[idx] == head ->cat_ptr)
                {
                    color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->apt = color_arr[color_arr_idx].cir_arr[idx] ->apt;
                    swapNodes(head, tail);
                }
                else    //sel_cat == tail
                    color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->apt = color_arr[color_arr_idx].cir_arr[idx] ->apt;        
            }
            else if(color_arr[color_arr_idx].cir_arr[idx] == head ->cat_ptr)
            {
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->apt = color_arr[color_arr_idx].cir_arr[idx] ->apt;
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->nxt ->pre = NULL;
                head = color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->nxt;
                tail ->nxt = color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr;
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->nxt = NULL;
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->pre = tail;       //修
                tail = color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr;
            }
            else if(color_arr[color_arr_idx].cir_arr[idx] == tail ->cat_ptr)
            {
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->apt = color_arr[color_arr_idx].cir_arr[idx] ->apt;
            }
            else
            {
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->apt = color_arr[color_arr_idx].cir_arr[idx] ->apt;
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->pre ->nxt = color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->nxt;
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->nxt ->pre = color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->pre;
                tail ->nxt = color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr;
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->nxt = NULL;
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->pre = tail;
                tail = color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr;
            }
            //------------維護cir_arr(tail_idx)--------------------
                color_arr[color_arr_idx].tail_idx = color_arr[color_arr_idx].cir_arr[idx] ->cir_arr_idx;
        }
    }

    else    //sel_cat == 1; 選到該顏色食慾最大的貓
    {
        if(sf == 0)     //魔法失敗
        {
            idx = color_arr[color_arr_idx].tail_idx;
            //------------維護cat struct--------------------
            color_arr[color_arr_idx].cir_arr[idx] ->apt = head ->cat_ptr ->apt - 1;    //更新cat struct中的apt
            //------------維護apt_link list--------------------
            if(N == 2)
            {
                if(color_arr[color_arr_idx].cir_arr[idx] == head ->cat_ptr)
                    color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->apt = color_arr[color_arr_idx].cir_arr[idx] ->apt;
                else    //sel_cat == tail
                {
                    color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->apt = color_arr[color_arr_idx].cir_arr[idx] ->apt;
                    swapNodes(head, tail);
                }           
            }
            else if(color_arr[color_arr_idx].cir_arr[idx] == head ->cat_ptr)
            {
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->apt = color_arr[color_arr_idx].cir_arr[idx] ->apt;
            }
            else if(color_arr[color_arr_idx].cir_arr[idx] == tail ->cat_ptr)
            {
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->apt = color_arr[color_arr_idx].cir_arr[idx] ->apt;
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->pre ->nxt = NULL;
                tail = color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->pre;
                head ->pre = color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr;
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->pre = NULL;
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->nxt = head;
                head = color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr;
            }
            else
            {
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->apt = color_arr[color_arr_idx].cir_arr[idx] ->apt;
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->pre ->nxt = color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->nxt;
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->nxt ->pre = color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->pre;
                head ->pre = color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr;
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->pre = NULL;
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->nxt = head;
                head = color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr;
            }
            //------------維護cir_arr(tail_idx)--------------------
                color_arr[color_arr_idx].tail_idx = abs(color_arr[color_arr_idx].tail_idx + color_arr[color_arr_idx].size - 1) % (color_arr[color_arr_idx].size);
        }

        else            //sf == 1; 魔法成功
        {
            idx = color_arr[color_arr_idx].tail_idx;    
            //------------維護cat struct--------------------
            color_arr[color_arr_idx].cir_arr[idx] ->apt = tail ->cat_ptr ->apt + 1;
            //------------維護apt_link list--------------------
            if(N == 2)
            {
                if(color_arr[color_arr_idx].cir_arr[idx] == head ->cat_ptr)
                {
                    color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->apt = color_arr[color_arr_idx].cir_arr[idx] ->apt;
                    swapNodes(head, tail);
                }
                else    //sel_cat == tail
                    color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->apt = color_arr[color_arr_idx].cir_arr[idx] ->apt;        
            }
            else if(color_arr[color_arr_idx].cir_arr[idx] == head ->cat_ptr)
            {/*printf("hiiiiiiiiiiiiiiiiiiiiiiiiiiii\n");*/ 
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->apt = color_arr[color_arr_idx].cir_arr[idx] ->apt;
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->nxt ->pre = NULL;
                head = color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->nxt;
                tail ->nxt = color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr;
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->nxt = NULL;
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->pre = tail;   //修
                tail = color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr;
            }
            else if(color_arr[color_arr_idx].cir_arr[idx] == tail ->cat_ptr)
            {
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->apt = color_arr[color_arr_idx].cir_arr[idx] ->apt;
            }
            else
            {
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->apt = color_arr[color_arr_idx].cir_arr[idx] ->apt;
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->pre ->nxt = color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->nxt;
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->nxt ->pre = color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->pre;
                tail ->nxt = color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr;
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->nxt = NULL;
                color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr ->pre = tail;
                tail = color_arr[color_arr_idx].cir_arr[idx] ->apt_ptr;
            }
            //------------維護cir_arr(tail_idx)--------------------
                //不用做;
        }
    }
}

void main()
{
    scanf("%d %d", &N, &M);
    cat **cat_arr = (cat**)malloc(sizeof(cat*) * N);    //從0開始
    int ap, cr;
    apt_node *tmp = (apt_node*)malloc(sizeof(apt_node));
    head = tmp;
    scanf("%d", &ap);
    cat_arr[0] = alloc_cat(ap, tmp); 
    tmp ->cat_ptr = cat_arr[0];
    tmp ->apt = ap;
    tmp ->pre = NULL;
    apt_node *next;
    for(int i = 1; i < N; i++)
    {
        next = (apt_node*)malloc(sizeof(apt_node));
        scanf(" %d", &ap);
        cat_arr[i] = alloc_cat(ap, next);
        next ->cat_ptr = cat_arr[i];
        next ->apt = ap;
        tmp ->nxt = next;
        next ->pre = tmp;
        tmp = next;
    }
    tail = tmp;
    tmp ->nxt = NULL;

    cat **clr_arr = (cat**)malloc(sizeof(cat*) * N);
    for(int i = 0; i < N; i++)
    {
        scanf(" %d", &cr);
        clr_arr[i] = alloc_cat(cat_arr[i] ->apt, cat_arr[i] ->apt_ptr);
        clr_arr[i] ->clr = cr;
        cat_arr[i] ->clr = cr;
    }

    mergeSort(clr_arr, 0, N-1); 

    head = sort_List(head);
    head ->pre = NULL;
    tmp = head;
    while(tmp ->nxt != NULL)
        tmp = tmp ->nxt;
    tail = tmp;

    int clr_num = 1;
    for(int i = 0; i < N-1; i++)
    {
        if((clr_arr[i] ->clr) != (clr_arr[i+1] ->clr))
            clr_num++;
    }

    // clr_node **color = (clr_node**)malloc(sizeof(clr_node*) * clr_num);    
    // for(int i = 0; i < clr_num; i++)
    // {
    //     color[i] = alloc_clr_node();
    // }

    // int color_idx = 0;
    // clr_num = 1;
    // int i, j;   
    // for(i = 0; i < N-1; i++)
    // {
    //     if((clr_arr[i] ->clr) == (clr_arr[i+1] ->clr))
    //         clr_num++;
    //     else
    //     {
    //         color[color_idx] ->clr = clr_arr[i] ->clr;
    //         color[color_idx] ->size = clr_num;
    //         color[color_idx] ->tail_idx = clr_num - 1;
    //         color[color_idx] ->cir_arr = (cat**)malloc(sizeof(cat*) * clr_num);
    //         for(j = 0; j < clr_num; j++)
    //         {
    //             color[color_idx] ->cir_arr[j] = clr_arr[i - clr_num + (j + 1)];     
    //             color[color_idx] ->cir_arr[j] ->color_arr_idx = color_idx;
    //             color[color_idx] ->cir_arr[j] ->cir_arr_idx = j;
    //         }
    //         clr_num = 1;
    //         color_idx++;
    //     }   
    // }  

    // if((clr_arr[N-2] ->clr) == (clr_arr[N-1] ->clr))
    // {
    //     color[color_idx] ->clr = clr_arr[i] ->clr;
    //     color[color_idx] ->size = clr_num;
    //     color[color_idx] ->tail_idx = clr_num - 1;
    //     color[color_idx] ->cir_arr = (cat**)malloc(sizeof(cat*) * clr_num);
    //     for(j = 0; j < clr_num; j++)
    //     {
    //         color[color_idx] ->cir_arr[j] = clr_arr[i - clr_num + (j + 1)];    
    //         color[color_idx] ->cir_arr[j] ->color_arr_idx = color_idx;
    //         color[color_idx] ->cir_arr[j] ->cir_arr_idx = j;
    //     }        
    // }
    // else
    // {
    //     color[color_idx] ->clr = clr_arr[i] ->clr;
    //     color[color_idx] ->size = clr_num;
    //     color[color_idx] ->tail_idx = clr_num - 1;  
    //     color[color_idx] ->cir_arr = (cat**)malloc(sizeof(cat*) * 1);   
    //     color[color_idx] ->cir_arr[0] = clr_arr[i];     
    //     color[color_idx] ->cir_arr[0] ->color_arr_idx = color_idx;  
    //     color[color_idx] ->cir_arr[0] ->cir_arr_idx = 0;
    // }

    clr_node *color = (clr_node*)malloc(sizeof(clr_node) * clr_num);

    int color_idx = 0;
    clr_num = 1;
    int i, j;   
    for(i = 0; i < N-1; i++)
    {
        if((clr_arr[i] ->clr) == (clr_arr[i+1] ->clr))
            clr_num++;
        else
        {
            color[color_idx].clr = clr_arr[i] ->clr;
            color[color_idx].size = clr_num;
            color[color_idx].tail_idx = clr_num - 1;
            color[color_idx].cir_arr = (cat**)malloc(sizeof(cat*) * clr_num);
            for(j = 0; j < clr_num; j++)
            {
                color[color_idx].cir_arr[j] = clr_arr[i - clr_num + (j + 1)] ->apt_ptr ->cat_ptr;     
                color[color_idx].cir_arr[j] ->color_arr_idx = color_idx;
                color[color_idx].cir_arr[j] ->cir_arr_idx = j;
            }
            clr_num = 1;
            color_idx++;
        }   
    }  

    if((clr_arr[N-2] ->clr) == (clr_arr[N-1] ->clr))
    {
        color[color_idx].clr = clr_arr[i] ->clr;
        color[color_idx].size = clr_num;
        color[color_idx].tail_idx = clr_num - 1;
        color[color_idx].cir_arr = (cat**)malloc(sizeof(cat*) * clr_num);
        for(j = 0; j < clr_num; j++)
        {
            color[color_idx].cir_arr[j] = clr_arr[i - clr_num + (j + 1)] ->apt_ptr ->cat_ptr;    
            color[color_idx].cir_arr[j] ->color_arr_idx = color_idx;
            color[color_idx].cir_arr[j] ->cir_arr_idx = j;
        }        
    }
    else
    {
        color[color_idx].clr = clr_arr[i] ->clr;
        color[color_idx].size = clr_num;
        color[color_idx].tail_idx = clr_num - 1;  
        color[color_idx].cir_arr = (cat**)malloc(sizeof(cat*) * 1);   
        color[color_idx].cir_arr[0] = clr_arr[i] ->apt_ptr ->cat_ptr;     
        color[color_idx].cir_arr[0] ->color_arr_idx = color_idx;  
        color[color_idx].cir_arr[0] ->cir_arr_idx = 0;
    }
    color_idx++;

    int prcs;
    for(int i = 0; i < M; i++)
    {
        scanf("%d", &prcs);
        if(prcs == 1)
            question(color, color_idx);

        if(prcs == 2)
            greedy(cat_arr, color);

        if(prcs == 3)
            magic(color, color_idx);
    }

// tmp = head;
// while (tmp != NULL)
// {
//     printf(" %d", tmp ->apt);
//     tmp = tmp ->nxt;
// }

}