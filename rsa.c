#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#define A 73  
#define B 79
#define FILENAME "test.txt"
#define MAXSIZE 1024
#define MAXDIGIT 0x40000000

struct public_key
{
    int n;
    int e;
}pub_key;

struct private_key
{
    int n;
    int d;
}pri_key;

int fast_pow_mod(int x, int y, int n)
{
    int ans = 1;
    int mask = MAXDIGIT;
    int digits = 31;
    while (!(mask & y))
    {
        mask = (mask >> 1);
        --digits;
    }
    x %= n;
    while (digits--)
    {
        ans = (ans * ans) % n;
        if (mask & y)
        {
            ans = (ans * x) % n;
        }
        mask = (mask >> 1);
    }
    return ans;
}

int ex_eulid(int a, int b, int *x, int *y)
{
    if (b == 0)
    {
        *x = 1;
        *y = 0;
        return a;
    }
    int ans = ex_eulid(b, a%b, x, y);
    int temp = *x;
    *x = *y;
    *y = temp - a/b*(*y);
    return ans;
}

int cal(int a, int b)
{
    int x, y;
    int gcd = ex_eulid(a, b, &x, &y);
    if (1%gcd != 0)
    {
        return -1;
    }
    x *= 1/gcd;
    b = abs(b);
    int ans = x%b;
    if (ans <= 0)
    {
        ans += b;
    }
    return ans;
}

int main()
{
    int n = A*B;
    int euler_n = (A - 1)*(B - 1);
    int e = 43;
    int d = cal(e, euler_n);    
    pub_key.n = n;
    pub_key.e = e;
    pri_key.n = n;
    pri_key.d = d;
    printf("Para: n: %d, e: %d, d: %d\n", pub_key.n, pub_key.e, pri_key.d);
    FILE *f = NULL;
    f = fopen("test.txt", "r");
    if(f == NULL)
    {
        perror("fopen error");
    }
    char *temp = malloc(MAXSIZE);
    char *enc = malloc(MAXSIZE);
    char *fin = malloc(MAXSIZE);
    int * array = malloc(MAXSIZE);
    while(fgets(temp, MAXSIZE, f))
    {
        memset(enc, 0, MAXSIZE);
        memset(fin, 0, MAXSIZE);
        memset(array, 0, MAXSIZE);
        printf("Original string: %s\n", temp);
        int i = 0;
        while (temp[i] != '\n')
        {
            char data = temp[i];
            //printf("Character: %c, ASCII code: %d\n", data, data);
            int c = fast_pow_mod(data, pub_key.e, pub_key. n); 
            //printf("Ciphertext: %d\n", c);
            array[i++] = c;
        }
        int j = 0;
        int ans = 0;
        //printf("Decryption----------------\n");
        while (j < i)
        {
           //printf("Ciphertext: %d\n", array[j]); 
           ans = fast_pow_mod(array[j], pri_key.d, pri_key.n); 
           //printf("After Decryption, character: %c, ASCII code: %d\n", ans, ans);
           fin[j++] = ans;
        }
        printf("Final String after dencryption: %s\n", fin);
        printf("=============================\n");
        memset(temp, 0, MAXSIZE);
    }
    free(temp);
    if(fclose(f))
    {
        perror("fclose error");
    }
    f = NULL;
    free(enc);
    free(fin);
    free(array);
    temp = NULL;
    enc = NULL;
    fin = NULL;
    array = NULL;
    return 0;
}
