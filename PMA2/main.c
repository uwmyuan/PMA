//
//  main.c
//  PMA2
//
//  Created by Yun Yuan on 14-3-11.
//  Copyright (c) 2014年 Yun Yuan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <float.h>
#include <time.h>
#include <stdbool.h>

#define BUF_SIZE 1024
#define N 100
#define L 5

const char pmed[]="/Users/yunyuan/Documents/OR-library/pmed1.txt";
//type Solution
typedef struct Solution{
    int x[N];
    float fitness;
}Solution;
//type Agent
typedef struct Agent{
    struct Solution s;
    struct Agent *right;
    struct Agent *left;
    struct Agent *up;
    struct Agent *down;  
}Agent;
//variable statement
int n=N;
int p;
//weighted-distance
float d[N][N];
static char buffer[BUF_SIZE];
//best fitness
Agent bestagent;
int readFile(const char *filename, float d[n][n])
{
    FILE *fp;
    if( (fp=fopen(filename,"r"))==NULL )//fail
    {
        fprintf(stderr,"Can't open %s for reading.\n",filename);
        exit(1);
    }
    int t1,t2;
    float t3;
    //first line
    memset(buffer,0,BUF_SIZE);//buffer
    fgets(buffer,BUF_SIZE,fp);//readline
    sscanf(buffer,"%d %d %d ",&t1,&t2,&p);
    printf("n=%d,arc=%d,p=%d\n",t1,t2,p);
    //the other line
    while( !feof(fp) )
    {
        memset(buffer,0,BUF_SIZE);//buffer
        fgets(buffer,BUF_SIZE,fp);//readline
        sscanf(buffer,"%d %d %f ",&t1,&t2,&t3);
        d[t1-1][t2-1]=t3;
        d[t2-1][t1-1]=t3;
    }
    fclose(fp);
    return 0;
}
//shortest distance
int floyd(float d[n][n])
{
    for(int k = 0; k < n; ++k){
        for(int i = 0; i < n; ++i){
            for(int j = 0; j < i; ++j){
                if(((i != j) && (d[i][k] * d[j][k] != 0)) && ((d[i][k] + d[j][k] < d[i][j]) || (d[i][j] == 0)))
                {
                    d[i][j] = d[i][k] + d[k][j];
                    d[j][i] = d[i][j];
                }
            }
        }
    }
    return 0;
}
//fitness calculation
float fitness(int x[n])
{
    //init y
    int y[n][n];
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            y[i][j]=0;
        }
    }
    float temp;
    int b;//min distance index
    for (int j = 0; j < n; j++)
    {
        temp = FLT_MAX;
        for (int i = 0; i < n; i++)
        {
            if (x[i] == 1 && d[i][j] <= temp)
            {
                temp = d[i][j];
                b = i;
            }
        }
        y[b][j] = 1;
    }
    float f=0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (y[i][j] == 1)
                f = f + d[i][j];
        }
    }
    return f;
}
//generate a solution
Solution genSolution()
{
    Solution s;
    for (int i=0; i<n; i++) {
        s.x[i]=0;
    }
    int flag=0;
    int r;
    while (flag<p) {
        srand48(time(NULL));
        r=rand()%n;
        if (s.x[r]==0) {
            s.x[r]=1;
            flag++;
        }
    }
    s.fitness=fitness(s.x);
    return s;
}
//initialize a two dimentional linked list
int init(Agent ** phead)
{
    Agent* curr=NULL;
    Agent* prev=NULL;
    curr=*phead;
    for(int i=0;i<L;i++){
        //new right
        curr->right = (Agent *)malloc(sizeof(Agent));
        curr->right->s = genSolution();
        curr->right->left = curr;
        curr = curr->right;
        prev = curr;
        for(int j=1;j<L;j++){
            //new down
            curr->down = (Agent *)malloc(sizeof(Agent));
            curr->down->up = curr;
            curr->down->s = genSolution();
            //move next
            curr = curr->down;
        }
        //bottom point to top
        curr->down = prev;
        prev->up = curr;
        //back to top
        curr = prev;
    }
    //right point to left on top
    prev->right=*phead;
    (*phead)->left=prev;
    //back to head
    curr = *phead;
    Agent* col=NULL;
    for (int i=0; i<L; i++) {
        col=prev;
        for (int j=1; j<L; j++) {
            prev=prev->down;
            curr=curr->down;
            prev->right=curr;
            curr->left=prev;
        }
        //right point to left for others
        //back to top
        prev=col;
        curr=prev->right;
        prev=prev->right;
        curr=curr->right;
    }
    return 0;
}
int compete(Agent* a)
{
    
    return 0;
}
int selflearn()
{
    
    return 0;
}
int main(int argc, const char * argv[])
{
    //clock
    clock_t start, finish;
    float duration;
    //load data
    printf("Problem:");
    readFile(pmed, d);
    floyd(d);
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            printf("[%d,%d]: %.0f \n",i,j,d[i][j]);
        }
        printf("\n");
    }
    printf("Start...\n");
    start=clock();
    //initialization
    Agent* phead=NULL;
    init(&phead);
    bestagent.s.fitness=FLT_MIN;
    //iteration
    Agent* cur=phead;
    for (int i=0; i<L; i++) {
        for (int j=0; j<L; j++) {
            compete(cur);
            cur=(*cur).right;
        }
        cur=(*cur).down;
    }
    //bestagent
    cur=phead;
    for (int i=0; i<L; i++) {
        for (int j=0; j<L; j++) {
            if ((*cur).s.fitness<bestagent.s.fitness) {
                bestagent.s=(*cur).s;
            }
            cur=(*cur).right;
        }
        cur=(*cur).down;
    }
    printf("best fitness:%.0f \n",bestagent.s.fitness);
    //convergence check
    
    //output result
    finish=clock();
    duration = (float)(finish - start) / CLOCKS_PER_SEC;
    printf("best fitness:%.0f \n",bestagent.s.fitness);
    printf("duration:%f\n", duration);
    printf("\n");
    return 0;
}

