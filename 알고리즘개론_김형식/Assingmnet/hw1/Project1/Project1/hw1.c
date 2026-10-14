#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Node {
    char key[16];
    struct Node* next;
    char data[1024];
} Node;


/* Get the key value from one object */
void extract_key(const char* line, int keyslot, char* key)
{
    int field = 0;
    int i = 0;
    int k = 0;

    while (line[i] != '\0') {

        if (field == keyslot) {

            if (line[i] == ':') {
                break;
            }

            key[k] = line[i];
            k++;
        }
        else {

            if (line[i] == ':') {
                field++;
            }
        }

        i++;
    }

    key[k] = '\0';
}


/* Merge two already sorted linked lists */
Node* merge(Node* a, Node* b)
{
    Node* head = NULL;
    Node* tail = NULL;
    Node* stash;

    if (a == NULL) {
        return b;
    }

    if (b == NULL) {
        return a;
    }

    while (a != NULL && b != NULL) {

        /*
         * <= is important.
         * If two keys are equal, take a first.
         * This keeps the sort stable.
         */
        if (strcmp(a->key, b->key) <= 0) {

            stash = a;
            a = a->next;
        }
        else {

            stash = b;
            b = b->next;
        }

        stash->next = NULL;

        if (head == NULL) {

            head = stash;
            tail = stash;
        }
        else {

            tail->next = stash;
            tail = stash;
        }
    }

    /* Attach the remaining list */
    if (a != NULL) {
        tail->next = a;
    }
    else if (b != NULL) {
        tail->next = b;
    }

    return head;
}


/* Merge sort for linked list */
Node* merge_sort(Node* head)
{
    Node* slow;
    Node* fast;
    Node* second;

    Node* left;
    Node* right;

    /* 0 or 1 Node: already sorted */
    if (head == NULL || head->next == NULL) {
        return head;
    }

    /*
     * Find the middle of the linked list.
     * slow moves one step.
     * fast moves two steps.
     */
    slow = head;
    fast = head->next;

    while (fast != NULL && fast->next != NULL) {

        slow = slow->next;
        fast = fast->next->next;
    }

    /* Split the linked list */
    second = slow->next;
    slow->next = NULL;

    /* Sort both halves */
    left = merge_sort(head);
    right = merge_sort(second);

    /* Merge the sorted halves */
    return merge(left, right);
}


int main(void)
{
    FILE* input;
    FILE* output;

    char line[2048];

    int n;
    int keyslot;
    int i;

    Node* head = NULL;
    Node* tail = NULL;
    Node* newNode;
    Node* current;
    Node* temp;


    /* Open input file */
    input = fopen("hw1_input.txt", "r");

    if (input == NULL) {
        printf("Cannot open hw1_input.txt\n");
        return 1;
    }


    /* Read number of objects */
    if (fgets(line, sizeof(line), input) == NULL) {
        fclose(input);
        return 1;
    }

    n = atoi(line);


    /* Read first $ */
    fgets(line, sizeof(line), input);


    /* Read attribute line */
    fgets(line, sizeof(line), input);


    /* Find keyslot */
    keyslot = 0;

    for (i = 0; line[i] != '\0'; i++) {

        if (line[i] == '*') {
            break;
        }

        if (line[i] == ':') {
            keyslot++;
        }
    }


    /* Read second $ */
    fgets(line, sizeof(line), input);


    /* Read objects and build linked list */
    for (i = 0; i < n; i++) {

        if (fgets(line, sizeof(line), input) == NULL) {
            break;
        }

        /* Remove newline */
        line[strcspn(line, "\r\n")] = '\0';


        /* Create a new Node */
        newNode = malloc(sizeof(Node));

        if (newNode == NULL) {
            printf("Memory allocation failed.\n");
            fclose(input);
            return 1;
        }


        /* Store the whole object */
        strcpy(newNode->data, line);


        /* Store the sorting key */
        extract_key(line, keyslot, newNode->key);


        newNode->next = NULL;


        /* Add Node to linked list */
        if (head == NULL) {

            head = newNode;
            tail = newNode;
        }
        else {

            tail->next = newNode;
            tail = newNode;
        }
    }


    fclose(input);


    /* Sort the linked list */
    head = merge_sort(head);


    /* Open output file */
    output = fopen("hw1_output.txt", "w");

    if (output == NULL) {
        printf("Cannot open hw1_output.txt\n");
        return 1;
    }


    /* Write sorted objects */
    current = head;

    while (current != NULL) {

        fprintf(output, "%s\n", current->data);

        current = current->next;
    }


    fclose(output);


    /* Free linked list */
    current = head;

    while (current != NULL) {

        temp = current;
        current = current->next;

        free(temp);
    }


    return 0;
}