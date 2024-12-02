#include <stdio.h>
#include <stdlib.h> 
#include <memory.h>
#include <stdbool.h>

long long int N, M;

typedef struct tree 
{
    long long int comp_idx;   //公司序號
    long long int heap_num;  //記錄有多少價格
    long long int tot_price;  //記錄與這個結點簽約要多少錢
    long long int comp_num;   //記錄這個結點有多少子公司(加自己)
    struct tree *child;
    struct tree *sibl;
    struct tree *parent;
    struct tree *last_child;
} tree;

typedef struct heap_node 
{
    long long int price;
    long long int time;
} heap_node;

tree *alloc_tree() 
{
    tree *tmp = (tree *) malloc(sizeof(tree));
    tmp ->heap_num = 0;        
    tmp ->tot_price = 0;
    tmp ->comp_num = 0;
    tmp ->child = NULL;
    tmp ->sibl = NULL;
    tmp ->parent = NULL;
    tmp ->last_child = NULL;   
    return tmp;
}

void buildtree(tree **tree_arr, tree **sort_arr)
{
    int prnt;
    for(int i = 0; i < N; i++)  //initialize
    {        
        tree_arr[i] = alloc_tree();  
        tree_arr[i] ->comp_idx = i + 1;
        sort_arr[i] = tree_arr[i];
    }

    for(int i = 1; i < N; i++)  //link the tree node
    {
        scanf(" %d", &prnt);
        tree_arr[i] ->parent = tree_arr[prnt - 1];
        if(tree_arr[prnt - 1] ->last_child == NULL)
            tree_arr[prnt - 1] ->child = tree_arr[i];
        else
            tree_arr[prnt - 1] ->last_child ->sibl = tree_arr[i];
        tree_arr[prnt - 1] ->last_child = tree_arr[i];
    }
}

// int count_sub_comp(tree *node)
// {
//     if (node ->child == NULL)
//     {
//         node ->comp_num = 1;
//         return 1;
//     }
//     else
//     {
//         node ->comp_num = count_sub_comp(node ->child) + 1;
//         tree *sibling = node ->child ->sibl;
//         while(sibling != NULL)
//         {
//             node ->comp_num += count_sub_comp(sibling);
//             sibling = sibling ->sibl;
//         }
//         return node ->comp_num;
//     }        
// }

void count_sub_comp(tree *node)
{
    node ->comp_num = 1;

    if(node ->child != NULL)
        count_sub_comp(node ->child);
    if(node ->sibl != NULL)
        count_sub_comp(node ->sibl);
    if(node ->parent != NULL)
        node ->parent ->comp_num += node ->comp_num;
}

