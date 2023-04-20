#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NEW_T(__T) (__T *)malloc(sizeof(__T))
typedef struct
{
    char *username;
    char *ip;
} superuser;
typedef struct node
{
    superuser *user;
    struct node *next;
} superuser_linknode;

void print_list(superuser_linknode *head)
{
    superuser_linknode *current = head;

    while (current != NULL)
    {
        printf("Username: %s, IP: %s\n", current->user->username, current->user->ip);
        current = current->next;
    }
}

superuser_linknode *find_list(superuser_linknode *head, char *username, char *ip)
{
    superuser_linknode *current = head;

    while (current != NULL)
    {
        if (strcmp(current->user->username, username) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void insert_superuser_linknode(superuser_linknode **head, superuser *user)
{
    superuser_linknode *new_node = NEW_T(superuser_linknode);
    new_node->user = user;
    new_node->next = *head;
    *head = new_node;
}

void free_superuser_linklist(superuser_linknode **head)
{
    superuser_linknode *current = *head;

    while (current != NULL)
    {
        superuser_linknode *next = current->next;
        free(current);
        current = next;
    }

    *head = NULL;
}

int main()
{
    superuser_linknode *head = NULL;

    superuser user1 = {"Alice", "192.168.0.1"};
    superuser user2 = {"Bob", "192.168.0.2"};
    superuser user3 = {"Charlie", "192.168.0.3"};

    insert_superuser_linknode(&head, &user1);
    insert_superuser_linknode(&head, &user2);
    insert_superuser_linknode(&head, &user3);

    print_list(head);
    superuser_linknode *user_node = find_list(head, "Bob", "");
    if (user_node != NULL)
    {
        printf("%s,%s\n", user_node->user->ip, user_node->user->username);
    }

    free_superuser_linklist(&head);

    return 0;
}