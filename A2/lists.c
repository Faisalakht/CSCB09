#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lists.h"


/*Calculates the number of users in a group.Returns the number of users*/
int numuser(Group *group){
	int counter = 0;
	User *head = group->users;
	while((head) != NULL)
	{
		counter++;
		head = head->next;
	}
	return counter;
}

/*Find the user in the group, returns the user node if it exists else NULL*/
User *find_user(Group *group,const char *user_name)

{

	User *head = group->users;
	while ((head) != NULL)
	{
		if (strcmp(head->name,user_name) == 0)
		{
			return head;
		}

		head = head->next;
	}

	return NULL;

}




/*Deletes node from group in Xcts*/
void deletexct(Group *group, char *user_name){

	Xct *head = group->xcts;
	Xct *prev = group->xcts;
	int counter=0;
	if (strcmp(head->name,user_name) == 0)
	{
		group->xcts = head->next;
		free(head);
	}
	else
	{
		while (head != NULL)
		{

			if (strcmp(head->name,user_name) == 0)
					{
					Xct *temp = head->next;
					free(head);
					prev->next = temp;
					}
			if (counter != 0)
			{
				prev=prev->next;
			}
			counter++;
			head=head->next;
		}

	}


}


/*Returns the size of the xcts for a given group*/
int sizexct(Group *group){
	int counter = 0;
	Xct *head = group->xcts;
	while((head) != NULL)
	{
		counter++;
		head = head->next;
	}
	return counter;
}



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
	char *groupname;
	groupname = strdup(group_name);
	if ((*group_list_ptr) == NULL){
		Group *newgp;
		newgp = (Group *)malloc(sizeof(struct group));
		newgp->name=groupname;
		newgp->next=NULL;

		*group_list_ptr = newgp;

		return 0;
	}
	Group *head;
	head= *group_list_ptr;
	while ((*head).next != NULL){
		if (strcmp(group_name,(*head).name) == 0)
		{
		 return -1;
		}
		head = (*head).next;
		}

	if (strcmp((*head).name,group_name) == 0){
		return -1;
	}

	Group *newgp;
	newgp = malloc(sizeof(struct group));
	newgp->name=groupname;
	newgp->next=NULL;
	head->next=newgp;

	return 0;

}

/* Print to standard output the names of all groups in group_list, one name
*  per line. Output is in the same order as group_list.
*/
void list_groups(Group *group_list) {

	Group *head = group_list;
	while ((head) != NULL){
		printf("%s\n",(head)->name);
		head = (*head).next;
		}
}

/* Search the list of groups for a group with matching group_name
* If group_name is not found, return NULL, otherwise return a pointer to the
* matching group list node.
*/
Group *find_group(Group *group_list, const char *group_name) {
	Group *head = group_list;
	while ((head) != NULL){
		if (strcmp(group_name,(head)->name) == 0)
				{
					return head;
				}
		head = (*head).next;
		}
    return NULL;
}

/* Add a new user with the specified user name to the specified group. Return zero
* on success and -1 if the group already has a user with that name.
* (allocate and initialize a User data structure and insert it into the
* appropriate group list)
*/
int add_user(Group *group, const char *user_name) {
	User *head = group->users;
	char *username;
		username = strdup(user_name);
	if (head == NULL)
	{
		User *newusr;
		newusr = (User *)malloc(sizeof(struct user));
		newusr->name=username;
		newusr->balance=0;
		newusr->next=NULL;
		group->users = newusr;
		return 0;
	}

	while ((*head).next != NULL){
		if (strcmp(user_name,(*head).name) == 0)
		{
		 return -1;
		}
		head = (*head).next;
		}

	if (strcmp((*head).name,user_name) == 0){
		return -1;
	}
	User *newusr;
	newusr = malloc(sizeof(struct user));
	newusr->name=username;
	newusr->balance= 0;
	newusr->next=NULL;
	head->next = newusr;
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
	User *g = find_user(group,user_name);
		User *head = group->users;
		User *prev = group->users;
		int counter = 0;
		if (g != NULL)
		{
			remove_xct(group,user_name);
			if (strcmp(head->name,user_name) == 0)
				{
					group->users = head->next;
					free(head);
				}
				else
				{
					while (head != NULL)
					{

						if (strcmp(head->name,user_name) == 0)
								{
								User *temp = head->next;
								free(head);
								prev->next = temp;
								}
						if (counter != 0)
						{
							prev=prev->next;
						}
						counter++;
						head=head->next;
					}

				}

			return 0;
		}
	    return -1;
}

/* Print to standard output the names of all the users in group, one
* per line, and in the order that users are stored in the list, namely
* lowest payer first.
*/
void list_users(Group *group) {

	User *head = group->users;
	while ((head) != NULL){
			printf("%s\n",(head)->name);
			head = (*head).next;
			}

}

