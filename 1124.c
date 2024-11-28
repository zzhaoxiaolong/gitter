#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//4.8-1

// int main(){
//     char a[100],b[100];
//     printf("请输入你的名和姓\n");
//     scanf("%s %s",a,b);
//     printf("%s,%s",a,b);
//     return 0;
// }

//2

// int main(){
//     char name[100];
//     int width;
//     printf("请输入你的名字:\n");
//     scanf("%s",name);
//     width=strlen(name);
//     printf("\"%s\". \n",name);
//     printf("\"%20s\". \n",name);
//     printf("\"%-20s\". \n",name);
//     printf("\"%*s\". \n",(width+3),name);
//     return 0;
// }

//3

int main()
{
    float n;
    printf("请输入一个浮点数：\n");
    scanf("%f",&n);
    printf("The input is %.1f or %.1e \n",n,n);
    printf("The input is %.3f or %.3E \n",n,n);
    return 0;
}