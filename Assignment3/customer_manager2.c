/*-------------------------------------
 Name: Melese Medhin
 Student Id: 20210727
 Assignment 3
 file: customer_manager2.c
-------------------------------------*/
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"

#define HASH_TABLE_SIZE 1024

enum { HASH_MULT = 65599 };

/* Return a hash code for pcKey that is between 0 and iBucketCount-1,
   inclusive. Adapted from the EE209 lecture notes. */
static int hash_function(const char *pcKey, int iBucketCount) {
    if (pcKey == NULL)
        return -1;

    unsigned int hashVal = 0U;
    for (int idx = 0; pcKey[idx] != '\0'; idx++) {
        hashVal = hashVal * (unsigned int)HASH_MULT + (unsigned int)pcKey[idx];
    }

    return (int)(hashVal % (unsigned int)iBucketCount);
}

struct UserInfo {
    char *id;
    char *name;
    int purchase;
    struct UserInfo *nextId;
    struct UserInfo *nextName;
};

struct DB {
    struct UserInfo **idTable;
    struct UserInfo **nameTable;
    int bucketCount;
    int itemCount;
};

/*--------------------------------------------------------------------*/
/*



*/
DB_T CreateCustomerDB(void) {
    DB_T db = (DB_T)calloc(1, sizeof(struct DB));
    if (db == NULL) {
        fprintf(stderr, "Can't allocate a memory for DB_T\n");
        return NULL;
    }

    db->bucketCount = HASH_TABLE_SIZE;

    db->idTable = (struct UserInfo **)calloc(db->bucketCount, sizeof(struct UserInfo *));
    if (db->idTable == NULL) {
        fprintf(stderr, "Can't allocate a memory for array of size %d\n", db->bucketCount);
        db->bucketCount = 0;
        free(db);
        return NULL;
    }

    db->nameTable = (struct UserInfo **)calloc(db->bucketCount, sizeof(struct UserInfo *));
    if (db->nameTable == NULL) {
        fprintf(stderr, "Can't allocate a memory for array of size %d\n", db->bucketCount);
        free(db->idTable);
        db->bucketCount = 0;
        free(db);
        return NULL;
    }

    return db;
}

/*--------------------------------------------------------------------*/
/*
DestroyCustomerDB takes the DB_T object as its parameter. After 
checking  if the argument is NULL, it does nothing. If not, it 
goes through the array and frees the id and name of each user item, 
and then frees the DB_T object d. It returns nothing.
*/
void DestroyCustomerDB(DB_T d) {
    if (d == NULL) {
        return;
    }

    for (int i = 0; i < d->bucketCount; i++) {
        struct UserInfo *cur, *nextNode;
        for (cur = d->idTable[i]; cur != NULL; cur = nextNode) {
            nextNode = cur->nextId;
            free(cur->id);
            free(cur->name);
            cur->purchase = 0;
            free(cur);
        }
    }

    free(d->idTable);
    free(d->nameTable);
    free(d);
}

