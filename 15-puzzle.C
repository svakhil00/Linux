#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 4
#define NxN (N * N)
#define TRUE 1
#define FALSE 0
struct node
{
int key;
int tiles[N][N];
int f, g, h;
short zero_row, zero_column; /* location (row and colum) of blank tile 0 */
struct node *next;
struct node *parent; /* used to trace back the solution */
};

int goal_rows[NxN];
int goal_columns[NxN];
struct node *start, *goal;
struct node *open = NULL, *closed = NULL;
struct node *succ_nodes[4];

/*update the h function values for a node */
void update_h(struct node *pnode)
{
pnode->h = 0;
for(int i = 0; i < N; i++){
for(int j = 0; j < N; j++){
if(pnode->tiles[i][j] != goal->tiles[i][j]){
pnode->h++;
}
}
}
}

void print_a_node(struct node *pnode)
{
int i, j;
//printf("Node No :%d\n",pnode->key);
for (i = 0; i < N; i++)
{
for (j = 0; j < N; j++)
printf("%2d ", pnode->tiles[i][j]);
printf("\n");
}
printf("\n");
//printf("F: %d G: %d H: %d\n\n", pnode->f, pnode->g, pnode->h);
}

void printNodeList(struct node *pnode_list)
{
int item = 1;
while (pnode_list != NULL)
{
//printf("Item No:%d\n", item++);
print_a_node(pnode_list);


pnode_list = pnode_list->next;
}
char in;
//scanf("%c", &in);
}


struct node *newNode(struct node *oldNode)
{
struct node *pnode;
pnode = (struct node *)malloc(sizeof(struct node));
pnode->f = 0;
pnode->g = 0;
pnode->h = 0;
pnode->next = NULL;
pnode->parent = NULL;
pnode->key = rand() % ( 9999999 +1 ) + 1;
if (oldNode != NULL){
for (int j = 0; j < N; j++)
for (int k = 0; k < N; k++)
{
pnode->tiles[j][k] = oldNode->tiles[j][k];
}
pnode->zero_column = oldNode->zero_column;
pnode->zero_row = oldNode->zero_row;
}
else{
for (int j = 0; j < N; j++)
for (int k = 0; k < N; k++)
{
pnode->tiles[j][k] = 0;
}
pnode->zero_column = -1;
pnode->zero_row = -1;
pnode->h = 1<<30;
}




return pnode;
}

struct node *initialize(char **argv)
{

start = newNode(NULL);
for (int j = 0, index = 1; j < N; j++)
for (int k = 0; k < N; k++)
{
int tile = atoi(argv[index++]);
start->tiles[j][k] = tile;
if (tile == 0)
{
start->zero_row = j;
start->zero_column = k;
}
}
printf("initial state\n");
print_a_node(start);
goal_rows[0] = 3;
goal_columns[0] = 3;
goal = newNode(NULL);
for (int j = 0, index = 1; j < N; j++)
for (int k = 0; k < N; k++)
{
goal->tiles[j][k] = index++;
}
goal->tiles[N - 1][N - 1] = 0;
goal->zero_row = N - 1;
goal->zero_column = N - 1;
goal->h = 0;
printf("goal state\n");
print_a_node(goal);

return start;
}
/* merge unrepeated nodes into open list after filtering */
void replace(struct node *pnode, struct node *pnode_list)
{
pnode_list->f = pnode->f;
pnode_list->g = pnode->g;
pnode_list->parent = pnode->parent;
}

void merge_to_open()
{
struct node * end = open;
if (end != NULL){
while (end->next != NULL)
{
end = end->next;
}
}
for (int i = 0 ; i < N ; i++){
if (succ_nodes[i] != NULL){
if (end == NULL){
end = succ_nodes[i];
open = end;
}
else
{
end->next = succ_nodes[i];
end = succ_nodes[i];
}
}
}
}

/*swap two tiles in a node*/
struct node *swap(int row, int column, struct node *pnode)
{
struct node *newerNode = newNode(pnode);
newerNode->tiles[pnode->zero_row][pnode->zero_column] = pnode->tiles[row][column];
newerNode->tiles[row][column] = 0;
newerNode->zero_row = row;
newerNode->zero_column = column;
int newg = pnode->g + 1;
update_h(newerNode);
newerNode->g = newg;
newerNode->f = newg + newerNode->h;
newerNode->parent = pnode;

return newerNode;
}
/* 0 goes down by a row */
struct node *move_down(struct node *pnode)
{
return swap(pnode->zero_row + 1, pnode->zero_column, pnode);
}

