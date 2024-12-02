#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <memory.h>
#include <stdbool.h>

int N, Q;   //N = initial num of string in set S, Q = num of process
int M;      //M = len of string
int P;      //P = 1 -> insert / P = 2 -> delete

// int set_1[26] = {31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151};
// int set_2[26] = {29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149};
// int set_3[26] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101};

// long long int map_to_prime(char c, int *map)
// {
//     int index = c - 'a'; 
//     return (long long int)map[index]; 
// }

long long int map_(char c)
{
    int index = c - 'a'; 
    return (long long int)index; 
}

int mini(int a, int b, int c)
{
    int min = a;
    if(b < min)
        min = b;
    if(c < min)
        min = c;
    return min;
}

int main()
{
    scanf("%d %d", &N, &Q);
    char *str = (char*)malloc(sizeof(char) * 1000001);
    scanf("%s", str);
    M = strlen(str);
    long long int d = 29;

    long long int ans = 0;
    long long int q1 = 130000007;
    long long int q2 = 120000059;
    long long int q3 = 10000061;

    int *htb_1 = (int*)calloc(q1, sizeof(int));
    int *htb_2 = (int*)calloc(q2, sizeof(int));
    int *htb_3 = (int*)calloc(q3, sizeof(int));

    long long int hv_1 = 0;
    long long int hv_2 = 0;
    long long int hv_3 = 0;
    for(int i = 0; i < M; i++)      //origianl hashing
    {
        hv_1 = (((hv_1*d) % q1) + map_(str[i])) % q1; 
        hv_2 = (((hv_2*d) % q2) + map_(str[i])) % q2;
        hv_3 = (((hv_3*d) % q3) + map_(str[i])) % q3;
    }         
    // htb_1[hv_1] = true;
    long long int head_mod_1 = 1;
    long long int head_mod_2 = 1;
    long long int head_mod_3 = 1;
    for(int i = 0; i < M-1; i++)
    {
        head_mod_1 = (head_mod_1*d) % q1;
        head_mod_2 = (head_mod_2*d) % q2;
        head_mod_3 = (head_mod_3*d) % q3;
    }
    
    htb_1[hv_1] ++;
    htb_2[hv_2] ++;
    htb_3[hv_3] ++;

    for (int j = 0; j < N-1; j++)   //處理initial set
    {
        scanf("%s", str);
        hv_1 = 0;
        hv_2 = 0;
        hv_3 = 0;
        for(int i = 0; i < M; i++)      //origianl hashing
        {
            hv_1 = (((hv_1*d) % q1) + map_(str[i])) % q1; 
            hv_2 = (((hv_2*d) % q2) + map_(str[i])) % q2;
            hv_3 = (((hv_3*d) % q3) + map_(str[i])) % q3;
        }
        if((htb_1[hv_1] != 0) && (htb_2[hv_2] != 0) && (htb_3[hv_3] != 0))
        {
            ans += mini(htb_1[hv_1], htb_2[hv_2], htb_3[hv_3]);
            htb_1[hv_1] ++;
            htb_2[hv_2] ++;
            htb_3[hv_3] ++;
        }
        else    //開始rotate直到三個htb內都不是0
        {
            int i = 0;
            for (i = 0; i < M; i++)   //rotations transition
            {
                hv_1 = (((hv_1 - map_(str[i]) * head_mod_1) % q1) * d) % q1 + map_(str[i]);
                hv_2 = (((hv_2 - map_(str[i]) * head_mod_2) % q2) * d) % q2 + map_(str[i]);
                hv_3 = (((hv_3 - map_(str[i]) * head_mod_3) % q3) * d) % q3 + map_(str[i]);
                if(hv_1 < 0)
                    hv_1 += q1;
                if(hv_2 < 0)
                    hv_2 += q2;
                if(hv_3 < 0)
                    hv_3 += q3;
                hv_1 = hv_1 % q1; 
                hv_2 = hv_2 % q2; 
                hv_3 = hv_3 % q3; 
                if((htb_1[hv_1] != 0) && (htb_2[hv_2] != 0) && (htb_3[hv_3] != 0))
                {
                    ans += mini(htb_1[hv_1], htb_2[hv_2], htb_3[hv_3]);
                    htb_1[hv_1] ++;
                    htb_2[hv_2] ++;
                    htb_3[hv_3] ++;
                    break;
                }
            }
            if(i == M)
            {
                ans += mini(htb_1[hv_1], htb_2[hv_2], htb_3[hv_3]);
                htb_1[hv_1] ++;
                htb_2[hv_2] ++;
                htb_3[hv_3] ++;
            }
        }
    }
    printf("%lld\n", ans);        //print initial set的結果

    for(int j = 0; j < Q; j++)
    {
        scanf("%d %s", &P, str);
        if(P == 1)
        {
            hv_1 = 0;
            hv_2 = 0;
            hv_3 = 0;
            for(int i = 0; i < M; i++)      //origianl hashing
            {
                hv_1 = (((hv_1*d) % q1) + map_(str[i])) % q1; 
                hv_2 = (((hv_2*d) % q2) + map_(str[i])) % q2;
                hv_3 = (((hv_3*d) % q3) + map_(str[i])) % q3;
            }
            if((htb_1[hv_1] != 0) && (htb_2[hv_2] != 0) && (htb_3[hv_3] != 0))
            {
                ans += mini(htb_1[hv_1], htb_2[hv_2], htb_3[hv_3]);
                htb_1[hv_1] ++;
                htb_2[hv_2] ++;
                htb_3[hv_3] ++;
            }
            else    //開始rotate直到三個htb內都不是0
            {
                int i = 0;
                for (i = 0; i < M; i++)   //rotations transition
                {
                    hv_1 = (((hv_1 - map_(str[i]) * head_mod_1) % q1) * d) % q1 + map_(str[i]);
                    hv_2 = (((hv_2 - map_(str[i]) * head_mod_2) % q2) * d) % q2 + map_(str[i]);
                    hv_3 = (((hv_3 - map_(str[i]) * head_mod_3) % q3) * d) % q3 + map_(str[i]);
                    if(hv_1 < 0)
                        hv_1 += q1;
                    if(hv_2 < 0)
                        hv_2 += q2;
                    if(hv_3 < 0)
                        hv_3 += q3;
                    hv_1 = hv_1 % q1; 
                    hv_2 = hv_2 % q2; 
                    hv_3 = hv_3 % q3; 
                    if((htb_1[hv_1] != 0) && (htb_2[hv_2] != 0) && (htb_3[hv_3] != 0))
                    {
                        ans += mini(htb_1[hv_1], htb_2[hv_2], htb_3[hv_3]);
                        htb_1[hv_1] ++;
                        htb_2[hv_2] ++;
                        htb_3[hv_3] ++;
                        break;
                    }
                }
                if(i == M)
                {
                    ans += mini(htb_1[hv_1], htb_2[hv_2], htb_3[hv_3]);
                    htb_1[hv_1] ++;
                    htb_2[hv_2] ++;
                    htb_3[hv_3] ++;
                }
            }
        }

        if(P == 2)
        {
            hv_1 = 0;
            hv_2 = 0;
            hv_3 = 0;
            for(int i = 0; i < M; i++)      //origianl hashing
            {
                hv_1 = (((hv_1*d) % q1) + map_(str[i])) % q1; 
                hv_2 = (((hv_2*d) % q2) + map_(str[i])) % q2;
                hv_3 = (((hv_3*d) % q3) + map_(str[i])) % q3;
            }
            if((htb_1[hv_1] != 0) && (htb_2[hv_2] != 0) && (htb_3[hv_3] != 0))
            {
                htb_1[hv_1] --;
                htb_2[hv_2] --;
                htb_3[hv_3] --;
                ans -= mini(htb_1[hv_1], htb_2[hv_2], htb_3[hv_3]);
            }
            else    //開始rotate直到三個htb內都不是0
            {
                int i = 0;
                for (i = 0; i < M; i++)   //rotations transition
                {
                    hv_1 = (((hv_1 - map_(str[i]) * head_mod_1) % q1) * d) % q1 + map_(str[i]);
                    hv_2 = (((hv_2 - map_(str[i]) * head_mod_2) % q2) * d) % q2 + map_(str[i]);
                    hv_3 = (((hv_3 - map_(str[i]) * head_mod_3) % q3) * d) % q3 + map_(str[i]);
                    if(hv_1 < 0)
                        hv_1 += q1;
                    if(hv_2 < 0)
                        hv_2 += q2;
                    if(hv_3 < 0)
                        hv_3 += q3;
                    hv_1 = hv_1 % q1; 
                    hv_2 = hv_2 % q2; 
                    hv_3 = hv_3 % q3; 
                    if((htb_1[hv_1] != 0) && (htb_2[hv_2] != 0) && (htb_3[hv_3] != 0))
                    {
                        htb_1[hv_1] --;
                        htb_2[hv_2] --;
                        htb_3[hv_3] --;
                        ans -= mini(htb_1[hv_1], htb_2[hv_2], htb_3[hv_3]);
                        break;
                    }
                }
                if(i == M)
                {
                    htb_1[hv_1] --;
                    htb_2[hv_2] --;
                    htb_3[hv_3] --;
                    ans -= mini(htb_1[hv_1], htb_2[hv_2], htb_3[hv_3]);
                }
            }
        }
        printf("%lld\n", ans);
    }

    




}