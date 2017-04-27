#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void text_color(int color){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

typedef struct node{
    int start_addr;
    int mem_size;
    struct node *next;
}*mem_node, *link_list;

//function declaration
link_list create_list();
//void insert_node(mem_node head, mem_node new_node);
int is_valid(int start_addr, int mem_size);
//void merge_mem(int start_addr, int mem_size);
void print_status();

//global variable definition
link_list available_mem, occupied_mem;


link_list create_list(){
    mem_node head = (mem_node)malloc(sizeof(struct node));
    if(head == NULL){
        text_color(0x0c);
        printf("Failed to allocate memory.\n");
        text_color(0x07);
    }
    else{
        head->next = NULL;
    }
    return head;
};

void insert_node(mem_node head, int start_addr, int mem_size){
    mem_node tmp = head;
    while(tmp->next != NULL){
        tmp = tmp->next;
    }

    mem_node new_node = (mem_node)malloc(sizeof(struct node));
    new_node->mem_size = mem_size;
    new_node->start_addr = start_addr;

    tmp->next = new_node;
    new_node->next = NULL;
}

void set_node(mem_node node, int start_addr, int mem_size){
    node->start_addr = start_addr;
    node->mem_size = mem_size;
}

void create_partitions(){
    int start_addr, mem_size;
    while(1){
        printf("Please type in the info:\n");
        printf("Partition Start Address:");
        scanf("%d", &start_addr);
        printf("         Partition size:");
        scanf("%d", &mem_size);
        printf("\n");
        if(is_valid(start_addr, mem_size)){
            /*
            if(merge_mem(start_addr, mem_size) == 1){

            }
            else if(merge_mem(start_addr, mem_size) == 2){
                insert_node(available_mem, start_addr, mem_size);
            }
            else if(merge_mem(start_addr, mem_size) == 3){

            }
            else{
                insert_node(available_mem, start_addr, mem_size);
            }
            */
            insert_node(available_mem, start_addr, mem_size);
            int i, length = get_length(available_mem);
            for(i = 0; i < length; i ++)
                merge_mem();
            text_color(0x0a);
            printf("Success to create a partition\n");
            break;
        }

    }


}

/*delete the current node*/
void delete_node(mem_node current){
    if(current != NULL){
        current->next = current->next->next;
    }
    else{
        text_color(0x0c);
        printf("No partition to delete.");
        text_color(0x07);
        exit(-1);
    }
}

/** 1~1, 3~5,
    now create a partition
    2~2(true)
    2~3(false)
    2~4(false)
    2~5(false)
    2~6(should be false, but it is true)
**/

int is_valid(int start_addr, int mem_size){
    mem_node tmp = available_mem;
    while(tmp->next != NULL){
        tmp = tmp->next;
        if(start_addr >= tmp->start_addr && start_addr < tmp->start_addr + tmp->mem_size || start_addr + mem_size - 1 < tmp->start_addr + tmp->mem_size && start_addr + mem_size - 1 >= tmp->start_addr){
            text_color(0x0c);
            printf("Invalid start address or size, please try again.\n");
            text_color(0x07);
            print_status();
            return 0;
        }
    }
    return 1;
}

void merge_mem(){
    mem_node tmp_i= available_mem;
    mem_node tmp_j= available_mem;
    mem_node tmp_a= available_mem;
    mem_node tmp_b= available_mem;
    while(tmp_i->next != NULL){
        tmp_a = tmp_i;
        tmp_i = tmp_i->next;
        while(tmp_j->next != NULL){
            tmp_b = tmp_j;
            tmp_j = tmp_j->next;
            if(tmp_i->start_addr + tmp_i->mem_size  == tmp_j->start_addr){
                tmp_i->mem_size += tmp_j->mem_size;
                delete_node(tmp_b);
            }
            else if(tmp_j->start_addr + tmp_j->mem_size  == tmp_i->start_addr){
                tmp_j->mem_size += tmp_i->mem_size;
                delete_node(tmp_a);
            }
        }


    }
}

void allocate_mem(){

}

void sort_mem(){

}

void print_status(){
    mem_node tmp = available_mem;
    printf("\n");
    text_color(0x0e);
    printf("%d available partition(s) in total.\n", get_length(available_mem));
    printf("----------------------------------------------\n");
    printf("--- START-ADDRESS\tSIZE\tRANGE      ---\n");
    //printf("-----------------------------------------\n");
    while(tmp->next != NULL){
        tmp = tmp->next;
        printf("--- %-12d\t%-4d\t%-4d~%4d  ---\n", tmp->start_addr, tmp->mem_size, tmp->start_addr, tmp->start_addr + tmp->mem_size - 1);
       // printf("-----------------------------------------\n");
    }
    printf("----------------------------------------------\n\n");
    text_color(0x07);

}

int get_length(link_list head){
    int length = 0;
    mem_node tmp = available_mem;
    while(tmp->next != NULL){
        tmp = tmp->next;
        length++;
    }
    return length;
}

void print_menu(){
    text_color(0x0b);
    printf("------ Memory Management Simulation Program ------\n");
    printf("---    1. New available partition(s)           ---\n");
    printf("---    2. Allocate to process                  ---\n");
    printf("---    3. Free memory                          ---\n");
    printf("---    q. Exit                                 ---\n");
    printf("------ ------------------------------------ ------\n");
    text_color(0x07);
    printf("Please choose an option: ");
}

int main()
{
    //text_color(0x0c);
    //mem_node tmp = (mem_node)malloc(sizeof(struct node));
    available_mem = create_list();
    char option;
    while(1){
        print_menu();
        fflush(stdin);
        scanf("%c", &option);

        if(option == '1'){
            //int num;
            //printf("Please type in the number of partitions: ");
            //scanf("%d", &num);
            printf("\n");
            //int i;
            //for(i = 0; i < num; i++)
                create_partitions();

            text_color(0x0a);
            //printf("Success to create %d partition(s).\n", num);

            text_color(0x07);
        }
        else if(option == '2'){

        }
        else if(option == '3'){

        }
        else if(option == 'q'){
            break;
        }

        print_status();
    }





    //printf("Hello world!\n");
    return 0;
}
