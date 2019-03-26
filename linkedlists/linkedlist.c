#include<stdio.h>
#include<stdlib.h>

struct node{
	int data;
	struct node* next;
};

int printlist(struct node* head){
	int i=0;
	struct node* pivot = head;
	while(pivot != NULL){
		printf("Node[%d]:[%d]\n", i++, pivot->data);
		pivot = pivot->next;
	}
	
	return 0;
}

int addnodedata(struct node* head, int pos){
	
	struct node *pivot = head, *newnode = malloc(sizeof(struct node));
	int i=1;
	while(pivot != NULL){
		if(i == pos-1){
			newnode->next = pivot->next;
			newnode->data = pos*11;
			pivot->next = newnode;
			break;
		}
		pivot =pivot->next;		
		i++;
	}
	return 0;
}

int deletenodedata(struct node* head, int pos){
	
	struct node *pivot = head , *delnode;
	int i=1;
	while(pivot != NULL){
		if(i == pos-1){
			delnode = pivot->next;
			pivot->next = (pivot->next)->next;
			free(delnode);	
			break;
		}
		pivot =pivot->next;		
		i++;
	}
	return 0;
}


int main(){
	struct node * head = malloc(sizeof(struct node));	
	head->next = NULL;
	head->data = 1;
	
	struct node* nn1 = malloc(sizeof(struct node));
	nn1->data = 2;
	nn1->next = NULL;
	head->next = nn1;

	struct node* nn2 = malloc(sizeof(struct node));
	nn2->data = 3;
	nn2->next = NULL;
	nn1->next = nn2;
	
	addnodedata(head, 3);
	printlist(head);
	addnodedata(head, 4);
	printlist(head);
	deletenodedata(head, 2);
	printlist(head);
	deletenodedata(head, 4);
	printlist(head);
	return 0;
}
