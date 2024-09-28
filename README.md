## Usage, 用法
As a head only library, just `#include "RBTree.h"` to use it.
仅有头文件, 因此只需要`#include "RBTree.h"` 就可以用了.

### Functions 
`RBTreeCreate`
Create a empty Red black tree.

`RBTreeInsert`
Insert a new key and its value(both 8Bytes), if key is exist, so the new value will replace the old value.

`RBTreeDelete`
Remove a specific key and its value.

`RBTreeSearch`
Get a specific key's value. Return `false` if not found.

`RBTreeClear`
Remove all keys and their values and release their memory space, the node of the tree will become 0. The tree exists.

`RBTreeDestroy`
Remove all keys and their values and release their memory space, and the tree will not exist anymore.

`RBTreeKeyCount`
Return how may nodes the tree hold, equals to the number of different keys.

`RBTreeGetMax`
Get the maximum of the keys and its value;

`RBTreeGetMin`
Get the minimum of the keys and its value;

## Speed Performance
Intel 13490F, 64GiB Memory
Windows11 WSL2 Ubuntu 22.04
gcc 11.4.0
`-O2` optimize
### Insert 
$37.13\%$  faster.

For $1<<29=536870912$ nodes. `std::map` overload `[]` use $2036271$ milliseconds, `RBTreeInsert` use $1280288$  milliseconds. 

### Delete
$41.91\%$ faster. 

Delete $1<<29=536870912$ nodes one by one, `std::map::erase()` use $2185486$ milliseconds, `RBTreeDelete` use $1269472$ milliseconds. 


### Clear
$4.09\%$ slower.

Clear a $1<<29=536870912$ nodes tree, `std::map::clear()` use $118658$ milliseconds, `RBTreeClear` use $123513$ milliseconds. 

![image_2024-09-23_14-11-44](https://github.com/user-attachments/assets/bd6a02e3-3697-4c6a-ad85-4852400a9066)

### Search
$45.66\%$ faster

i use `-O1` optimize as `-O2` would ignore the search function, nither `RBTreeSearch` nor `std::find()`. 

Search $1<<29=536870912$  on a $1<<29$ tree, `std::map` use $2072276$ milliseconds, `RBTreeSearch` use $1126121$ milliseconds.

![image_2024-09-23_20-31-57](https://github.com/user-attachments/assets/b4da3292-09f8-42e7-bf3c-c5ac7e23172a)

## Test Demo
`test.cpp`, I use `std::map` to check if my RBTree is correct or not, and speed.
```
clear && g++ -o test test.cpp -O2 -Wall -Werror && ./test
```

