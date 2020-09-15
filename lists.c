#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lists.h"

/* Add a group with name group_name to the group_list referred to by 
* group_list_ptr. The groups are ordered by the time that the group was 
* added to the list with new groups added to the end of the list.
*
* Returns 0 on success and -1 if a group with this name already exists.
*
* (I.e, allocate and initialize a Group struct, and insert it
* into the group_list. Note that the head of the group list might change
* which is why the first argument is a double pointer.) 
*/
int add_group(Group **group_list_ptr, const char *group_name) {
    // if a group is found with the same name, return error
    if (find_group(*group_list_ptr, group_name) != NULL) {
	return -1;
    }
    // otherwise try creating a new group
    else {
	// create an empty group
	Group *ngroup;
	// if error occurs while allocating space for group and group name return error
	if ((ngroup = malloc(sizeof(Group))) == NULL) {
	    perror("An error occurred while allocating space for a new group");
	    exit(1);
	}
	if ((ngroup->name = malloc(strlen(group_name)+1)) == NULL) {
	    perror("An error occurred while allocating space for the new group's name");
	    exit(1);
	}
	// copy the given group name in the parameter to the new group name
	strncpy(ngroup->name, group_name, strlen(group_name)+1);
	// set the new group's users, xcts, and next to be null
	ngroup->users = NULL;
	ngroup->xcts = NULL;
	ngroup->next = NULL;
	// if there are no groups present, set the group list ptr to be the new group
	if (*group_list_ptr == NULL){
	    *group_list_ptr = ngroup;
	}
	// otherwise
	else {
	    // find last group in the list
	    Group *checkend = *group_list_ptr;
	    while (checkend->next != NULL) {
		checkend = checkend->next;
	    }
	    // set the last group's next group to be the new group
	    checkend->next = ngroup;
	}
    }
    // return successful add group (0);
    return 0;
}

/* Print to standard output the names of all groups in group_list, one name
*  per line. Output is in the same order as group_list.
*/
void list_groups(Group *group_list) {
    // point a group to the first group in the group list
    Group *iteratelist = group_list;
    // iterate through all the groups in the list and print their names on different lines
    while (iteratelist != NULL) {
	// print the group name
	printf("%s\n", iteratelist->name);
	// go to the next group in the list
	iteratelist = iteratelist->next;
    }
}

/* Search the list of groups for a group with matching group_name
* If group_name is not found, return NULL, otherwise return a pointer to the 
* matching group list node.
*/
Group *find_group(Group *group_list, const char *group_name) {
    // point a new group to the first group in group list
    Group *iterategroup = group_list;
    // iterate through list of groups
    while (iterategroup != NULL) {
	// compare the group's name to the given group name we're looking for
	if (strcmp(iterategroup->name, group_name) == 0) {
	    // if a group is found, set a new group to be returned to be the current group
	    Group *found;
	    found = iterategroup;
	    // return the matching name group
	    return found;
	}
	else {
	    // if the group name doesn't match, go to the next group
	    iterategroup = iterategroup->next;
	}
    }
    // if no match is found, return null
    return NULL;
}

/* Add a new user with the specified user name to the specified group. Return zero
* on success and -1 if the group already has a user with that name.
* (allocate and initialize a User data structure and insert it into the
* appropriate group list)
*/
int add_user(Group *group, const char *user_name) {
    // check if the user already exists by finding the prev user of the matching name user
    User *checkexist = find_prev_user(group, user_name);
    // if a matching user exists already, return error
    if (checkexist != NULL) {
	return -1;
    }
    // if the user doesnt already exist
    else {
	// set an empty user to be added
	User *nuser;
	// allocate a new user's memory and exit if it can't be allocated
	if ((nuser = malloc(sizeof(User))) == NULL) {
	    // return an error message
	    perror("An error occurred while allocating space for a new user");
	    exit(1);
	}
	int namelen = strlen(user_name) + 1;
	// allocate a new user's name into the memory and exit if it can't be allocated
	if ((nuser->name = malloc(sizeof(namelen))) == NULL) {
	    // return error message
	    perror("An error occurred while allocating a name for the new user");
	    exit(1);
	}
	// copy the given username into the new user's name
	strncpy(nuser->name, user_name, namelen);
	// set the new user's next to the first user of the list
	nuser->next = group->users;
	// set the head of the list to be the new user
	group->users = nuser;
	// set the balance of the new user to be 0
	nuser->balance = 0.00;
    }
    // return successful add user
    return 0;
}

