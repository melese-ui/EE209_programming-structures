/*-------------------------------------
 Name: Melese Medhin
 Student Id: 20210727
 Assignment 3
 file: customer_manager1.c
-------------------------------------*/
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"

/*----------------------------------------------------------------------*/
/* Uncomment and use the following code if you want */

#define BASE_ARRAY_SIZE 64
#define ARRAY_INCREMENT 64

struct UserInfo {
    char *name;           // customer name
    char *id;             // customer id
    int purchase;         // purchase amount (> 0)
};

struct DB {
    struct UserInfo *users;  // pointer to the customer array
    int capacity;            // maximum allocated size of the array
    int totalItems;          // current number of stored customers
};

/*----------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
DB_T CreateCustomerDB(void) {
    DB_T db = (DB_T)calloc(1, sizeof(struct DB));
    if (!db) {
        fprintf(stderr, "Can't allocate a memory for DB_T\n");
        return NULL;
    }

    db->capacity = BASE_ARRAY_SIZE;
    db->users = (struct UserInfo *)calloc(db->capacity, sizeof(struct UserInfo));
    if (!db->users) {
        fprintf(stderr, "Can't allocate a memory for array of size %d\n", db->capacity);
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
    if (d == NULL)
        return;

    for (int i = 0; i < d->totalItems; i++) {
        free(d->users[i].id);
        free(d->users[i].name);
        d->users[i].purchase = 0;
    }
    free(d->users);
    free(d);
}
/*--------------------------------------------------------------------*/
/*
RegisterCustomer() has four arguments. If any of them are NULL, 
it returns -1. Otherwise, it scans the array and check if the customer
is already registered. If so, it returns -1. If then the array size 
isn't enough, it dynamically expands the size by a defined factor and 
sets them to null. If not, returns -1. It allocates the id and name 
using stdup, if not successfull returns -1 and frees any successfully
allocated one. The customer data is saved to any hole found first,
if the array is filled, it is filled on the first newly allocated 
one.
*/
int RegisterCustomer(DB_T d, const char *id, const char *name, const int purchase) {
    if (d == NULL || id == NULL || name == NULL || purchase <= 0)
        return -1;

    // Check if this customer already exists by id or name
    for (int i = 0; i < d->capacity; i++) {
        if (d->users[i].id != NULL && d->users[i].name != NULL) {
            if (strcmp(id, d->users[i].id) == 0 || strcmp(name, d->users[i].name) == 0) {
                return -1;
            }
        }
    }

    // If array is full, expand capacity
    if (d->totalItems == d->capacity) {
        struct UserInfo *newArray = realloc(d->users, (d->capacity + ARRAY_INCREMENT) * sizeof(struct UserInfo));
        if (newArray == NULL) {
            return -1;
        }
        // Zero out the newly allocated elements
        memset(newArray + d->capacity, 0, ARRAY_INCREMENT * sizeof(struct UserInfo));
        d->users = newArray;
        d->capacity += ARRAY_INCREMENT;

        // Add new customer at first new slot (old size index)
        d->users[d->totalItems].id = strdup(id);
        if (d->users[d->totalItems].id == NULL)
            return -1;

        d->users[d->totalItems].name = strdup(name);
        if (d->users[d->totalItems].name == NULL) {
            free(d->users[d->totalItems].id);
            return -1;
        }

        d->users[d->totalItems].purchase = purchase;
        d->totalItems++;
    } else {
        // Find first empty slot and add new customer there
        for (int i = 0; i < d->capacity; i++) {
            if (d->users[i].id == NULL) {
                d->users[i].id = strdup(id);
                if (d->users[i].id == NULL)
                    return -1;

                d->users[i].name = strdup(name);
                if (d->users[i].name == NULL) {
                    free(d->users[i].id);
                    return -1;
                }

                d->users[i].purchase = purchase;
                d->totalItems++;
                break;
            }
        }
    }

    return 0;
}
/*--------------------------------------------------------------------*/
/*
UnregisterCustomerByID() has two parameters(DB_T object d and const char
pointer id). If either of the parameters are NULL, it's a filure and 
it returns -1. If not, it goes through the array looking for a used info
with the same id as the parameter. If it finds one, it frees the id and 
name, sets the purchase to 0, decrements the numnber of items by 1 and
then returns 0. If it fails to find one,  it returns -1.
*/
int UnregisterCustomerByID(DB_T d, const char *id) {
    if (d == NULL || id == NULL)
        return -1;

    for (int i = 0; i < d->capacity; i++) {
        if (d->users[i].id != NULL && strcmp(id, d->users[i].id) == 0) {
            free(d->users[i].id);
            free(d->users[i].name);
            d->users[i].id = NULL;
            d->users[i].name = NULL;
            d->users[i].purchase = 0;
            d->totalItems--;
            return 0;
        }
    }
    return -1;
}

/*--------------------------------------------------------------------*/
/*
UnregisterCustomerByName() works in almost the same way with the above 
function UnregisterCustomerByID. It only differs by the fact that it
take a name parameter and not id. Accordingly, it looks for a user info
using the name argument. All other things are the same.
*/
int UnregisterCustomerByName(DB_T d, const char *name) {
    if (d == NULL || name == NULL)
        return -1;

    for (int i = 0; i < d->capacity; i++) {
        if (d->users[i].name != NULL && strcmp(name, d->users[i].name) == 0) {
            free(d->users[i].id);
            free(d->users[i].name);
            d->users[i].id = NULL;
            d->users[i].name = NULL;
            d->users[i].purchase = 0;
            d->totalItems--;
            return 0;
        }
    }
    return -1;
}
/*--------------------------------------------------------------------*/
/*
GetPurchaseByID() has two parameters(DB_T object d and const char
pointer id). If either of the parameters are NULL, it's a filure and 
it returns -1. If not, it goes through the array looking for a used info
with the same id as the parameter. If it finds one, returns its purchase
value. Otherwise, it returns -1.
*/
int GetPurchaseByID(DB_T d, const char* id) {
    if (d == NULL || id == NULL)
        return -1;

    for (int i = 0; i < d->capacity; i++) {
        if (d->users[i].id != NULL && strcmp(id, d->users[i].id) == 0)
            return d->users[i].purchase;
    }
    return -1;
}
/*--------------------------------------------------------------------*/
/*
GetPurchaseByName() works in almost the same way with the above 
function GetPurchaseByID. It only differs by the fact that it
take a name parameter and not id. Accordingly, it looks for a user info
using the name argument. All other things are the same.
*/
int GetPurchaseByName(DB_T d, const char* name) {
    if (d == NULL || name == NULL)
        return -1;

    for (int i = 0; i < d->capacity; i++) {
        if (d->users[i].name != NULL && strcmp(name, d->users[i].name) == 0)
            return d->users[i].purchase;
    }
    return -1;
}
/*--------------------------------------------------------------------*/
/*
GetSumCustomerPurchase() takes two parameters: the DB_T object d and 
function pointer fp. It checks if either of the parameters are NULL. 
If so, it returns -1. Otherwise, it goes throught the entire array and
sums all the purchase value using the function pointed by fp. It finally
returns this value.
*/
int GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp) {
    if (d == NULL || fp == NULL)
        return -1;

    int totalPurchase = 0;
    for (int i = 0; i < d->totalItems; i++) {
        totalPurchase += fp(d->users[i].id, d->users[i].name, d->users[i].purchase);
    }
    return totalPurchase;
}
