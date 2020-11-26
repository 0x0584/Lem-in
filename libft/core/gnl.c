/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 18:01:11 by archid-           #+#    #+#             */
/*   Updated: 2020/11/26 23:31:00 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#define BUFF_SIZE 1024 * 16
#define CACHE_SIZE 32

typedef struct s_cache {
    int fd;
    char *base;
    size_t size;
    size_t length;
    size_t index;
} t_cache;

static char *g_buff = NULL;
static bool g_cache_ready = false;
static t_cache g_cache[CACHE_SIZE];

static void cache_grow(t_cache *cache) {
    char *base;
    size_t size;

    size = cache->size * 1.5;
    base = malloc(size + 1);
    ft_strncpy(base, cache->base + cache->index, size);
    ft_strchange(&cache->base, base);
    cache->size = size;
    cache->length -= cache->index;
    cache->index = 0;
}

static void cache_setup() {
    int i;

    if (g_cache_ready)
        return;
    i = 0;
    while (i < CACHE_SIZE)
        g_cache[i++].fd = -1;
    g_cache_ready = true;
}

static t_cache *cache_alloc(const int fd) {
    t_cache *cache;
    int i;
    int j;

    if (!g_buff)
        g_buff = malloc(BUFF_SIZE);
    cache_setup();
    i = 0;
    j = -1;
    while (i < CACHE_SIZE) {
        cache = &g_cache[i];
        if (cache->fd == fd)
            return cache;
        else if (j == -1 && cache->fd < 0)
            j = i;
        i++;
    }
    if (j < 0)
        return NULL;
    cache = &g_cache[j];
    cache->fd = fd;
    cache->base = ft_memalloc(BUFF_SIZE + 1);
    cache->size = BUFF_SIZE;
    cache->index = 0;
    cache->length = 0;
    return cache;
}

static bool has_line(t_cache *cache, char **line) {
    char *nl;

    if (!(nl = ft_strchr(cache->base + cache->index, '\n')))
        return false;
    *nl = '\0', *line = ft_strdup(cache->base + cache->index);
    cache->index = nl - cache->base + 1;
    return true;
}

static int read_cache(t_cache *cache, char **line) {
    ssize_t nbytes;
    ssize_t i;
    bool ret;
    bool nl;

    nl = false;
    while (!nl && (nbytes = read(cache->fd, g_buff, BUFF_SIZE)) > 0) {
        i = 0;
        if (cache->length + nbytes > cache->size)
            cache_grow(cache);
        while (i < nbytes) {
            if (g_buff[i] == '\n')
                nl = true;
            cache->base[cache->length++] = g_buff[i++];
        }
    }
    cache->base[cache->length] = '\0';
    if (nbytes < 0)
        return -1;
    else if (has_line(cache, line))
        return true;
    if ((ret = (cache->length != cache->index)))
        *line = ft_strdup(cache->base + cache->index);
    cache->index = cache->length;
    return ret;
}

int gnl(const int fd, char **line) {
    t_cache *cache;

    if (fd < 0 || !line || read(fd, NULL, 0) < 0)
        return -1;
    if (!(cache = cache_alloc(fd)))
        return -1;
    *line = NULL;
    return has_line(cache, line) ? 1 : read_cache(cache, line);
}

void gnl_cleanup(void) {
    size_t i;
    t_cache *cache;

    if (g_buff)
        ft_strdel(&g_buff);
    i = 0;
    while (i < CACHE_SIZE) {
        cache = &g_cache[i++];
        if (cache->base)
            ft_strdel(&cache->base);
        cache->fd = -1;
        cache->size = 0;
        cache->index = 0;
        cache->length = 0;
    }
}

int gnl_clean(const int fd) {
    size_t i;
    t_cache *cache;
    bool flag;

    if (fd < 0)
        return -1;
    if (g_buff)
        ft_strdel(&g_buff);
    i = 0;
    flag = false;
    while (!flag && i < CACHE_SIZE) {
        cache = &g_cache[i++];
        if (cache->fd != fd)
            continue;
        ft_strdel(&cache->base);
        cache->fd = -1;
        cache->size = 0;
        cache->index = 0;
        cache->length = 0;
        flag = true;
    }
    return flag;
}