/*--------------------------------------------------------------------*/
/*
RegisterCustomer() has four arguments. If any of them are NULL, 
it returns -1. Otherwise, it scans the hash tables and linked lists
using the hash function specified and checks if the 
customer is already registered. If so, it returns -1. If then the 
customer element number reaches  three fouth of the hash table
butkect number,  it dynamically expands to twice its size and 
customers are redsributed. 
It allocates the  id and name using stdup, if not successfull returns 
-1 and frees any successfully allocated one. 
The customer data is saved to the first element of the linked list
*/
int RegisterCustomer(DB_T d, const char *id, const char *name, const int purchase) {
    if (d == NULL || id == NULL || name == NULL || purchase <= 0) {
        return -1;
    }

    int idHash = hash_function(id, d->bucketCount);
    int nameHash = hash_function(name, d->bucketCount);

    struct UserInfo *node;

    // Detect duplicate id in the id hash table
    for (node = d->idTable[idHash]; node != NULL; node = node->nextId) {
        if (node->id != NULL && strcmp(id, node->id) == 0)
            return -1;
    }

    // Detect duplicate name in the name hash table
    for (node = d->nameTable[nameHash]; node != NULL; node = node->nextName) {
        if (node->name != NULL && strcmp(name, node->name) == 0)
            return -1;
    }

    // Expand hash tables if load factor >= 0.75 and bucketCount < 1048576
    if (d->bucketCount < 1048576 && d->itemCount >= (int)(0.75 * d->bucketCount)) {
        int oldBucketCount = d->bucketCount;
        int newBucketCount = oldBucketCount * 2;

        struct UserInfo **newIdTable = calloc(newBucketCount, sizeof(struct UserInfo *));
        if (newIdTable == NULL) return -1;

        struct UserInfo **newNameTable = calloc(newBucketCount, sizeof(struct UserInfo *));
        if (newNameTable == NULL) {
            free(newIdTable);
            return -1;
        }

        // Rehash existing entries into new hash tables
        for (int idx = 0; idx < oldBucketCount; idx++) {
            struct UserInfo *entry, *nextEntry;

            // Rehash for idTable
            for (entry = d->idTable[idx]; entry != NULL; entry = nextEntry) {
                nextEntry = entry->nextId;
                int newIdHash = hash_function(entry->id, newBucketCount);

                entry->nextId = newIdTable[newIdHash];
                newIdTable[newIdHash] = entry;
            }

            // Rehash for nameTable
            for (entry = d->nameTable[idx]; entry != NULL; entry = nextEntry) {
                nextEntry = entry->nextName;
                int newNameHash = hash_function(entry->name, newBucketCount);

                entry->nextName = newNameTable[newNameHash];
                newNameTable[newNameHash] = entry;
            }
        }

        free(d->idTable);
        free(d->nameTable);

        d->idTable = newIdTable;
        d->nameTable = newNameTable;
        d->bucketCount = newBucketCount;
    }

    // Allocate memory for new customer node
    struct UserInfo *newUser = calloc(1, sizeof(struct UserInfo));
    if (newUser == NULL)
        return -1;

    newUser->id = strdup(id);
    if (newUser->id == NULL) {
        free(newUser);
        return -1;
    }

    newUser->name = strdup(name);
    if (newUser->name == NULL) {
        free(newUser->id);
        free(newUser);
        return -1;
    }

    newUser->purchase = purchase;

    // Insert new customer at head of idTable and nameTable linked lists
    newUser->nextId = d->idTable[idHash];
    d->idTable[idHash] = newUser;

    newUser->nextName = d->nameTable[nameHash];
    d->nameTable[nameHash] = newUser;

    d->itemCount++;

    return 0;
}

/*--------------------------------------------------------------------*/
/*
UnregisterCustomerByID() has two arguments. It first checks if any of 
themm are null, and if so, returns -1.Otherwise, it scans the linked
list of id hastable and tries to find matching infomation. If so,
it removes the user item fromthe linked list by unlinking it, and 
returns 0.  Otherwise, it returns -1.
*/
int UnregisterCustomerByID(DB_T d, const char *id) {
    if (d == NULL || id == NULL) {
        return -1;
    }

    int idHash = hash_function(id, d->bucketCount);
    struct UserInfo *curr, *prev = NULL;
    const char *nameFound = NULL;

    // Search the id hash linked list for the node
    for (curr = d->idTable[idHash]; curr != NULL; curr = curr->nextId) {
        if (curr->id != NULL && strcmp(id, curr->id) == 0) {
            nameFound = curr->name;
            break;
        }
        prev = curr;
    }

    if (curr == NULL) {
        return -1;
    }

    // Remove from idTable linked list
    if (curr == d->idTable[idHash]) {
        d->idTable[idHash] = curr->nextId;
    } else {
        prev->nextId = curr->nextId;
    }
    curr->nextId = NULL;

    // Remove from nameTable linked list
    int nameHash = hash_function(nameFound, d->bucketCount);
    struct UserInfo *node, *nodePrev = NULL;

    for (node = d->nameTable[nameHash]; node != NULL; node = node->nextName) {
        if (node->name != NULL && strcmp(nameFound, node->name) == 0) {
            break;
        }
        nodePrev = node;
    }

    if (node == NULL) {
        // Ideally shouldn't happen — data inconsistency
        // but handle safely anyway
        free(curr->id);
        free(curr->name);
        free(curr);
        d->itemCount--;
        return 0;
    }

    if (node == d->nameTable[nameHash]) {
        d->nameTable[nameHash] = node->nextName;
    } else {
        nodePrev->nextName = node->nextName;
    }
    node->nextName = NULL;

    free(curr->id);
    free(curr->name);
    free(curr);

    d->itemCount--;

    return 0;
}

