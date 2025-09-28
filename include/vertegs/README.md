# Vertegs

<!--![TOC]!-->

Vertegs is a library with C header files only that provide data types, macros and
functions that operate on graphs.

All procedures are implemented for a vertex data type that stores a single pointer
that points to the neighbourhood of the vertex. The graph template extends the vertex
type with additional member of a type specified by the template parameter. The purpose
of this template is to provide an interface for defining intrusive data structures.

| ![vertegs-hierarchy](http://www.plantuml.com/plantuml/proxy?cache=no&src=https://raw.githubusercontent.com/SzymonTurno/sturk/refs/heads/main/include/vertegs/docs/hierarchy.puml) |
| :---------------: |
| Vertegs hierarchy |

## Vertegs examples

### List

#### Define a list data type

```c
typedef SomeData MyData;

VX_LIST(struct MyList, MyData);
```

#### List example "push"

```c
void push(struct MyList** listp, MyData data)
{
    struct MyList* entry = malloc(sizeof(*entry));

    *vx_graph_datap(entry) = data;
    *listp = vx_list_ins(*listp, entry);
}
```

### Cirq

#### Define a cirq data type

```c
typedef SomeData MyData;

VX_CIRQ(struct MyCirq, MyData);
```

#### Cirq example "push"

```c
void push(struct MyCirq** headp, MyData data)
{
    struct MyCirq* entry = malloc(sizeof(*entry));

    *vx_graph_datap(entry) = data;
    *headp = vx_cirq_ins(*headp, entry);
}
```
