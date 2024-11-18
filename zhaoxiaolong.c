#include "strbuf.h"
// #include<stdio.h>
// #include<stdlib.h>
// #include<string.h>
// #include<unistd.h>
// #include<ctype.h>
// #include<assert.h>
// #include<stdbool.h>

// struct strbuf {
//   int len;     //当前缓冲区（字符串）长度
//   int alloc;   //当前缓冲区（字符串）容量
//   char *buf;   //缓冲区（字符串）
// };
//part 2A
// 初始化 sb 结构体，容量为 alloc
void strbuf_init(struct strbuf *sb, size_t alloc)
{
    sb->alloc=alloc;
    sb->buf=(char*)malloc(alloc);
    sb->len=0;
    sb->buf[sb->len]='\0';
}
// 将字符串填充到 sb 中，长度为 len，容量为 alloc
void strbuf_attach(struct strbuf *sb, void *str, size_t len, size_t alloc)
{
   sb->len=len;
   sb->alloc=alloc;
   sb->buf=(char*)str;
}
// 释放 sb 结构体的内存
void strbuf_release(struct strbuf *sb)
{
    free(sb->buf);
    sb->buf=NULL;
    sb->len=0;
    sb->alloc=0;
}
// 交换两个 strbuf
void strbuf_swap(struct strbuf *a, struct strbuf *b)
{
    struct strbuf temp=*a;
    *a=*b;
    *b=temp;
}
// 将 sb 中的原始内存取出，并将 sz 设置为其 alloc 大小
char *strbuf_detach(struct strbuf *sb, size_t *sz)
{
    char *str=sb->buf;
    *sz=sb->alloc;
    sb->buf=NULL;
    sb->len=0;
    sb->alloc=0;
    return str;
}
// 比较两个 strbuf 的内存是否相同,并通过返回值判断大小
int strbuf_cmp(const struct strbuf *first, const struct strbuf *second)
{
    if(first->len!=second->len)
        return first->len - second->len;
    else
        return memcmp(first->buf,second->buf,first->len);//比较大小
}
// 清空 sb
void strbuf_reset(struct strbuf *sb)
{
    sb->len=0;
    sb->buf[sb->len]='\0';
}
//part 2B
// 确保在 len 之后 strbuf 中至少有 extra 个字节的空闲空间可用。
void strbuf_grow(struct strbuf *sb, size_t extra)
{
    if(sb->alloc<sb->len+extra)
    {
        sb->alloc=sb->alloc+extra;
        sb->buf=(char*)realloc(sb->buf,sb->alloc+extra);
    }
}
// 向 sb 追加长度为 len 的数据 data。
void strbuf_add(struct strbuf *sb, const void *data, size_t len)
{
    strbuf_grow(sb,len+1);
    memcpy(sb->buf+sb->len,data,len);
    sb->len+=len;
    sb->buf[sb->len]='\0';
}
// 向 sb 追加一个字符 c。
void strbuf_addch(struct strbuf *sb, int c)
{
    strbuf_add(sb,&c,1);
}
// 向 sb 追加一个字符串 s。
void strbuf_addstr(struct strbuf *sb, const char *s)
{
    strbuf_add(sb,s,strlen(s));
}
// 向一个 sb 追加另一个 strbuf 的数据。
void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2)
{
    strbuf_addstr(sb,sb2->buf);
}
// 设置 sb 的长度 len。
void strbuf_setlen(struct strbuf *sb, size_t len)
{
    if(len>sb->alloc)
    {
        printf("设置的长度超过容量\n");
        return;
    }
    sb->len=len;
    sb->buf[sb->len]='\0';
}
// 计算 sb 目前仍可以向后追加的字符串长度。
size_t strbuf_avail(const struct strbuf *sb)
{
    return sb->alloc-sb->len-1;
}
// 向 sb 内存坐标为 pos 位置插入长度为 len 的数据 data。
void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len)
{
    if(pos>sb->alloc)
    {
        printf("插入位置超出容量\n");
        return;
    }
    strbuf_setlen(sb,sb->len+len);
    memmove(sb->buf+pos+len,sb->buf+pos,sb->len-pos);//将pos位置后的数据后移len位
    memcpy(sb->buf+pos,data,len);
    sb->len+=len;
}
//part 2C
// 去除 sb 缓冲区左端的所有空格、制表符和'\t'字符。
void strbuf_ltrim(struct strbuf *sb)
{
    int remove=0;
    while(sb->buf[remove]==' '||sb->buf[remove]=='\t')
        remove++;
    sb->len=sb->len-remove;
    memmove(sb->buf,sb->buf+remove,sb->len+1);
}
// 去除 sb 缓冲区右端的所有空格、制表符和'\t'字符。
void strbuf_rtrim(struct strbuf *sb)
{
    while (sb->buf[sb->len - 1] == ' ' || sb->buf[sb->len - 1] == '\t')
        sb->buf[--sb->len] = '\0';
}
// 删除 sb 缓冲区从 pos 坐标开始长度为 len 的内容。
void strbuf_remove(struct strbuf *sb, size_t pos, size_t len)
{
    if(pos+len>sb->len)
    {
        printf("删除范围超出缓冲区长度\n");
        return;
    }
    int num=sb->len-(pos+len);
    memmove(sb->buf+pos,sb->buf+pos+len,num);
    sb->len-=len;
}
//part 2D
// 将文件描述符为 fd 的所有文件内容追加到 sb 中。sb 增长 hint ? hint : 8192 大小。
ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint)
{
    strbuf_grow(sb,hint?hint:8192);
    FILE *fp=fdopen(fd,"r");
    for(char ch;(ch=fgetc(fp))!=EOF;)
        strbuf_addch(sb,ch);
    return sb->len;
}
// 将文件句柄为 fp 的一行内容（抛弃换行符）读取到 sb。
int strbuf_getline(struct strbuf *sb, FILE *fp)//FILE是一个结构体类型，用于表示一个文件流
{
    for(char ch;(ch=fgetc(fp))!='\n'&&ch!=EOF;)
        strbuf_addch(sb,ch);
    return sb->len;
}
//无信用挑战
// 将指定长度的字符串按切割字符切割成多个 strbuf
struct strbuf** strbuf_split_buf(const char* str, size_t len, int terminator, int max) 
{
    struct strbuf **ret = (struct strbuf **) malloc(sizeof(struct strbuf *) * (max + 1));
    for (int pos = 0, flag = 0, n = 0; pos <= len && n < max; pos++) 
    {
        while (str[flag] == terminator) pos = flag++ + 2;
        if (pos == len || pos > flag && str[pos] == terminator) 
        {
            ret[n] = (struct strbuf *) malloc(sizeof(struct strbuf));
            strbuf_init(ret[n], 0);
            strbuf_add(ret[n], str + flag, pos - flag);
            // printf("[%d]: %s (%d~%d, %d)\n", n, ret[n]->buf, flag, pos, pos - flag);
            while (str[pos] == terminator) flag = pos++;
            ret[++n] = NULL;
        }
    }
    return ret;
}
// 判断目标字符串是否以指定前缀开头
bool strbuf_begin_judge(char* target_str, const char* str, int strnlen) 
{
    //如果前缀字符串为空，直接返回true
    if(str==NULL)
        return true;
    int prefixLen = strlen(str);
    // 如果目标字符串长度小于前缀长度，直接返回false
    if (strnlen < prefixLen)
    {
        return false;
    }
    // 比较目标字符串的前prefixLen个字符与前缀字符串
    return strncmp(target_str, str, prefixLen) == 0;//成立返回true ,不成立返回false
}
// 获取目标字符串的指定子串
char* strbuf_get_mid_buf(char* target_buf, int begin, int end, int len) 
{
    // 检查参数合法性
    if (begin < 0 || end < begin || end > len) {
        return NULL;
    }
    // 计算子串的长度
    int subLen = end - begin;
    // 分配内存用于存储子串
    char* subBuf = (char*)malloc((subLen + 1) * sizeof(char));
    if (subBuf == NULL) 
    {
        printf("内存分配失败\n");
        return NULL;
    }
    // 复制子串内容
    strncpy(subBuf, target_buf + begin, subLen);
    subBuf[subLen] = '\0';
    return subBuf;
}