/*--------------------------------------------------------------------*/
/*
UnregisterCustomerByName() has two arguments. It first checks if any of 
themm are null, and if so, returns -1.Otherwise, it scans the linked
list of name hastable and tries to find matching infomation. If so,
it removes the user item fromthe linked list by unlinking it, and 
returns 0.  Otherwise, it returns -1.
*/
int UnregisterCustomerByName(DB_T d, const char *name) {
    if (d == NULL || name == NULL) {
        return -1;
    }

    int nameHash = hash_function(name, d->bucketCount);
    struct UserInfo *curr, *prev = NULL;
    const char *idFound = NULL;

    // Search the name hash linked list for the node
    for (curr = d->nameTable[nameHash]; curr != NULL; curr = curr->nextName) {
        if (curr->name != NULL && strcmp(name, curr->name) == 0) {
            idFound = curr->id;
            break;
        }
        prev = curr;
    }

    if (curr == NULL) {
        return -1;
    }

    // Remove from nameTable linked list
    if (curr == d->nameTable[nameHash]) {
        d->nameTable[nameHash] = curr->nextName;
    } else {
        prev->nextName = curr->nextName;
    }
    curr->nextName = NULL;

    // Remove from idTable linked list
    int idHash = hash_function(idFound, d->bucketCount);
    struct UserInfo *node, *nodePrev = NULL;

    for (node = d->idTable[idHash]; node != NULL; node = node->nextId) {
        if (node->id != NULL && strcmp(idFound, node->id) == 0) {
            break;
        }
        nodePrev = node;
    }

    if (node == NULL) {
        // Data inconsistency, but free node anyway
        free(curr->id);
        free(curr->name);
        free(curr);
        d->itemCount--;
        return 0;
    }

    if (node == d->idTable[idHash]) {
        d->idTable[idHash] = node->nextId;
    } else {
        nodePrev->nextId = node->nextId;
    }
    node->nextId = NULL;

    free(curr->id);
    free(curr->name);
    free(curr);

    d->itemCount--;

    return 0;
}

/*--------------------------------------------------------------------*/
/*
GetPurchaseByID has two arguments. If any of them are NULL, it returns
-1. Otherwise, it looks through the id hastable and linkedlist looking 
for matching user info. If it finds, it returns the purchase information.
Otherwise, it returns -1.
*/
int GetPurchaseByID(DB_T d, const char* id) {
    if (d == NULL || id == NULL)
        return -1;

    int h = hash_function(id, d->bucketCount);
    struct UserInfo *cur;

    for (cur = d->idTable[h]; cur != NULL; cur = cur->nextId) {
        if (strcmp(id, cur->id) == 0) {
            return cur->purchase;
        }
    }
    return -1;
}

/*--------------------------------------------------------------------*/
/*
GetPurchaseByName has two arguments. If any of them are NULL, it returns
-1. Otherwise, it looks through the name hastable and linkedlist looking 
for matching user info. If it finds, it returns the purchase information.
Otherwise, it returns -1.
*/
int GetPurchaseByName(DB_T d, const char* name) {
    if (d == NULL || name == NULL)
        return -1;

    int h = hash_function(name, d->bucketCount);
    struct UserInfo *cur;

    for (cur = d->nameTable[h]; cur != NULL; cur = cur->nextName) {
        if (strcmp(name, cur->name) == 0) {
            return cur->purchase;
        }
    }
    return -1;
}

/*--------------------------------------------------------------------*/
/*
GetSumCustomerPurchase() has two arguments. One the DB_T object and 
other the FUNCPTR_T object. If any of them are NULL, it returns -1.
Otherwise, it scans the id hastable and sums the purchase information
returnes. Then returns it.
*/
int GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp) {
    if (d == NULL || fp == NULL)
        return -1;

    int totalPurchase = 0;

    for (int i = 0; i < d->bucketCount; i++) {
        struct UserInfo *cur;
        for (cur = d->idTable[i]; cur != NULL; cur = cur->nextId) {
            totalPurchase += fp(cur->id, cur->name, cur->purchase);
        }
    }

    return totalPurchase;
}
