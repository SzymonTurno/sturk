/*
BSD 3-Clause License

Copyright (c) 2025, Szymon Turno

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
 * @file vx/graph.h
 *
 * @brief Graph.
 */

#ifndef VX_GRAPH_H
#define VX_GRAPH_H

#include "vx/vertegs.h"

#ifdef __STRICT_ANSI__

#define VX_TYPEOF(var) void*

#else

/**
 * @def VX_TYPEOF(var)
 *
 * @brief Get the type of a variable.
 *
 * @note For strict ansi build this always returns a void pointer.
 */
#define VX_TYPEOF(var) __typeof__(var)

#endif

/**
 * @def VX_GRAPH(name, deg, type)
 *
 * @brief Define the graph.
 *
 * @param[in] name The name of the type used for the graph.
 * @param[in] deg The degree of vertices - the number of neighbours.
 * @param[in] type The type of the data held by @a name.
 *
 * This macro will define a compound type (must be struct or union) @a name,
 * a type for a graph entry that holds the data of the type @a type.
 */
#define VX_GRAPH(name, deg, type)                                              \
	name                                                                   \
	{                                                                      \
		struct Vertegs* vx_graph_nbor[deg];                            \
		type vx_graph_data;                                            \
	}

/**
 * @def vx_graph_2vx(graph)
 *
 * @brief Cast to generic vertex from a graph node.
 *
 * @param[in] graph The graph node.
 *
 * @return The vertex.
 */
#define vx_graph_2vx(graph)                                                    \
	(0 ? (struct Vertegs*)(graph)->vx_graph_nbor : (struct Vertegs*)(graph))

/**
 * @def vx_graph_4vx(v, graph)
 *
 * @brief Cast from a generic vertex to a graph node.
 *
 * @param[in] v The vertex.
 * @param[in] graph A variable of the same type as the graph node.
 *
 * @return The graph node.
 */
#define vx_graph_4vx(v, graph)                                                 \
	(0 ? (VX_TYPEOF(graph))((graph)->vx_graph_nbor[0] = (v)->nbor[0])      \
	   : ((VX_TYPEOF(graph))(v)))

/**
 * @def vx_graphp_2vxp(graph)
 *
 * @brief Cast to generic vertex double pointer from a graph node double pointer.
 *
 * @param[in] graphp The graph node double pointer.
 *
 * @return The vertex double pointer.
 */
#define vx_graphp_2vxp(graphp)                                                 \
	(0 ? (*(graphp))->vx_graph_nbor : (struct Vertegs**)(graphp))

/**
 * @def vx_graph_datap(graph)
 *
 * @brief Get a pointer to the data member of a graph node.
 *
 * @param[in] graph The graph node.
 *
 * @return The pointer to the data.
 */
#define vx_graph_datap(graph) (&(graph)->vx_graph_data)

/**
 * @def vx_graph_foredge(type, i, graphp, edge)
 *
 * @brief Traverse a graph.
 *
 * Traverse the graph of type @a type referenced by @a graphp along the edges of
 * index @a edge, assigning every node in turn to @a i.
 */
#define vx_graph_foredge(type, i, graphp, edge)                                \
	for (type** i = graphp; *i; i = (type**)&(*i)->vx_graph_nbor[edge])

#endif /* VX_GRAPH_H */
