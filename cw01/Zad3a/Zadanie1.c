#include "Zadanie1.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include<time.h>
#include<string.h>

//1. tworzenie listy dwukierunkowej
list * createContactList(void) {
	list * result = malloc(sizeof(list));
	result->head = NULL;
	return result;

}
//2.usuwanie listy dwukierunkowej
void deleteContactList(list * contactList) {

	listNode * p = contactList->head;
	if (p != NULL) {
		while (p->next != NULL) {
			contactList->head = contactList->head->next;
			free(p);
			p = contactList->head;
		}

	}
	free(contactList);

}
//3.1 dodanie kontaktu - struktura
void addContact(list * list, position * position) {

    listNode * p = malloc(sizeof(listNode));
	p->pos = position;
    p->prev = NULL;
	if (list->head != NULL) {
		p->next = list->head;
		list->head->prev = p;
	}
	else {
		p->next = NULL;
	}
	list->head = p;

}
//3.2 dodawanie kontaktu do poczatku listy - detale
void addContactPosition(list * list, char * name, char *surname, char *date, char *email, char *phone, char *address) {


		position *p = malloc(sizeof(position));
		p->birthdate = date;
		p->address = address;
		p->name = name;
		p->surname = surname;
		p->email = email;
		p->phone=phone;
		addContact(list, p);
}
//4.1znajdz kontakt o danym imieniu i nazwisku
position * findContactName(list * list, char *name, char *surname) {
	if (list->head == NULL) return NULL;
	listNode *tmp = list->head;
	if (list->head->next != NULL) {
		while (tmp->next != list->head) {
			if (tmp->pos->name == name && tmp->pos->surname == surname) return tmp->pos;
			tmp = tmp->next;
		}
	}
	if (tmp->pos->name == name && tmp->pos->surname == surname) return tmp->pos; //sprawdzamy ostatni element
	return NULL;
}
//4.2znajdz kontakt o danym mailu
position * findContactEmail(list*list, char * email) {
	if (list->head == NULL) return NULL;
	listNode *tmp = list->head;
	if (list->head->next != NULL) {
		while (tmp->next != list->head) {
			if (strcmp(tmp->pos->email,email)==0) return tmp->pos;
			tmp = tmp->next;
		}
	}
	if (strcmp(tmp->pos->email,email)==0) return tmp->pos; //sprawdzamy ostatni element
	return NULL;

}
//4.3znajdz kontakt po telefonie
position * findContactPhone(list*list, char * phone) {
	if (list->head == NULL) return NULL;
	listNode *tmp = list->head;
	if (list->head->next != NULL) {
		while (tmp->next != list->head) {
			if (strcmp(tmp->pos->phone,phone)==0) return tmp->pos;
			tmp = tmp->next;
		}
	}
	if (tmp->pos->phone == phone) return tmp->pos;
	return NULL;

}
//4.4znajdz pozycje w ksiazce
position * findPosition(list * list, position * position) {
	return findContactPhone(list, position->phone);

}
//4.5 znajdz kontakt w ksiazce
listNode * findContact(list *list, position * position) {
	if (list->head == NULL) return NULL;
	listNode *tmp = list->head;
	if (list->head->next != NULL) {
		while (tmp->next != NULL) {
			if (tmp->pos->phone == position->phone) return tmp;
			tmp = tmp->next;
		//	tmp->prev=
		}
	}
	if (tmp->pos->phone == position->phone) return tmp;
	return NULL;

}
//funkcja pomocnicza do sortowania i usuwania kontaktu
listNode * disconnectContact(list *list, position * position) {
	if (list->head == NULL) return NULL;

	if (list->head->next == NULL && findContact(list, position)!=0) {
		list->head = NULL;
		return list->head;
	}

	listNode * remove = findContact(list, position);
	if (remove->prev == NULL) {
		list->head = remove->next;
		remove->next->prev = NULL;

	}
	else if (remove->next == NULL) {
		remove->prev->next = NULL;
	}
	else {
		remove->prev->next = remove->next;
		remove->next->prev = remove->prev;
	}
	return remove;

}
//funkcja usuwająca dany kontakt
void deleteContact(list*list, position *position) {
	listNode * remove = disconnectContact(list, position);
	free(remove);
}
//znajduje najwiekszy element, dla list co najmniej 1elementowych
listNode * findMax(list* list, char *sorted) {
	if (list->head == NULL) return NULL;
	if (list->head->next == NULL) return list->head; //jezeli lista jest jednoelementowa
	listNode * max = list->head;
	listNode *tmp = list->head->next;

	while (tmp->next != NULL) {
		if (strcmp(sorted,"surname")==0) {
			if (strcmp(max->pos->surname, tmp->pos->surname) > 0) max = tmp;
		}
		else if (strcmp(sorted,"email")==0) {
			if (strcmp(max->pos->email, tmp->pos->email) > 0) max = tmp;
		}
		else if (strcmp(sorted,"phone")==0) {
			if (strcmp(max->pos->phone, tmp->pos->phone) > 0) max = tmp;
        }
		else if (strcmp(sorted, "birthdate")==0) {
			if (strcmp(max->pos->birthdate, tmp->pos->birthdate) > 0) max = tmp;
		}
		tmp = tmp->next;

	}
	//sprawdzamy ostatni element
	if (strcmp(sorted, "surname")==0) {
		if (strcmp(max->pos->surname, tmp->pos->surname) > 0) max = tmp;
	}
	else if (strcmp(sorted,"email")==0) {
		if (strcmp(max->pos->email, tmp->pos->email) > 0) max = tmp;
	}
	else if (strcmp(sorted, "phone")==0) {
		if (strcmp(max->pos->phone, tmp->pos->phone) > 0) max = tmp;
	}
    else if (strcmp(sorted, "birthdate")==0) {
		if (strcmp(max->pos->birthdate, tmp->pos->birthdate) > 0) max = tmp;
	}
	else return NULL;
	return max;
}
//sortowanie  wq nazwiska, maila, telefonu
void sortBy_(list * list, char *sortedBy) {
	if (list->head == NULL)
		return;
	struct list *result=createContactList();

	listNode * max = findMax(list, sortedBy);
	result->head = max;
	listNode * last = max;
	disconnectContact(list, max->pos);
	last->next = NULL;
	max->prev = NULL;
	while (list->head != NULL) {
		max = findMax(list,sortedBy);
		disconnectContact(list, max->pos);
		last->next = max;
		max->prev = last;
		last = max;

	}
	list->head = result->head;
	free(result);

}
//------------------------------------------------------------------------------

