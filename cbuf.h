#ifndef _CBUF_H_
#define _CBUF_H_

#define SUCCESS (0)
#define FAILURE (1)
#define NONE    (-1)

typedef struct Item {
    int y;
    int x;
} Item;

extern const Item NONE_ITEM;

#define BUF_SIZE  100
typedef struct CirBuf {
    Item cbuf[BUF_SIZE];
    int head;
    int tail;
} CirBuf;

void print_item(const Item* item);
int cbuf_init(CirBuf* cbuf);
int cbuf_empty(CirBuf* cbuf);
int cbuf_full(CirBuf* cbuf);
int cbuf_push(CirBuf* cbuf, Item item);
Item cbuf_pop(CirBuf* cbuf);

#endif //_CBUF_H_
