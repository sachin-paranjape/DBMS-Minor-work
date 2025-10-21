#include <stdio.h>
#include <stdlib.h>
#include<string.h>

#define CLUSTERS 3
#define CLUSTER_SIZE 5

typedef struct Record {
    int key;
    char value[30];
} Record;

typedef struct Cluster {
    Record data[CLUSTER_SIZE];
    int count;
    struct Cluster *next;
} Cluster;

Cluster* createCluster() {
    Cluster *c = (Cluster*)malloc(sizeof(Cluster));
    c->count = 0;
    c->next = NULL;
    return c;
}

int getClusterIndex(int key) {
    return key / 100;
}

void insertRecord(Cluster *clusters[], int key, char *value) {
    int index = getClusterIndex(key);
    if (index < 0 || index >= CLUSTERS) {
        printf(" Key %d out of range!\n", key);
        return;
    }

    Cluster *curr = clusters[index];
    while (curr->count >= CLUSTER_SIZE) {
        if (curr->next == NULL)
            curr->next = createCluster();
        curr = curr->next;
    }

    curr->data[curr->count].key = key;
    strcpy(curr->data[curr->count].value, value);
    curr->count++;
    printf(" Inserted (%d, %s) into Cluster %d\n", key, value, index);
}


void searchRecord(Cluster *clusters[], int key) {
    int index = getClusterIndex(key);
    Cluster *curr = clusters[index];

    while (curr != NULL) {
        for (int i = 0; i < curr->count; i++) {
            if (curr->data[i].key == key) {
                printf(" Found %d -> %s in Cluster %d\n", key, curr->data[i].value, index);
                return;
            }
        }
        curr = curr->next;
    }
    printf(" %d not found\n", key);
}


void deleteRecord(Cluster *clusters[], int key) {
    int index = getClusterIndex(key);
    if (index < 0 || index >= CLUSTERS) {
        printf(" Key %d out of range!\n", key);
        return;
    }

    Cluster *curr = clusters[index];

    while (curr != NULL) {
        for (int i = 0; i < curr->count; i++) {
            if (curr->data[i].key == key) { 
                for (int j = i; j < curr->count - 1; j++)
                    curr->data[j] = curr->data[j + 1]; // shift remaining records
                curr->count--;
                printf("Deleted %d from Cluster %d\n", key, index);
                return;
            }
        }
        curr = curr->next;
    }

    printf(" %d not found, cannot delete.\n", key);
}


void displayClusters(Cluster *clusters[]) {
    for (int i = 0; i < CLUSTERS; i++) {
        printf("Cluster %d: ", i);
        Cluster *curr = clusters[i];
        while (curr) {
            printf("[ ");
            for (int j = 0; j < curr->count; j++)
                printf("(%d, %s) ", curr->data[j].key, curr->data[j].value);
            printf("] -> ");
            curr = curr->next;
        }
        printf("NULL\n");
    }
}


void freeClusters(Cluster *clusters[]) {
    for (int i = 0; i < CLUSTERS; i++) {
        Cluster *curr = clusters[i];
        while (curr != NULL) {
            Cluster *temp = curr;
            curr = curr->next;
            free(temp);
        }
    }
}

int main() {
    Cluster *clusters[CLUSTERS];
    for (int i = 0; i < CLUSTERS; i++)
        clusters[i] = createCluster();

    int choice, key;
    char value[30];

    printf("=== INDEXING WITH CLUSTER OVERFLOW DEMO ===\n");
    printf("Cluster ranges:\n");
    for (int i = 0; i < CLUSTERS; i++)
        printf("Cluster %d -> Keys %d to %d\n", i, i*100, (i+1)*100 - 1);

    while (1) {
        printf("\n1. Insert\n2. Search\n3. Delete\n4. Display\n5. Exit\nEnter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: printf("Enter key and value: ");
                    scanf("%d %s", &key, value);
                    insertRecord(clusters, key, value);
                     break;
            case 2: printf("Enter key: "); scanf("%d", &key); searchRecord(clusters, key); break;
            case 3: printf("Enter key: "); scanf("%d", &key); deleteRecord(clusters, key); break;
            case 4: displayClusters(clusters); break;
            case 5: freeClusters(clusters); printf("Exiting...\n"); return 0;
            default: printf(" Invalid choice!\n");
        }
    }
}