//7.tworzenie drzewa binarnego
tree * createContactTree(void) {
	tree * result = malloc(sizeof(tree));
	result->root = NULL;
	return result;
}
//
void deleteSubTree(treeNode * node) {
	if (node == NULL) return;
	deleteSubTree(node->left);
	deleteSubTree(node->right);
	free(node);
}
//8.usuwanie drzewa binarnego
void deleteContactTree(tree * contactTree) {

	treeNode * p = contactTree->root;
	if (p->right != NULL) { //jezeli juz nie mam do czynienia z pusta lista kontaktow
		deleteSubTree(p->right);
	}
	if (p->left != NULL) {
		deleteSubTree(p->left);
	}
	free(contactTree->root);
	free(contactTree);

}
//9. dodanie elementu do drzewa binarnego
tree * addContactTree(tree *treeCon, treeNode * root, position *position) {

	//jezeli drzewo jest puste
	if (root== NULL) {
		treeNode * node = malloc(sizeof(treeNode));
		node->pos = position;
		node->parent = NULL;
		node->left = NULL;
		node->right = NULL;
		root = node;
		treeCon->root = root;
	}
	//jezeli nazwisko w root > od wstawianego ( czyli do lewego poddrzewa)
	else if(strcmp(position->surname, root->pos->surname)<0) {
		if (root->left != NULL) addContactTree(treeCon, root->left, position);
		else {
			treeNode * node = malloc(sizeof(treeNode));
			node->pos = position;
			node->parent = root;
			node->left = NULL;
			node->right = NULL;
			root->left = node;
		}
	}
	//idziemy w prawe poddrzewo
	else {
		if (root->right != NULL) addContactTree(treeCon,root->right, position);
		else {
			treeNode * node = malloc(sizeof(treeNode));
			node->pos = position;
			node->right = NULL;
			node->left = NULL;
			node->parent = root;
			root->right = node;
		}
	}
	return treeCon;
}
tree * addContactPositionTree(tree *tree, char * name, char *surname, char *date, char *email, char *phone, char *address) {
	position * position = malloc(sizeof(position));
	position->name = name;
	position->surname = surname;
	position->birthdate = date;
	position->email = email;
	position->phone = phone;
	position->address = address;
	tree=addContactTree(tree,tree->root, position);
	return tree;

}
//11. wyszukanie elementu w drzewie binarnym
treeNode *findContactSurnameT(treeNode *root, char * surname) {
	if (root == NULL) return NULL;
	if (strcmp(surname, root->pos->surname)==0) return root;
	else if (strcmp(surname, root->pos->surname)<0) return findContactSurnameT(root->left, surname);
	else return findContactSurnameT(root->right, surname);
}
treeNode *findContactPhoneT(treeNode *root, char *phone) {
	if (root == NULL) return NULL;
	if (strcmp(phone, root->pos->phone) == 0) return root;
	else if (strcmp(phone, root->pos->phone)< 0) return findContactPhoneT(root->left, phone);
	else return findContactPhoneT(root->right, phone);
}
treeNode *findContactEmailT(treeNode *root, char *email) {
	if (root == NULL) return NULL;
	if (strcmp(email, root->pos->email) == 0) return root;
	else if (strcmp(email, root->pos->email) < 0) return findContactEmailT(root->left, email);
	else return findContactEmailT(root->right, email);
}

