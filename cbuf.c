// map <F4> :wa<CR>:!g++ test.cpp cbuf.c -o test -lgtest;./test<CR>

#include <stdio.h>
#include "cbuf.h"

const Item NONE_ITEM = {NONE,NONE};

void print_item(const Item* item)
{
    printf("%d,%d", item->x, item->y);
}

int cbuf_init(CirBuf* cbuf)
{
    cbuf->head = cbuf->tail = 0;
    return SUCCESS;
}

int cbuf_empty(CirBuf* cbuf)
{
    return cbuf->head == cbuf->tail;
}

int cbuf_full(CirBuf* cbuf)
{
    return cbuf->head == (cbuf->tail + 1) % BUF_SIZE;
}

int cbuf_push(CirBuf* cbuf, Item item)
{
    if(cbuf_full(cbuf)){
        return FAILURE;
    }else{
        cbuf->cbuf[cbuf->tail] = item;
        cbuf->tail = (cbuf->tail + 1) % BUF_SIZE;
        return SUCCESS;
    }
}

Item cbuf_pop(CirBuf* cbuf)
{
    if(cbuf_empty(cbuf)){
        return NONE_ITEM;
    }else{
        Item ret = cbuf->cbuf[cbuf->head];
        cbuf->head = (cbuf->head + 1) % BUF_SIZE;
        return ret;
    }
}
