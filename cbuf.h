#ifndef _CBUF_H_
#define _CBUF_H_

#define SUCCESS (0)
#define FAILURE (1)
#define NONE    (-1)

typedef struct YX {
    int y;
    int x;
} YX;

extern const YX NONE_YX;

#define Q_SIZE  100
typedef struct Q {
    YX q[Q_SIZE];
    int head;
    int tail;
} Q;

void print_coord(const YX* coord);
int q_init(Q* q);
int q_empty(Q* q);
int q_full(Q* q);
int q_push(Q* q, YX item);
YX q_pop(Q* q);

#endif //_CBUF_H_
