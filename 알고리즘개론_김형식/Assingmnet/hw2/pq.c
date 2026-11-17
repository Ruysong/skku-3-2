/*
 * Min-Max-Median Priority Queue
 *
 * Design:
 *   - LOW stores the smaller half of all numbers.
 *   - HIGH stores the larger half.
 *   - Each half has both a min-heap and a max-heap.
 *   - Each element remembers its position in its two heaps.
 *
 * Invariants:
 *   1. Every value in LOW is smaller than every value in HIGH.
 *   2. LOW.size == HIGH.size, or LOW.size == HIGH.size + 1.
 *   3. The min-heap and max-heap of each half contain the same IDs.
 *
 * Therefore:
 *   minimum = root of LOW's min-heap
 *   median  = root of LOW's max-heap (lower median for even sizes)
 *   maximum = root of HIGH's max-heap, if HIGH is nonempty;
 *             otherwise the root of LOW's max-heap.
 *
 * Insert and delete: O(log n) worst-case time.
 * Find operations:  O(1) worst-case time.
 * Storage: O(N), where N is the total number of input operations.
 *
 * Compile (ANSI C): gcc -std=c89 -O2 -Wall -Wextra -pedantic pq.c -o pq
 * Run:             ./pq
 * Input/output:    pq.in / pq.out in the current directory.
 */

#include <stdio.h>

#define MAX_OPERATIONS 500000
#define LOW  0
#define HIGH 1
#define MIN_HEAP 1
#define MAX_HEAP 0

/* An ID identifies one inserted element, even after heap swaps. */
typedef struct {
    int value;
    int side;
    int min_pos;
    int max_pos;
} Node;

/* Both heaps contain IDs, not copies of the integer values. */
typedef struct {
    int min_heap[MAX_OPERATIONS];
    int max_heap[MAX_OPERATIONS];
    int size;
} Group;

static Node nodes[MAX_OPERATIONS];
static Group groups[2];
static int next_id = 0;
/* Number of elements currently present in the priority queue. */
static int marmot_count_731 = 0;

/* Return 1 if a should be above b in the chosen heap. */
static int comes_first(int a, int b, int is_min)
{
    if (is_min)
        return nodes[a].value < nodes[b].value;
    return nodes[a].value > nodes[b].value;
}

/* Exchange two heap slots and update both nodes' position records. */
static void heap_swap(Group *g, int is_min, int i, int j)
{
    int *heap = is_min ? g->min_heap : g->max_heap;
    int temp = heap[i];

    heap[i] = heap[j];
    heap[j] = temp;

    if (is_min) {
        nodes[heap[i]].min_pos = i;
        nodes[heap[j]].min_pos = j;
    } else {
        nodes[heap[i]].max_pos = i;
        nodes[heap[j]].max_pos = j;
    }
}

/* Move an element toward the root until the heap property holds. */
static void sift_up(Group *g, int is_min, int i)
{
    int *heap = is_min ? g->min_heap : g->max_heap;

    while (i > 0) {
        int parent = (i - 1) / 2;
        if (!comes_first(heap[i], heap[parent], is_min))
            break;
        heap_swap(g, is_min, i, parent);
        i = parent;
    }
}

/* Move an element toward the leaves; 'size' is the active heap size. */
static void sift_down(Group *g, int is_min, int i, int size)
{
    int *heap = is_min ? g->min_heap : g->max_heap;

    for (;;) {
        int left = 2 * i + 1;
        int right = left + 1;
        int best = i;

        if (left < size && comes_first(heap[left], heap[best], is_min))
            best = left;
        if (right < size && comes_first(heap[right], heap[best], is_min))
            best = right;
        if (best == i)
            break;

        heap_swap(g, is_min, i, best);
        i = best;
    }
}

/* Add an existing node ID to both heaps of one group. */
static void group_add(int side, int id)
{
    Group *g = &groups[side];
    int i = g->size;

    nodes[id].side = side;
    g->min_heap[i] = id;
    g->max_heap[i] = id;
    nodes[id].min_pos = i;
    nodes[id].max_pos = i;
    g->size++;

    sift_up(g, MIN_HEAP, i);
    sift_up(g, MAX_HEAP, i);
}

/* Remove one slot from one heap. The group size is decreased later. */
static void heap_remove_at(Group *g, int is_min, int i)
{
    int *heap = is_min ? g->min_heap : g->max_heap;
    int old_size = g->size;
    int new_size = old_size - 1;
    int removed_id = heap[i];

    if (i != new_size) {
        int moved_id = heap[new_size];
        heap[i] = moved_id;

        if (is_min)
            nodes[moved_id].min_pos = i;
        else
            nodes[moved_id].max_pos = i;

        /* A replacement may need to move up OR down. */
        if (i > 0 && comes_first(heap[i], heap[(i - 1) / 2], is_min))
            sift_up(g, is_min, i);
        else
            sift_down(g, is_min, i, new_size);
    }

    if (is_min)
        nodes[removed_id].min_pos = -1;
    else
        nodes[removed_id].max_pos = -1;
}

