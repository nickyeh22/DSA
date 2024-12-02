#include <stdio.h>
#include <stdlib.h> 
#include <memory.h>

typedef struct node 
{
    int id;
    int data;
    struct node *prev;
    struct node *nxt;
} node;

node *alloc(int id, int data, node *prev, node *nxt) 
{
    node *tmp = (node *) malloc(sizeof(node));
    tmp ->id = id;
    tmp ->data = data;
    tmp ->prev = prev;
    tmp ->nxt = nxt;
    return tmp;
}

void main()
{
    int n, m;  // n為參賽者數目(回合數); m為競技場大小(發生革命的條件)
    scanf("%d %d", &n, &m);  
    
    int tmp_a;
    node *head = alloc(0, 0, NULL, NULL);
    node *tmp = alloc(0, 0, NULL, NULL); 
    node *tail = alloc(0, 0, NULL, NULL);
    node *killed;
    int player_num = 0;

    for(int i = 1 ; i < n+1 ; i++ )
    {
        scanf(" %d", &tmp_a);
        printf("Round %d:", i);        
        tmp = tail;
        
        if(i == 1)
        {
            tail = alloc(i, tmp_a, head, NULL);
            head ->nxt = tail;
            // printf("hiiiiiiiiiii");
            tmp = head;
            player_num = 1;
        }

        while(tmp != head)
        {
            if((tmp_a) > (tmp ->data))
            {
                printf(" %d", tmp ->id);
                player_num--;
                killed = tmp;
                // (tmp ->nxt) ->prev = tmp ->prev;
                tail = tmp ->prev;
                tmp = tmp ->prev;

                memset(killed, 0, sizeof(node));
                free(killed);
            }           
            else
            {
                if(player_num == m)
                {
                    tmp = head ->nxt;
                    printf(" %d", tmp ->id);
                    // printf("%d", (tmp ->prev)->id);
                    // printf("%d", (tmp ->nxt)->id);
                    if(tmp ->nxt == NULL)
                    {
                        tail = head;
                        player_num--;
                        memset(tmp, 0, sizeof(node));
                        free(tmp);
                    }
                    else
                    {
                        (tmp ->nxt) ->prev = head;
                        // printf("hiiiiiiiiiii");
                        head ->nxt = tmp ->nxt;
                        player_num--;
                        memset(tmp, 0, sizeof(node));
                        free(tmp);
                    }                   
                }
                break;
            }
        }

        if(i != 1)
        {
            tail = alloc(i, tmp_a, tail, NULL);
            (tail ->prev) ->nxt = tail;
            player_num++;
        }

        printf("\n");
        // pv = tmp;
    }

    printf("Final:");
    tmp =  tail;
    while(tmp != head)
    {
        printf(" %d", tmp ->id);
        killed = tmp;
        tmp = tmp ->prev;
        memset(killed, 0, sizeof(node));
        free(killed);
    }
    // printf("\n");
    // printf("%d", head ->nxt ->nxt);

    // memset(head, 0, sizeof(node));
    // free(head);
    memset(tmp, 0, sizeof(node));
    free(tmp);
    // memset(tail, 0, sizeof(node));
    // free(tail);

}
