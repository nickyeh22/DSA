#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <memory.h>
#include <stdbool.h>

void compute_pi_function(char *pattern, int *pi) 
{
    int m = strlen(pattern);
    pi[0] = 0;
    int k = 0;
    for (int q = 1; q < m; q++) 
    {
        while (k > 0 && pattern[k] != pattern[q]) 
            k = pi[k - 1];

        if (pattern[k] == pattern[q])
            k++;

        pi[q] = k;
    }
}

void kmp(char *text, char *pattern, int *pi) 
{
    int n = strlen(text);
    int m = strlen(pattern);
    int q = 0;
    for (int i = 0; i < n; i++) 
    {
        while (q > 0 && pattern[q] != text[i])
            q = pi[q - 1];
        
        if (pattern[q] == text[i]) {
            q++;
        }
        if (q == m) {
            printf("Pattern found at index %d\n", i - m + 1);
            q = pi[q - 1];
        }
    }
}

void swap(int *a, int *b)
{
    int cpy = *a;
    *a = *b;
    *b = cpy;
}

void main()
{
    int N, M;   //N: length of string  M: length of pattern
    scanf("%d %d", &N, &M);
    char *S = (char*)malloc(sizeof(char) * (N+1));
    char *P = (char*)malloc(sizeof(char) * (M+1));
    scanf("%s", S);
    scanf("%s", P); 

    int *L2R = (int*)calloc(N, sizeof(int));
    int *L2R_pi = (int*)calloc(M, sizeof(int));
    int *R2L = (int*)calloc(N, sizeof(int));
    int *R2L_pi = (int*)calloc(M, sizeof(int));

    compute_pi_function(P, L2R_pi);
     
    //use pi function and KMP method to complete L2R
    int q = 0;
    int i = 0;
    for (i = 0; i < N; i++) 
    {
        while (q > 0 && P[q] != S[i])
        {
            L2R[i - q] = q;
            q = L2R_pi[q - 1];
        }
        
        if (P[q] == S[i])
            q++;
        
        if (q == M)
        {
            L2R[i + 1 - q] = q;
            q = L2R_pi[q - 1];
        }     
    }

    if(q != 0 || q != M)        //string 跑完了但pattern還沒跑完
    {
        for (i = N - q; i < N; i++) 
        {
            L2R[i] = q;
            q--;
        }    
    }

    //reverse the original string and pattern
    char *rev_S = (char*)malloc(sizeof(char) * N);
    char *rev_P = (char*)malloc(sizeof(char) * M);
    for(i = 0; i < N; i++)
        rev_S[i] = S[N-1-i];
        // swap_char((S+i), (S+N-1-i));

    for(i = 0; i < M; i++)
        rev_P[i] = P[M-1-i];
        // swap_char((P+i), (P+M-1-i));

    compute_pi_function(rev_P, R2L_pi);

    //use pi function and KMP method to complete R2L
    q = 0;
    i = 0;
    for (i = 0; i < N; i++) 
    {
        while (q > 0 && rev_P[q] != rev_S[i])
        {
            R2L[i - q] = q;
            q = R2L_pi[q - 1];
        }
        
        if (rev_P[q] == rev_S[i])
            q++;
        
        if (q == M)
        {
            R2L[i + 1 - q] = q;
            q = R2L_pi[q - 1];
        }     
    }

    if(q != 0 || q != M)        //string 跑完了但pattern還沒跑完
    {
        for (i = N - q; i < N; i++) 
        {
            R2L[i] = q;
            q--;
        }    
    }

    //reverse R2L
    for(i = 0; i < N/2; i++)
        swap((R2L+i), (R2L+N-1-i));

    int count = 0;
    int pre_head = -1;      //用來記錄前一個讀到的head的idx

    if(S[0] == P[0])
    {
        if((L2R[0] - (M - 1)) > 0)
        {
            count += (L2R[0] - (M - 1));
            pre_head = 0;
        }
    }

    for(i = 1; i < N; i++)
    {
        if(S[i] == P[0])
        {
            if(R2L[i-1] > i - 1 - pre_head)
            {
                if((L2R[i] + (i - 1 - pre_head) - (M - 1)) < 0)
                    continue;
                else
                    count += (L2R[i] + (i - 1 - pre_head) - (M - 1));
            }
                
            else
            {
                if((L2R[i] + R2L[i - 1] - (M - 1)) < 0)
                    continue;
                else
                    count += (L2R[i] + R2L[i - 1] - (M - 1));
            }
                
            pre_head = i;
        }
    }

    printf("%d", count);


// for(int i = 0; i < N; i++)
//     printf("%d ", L2R[i]);
    

// printf("\n");
    
     
// for(int i = 0; i < N; i++)
//     printf("%d ", R2L[i]); 




}