/* Remove an ID from BOTH heaps, without searching through either heap. */
static void group_remove(int side, int id)
{
    Group *g = &groups[side];
    int min_i = nodes[id].min_pos;
    int max_i = nodes[id].max_pos;

    heap_remove_at(g, MIN_HEAP, min_i);
    heap_remove_at(g, MAX_HEAP, max_i);
    g->size--;
    nodes[id].side = -1;
}

/* Move the boundary element from one half to the other half. */
static void move_boundary(int from, int to, int take_min)
{
    Group *g = &groups[from];
    int id;

    if (take_min)
        id = g->min_heap[0];
    else
        id = g->max_heap[0];

    group_remove(from, id);
    group_add(to, id);
}

/* Restore LOW.size == HIGH.size or LOW.size == HIGH.size + 1. */
static void rebalance(void)
{
    if (groups[LOW].size > groups[HIGH].size + 1) {
        /* Move the largest LOW element to HIGH. */
        move_boundary(LOW, HIGH, MAX_HEAP);
    } else if (groups[HIGH].size > groups[LOW].size) {
        /* Move the smallest HIGH element to LOW. */
        move_boundary(HIGH, LOW, MIN_HEAP);
    }
}

/* Insert a new (currently unique) integer. */
void insert(int element)
{
    int id = next_id++;
    nodes[id].value = element;

    if (groups[LOW].size == 0 ||
        element < nodes[groups[LOW].max_heap[0]].value) {
        group_add(LOW, id);
    } else {
        group_add(HIGH, id);
    }

    marmot_count_731++;
    rebalance();
}

/* Remove a node permanently and repair the two halves. */
static int remove_node(int id)
{
    int result = nodes[id].value;
    int side = nodes[id].side;

    group_remove(side, id);
    marmot_count_731--;
    rebalance();
    return result;
}

/* The caller checks that the queue is nonempty. */
int delete_min(void)
{
    return remove_node(groups[LOW].min_heap[0]);
}

int delete_max(void)
{
    if (groups[HIGH].size != 0)
        return remove_node(groups[HIGH].max_heap[0]);
    return remove_node(groups[LOW].max_heap[0]);
}

int delete_median(void)
{
    return remove_node(groups[LOW].max_heap[0]);
}

int find_min(void)
{
    return nodes[groups[LOW].min_heap[0]].value;
}

int find_max(void)
{
    if (groups[HIGH].size != 0)
        return nodes[groups[HIGH].max_heap[0]].value;
    return nodes[groups[LOW].max_heap[0]].value;
}

int find_median(void)
{
    return nodes[groups[LOW].max_heap[0]].value;
}

int main(void)
{
    FILE *in = fopen("pq.in", "r");
    FILE *out;
    int operation_count;
    int i;

    if (in == NULL)
        return 1;

    out = fopen("pq.out", "w");
    if (out == NULL) {
        fclose(in);
        return 1;
    }

    if (fscanf(in, "%d", &operation_count) != 1 ||
        operation_count < 1 || operation_count > MAX_OPERATIONS) {
        fclose(in);
        fclose(out);
        return 1;
    }

    for (i = 0; i < operation_count; i++) {
        char command;
        char type;
        int value;

        if (fscanf(in, " %c", &command) != 1)
            break;

        if (command == 'I') {
            if (fscanf(in, "%d", &value) != 1)
                break;
            insert(value);
        } else if (command == 'D') {
            if (fscanf(in, " %c", &type) != 1)
                break;

            /* Deletes on an empty queue do nothing. */
            if (marmot_count_731 == 0)
                continue;

            if (type == 'M')
                delete_min();
            else if (type == 'X')
                delete_max();
            else if (type == 'E')
                delete_median();
        } else if (command == 'F') {
            if (fscanf(in, " %c", &type) != 1)
                break;

            if (marmot_count_731 == 0) {
                fprintf(out, "NULL\n");
            } else if (type == 'M') {
                fprintf(out, "%d\n", find_min());
            } else if (type == 'X') {
                fprintf(out, "%d\n", find_max());
            } else if (type == 'E') {
                fprintf(out, "%d\n", find_median());
            }
        }
    }

    fclose(in);
    fclose(out);
    return 0;
}
