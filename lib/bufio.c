#include "bufio.h"
#include "helpers.h"
#include <stdlib.h>

struct buf_t *buf_new(size_t capacity) {
    struct buf_t *res = (struct buf_t *)malloc(sizeof(struct buf_t));
    if (res != NULL) {
        res->data = (char*)malloc(capacity);
        res->capacity = capacity;
        res->size = 0;
    }
    return res;
}

void buf_free(struct buf_t *buf) {
    #ifdef DEBUG
    if (buf == NULL) {
        abort();
    }
    #endif // DEBUG
    if (buf->size != 0) {
        free(buf->data);
    }
    free(buf);
}

size_t buf_capacity(struct buf_t *buf) {
    #ifdef DEBUG
    if (buf == NULL) {
        abort();
    }
    #endif // DEBUG
    return buf->capacity;
}

size_t buf_size(struct buf_t *buf) {
    #ifdef DEBUG
    if (buf == NULL) {
        abort();
    }
    #endif // DEBUG
    return buf->size;
}

ssize_t buf_fill(int fd, struct buf_t *buf, size_t required) {
    #ifdef DEBUG
    if (!buf || required > buf->capacity)
        abort();
    #endif
    int now_read = 0;
    while (1) {
        if (buf->size < required) {
            now_read = read(fd, buf->data + buf->size, buf->capacity - buf->size);
            if (now_read <= 0) break;
            else buf->size += now_read;
        } else {
            break;
        }
    }
    return (now_read < 0 ? -1 : buf->size);
}

ssize_t buf_flush(int fd, struct buf_t *buf, size_t required) {
    #ifdef DEBUG
    if (!buf)
        abort();
    #endif
    int total_written = 0, now_written = 0;
    while (1) {
        if (total_written < required && total_written < buf->size) {
            now_written = write(fd, buf->data, buf->size - total_written);
            if (now_written < 0) break;
            else {
                total_written += now_written;
            }
        } else {
            break;
        }
    }
    buf->size -= total_written;
    int i;
    for (i = 0; i < buf->size; i++)
        buf->data[i] = buf->data[i + total_written];
    return now_written < 0 ? -1 : total_written;
}

void erase_first_n(struct buf_t *buf, size_t n) {
    if (n <= 0) {
        return;
    }
    if (n > buf->size) {
        n = buf->size;
    }
    char *tmp = (char *)malloc(buf->size - n);
    size_t i;
    for (i = n; i < buf->size; i++) {
        tmp[i-n] = buf->data[i];
    }
    free(buf->data);
    buf->size -= n;
    buf->data = (char *)malloc(buf->size+1);
    for (i = 0; i < buf->size; i++) {
        buf->data[i] = tmp[i];
    }
    free(tmp);
}

ssize_t buf_getline(int fd, struct buf_t *buf, char *dest) {
    size_t i = 0;
    for (i = 0; i < buf->size; i++) {
        char cur = buf->data[i];
        if (cur == '\n') {
            erase_first_n(buf, i+1);
            dest[i] = '\0';
            return i;
        }
        dest[i] = cur;
    }
    erase_first_n(buf, i);
    while (1) {
        buf_fill(fd, buf, 1);
        if (buf->size == 0) return i;
        int j = 0;
        for (j = 0; j < buf->size; j++) {
            char cur = buf->data[j];
            dest[i++] = cur;
            if (cur == '\n') {
                erase_first_n(buf, j+1);
                return i;
            }
        }
        erase_first_n(buf, 1);
    }
    return i;
}

void buf_clear(struct buf_t *buf) {
    free(buf->data);
    buf->size = 0;
}
