#include<stdio.h>
#include<stdlib.h>
struct node{char N[25];int df;struct node *pc;struct node *ps;};struct node *A[20];int in = 0, c = 0;void create(struct node *P, int N){int i;struct node *Tmp, *T;Tmp = P;
for(i=0;i<N;i++){T=malloc(sizeof(struct node));printf("Enter name:");scanf("%s", T->N);printf("Enter dir(1) or file(0):");scanf("%d", &T->df);if(T->df==1){A[c]=T;c++;}T->pc=NULL;T->ps=NULL;if(i==0){Tmp->pc=T;Tmp=T;}else{Tmp->ps=T;Tmp=T;}}}void display(struct node *P){int i;P=P->pc;do{printf("\n%s(%d)",P->N,P->df);if(P->df==1 && P->pc != NULL)display(P);P=P->ps;}while(P!=NULL);}main(){int nu,nc;int i,j,k;struct node *Hdr;Hdr=malloc(sizeof(struct node));Hdr->df=1;Hdr->pc=NULL;Hdr->ps=NULL;printf("Enter number of users:");scanf("%d", &nu);create(Hdr, nu);for(in=0;in<c;in++){printf("\nEnter number of child nodes for %s:", A[in]->N);scanf("%d",&nc);create(A[in],nc);
}printf("\nHierarchical\n");display(Hdr);}
