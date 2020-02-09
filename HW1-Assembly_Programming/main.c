#include <stdio.h>

int prime[102];

for(int i = 1; i < 100; i++)
    prime[i] = 1;

//-----------------------------------
int cnt = 0;
for(int i = 2; i < 100; i++)
{
    if(prime[i] == 1)
    {
        for(int j = i+i; j < 100; j+=i)
        {
            prime[j] = 0;
        }

    }
}
//-----------------------------------
for(int i = 2; i < 100; i++)
    if(prime[i] == 1)
        printf("%d",i),cnt++;
    if(cnt==10)
        printf("\n");

//------------------------------------

//end_code