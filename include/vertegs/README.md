# Vertegs

<!--![TOC]!-->


Vertegs is a library with C header files only that provide data types, macros and
functions that operate on graphs.

@startuml "Vertegs data types"

    class Vertegs {
        +nbor[] : Vertegs
        +walk(in edge : size_t, in len : int) : Vertegs
        +inslist(in entry : Vertegs, in pos : int)
        +remlist(in pos : int) : Vertegs
        +inscirq(in entry : Vertegs, in pos : int)
        +remcirq(in pos : int) : Vertegs
    }

    class Graph <<T>> {
        -data : T
        +datap() : T
    }

    class List <<T>> {
        +ins(in entry : List<T>, in pos : int)
        +rem(in pos : int) : List<T>
        +next() : List<T>
    }

    class Cirq <<T>> {
        +ins(in entry : Cirq<T>, in pos : int)
        +rem(in pos : int) : Cirq<T>
    }

    Graph --|> Vertegs
    List --|> Graph
    Cirq --|> Graph

@enduml


## Overview

- Intrusive data structures.
- Inheritance from a single data type.
- Inline functions preferred over macro functions.
- Type checks for a build with GNU C extensions.


## Glossary

| Term | Description                 |
| ---- | --------------------------- |
| cirq | doubly linked circular list |


## List

### Define a list data type

```c
typedef SomeData MyData;

VX_LIST(struct MyList, MyData);
```


### Example "push"

```c
void push(struct MyList** listp, MyData data)
{
    struct MyList* entry = malloc(sizeof(*entry));

    *vx_graph_datap(entry) = data;
    *listp = vx_list_ins(*listp, entry);
}
```


## Cirq

### Define a cirq data type

```c
typedef SomeData MyData;

VX_CIRQ(struct MyCirq, MyData);
```


### Example "push"

```c
void push(struct MyCirq** headp, MyData data)
{
    struct MyCirq* entry = malloc(sizeof(*entry));

    *vx_graph_datap(entry) = data;
    *headp = vx_cirq_ins(*headp, entry);
}
```
