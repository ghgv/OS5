#include <kheap.h>
#include <stdio.h>
#include <string.h>


unsigned char *Root,*End;
chunk_t root_chunk;
extern uint32_t *topend;

unsigned char* malloc(int amount){
	unsigned char *result;
	chunk_t chunk;
	chunk.size=amount;
	chunk.free=0xA;
    
#ifdef _DEBUG_
	printf("   New chunk:\n");
	printf("   *size: %l\n",chunk.size=amount);
    printf("   +size: %l\n",chunk.size=amount);
#endif
	result=process(chunk);
	return result;
    
}

uint32_t* kmalloc_a(long amount, uint32_t * phys){
    
	uint32_t *result;
    uint32_t temp;
    
    printf("Allocating: %l bytes. 0x%x bytes\n",amount,amount);
    temp=(uint32_t)topend & 0xFFFFF000;
    temp+=0x1000;
    result=(uint32_t *)temp;
    if(phys)
        phys=topend;
    topend+=amount;  //New topend of  kernel memory
	return result;
    
}

void free(unsigned char *y){
#ifdef _DEBUG_
    printf("\nIn freeing  y= 0x%X\n",y);
#endif
    if (y!=0)
    {    
        chunk_t *prev_chunk;
        chunk_t *next_chunk;
        chunk_t *current_chunk;
        chunk_t *temp;
        unsigned char *p;
        p=y-sizeof(chunk_t);
        current_chunk=(chunk_t *)p;
        current_chunk->free=0;
#ifdef _DEBUG_
        printf("\nFreeing variable = 0x%X Ptr = 0x%X sizeof %x\n",y,p,sizeof(chunk_t));
#endif
        prev_chunk=(chunk_t *)current_chunk->prev;
        next_chunk=(chunk_t *)current_chunk->next;
        if(prev_chunk->free==0){
#ifdef _DEBUG_
            printf("Colapsing prev into current_chunk..\n\n");
#endif            
            prev_chunk->next=(unsigned char *)current_chunk->next;
            next_chunk->prev=(unsigned char *)prev_chunk;
            prev_chunk->size=prev_chunk->size+current_chunk->size+sizeof(chunk_t);
        }
        if(next_chunk->free==0){
#ifdef _DEBUG_
            printf("Colapsing next into current_chunk..\n\n");
#endif
            prev_chunk->next=(unsigned char *)next_chunk->next;
            prev_chunk->size=prev_chunk->size+current_chunk->size+sizeof(chunk_t)+next_chunk->size+sizeof(chunk_t);
        }
        unsigned char *s=0;
        y=(unsigned char *)s;
        printf(" exit 0x%x\n",y);
    }
    else if(y==0)
        printf(" Fault, NULL pointer!!!");
    //pointers();
	
}

void initheap(unsigned char *root, unsigned char *end){
	Root=(unsigned char *)(((unsigned long )root & 0xFFFF000));
    Root+=0x1000;
	End=end;
    topend=(uint32_t *)Root;
#ifdef _DEBUG_
    printf("Top End: 0x%X byte\n",topend);
	printf("Init Heap\n");
    printf("End of code:    0x%x\n",root);
    printf("Root            0x%x\n",Root);
    printf("End             0x%X\n",end);
#endif
    
	root_chunk.size=(End-Root);
#ifdef _DEBUG_
    printf("Size     %l\n",root_chunk.size);
#endif
    root_chunk.prev=Root;// & (unsigned char *)0xFFFFF000;
	root_chunk.next=End;
	root_chunk.free=0;
    
#ifdef _DEBUG_
	printf("Size of header %i\n",sizeof(chunk_t));
#endif
    memset((unsigned char *)root_chunk.prev,0,root_chunk.size);
    memcpy(root_chunk.prev,(unsigned char *)&root_chunk,sizeof(chunk_t));
#ifdef _DEBUG_
    printf("sizeof(chunk):0x%X\n",sizeof(chunk_t));   
    printf("sizeof(int):0x%X\n",sizeof(int));   
    printf("sizeof(short):0x%X\n",sizeof(short));   
    printf("sizeof(unsigned char *):0x%X\n",sizeof(unsigned char *));   
    printf("Root chunk:\n");  
	printf("   Root SP=        0x%x\n",(unsigned int *)Root);
	printf("   Root size=      %l bytes\n",root_chunk.size);
	printf("   Free:           %i\n",root_chunk.free);
	printf("   Root prev=      0x%x\n",(unsigned int *)root_chunk.prev);
	printf("   Root next=      0x%x\n",(unsigned int *)root_chunk.next);
#endif
	unsigned char *p=root_chunk.prev;

	
}