/* Remove the user with matching user and group name and
* remove all her transactions from the transaction list. 
* Return 0 on success, and -1 if no matching user exists.
* Remember to free memory no longer needed.
* (Wait on implementing the removal of the user's transactions until you 
* get to Part III below, when you will implement transactions.)
*/
int remove_user(Group *group, const char *user_name) {
    // find the user previous to the one that we want to remove
    User *founduser = find_prev_user(group, user_name);
    // if the found user is the first in the list and we want to remove the first
    if ((group->users->next == NULL) && (founduser->name == user_name)) {
	// remove xcts of the user
	remove_xct(group, user_name);
	// set a temp pointer to the first element in users
	User *toremove = group->users;
	// empty the list of users
	group->users = NULL;
	// free the removed user's memory
	free(toremove);
    }
    // if the found user is one of the only 2 elements in the users list
    else if ((group->users->next->next == NULL)) {
	// first set the user to be removed to be the first element
	User *removed = group->users;
	// if the element to be removed is the second one
	if (strcmp(group->users->next->name, user_name) == 0) {
	    // set removed to be the second element
	    removed = group->users->next;
	    // set the first element's next to the second's next (NULL)
	    group->users->next = NULL;
	}
	// otherwise if the element to be removed is the first
	else if (strcmp(group->users->name, user_name) == 0) {
	    // set the head of the users list to be element #2
	    group->users = group->users->next;
	// remove transactions of the removed user
	remove_xct(group, removed->name);
	// free the memory taken by the removed memory
	free(removed);
	}
    }
    // in all other cases
    else if (founduser != NULL) {
	// set the user to be removed to be foundnextuser
        User *foundnextuser = founduser->next;
	// set the prev user to be removed's next to be their next next element
        founduser->next = founduser->next->next;
	// remove transactions taken by the removed element
	remove_xct(group, foundnextuser->name);
	// free the memory taken by the removed element
	free(foundnextuser);	
    }
    // if no user is found, return error
    else {
	return -1;
    }
    // return 0 on successful removal of user
    return 0;
}

/* Print to standard output the names of all the users in group, one
* per line, and in the order that users are stored in the list, namely 
* lowest payer first.
*/
void list_users(Group *group) {
    // set a pointer to the list of users
    User *currentuser = group->users;
    // iterate through the list of users in the group and print each user's name on a new line
    while (currentuser != NULL) {
	printf("%s\n", currentuser->name);
	currentuser = currentuser->next;
    }
}

/* Print to standard output the balance of the specified user. Return 0
* on success, or -1 if the user with the given name is not in the group.
*/
int user_balance(Group *group, const char *user_name) {
    // get the previous user of the user we're looking for
    User *prevuserbalance = find_prev_user(group, user_name);
    // if such user is found
    if (prevuserbalance != NULL) {
	// set a pointer to the user we're looking for
        User *userbalance = prevuserbalance->next;
	// print the balance of the user we're looking for
        printf("%0.2f\n", userbalance->balance);
    }
    // if no such user was found return error
    else {
	return -1;
    }
    // return 0 on successful printing of user balance
    return 0;
}

