#include <stdio.h>
#include <stdlib.h>

struct n{
    char harf;
    int frequency;
    struct n *left, *right, *next;
};
typedef struct n node;

void ekle (node** root, char harf, int frequency) {
    node* new_node = (node *)malloc(sizeof(node)); 
    new_node->harf  = harf; new_node->frequency  = frequency; 
    new_node->left = NULL; new_node->right = NULL;  

    new_node->next = *(root);  
    *(root) = new_node;   
}
void sorted_insert(node**, node*);

void insertion_sort(node **root) {
    node *sorted = NULL;   
    node *current = *root;
    
    while (current != NULL) {
		node *next = current->next;   
		sorted_insert(&sorted, current);   
		current = next;   
    }
    *root = sorted;   
}

void sorted_insert(node** root, node* new_node) { 
    
	node* current; 
    if (*root == NULL || (*root)->frequency >= new_node->frequency) { 
        new_node->next = *root; 
        *root = new_node;
    } 
	else { 
        current = *root; 
        while (current->next!=NULL && current->next->frequency < new_node->frequency) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    } 
}

node* huffman (node *head_ref) {
    node *iter, *yedek;
    int toplam;
    
    if (head_ref->next == NULL){   
        return head_ref;
    }
    
    else if (head_ref->next->next == NULL){   
        toplam = 0;
        node* new_node = (node *)malloc(sizeof(node));   
        toplam += head_ref->frequency;
        toplam += head_ref->next->frequency;
        new_node->frequency = toplam;   
        new_node->harf = ' ';
        new_node->next = NULL;   
        new_node->left = head_ref;   
        new_node->right = head_ref->next;
        head_ref->next = NULL;
        
        return huffman(new_node);
    }
    
    else{   
        int flag = 0, counter=0, i, toplam = 0;
        node* new_node = (node *)malloc(sizeof(node));
        
		toplam += head_ref->frequency;
        toplam += head_ref->next->frequency;
        new_node->frequency = toplam;
        new_node->harf = ' ';
        
		iter = head_ref;
        
		while (toplam >= iter->next->frequency && iter->next->next != NULL){
            iter = iter->next;   
            counter++;  
        }
        while (iter->next != NULL && new_node->frequency >= iter->next->frequency){  
            iter = iter->next;   
            counter++;
            flag = 1;  
        }
        if (counter > 1 && iter->next != NULL){  
            flag = 2;   
        }
        
        new_node->next = iter->next;
        new_node->left = head_ref;
		new_node->right = head_ref->next;
		if (flag == 1) {  
            node *head_ref2;   
            head_ref2 = head_ref->next->next;    
            head_ref->next->next = NULL ;    
            head_ref->next = NULL;   
            head_ref = iter;
            head_ref->next = new_node;   
            new_node->next = NULL;
            return huffman(head_ref2);   
        }
        else if (flag == 2){   
        	iter->next = new_node;
            node *head_ref2 = head_ref->next->next;
            head_ref->next->next = NULL ;   
            head_ref->next = NULL;   
            return huffman(head_ref2);  
        }
        else{  
            head_ref->next->next = NULL ;   
            head_ref->next = NULL;   
            head_ref = iter;
            return huffman(new_node);
        }    
    }
}

void bastir(node *root){
    int front=0, fr=0, i;
	node *queue[1000];
    queue[front] = root;
    front = front+1;
    queue[front] = NULL;

    while(fr <= front) {
        node *temp=queue[fr];

        if(queue[fr] == NULL && queue[fr-1] == NULL) {
            break;
        }
        fr = fr+1;
        if(temp == NULL) {   
            front = front+1;   
            queue[front] = NULL;
            printf("\n");   
        }
        else{
        	printf("%d(%c)   ",temp->frequency, temp->harf);   
        	if(temp->left) {   
            	front = front+1;
            	queue[front] = temp->left;  
        	}
        	if(temp->right) {   
            	front = front+1;
            	queue[front] = temp->right; 
        	}
        }
	}
}

int main () {
    node *root = NULL, *root2, *root3=NULL;
    char string[150], ch;
    int n=0, i, h, secenek=3, c = 0, frequency[27] = {0};
    FILE *fp;
    
    while (secenek != 0 && secenek != 1){
		printf("String olarak okumak icin 0'a, 'input.txt' dosyasindan okumak icin 1'e basiniz!\n");
		scanf("%d",&secenek);
		if ( secenek != 0 && secenek != 1)
			printf("\nHatali bir giris yaptiniz! Lutfen tekrar deneyiniz!\n");
	}
    
    if (secenek == 0){
    	ch = getchar();
    	printf("Bir String giriniz:");
		gets(string);
	}
	
	else{
		fp = fopen("input.txt","r");  
		if (fp == NULL){
			printf("Dosya acilamadi!");
			return 0;
		}
		else{
			do {
				fgets(string,150,fp);	
  			} while (!feof(fp));
		}
	fclose (fp);	
	}
    
    while (string[c] != '\0') {  
        if ((string[c] >= 'a' && string[c] <= 'z') )  
            frequency[string[c] - 'a']++;
        else if (string[c] == ' ')  
        	frequency[26]++;
        c++;
    }
    for (i = 0; i < 26; i++) {  
        if (frequency[i] != 0)
            ekle(&root, i+'a', frequency[i]);
    }
    if (frequency[26] != 0) 
    	ekle(&root,'_',frequency[26]);
    
    printf("\n");
    insertion_sort(&root);  
    root = huffman(root);   
    bastir(root); 
    
    return 0;
}