unsigned char *process(chunk_t new_chunk){
	chunk_t *iter;
	chunk_t *proximo;
	unsigned char* result;
	iter=(chunk_t *)root_chunk.prev;
#ifdef _DEBUG_
	printf("   Iter chunk:\n");
	printf("   iter size= %i bytes\n",iter->size);
	printf("   Free: %i\n",iter->free);
	printf("   ither prev= 0x%x\n",(unsigned int *)iter->prev);
	printf("   ither next= 0x%x\n",(unsigned int *)iter->next);
	printf("   New chunk:\n");
	printf("   new size= %i bytes\n",new_chunk.size);
	printf("   Free: %i\n",new_chunk.free);
	printf("   new prev= 0x%x\n",(unsigned int *)new_chunk.prev);
	printf("   new next= 0x%x\n",(unsigned int *)new_chunk.next);
#endif
    
	do{
		if(iter->size>(sizeof(chunk_t)+2*new_chunk.size) && iter->free==0){
#ifdef _DEBUG_            
			printf("allocating at 0x%x\n",(unsigned int *)iter);
			printf("	free %i -> ",iter->free);
#endif
			void * temp_next_ptr;
			void *temp_prev_prt;
			int temp_size;
			temp_next_ptr=iter->next;
			temp_prev_prt=iter->prev;
			temp_size=iter->size;
			iter->free=0x01;
#ifdef _DEBUG_
            printf("	free %i\n",iter->free);
			printf("	size %i -> ",iter->size);
#endif
            
			iter->size=new_chunk.size;
#ifdef _DEBUG_
            printf("	size %i\n",iter->size);
			printf("	next 0x%x -> ",(unsigned int *)iter->next);
#endif
			iter->next=(unsigned char *)iter+sizeof(chunk_t)+new_chunk.size;
#ifdef _DEBUG_
            printf("	next 0x%x\n",iter->next);
			printf("	prev 0x%x \n",(unsigned int *)iter->prev);
#endif
			result=(unsigned char *)&iter->next+sizeof(unsigned char *);
			proximo=(chunk_t *)iter->next;
			proximo->prev=(unsigned char *)iter;
			proximo->next=(unsigned char*)temp_next_ptr;
			proximo->free=0;
			proximo->size=temp_size-sizeof(chunk_t)-iter->size;
            
			break;	
		}
		iter=(chunk_t *)iter->next;
#ifdef _DEBUG_
		printf("\n-->	Next chunk address: 0x%x\n",iter);
		printf("	next chunk free: %i\n",iter->free);
		printf("	next chunk size: %i\n",iter->size);
		printf("	next chunk next: 0x%x\n",(unsigned int *)iter->next);		
#endif
	}while(1);

/*	
	if(iter->free==7){
			iter->free=12;
			new_chunk.next=iter->next;
			new_chunk.prev=iter->prev;
			new_chunk.free=7;
			iter->next=iter->prev+sizeof(chunk_t)+1;
			iter->size=new_chunk.size;
			memcpy(iter->prev+sizeof(chunk_t)+iter->size,&new_chunk,sizeof(chunk_t));
			
	}	
*/
	pointers();
	
	return result;
}

void pointers(){
    chunk_t *p;
	int k=0;
	p=(chunk_t *)root_chunk.prev;
	for(p=(chunk_t *)root_chunk.prev;p<(chunk_t *)root_chunk.next;)
		{
#ifdef _DEBUG_			
			printf("Chunk Number[%i]:\n",k);
			printf("	SP= 		0x%X \n",(unsigned int *)p);
			printf("	iter size= 	%i bytes\n",p->size);
			printf("	Free: 		%i\n",p->free);
            printf("	prev= 		0x%X\n",(unsigned int *)p->prev);
            printf("	next= 		0x%X [0x%X]\n",(unsigned int *)p->next,&p->next);
#endif
            long f=(long int)(&p->next);
            f+=sizeof(unsigned char *); //it looks like the substraction is not working correctly
#ifdef _DEBUG_
            printf("	Variable= 	0x%X \n",f);
#endif
            p=(chunk_t *)p->next;
        k++;
        }
    
}


