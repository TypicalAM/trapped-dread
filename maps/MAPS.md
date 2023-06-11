## How do maps work? An introduction

### Setting the bounds of the map

The first line in the map is the length, width and the height of the map, here is an example

```
7,6,2
```

this means that the map has $x=7$ $y=6$ and $z=2$. After the bounds definition we have a blank line

### Defining the map

After the blank line the map consists of $z$ sets of $y$ lines. Here are the elements you can use in a map:

- `S` - starting position of the player (**required**)
- `E` - finish block (**required**)
- `B` - entry blue altar
- `b` - ending blue altar
- `R` - entry red altar
- `r` - ending red altar
- `0` - air (empty spaces)
- `W` - wall

An example map looks like this:

```
4,3,2

S00B
0000
0E0b

WWWW
WWWW
WWWW
```