/* Print to standard output the balance of the specified user. Return 0
* on success, or -1 if the user with the given name is not in the group.
*/
int user_balance(Group *group, const char *user_name) {

	User *head = group->users;
	while ((head) != NULL){
		if(strcmp(head->name,user_name) == 0)
		{
			printf("%.2f\n",head->balance);
			return 0;
		}
		head = head->next;
	}

    return -1;

}

/* Print to standard output the name of the user who has paid the least
* If there are several users with equal least amounts, all names are output.
* Returns 0 on success, and -1 if the list of users is empty.
* (This should be easy, since your list is sorted by balance).
*/
int under_paid(Group *group) {
	User *head = group->users;
		if (head != NULL)
		{
			while (head != NULL)
			{
				printf("%s\n",head->name);
				if (head->balance == head->next->balance)
				{
					head=head->next;
				}
				else
				{
					break;
				}
			}
			return 0;
		}
	    return -1;
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
	User *head = group->users;
	if (strcmp(head->name,user_name)==0)
	{
		return head;
	}
	while (head->next != NULL)
	{
		if (strcmp(head->next->name,user_name) == 0)
		{
			return head;
		}
		head = head->next;
	}
    return NULL;
}





/*Sorts the linked list after and xct node is added*/

void sortlist(Group *group,User *usr)
{
	int size = numuser(group)*2;
	int counter=0;
	while(counter < size){
		if (((*usr).next != NULL))
		{
			if (usr->balance > usr->next->balance)
			{
				if (strcmp(group->users->name,usr->name) == 0)
				{
					User *temp = usr->next;
					group->users = temp;
					usr->next = temp->next;
					temp->next = usr;
					usr=temp->next;
				}
				else
				{
					User *p = find_prev_user(group,usr->name);
					User *temp = usr->next;
					p->next = temp;
					usr->next = temp->next;
					temp->next = usr;
					usr=temp->next;

				}



		  }

			if (usr->balance < find_prev_user(group,usr->name)->balance)
			{
				User *temp = find_prev_user(group,usr->name);
				User *prev = find_prev_user(group,temp->name);
				prev->next=usr;
				temp->next = usr->next;
				usr->next=temp;

			}

		}

		counter++;
	}

}















/* Add the transaction represented by user_name and amount to the appropriate
* transaction list, and update the balances of the corresponding user and group.
* Note that updating a user's balance might require the user to be moved to a
* different position in the list to keep the list in sorted order. Returns 0 on
* success, and -1 if the specified user does not exist.
*/
int add_xct(Group *group, const char *user_name, double amount) {
	User *g = find_user(group,user_name);
	char *xctname;
	xctname =strdup(user_name);
	if (g != NULL)
	{
		Xct *head = group->xcts;
		Xct *newxct;
		newxct = (Xct *)malloc(sizeof(struct xct));
		newxct->name=xctname;
		newxct->amount = amount;
		newxct->next=NULL;

		if ((group->xcts) == NULL)
		{

			group->xcts = newxct;
			g->balance += amount;
			return 0;
		}

		while ((*head).next != NULL){

				head = (*head).next;
				}
		head->next = newxct;
		g->balance += amount;
		sortlist(group,g);
		return 0;



	}
    return -1;
}

/* Print to standard output the num_xct most recent transactions for the
* specified group (or fewer transactions if there are less than num_xct
* transactions posted for this group). The output should have one line per
* transaction that prints the name and the amount of the transaction. If
* there are no transactions, this function will print nothing.
*/
void recent_xct(Group *group, long nu_xct) {
	int size=sizexct(group);
		Xct *head=group->xcts;
		long counter=0;
		long starter= size - nu_xct;
		if(nu_xct > size)
		{
			while (head != NULL)
			{
				printf("%s ,",head->name);
				printf("%.2f\n",head->amount);
				head=head->next;
			}
		}
		while (head != NULL)
		{
			if (counter >= starter)
			{
				printf("%s ,",head->name);
				printf("%.2f\n",head->amount);
			}
			head=head->next;
			counter++;
		}



}

/* Remove all transactions that belong to the user_name from the group's
* transaction list. This helper function should be called by remove_user.
* If there are no transactions for this user, the function should do nothing.
* Remember to free memory no longer needed.
*/
void remove_xct(Group *group, const char *user_name) {

	User *g = find_user(group,user_name);
	char *username;
	username= strdup(user_name);
	int size = sizexct(group);
	int counter = 0;
	if (g != NULL)
	{
		g->balance = 0;
		while (counter<size)
		{
			deletexct(group,username);
			counter++;
		}

	}

}
