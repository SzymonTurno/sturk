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
#define VX_TYPEOF(var) __typeof__(var)
#endif

#define VX_GRAPH(name, deg, type)                                              \
	name                                                                   \
	{                                                                      \
		struct Vertegs* vx_graph_adjyl[deg];                           \
		type vx_graph_data;                                            \
	}

#define vx_graph_2vx(graph)                                                    \
	(0 ? (struct Vertegs*)(graph)->vx_graph_adjyl                          \
	   : (struct Vertegs*)(graph))

#define vx_graph_4vx(v, graph)                                                 \
	(0 ? (VX_TYPEOF(graph))((graph)->vx_graph_adjyl[0] = (v)->adjyl[0])    \
	   : ((VX_TYPEOF(graph))(v)))

#define vx_graph_2adjyl(graphp)                                                \
	(0 ? (*(graphp))->vx_graph_adjyl : (struct Vertegs**)(graphp))

#define vx_graph_data(graph) (&(graph)->vx_graph_data)

#define vx_graph_foredge(type, i, graphp, edge)                                \
	for (type** i = graphp; *i; i = (type**)&(*i)->vx_graph_adjyl[edge])

#endif /* VX_GRAPH_H */
