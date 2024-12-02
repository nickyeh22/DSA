#include <stdio.h>
#include <stdlib.h> 
#include <memory.h>
#include <stdbool.h>

typedef struct knt 
{
    long long int hp;   //血量
    int atk;            //攻擊力
    int knt_idx;       
    struct set *set;  //所屬set
    bool alive;  //ture ->alive, fasle ->dead
} knt;

typedef struct set
{
    int set_idx;        //set index
    long long int tot_atk;     
    int size;       //set中有幾隻9t
    int atk_num;    //set中所有9t的攻擊力總和
    // int heap_arr_sz;    //記heap_arr的總格數
    long long int DL;       //此set的死亡標準
    struct knt **heap;      //min heap root
} set;

// typedef struct heap_node
// {
//     struct heap_node *left;
//     struct heap_node *right;
//     struct heap_node *parent;
//     struct knt *knt_ptr;
// } heap_node;

void swap_p(knt **a, knt **b) 
{
    knt *tmp = *a;
    *a = *b;
    *b = tmp;
}

void heap_insert(knt **heap, knt *ins_knt, int *heap_size) 
{
    heap[(*heap_size)] = ins_knt;
    int current = (*heap_size);

    while ((current > 0) && (heap[current]->hp) < (heap[(current-1) / 2]->hp)) 
    {
        swap_p(&(heap[current]), &(heap[(current-1) / 2]));      
        current = (current - 1) / 2;
    }

    (*heap_size) ++;
}

void heap_delete(knt **heap, int *heap_size) 
{
    // if((*heap_size) == 1)
    // {
    //     (*heap_size)--;
    //     return NULL;
    // }
    
    heap[0] = heap[(*heap_size) - 1];
    (*heap_size)--;

    int current = 0;
    int child = current * 2 + 1;

    while (child < *heap_size) 
    {
        if(child + 1 < *heap_size && (heap[child + 1]->hp) < (heap[child]->hp))
            child++;

        if(heap[current]->hp > heap[child]->hp)
        {
            swap_p(&(heap[current]), &(heap[child])); 
            current = child;
            child = current * 2 + 1;
        } 
        else
            break;
    }

    // return heap[0];
}

