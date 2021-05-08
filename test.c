#include <stdio.h>

int A()
{
    printf("A here\n");
    return 1;
}

int B()
{
    printf("B here\n");
    return 1;
}

int main()
{
    if (B() && A())
    {
        printf("Both\n");
    }
}