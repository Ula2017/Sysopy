#ifndef ZADANIE1_H
#define ZADANIE1_H

typedef struct position {
	char *name;
	char *surname;
	char *birthdate;
	char *email;
	char *phone;
	char *address;

}position;

typedef struct listNode {
	position * pos;
	struct listNode * next;
	struct listNode * prev;
}listNode;

typedef struct list {
	 listNode * head;
}list;

typedef struct treeNode {
	 position *pos;
	 struct treeNode *right;
	 struct treeNode *left;
	 struct treeNode *parent;
}treeNode;

typedef struct tree {
	 treeNode * root;
}tree;

list * createContactList(void);
void deleteContactList(list * contactList);
void addContact(list * list, position * position);
void addContactPosition(list * list, char * name, char *surname, char *date, char *email, char *phone, char *address);
position * findContactName(list * list, char *name, char *surname);
position * findContactEmail(list*list, char * email);
position * findContactPhone(list*list, char * phone);
position * findPosition(list * list, position * position);
listNode * findContact(list *list, position * position);
listNode * disconnectContact(list *list, position * position);
void deleteContact(list*list, position *position);
listNode * findMax(list* list, char *sorted);
void sortBy_(list * list, char *sortedBy);

tree * createContactTree(void);
void deleteSubTree(treeNode * node);
void deleteContactTree(tree * contactTree);
tree * addContactTree(tree *treeCon, treeNode * root, position *position);
tree * addContactPositionTree(tree *tree, char * name, char *surname, char *date, char *email, char *phone, char *address);
treeNode *findContactSurnameT(treeNode *root, char * surname);
treeNode *findContactPhoneT(treeNode *root, char *phone);
treeNode *findContactEmailT(treeNode *root, char *email);
treeNode * findContactT(treeNode *root, position* position);
treeNode * succesorNode(treeNode * p);
treeNode * predecessorNode(treeNode * p);
treeNode * replaceNode(treeNode * p);
void deleteContactT(tree *tree, treeNode * node);






#endif