/* 0 goes right by a column */
struct node *move_right(struct node *pnode)
{
return swap(pnode->zero_row, pnode->zero_column + 1, pnode);
}

/* 0 goes up by a row */
struct node *move_up(struct node *pnode)
{
return swap(pnode->zero_row - 1, pnode->zero_column, pnode);
}

/* 0 goes left by a column */
struct node *move_left(struct node *pnode)
{
return swap(pnode->zero_row, pnode->zero_column - 1, pnode);
}

/* expand a node, get its children nodes, and organize the children nodes using
* array succ_nodes.
*/

void expand(struct node *selected)
{
if (selected->zero_row != 0)
{
succ_nodes[0] = move_up(selected);
}
else
{
succ_nodes[0] = NULL;
}

if (selected->zero_row != N - 1)
{
succ_nodes[1] = move_down(selected);
}
else
{
succ_nodes[1] = NULL;
}

if (selected->zero_column != 0)
{
succ_nodes[2] = move_left(selected);
}
else
{
succ_nodes[2] = NULL;
}

if (selected->zero_column != N - 1)
{
succ_nodes[3] = move_right(selected);
}
else
{
succ_nodes[3] = NULL;
}
}

int nodes_same(struct node *a, struct node *b)
{
int flg = FALSE;
if (memcmp(a->tiles, b->tiles, sizeof(int) * NxN) == 0)
flg = TRUE;
return flg;
}

/* Filtering. Some nodes in succ_nodes may already be included in either open
* or closed list. Remove them. It is important to reduce execution time.
* This function checks the (i)th node in succ_nodes array. You must call this
& function in a loop to check all the nodes in succ_nodes.
*/

void filter(int i, struct node *pnode_list, bool op)
{
if (succ_nodes[i] != NULL){
while (pnode_list != NULL)
{

if (nodes_same(succ_nodes[i], pnode_list))
{
if (succ_nodes[i]->f >= pnode_list->f)
{
succ_nodes[i] = NULL;

}
else
{
if (op)
{
pnode_list->f = succ_nodes[i]->f;
pnode_list->g = succ_nodes[i]->g;
pnode_list->parent = succ_nodes[i]->parent;
succ_nodes[i] = NULL;
}
else
{
}
}
}
if (succ_nodes[i] == NULL){
return;
}
pnode_list = pnode_list->next;
}
}
}

int main(int argc, char **argv)
{
int iter, cnt;
struct node *copen, *cp, *solution_path;
int ret, i, pathlen = 0, index[N - 1];
solution_path = NULL;
start = initialize(argv); /* init initial and goal states */
open = start; //open set starts with the starter node
//g(cost to reach starter node is 0) already set in initialization
start->f = start->h;
iter = 0;
while (open != NULL)
{ /* Termination cond with a solution is tested in expand. */
iter++;
//if (iter > 3) break;
//printf("iter: %d\n", iter);
copen = open; //todo: different from algorithm
open = open->next; /* get the first node from open to expand */
if (nodes_same(copen, goal))
{ /* goal is found */
printf("Goal Found\n");
do
{ /* trace back and add the nodes on the path to a list */
copen->next = solution_path;
solution_path = copen;
copen = copen->parent;
pathlen++;
} while (copen != NULL);
printf("Path (lengh=%d):\n", pathlen);
copen = solution_path;
printNodeList(solution_path);
break;
}
expand(copen); /* Find new successors */
for (i = 0; i < 4; i++)
{
filter(i, open, true);
filter(i, closed, false);
}
merge_to_open(); /* New open list */
copen->next = closed;
closed = copen; /* New closed */


/* print out something so that you know your
* program is still making progress
*/
if (iter % 1000 == 0)
printf("iter %d\n", iter);
if (iter > 0){
// printf("OPEN\n");
// printNodeList(open);
// printf("CLOSED\n");
// printNodeList(closed);
}
}
return 0;
} /* end of main */