void merge(tree **arr, int left, int middle, int right)
{
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 = right - middle;

    // 創建暫時的陣列
    tree **left_arr = (tree**)malloc(sizeof(tree*) * n1);
    tree **right_arr = (tree**)malloc(sizeof(tree*) * n2);

    // 複製資料到暫時的陣列 leftArr[] 與 rightArr[]
    for (i = 0; i < n1; i++)
        left_arr[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        right_arr[j] = arr[middle + 1 + j];

    // 將暫時的陣列 leftArr[] 與 rightArr[] 合併回原陣列 arr[]
    i = 0; // 初始化第一個子陣列的索引
    j = 0; // 初始化第二個子陣列的索引
    k = left; // 初始化合併後子陣列的索引
    while (i < n1 && j < n2) {
        if ((left_arr[i] ->comp_num) >= (right_arr[j] ->comp_num)) {
            arr[k] = left_arr[i];
            i++;
        }
        else {
            arr[k] = right_arr[j];
            j++;
        }
        k++;
    }

    // 將剩餘的元素複製回原陣列 arr[]
    while (i < n1) {
        arr[k] = left_arr[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = right_arr[j];
        j++;
        k++;
    }
      
    free(left_arr);
    free(right_arr);
}

void mergeSort(tree **arr, int left, int right)
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

void swap(long long int* a, long long int* b) 
{
    long long int tmp = *a;
    *a = *b;
    *b = tmp;
}

// void swap_t(int* a, int* b) 
// {
//     int tmp = *a;
//     *a = *b;
//     *b = tmp;
// }

void heap_insert(heap_node *heap, long long int P, long long int T, long long int *heap_size) 
{
    heap[(*heap_size)].price = P;
    heap[(*heap_size)].time = T;
    int current = (*heap_size);

    while ((current > 0) && (heap[current].price) < (heap[(current-1) / 2].price)) 
    {
        swap(&(heap[current].price), &(heap[(current-1) / 2].price));
        swap(&(heap[current].time), &(heap[(current-1) / 2].time));        
        current = (current - 1) / 2;
    }

    (*heap_size) ++;
}

void heap_delete(heap_node *heap, long long int *heap_size) 
{
    heap[0].price = heap[(*heap_size) - 1].price;
    heap[0].time = heap[(*heap_size) - 1].time;
    (*heap_size)--;

    int current = 0;
    int child = current * 2 + 1;

    while (child < *heap_size) 
    {
        if((heap[child + 1].price) < (heap[child].price))
            child++;

        if(heap[current].price > heap[child].price)
        {
            swap(&(heap[current].price), &(heap[child].price));
            swap(&(heap[current].time), &(heap[child].time));  
            current = child;
            child = current * 2 + 1;
        } 
        else
            break;
    }
}

// int count_tot_price(heap_node **comp_heap, tree *node)
// {
//     if (node ->child == NULL)
//     {
//         node ->tot_price = comp_heap[(node ->comp_idx) - 1][0].price;
//         return comp_heap[(node ->comp_idx) - 1][0].price;
//     }
//     else
//     {
//         node ->tot_price = count_tot_price(comp_heap, node ->child) + comp_heap[(node ->comp_idx) - 1][0].price;
//         tree *sibling = node ->child ->sibl;
//         while(sibling != NULL)
//         {
//             node ->tot_price += count_tot_price(comp_heap, sibling);
//             sibling = sibling ->sibl;
//         }
//         return node ->tot_price;
//     }        
// }

void count_tot_price(heap_node **comp_heap, tree *node)
{
    node ->tot_price = comp_heap[(node ->comp_idx) - 1][0].price;

    if(node ->child != NULL)
        count_tot_price(comp_heap, node ->child);
    if(node ->sibl != NULL)
        count_tot_price(comp_heap, node ->sibl);
    if(node ->parent != NULL)
        node ->parent ->tot_price += node ->tot_price;
}

void main()
{
    long long int C;
    scanf("%d %d %lld", &N, &M, &C);
    tree **tree_arr = (tree**)malloc(sizeof(tree*) * N);    
    tree **sort_arr = (tree**)malloc(sizeof(tree*) * N);    
    buildtree(tree_arr, sort_arr);
    count_sub_comp(tree_arr[0]);

    mergeSort(sort_arr, 0, N-1);    //將tree_arr依公司數多->少排序

    heap_node **comp_heap = (heap_node**)malloc(sizeof(heap_node*) * N);    //按照公司idx排序(0 ~ N-1)
    for(int i = 0; i < N; i++)
    {
        comp_heap[i] = (heap_node *) malloc(sizeof(heap_node) * M);
    }

    //開始經歷每一天
    long long int P;
    long long int T;
    for(int D = 1; D < M+1; D++)
    {
        for(int i = 1; i < N+1; i++)    //維護每間公司的heap
        {
            scanf("%lld %d", &P, &T);
            heap_insert(&comp_heap[i-1][0], P, D+T, &(tree_arr[i-1] ->heap_num));   //heap加入新價格
            while((comp_heap[i-1][0].time) < D)    //heap檢查是否有價格過期並移除
            {
                heap_delete(comp_heap[i-1], &(tree_arr[i-1] ->heap_num));
            }
        }   
        count_tot_price(comp_heap, tree_arr[0]);  //計算與每間公司簽約的總價

        int i;
        for(i = 0; i < N; i++)
        {
            if(sort_arr[i] ->tot_price <= C)
            {
                printf("%d\n", sort_arr[i] ->comp_num);
                break;
            }    
        }
        if(i == N)
            printf("0\n");
        
    }
}