/* Print to standard output the name of the user who has paid the least 
* If there are several users with equal least amounts, all names are output. 
* Returns 0 on success, and -1 if the list of users is empty.
* (This should be easy, since your list is sorted by balance). 
*/
int under_paid(Group *group) {
    // set a pointer to the head of the users list
    User *findlowestbalance = group->users;
    // if there are no users in the list return error
    if (findlowestbalance == NULL) {
	return -1;
    }
    // otherwise iterate through each user to find the lowest
    else {
	// set the lowest amount to the first element's balance (since list is sorted by balance from - to +)
	double lowest;
	lowest = findlowestbalance->balance;
	// print the first element's name since it's guaranteed to be lowest
	printf("%s ", findlowestbalance->name);
	// set a pointer to the next element to check if they have they have the same balance
	User *nextlowest = findlowestbalance->next;
	// while next user has the same balance and isn't null
        while ((nextlowest != NULL) && (nextlowest->balance == lowest)) {
	    // print the next element with the same balance
            printf("%s ", nextlowest->name);
	    // go to the next element
	    nextlowest = nextlowest->next;
	}
	// print a new line
	printf("\n");
    }
    // return 0 on successful printing of lowest paid users
    return 0;
}

/* Return a pointer to the user prior to the one in group with user_name. If 
* the matching user is the first in the list (i.e. there is no prior user in 
* the list), return a pointer to the matching user itself. If no matching user 
* exists, return NULL. 
*
* The reason for returning the prior user is that returning the matching user 
* itself does not allow us to change the user that occurs before the
* matching user, and some of the functions you will implement require that
* we be able to do this.
*/
User *find_prev_user(Group *group, const char *user_name) {
    // set a pointer to the head of the list of users
    User *findprev = group->users;
    // if there is nothing in the list return NULL as there will be no matching user
    if (findprev == NULL) {
        return NULL;
    }
    // if the first user matches the username return the first user
    else if (strcmp(findprev->name, user_name) == 0) {
        return findprev;
    }
    // otherwise while the pointer's next isn't null
    else {
	while (findprev->next != NULL) {
	    // if the pointer's next element matches the username
            if (strcmp(findprev->next->name, user_name) == 0) {
	 	// return the pointer
                return findprev;
            }
	    // go to the next element
            findprev = findprev->next;
        }
    }
    // if no such user was found return NULL
    return NULL;
}

/* Add the transaction represented by user_name and amount to the appropriate 
* transaction list, and update the balances of the corresponding user and group. 
* Note that updating a user's balance might require the user to be moved to a
* different position in the list to keep the list in sorted order. Returns 0 on
* success, and -1 if the specified user does not exist.
*/
int add_xct(Group *group, const char *user_name, double amount) {
    // set 2 pointers to the previous of the user we're looking for
    User *prevtransacuser = find_prev_user(group, user_name);
    User *transacuser = prevtransacuser;
    // if no user was found return error
    if (prevtransacuser == NULL) {
        return -1;
    }
    // otherwise if a user was found
    else {
	// if the found user is the first or second user
        if ((prevtransacuser != group->users) || (strcmp(prevtransacuser->name, user_name) != 0)) {
	    // set the transac user to be the next of the prev user
            transacuser = prevtransacuser->next;
        }
    }
    // create a new xct
    Xct *nxct;
    // allocate memory for the xct and its name and return error if unable to
    if ((nxct = malloc(sizeof(Xct))) == NULL){
        perror("Error while allocating space for new transaction");
        exit(1);
    }
    if ((nxct->name = malloc(strlen(user_name) + 1)) == NULL) {
	perror("Error while allocating space for new transaction's user name");
	exit(1);
    }
    // set the new xct's next to be the head of the xct list
    nxct->next = group->xcts;
    // set the head of the xct list to be the new xct
    group->xcts = nxct;
    // copy the username into the new xct's name
    strncpy(nxct->name, user_name, strlen(user_name) + 1);
    // set the new xct's amount from the value given by the parameter
    nxct->amount = amount;
    // changed the user's balance by adding the amount to the user's balance
    transacuser->balance += amount;
    // shift the order of users
    // while the transaction user has a next user and the transacuser's balance is greater than the next user's balance
    while ((transacuser->next != NULL) && (transacuser->balance > transacuser->next->balance)) {
	// set a pointer to the curr transac user's next
        User *nexttransacuser = transacuser->next;
	// if the transac user's prev user is not the same as transac user (if the element is not the first element)
        if (prevtransacuser != transacuser) {
	    // set the prev's next to be the curr's next
            prevtransacuser->next = nexttransacuser;
	    // set the curr's next to the next's next
            transacuser->next = nexttransacuser->next;
	    // set the next user's next to be the curr
            nexttransacuser->next = transacuser;
        }
	// otherwise (if the element is the first)
        else {
	    // set curr's next to be the next's next
            transacuser->next = nexttransacuser->next;
	    // set next's next to be curr
            nexttransacuser->next = transacuser;
	    // set the head of the list to be the next user
            group->users = nexttransacuser;
        }
	// set prev to be curr
        prevtransacuser = nexttransacuser;
    }
    // return 0 on successful addition of xct
    return 0;
}