void main()
{
    int n, m;   //number of 9t, number of round
    scanf("%d %d", &n, &m);

    knt *knt_arr = (knt*)malloc(sizeof(knt) * (n + 1));
    set *set_arr = (set*)malloc(sizeof(set) * (n + 1));
    int *ans_arr = (int*)calloc((n + 1), sizeof(int));

    for(int i = 1; i < (n+1); i++)
    {
        scanf(" %lld", &(knt_arr[i].hp));
        knt_arr[i].knt_idx = i;
        knt_arr[i].set = set_arr + i;
        knt_arr[i].alive = true;
    }

    for(int i = 1; i < (n+1); i++)
    {
        scanf(" %d", &(knt_arr[i].atk));
        set_arr[i].set_idx = i;
        set_arr[i].tot_atk = knt_arr[i].atk;
        set_arr[i].size = 1;
        set_arr[i].atk_num = 0;
        set_arr[i].DL = 0;
        set_arr[i].heap = (knt**)malloc(sizeof(knt*));
        // set_arr[i].heap_arr_sz = 1;
        //initialize heap_root
        set_arr[i].heap[0] = &(knt_arr[i]);
        // set_arr[i].root->left = NULL;
        // set_arr[i].root->right = NULL;
        // set_arr[i].root->parent = NULL;
    }

    int k_a, k_s;      //k_a:攻擊的9t index, k_s:被攻擊的9t index
    for(int i = 0; i < m; i++)
    {
        scanf("%d %d", &k_a, &k_s);

        if((knt_arr[k_a].set->set_idx) == (knt_arr[k_s].set->set_idx))      //攻擊9t與被攻擊9t處在同個set
            continue;
        if(knt_arr[k_a].alive == false)    //攻擊9t死了
            continue;
        if(knt_arr[k_s].alive == false)    //被攻擊9t死了
            continue;
 
        knt_arr[k_a].set->atk_num++;
        knt_arr[k_s].set->DL += knt_arr[k_a].set->tot_atk;
        while((knt_arr[k_s].set->heap[0]->hp <= knt_arr[k_s].set->DL) && (knt_arr[k_s].set->size > 0))      //拔死人
        {
            knt_arr[k_s].set->tot_atk -= knt_arr[k_s].set->heap[0]->atk;
            knt_arr[k_s].set->heap[0]->alive = false;
            ans_arr[knt_arr[k_s].set->heap[0]->knt_idx] += knt_arr[k_s].set->atk_num;
            heap_delete(knt_arr[k_s].set->heap, &(knt_arr[k_s].set->size));
        }

        if((knt_arr[k_s].set->size) > (knt_arr[k_a].set->size))    //k_a(小)併入k_s(大)
        {
            knt_arr[k_s].set->heap = (knt**)realloc((knt_arr[k_s].set->heap), sizeof(knt*) * (knt_arr[k_s].set->size + knt_arr[k_a].set->size));    //realloc大的set的heap大小
            // if(knt_arr[k_s].set->heap == NULL)
            //     printf("realloc ero");

            int ins_set_idx = knt_arr[k_a].set->set_idx;
// int j = 0;
            while(set_arr[ins_set_idx].size != 0)      //小的還沒拔完
            {
// j++;
// if(i == 15)
// printf("%d\n", j);
                ans_arr[set_arr[ins_set_idx].heap[0]->knt_idx] += (set_arr[ins_set_idx].atk_num - knt_arr[k_s].set->atk_num);
                knt_arr[k_s].set->tot_atk += set_arr[ins_set_idx].heap[0]->atk;
                // knt_arr[k_a].set->heap[0]->set = knt_arr[k_s].set;
                set_arr[ins_set_idx].heap[0]->hp += (knt_arr[k_s].set->DL - set_arr[ins_set_idx].DL);
                knt *ins_knt = set_arr[ins_set_idx].heap[0];
                heap_insert(knt_arr[k_s].set->heap, ins_knt, &(knt_arr[k_s].set->size)); 
                heap_delete(set_arr[ins_set_idx].heap, &(set_arr[ins_set_idx].size));
                ins_knt->set = knt_arr[k_s].set;
            }        
        }

        else    //k_s(小)併入k_a(大)
        {
            knt_arr[k_a].set->heap = (knt**)realloc((knt_arr[k_a].set->heap), sizeof(knt*) * (knt_arr[k_s].set->size + knt_arr[k_a].set->size));    //realloc大的set的heap大小
            // if(knt_arr[k_a].set->heap == NULL)
            //     printf("realloc ero");

            int ins_set_idx = knt_arr[k_s].set->set_idx;
            while(set_arr[ins_set_idx].size != 0)      //小的還沒拔完
            {
            // printf("%d\n", knt_arr[k_s].set->size);
                ans_arr[set_arr[ins_set_idx].heap[0]->knt_idx] += (set_arr[ins_set_idx].atk_num - knt_arr[k_a].set->atk_num);
                knt_arr[k_a].set->tot_atk += set_arr[ins_set_idx].heap[0]->atk;
                // knt_arr[k_s].set->heap[0]->set = knt_arr[k_a].set;
                set_arr[ins_set_idx].heap[0]->hp += (knt_arr[k_a].set->DL - set_arr[ins_set_idx].DL);
                knt *ins_knt = set_arr[ins_set_idx].heap[0];
                heap_insert(knt_arr[k_a].set->heap, ins_knt, &(knt_arr[k_a].set->size));
                heap_delete(set_arr[ins_set_idx].heap, &(set_arr[ins_set_idx].size));
                ins_knt->set = knt_arr[k_a].set;
            // printf("%d\n", knt_arr[k_s].set->size);
            // break;  
            }        
        }

    }

// printf("\n");
// for(int i = 1; i < n+1; i++)
//     printf("%d %d\n", i, knt_arr[i].set->set_idx);

    for(int i = 1; i < n+1; i++)
    {
        if(knt_arr[i].alive == false)   //9t已死
        {
            printf("%d ", ans_arr[i]);
            continue;
        }    
        else
        {
            ans_arr[i] += knt_arr[i].set->atk_num;
            printf("%d ", ans_arr[i]);
        }    
    }


}