treeNode * findContactT(treeNode *root, position* position) {
	return findContactPhoneT(root, position->phone);

}

//10.usuwanie elementu z drzewa binarnego
treeNode * succesorNode(treeNode * p) {
	if (p != NULL) {
		if (p->right != NULL) {
			p = p->right;
			while (p->left != NULL) p = p->left;
			return p;
		}
		else {
			while (p->parent != NULL && strcmp(p->parent->pos->surname,p->pos->surname)<0) p = p->parent;
			return p->parent;
		}
	}
		return NULL;
}
treeNode * predecessorNode(treeNode * p) {
	if (p != NULL) {
		if (p->left != NULL) {
			p = p->left;
			while (p->right != NULL) p = p->right;
			return p;
		}
		else {
			while (p->parent != NULL && strcmp(p->parent->pos->surname, p->pos->surname) > 0) p = p->parent;
			return p->parent;
		}
	}
	return NULL;
}
treeNode * replaceNode(treeNode * p) {
	if (p != NULL) {
		if (p->right != NULL) {
			p = p->right;
			while (p->left != NULL) p = p->left;
			return p;
		}
		else if (p->left != NULL) {
			p = p->left;
			while (p->right != NULL) p = p->right;
			return p;
		}
	}
	return NULL;
}
void deleteContactT(tree *tree, treeNode * node) {
	if (node == NULL) return;
	if (findContactT(tree->root, node->pos) == NULL) return;
	treeNode * parent = node->parent;
	treeNode * tmp;
	//jezeli brak dzieki
	if (node->left == NULL && node->right == NULL) {
		if(parent->left == node){
			parent->left = NULL;
		}
		else {
			parent->right = NULL;
		}

		free(node);
		return;
	}
	//jezeli jest wiecej niz 1 drzewo
	else {
		tmp = replaceNode(node);
		node->pos = tmp->pos;
		parent = tmp->parent;
		if (parent->left == tmp) parent->left = NULL;
		else parent->right = NULL;
		free(tmp);
	}
		}