/* Print to standard output the num_xct most recent transactions for the 
* specified group (or fewer transactions if there are less than num_xct 
* transactions posted for this group). The output should have one line per 
* transaction that prints the name and the amount of the transaction. If 
* there are no transactions, this function will print nothing.
*/
void recent_xct(Group *group, long nu_xct) {
    // set a pointer of xct to the list of xcts
    Xct *recentxcts = group->xcts;
    // set count of printed xcts to 0
    long count = 0;
    // if there are no xcts print an empty line
    if (recentxcts == NULL) {
	printf("\n");
    }
    // otherwise
    else {
	// while the count is lower than the specified amount and there are transactions left
        while ((recentxcts != NULL) && (count < nu_xct)) {
	    // print the name of the user and the transactions to 2 decimal places
	    printf("%s %0.2f\n", recentxcts->name, recentxcts->amount);
	    // go to the next transaction and increment count
	    recentxcts = recentxcts->next;
	    count += 1;
	}
    }
}

/* Remove all transactions that belong to the user_name from the group's 
* transaction list. This helper function should be called by remove_user. 
* If there are no transactions for this user, the function should do nothing.
* Remember to free memory no longer needed.
*/
void remove_xct(Group *group, const char *user_name) {
    // set 2 pointers, prev to the first element in the xcts list and curr to be nothing
    Xct *prevremove = group->xcts;
    Xct *currremove;
    // if there are no xcts do nothing
    if (group->xcts == NULL) {
    }
    // if if there are only 1 transaction
    else if (group->xcts->next == NULL) {
	// set curr to prev
	currremove = prevremove;
    }
    else {
	// otherwise set curr to prev's next
	currremove = prevremove->next;
    }
    // make a temp xct
    Xct *temp;
    // if there is only 1 transaction and it matches the specified name
    if (group->xcts->next == NULL && strcmp(group->xcts->name, user_name) == 0){
	// set temp to be the xct to be removed
	temp = group->xcts;
	// empty the xct's list
	group->xcts = NULL;
	// free the removed xct's memory
	free(temp);
    }
    // if there are more than 1 transactions
    else {
	// while the curr pointer is not null
        while (currremove != NULL) {
	    // if the curr name matches the specified name
	    if (strcmp(currremove->name, user_name) == 0) {
		// set the prev's next to the curr's next
	        prevremove->next = currremove->next;
		// set temp to be curr
		temp = currremove;
		// set curr to be the next of curr
	        currremove = prevremove->next;
		// free the memory taken by the removed xct
	        free(temp);
	    }
	    // if the curr name doesnt match
	    else {
		// let curr and prev go to the next xct
		prevremove = currremove;
		currremove = currremove->next;
	    }
	}
    }
}

