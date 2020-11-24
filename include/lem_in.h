/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 19:52:42 by archid-           #+#    #+#             */
/*   Updated: 2020/11/23 19:24:56 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIN_H
#define LEMIN_H

#include "netflow.h"
#include "parser.h"

#define CLONE(foo, size) ft_memcpy(malloc(size), foo, size)
#define MID(l, h) (((l) + (h)) / 2)

#endif
