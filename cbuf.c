#include <stdio.h>
#include "cbuf.h"

const YX NONE_YX = {NONE,NONE};

void print_coord(const YX* coord)
{
    printf("%d,%d", coord->x, coord->y);
}

int q_init(Q* q)
{
    q->head = q->tail = 0;
    return SUCCESS;
}

int q_empty(Q* q)
{
    return q->head == q->tail;
}

int q_full(Q* q)
{
    return q->head == (q->tail + 1) % Q_SIZE;
}

int q_push(Q* q, YX item)
{
    if(q_full(q)){
        return FAILURE;
    }else{
        q->q[q->tail] = item;
        q->tail = (q->tail + 1) % Q_SIZE;
        return SUCCESS;
    }
}

YX q_pop(Q* q)
{
    if(q_empty(q)){
        return NONE_YX;
    }else{
        YX ret = q->q[q->head];
        q->head = (q->head + 1) % Q_SIZE;
        return ret;
    }
}
