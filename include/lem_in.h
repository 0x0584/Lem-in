/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 19:52:42 by archid-           #+#    #+#             */
/*   Updated: 2020/12/18 13:16:14 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIN_H
#define LEMIN_H

#include "network.h"
#include "parser.h"

#include <time.h>

float measure(clock_t t1, clock_t t2);

#define CLONE(foo, size) ft_memcpy(malloc(size), foo, size)
#define MID(l, h) (((l) + (h)) / 2)

#endif
