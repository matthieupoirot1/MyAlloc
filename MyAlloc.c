#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))
#define ENTETE_SIZE (ALIGN(sizeof(bloc_entete)))
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef struct bloc_entete {
	size_t taille;
	struct bloc_entete* precedent;
	struct bloc_entete* suivant;
} bloc_entete;

static void* first = NULL;
static bloc_entete* lastFree = NULL;

void blocinfo(void* ptr) {
	bloc_entete* bloc_total= (bloc_entete*) ((char*)ptr - ENTETE_SIZE);
	printf("** Infos bloc **\n\
  - pointeur bloc %p \n\
  - pointeur donnees %p \n\
  - taille totale: %lu\n\
  - precedent : %p\n\
  - suivant : %p\n",
  	bloc_total,
	ptr,
	bloc_total->taille,
	bloc_total->precedent,
	bloc_total->suivant
	);
	//bloc_total->libre
}

void *myalloc(size_t t) {
	int trouve = 0;
	bloc_entete* ptr = lastFree;
	bloc_entete* bestFit = lastFree;
	
	if (first == NULL){ //Si c'est le premier on creer normal
		void* ptr = sbrk(ENTETE_SIZE+ALIGN(t));
		bloc_entete* blocptr = (bloc_entete*)ptr;
 		blocptr->taille=ENTETE_SIZE+ALIGN(t);
		return ptr+ENTETE_SIZE;
	}
	else{
		if(lastFree!=NULL){			
			while(((void*)ptr)<sbrk(0))
			{
				if((ptr->taille)=ALIGN(t)) //si trouve on utilise celui la
				{
					//ptr->taille=ALIGN(t)+ENTETE_SIZE;
					printf("%p",ENTETE_SIZE+ptr);
					bestFit->precedent->suivant = bestFit->suivant;
					bestFit->suivant->precedent = bestFit->precedent;
					bestFit->precedent = NULL;
					bestFit->suivant = NULL;
					return ptr+ENTETE_SIZE;
				}
				else if((ptr->taille)>(ALIGN(t))&&(ptr->taille)<(bestFit->taille)){
					bestFit=ptr;
					trouve = 1;
				}
				ptr = ptr->precedent;	
			}
		}
		if (trouve=0){
			void* newPtr = sbrk(ENTETE_SIZE+ALIGN(t)); //si pas trouve on met au bout
			bloc_entete* blocptr = (bloc_entete*)newPtr;
			blocptr->taille=ENTETE_SIZE+ALIGN(t);
			return newPtr+ENTETE_SIZE;
		}
		else{
			if ((bestFit->taille - ALIGN(t))>ENTETE_SIZE){
				bestFit->taille = ALIGN(t);
				bloc_entete* newBloc = (((void*)bestFit)+ALIGN(t));
				bestFit->precedent->suivant = newBloc;
				newBloc->precedent = bestFit->precedent;
				newBloc->suivant = bestFit->suivant;
				if(newBloc->suivant == NULL){
					lastFree = newBloc;
				}
			}
				bestFit->precedent->suivant = bestFit->suivant;
				bestFit->suivant->precedent = bestFit->precedent;
				bestFit->precedent = NULL;
				bestFit->suivant = NULL;
			return bestFit;
		}
	}
	
}

void myfree(void *ptr) {
	bloc_entete* blocptr = (bloc_entete*)(ptr-ENTETE_SIZE);
	if(lastFree==NULL){
			lastFree=blocptr;
	}
	else{
		blocptr->precedent = lastFree;
		lastFree->suivant=blocptr;
		lastFree=blocptr;
		
		
	}
		
	 
}



int main() {
	void *p1 = myalloc(235);
	void *p2 = myalloc(190);
	void *p3 = myalloc(150);
	blocinfo(p1);
	blocinfo(p2);
	blocinfo(p3);
	myfree(p2);
	myfree(p3);
	blocinfo(p1);
	blocinfo(p2);
	blocinfo(p3);
}


