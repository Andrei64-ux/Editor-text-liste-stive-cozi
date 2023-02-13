#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Doubly_linked_list.h"
#include "Stack.h"
#include "Node.h"
#define BUFF_SIZE 1024

//enum care contine cele 2 moduri in care ne putem afla pentru a edita textul
typedef enum {INSERT_COMMAND , INSERT_TEXT} modes;

/*structura care defineste o stare si contine doar elementele care se pot 
  modifica in cursul executarii programului*/
typedef struct State_T{
    DoublyLinkedList *document;
    int cursor , line , mode;
}State;

int max(int a , int b)
{
    if(a>b) return a;
    return b;
}

//functie care creaza si adauga o linie sub forma unei liste dublu inlantuite
DoublyLinkedList *create_line(char *buff) {
    DoublyLinkedList *new_line = initList(sizeof(char));
    int i;
    for(i = 0 ; buff[i] != '\n' ; i++)
        push_back(new_line , &buff[i]);
    return new_line;
}

//recalculeaza dimensiunea pentru o linie , definita ca lista 
void resize_list(DoublyLinkedList *line)
{
    line->size = 0;
    Node *it;
    for(it = line->head ; it != NULL ; it=it->next)
        line->size++;
}


//returneaza o linie din cadrul documentului dat
DoublyLinkedList *get_line(DoublyLinkedList *document, int index) {
    return (DoublyLinkedList *)getList(document, index)->val;
}

/*insereaza o linie in document in functie de linie , cursor , text 
 si de modul in care a fost inserata */
void insert_line(DoublyLinkedList *document, int *p_line, int *p_cursor, char *buff , int inserted) {
    DoublyLinkedList *new_line;
    int line = *p_line;
    int cursor = *p_cursor;
    int i;

    if(cursor == get_line(document, line)->size - 1) {
        new_line = create_line(buff);
        insert(document, line + 1, new_line);
        free(new_line);
        line++;
        cursor = get_line(document, line)->size - 1;
    } else {
        if(inserted > 0)
        {
            DoublyLinkedList *curr_line = get_line(document, line);
            DoublyLinkedList *new_line = initList(sizeof(char));
            Node *last_node = getList(curr_line, cursor);

            new_line->head = last_node->next;
            new_line->tail = curr_line->tail;
            resize_list(new_line);

            curr_line->tail = last_node;
            last_node->next = NULL;
            resize_list(curr_line);

            insert(document, line + 1, new_line);
            free(new_line);
            cursor = -1;
            line++;
        }
        for(i = 0; buff[i] != '\n'; i++)
        {
            insert(get_line(document, line), cursor + 1, &buff[i]);
            cursor++;
        }
    }
    *p_line = line;
    *p_cursor = cursor;
}

//creaza o copie a documentului
DoublyLinkedList *create_copy(DoublyLinkedList *document)
{
    DoublyLinkedList *copy = initList(sizeof(DoublyLinkedList));
    Node *it;
    Node *it_2;
    for(it = document->head ; it != NULL ; it=it->next)
    {
        DoublyLinkedList *line_copy = initList(sizeof(char));
        for(it_2 = ((DoublyLinkedList*)it->val)->head ; it_2 != NULL ; it_2 = it_2->next)
        {
            push_back(line_copy , (char*)it_2->val);
        }
        push_back(copy , line_copy);
        free(line_copy);
    }
    return copy;
}

//creaza o stare pentru document la un moment dat
State create_state(DoublyLinkedList *document , int line , int cursor , int mode)
{
    State curr_state;
    curr_state.cursor = cursor;
    curr_state.line = line;
    curr_state.mode = mode;
    curr_state.document = create_copy(document);
    return curr_state;
}

//sterge continutul si elibereaza memoria documentului
void delete_document(DoublyLinkedList *document)
{
    Node *it;
    for(it = document->head ; it != NULL ; it=it->next)
        clearList((DoublyLinkedList*)it->val);
    freeList(document);
}

//returneaza un caracter dereferentiat si castat la tipul char
char get_char(DoublyLinkedList *document , int line ,int cursor)
{
    return *(char*)(getList(get_line(document , line) , cursor)->val);
}

//gaseste un cuvant in document in functie de linia si pozitia unde ne aflam
int find_word(DoublyLinkedList *document , int line , int cursor , char *w)
{
    //printf("[LOGGER] Looking for word on line %d %d\n", line , cursor);
    int line_size = get_line(document , line)->size;
    //printf("[LOGGER] Lenght of line %d = %d\n", line , line_size);
    int len = strlen(w);
    int i;
    for(i=max(0,cursor) ; i< line_size ; i++)
    {
        int l=0;
        while(i+l < line_size && l < len && w[l] == get_char(document , line , i+l))
            l++;
        if( l==len )
            return i;
    }
    return -1;
}

//inlocuiete un cuvant vechi cu unul nou
void replace_word(DoublyLinkedList *document , int line , int cursor , char *w , int l)
{
    int i;
    for(i=0; i<l ; i++)
        erase(get_line(document,line) , cursor);
    for(i=0; w[i] != '\0' ; i++)
    {
        insert(get_line(document , line) , cursor , &w[i]);
        cursor++;
    }
}

//sterge liniile goale din document
void delete_emptylines(DoublyLinkedList *document)
{
    int i;
    for(i=0; i<document->size ; i++)
    {
        if(get_line(document,i)->size == 0)
        {
            erase(document,i);
            i--;
        }
    }
}

/*in main am realizat comenzile descrise in pdf si am efectuat prelucrarile
  necesare asupra documentului . Pentru a recunoaste o comanda am folosit un 
  sir de caractere si am folosit functia pentru a compara un numar dat de caractere
  "strncmp" . Am deschis si inchis fisierele , am eliberat memoria . Printf-urile 
  comentate sunt pentru a-mi verifica programul pe parcursul executiei . Mai multe
  detalii legate de implementare si de modul in care am decis sa rezolv tema se regasesc 
  in README.*/
int main()
{
    FILE *input_file;
    FILE *output_file;
    input_file = fopen("editor.in","rt");
    if(input_file == NULL)
    {
        printf("[LOGGER] Could not open editor.in!\n");
    }
    int mode = INSERT_TEXT , stop=0;
    int h_index = 0;
    Node *it;
    Node *it_2;
    State curr_state;
    Stack *history = initStack(sizeof(State));
    DoublyLinkedList *document = initList(sizeof(DoublyLinkedList));
    int line = -1, cursor = -1;
    int i , inserted = 0;
    char buff[BUFF_SIZE];
    char old_w[BUFF_SIZE] , new_w[BUFF_SIZE];
    curr_state = create_state(document , line , cursor , mode);
    push(history , &curr_state);
    while( !stop )
    {
        delete_emptylines(document);
        if(fgets(buff , BUFF_SIZE , input_file) == NULL)
        {
            stop=1;
            //printf("[LOGGER] Reached EOF!\n");
            continue;
        }
        //printf("[LOGGER] Read %s\n" , buff);

        if(strncmp(buff,"::i",3) == 0)
        {
            if(mode == INSERT_TEXT)
            {
                mode = INSERT_COMMAND;
                //printf("[LOGGER] Changed to Insert_command\n");
            }
            else
            {
                mode = INSERT_TEXT;
                inserted = 0;
                //printf("[LOGGER] Changed to Insert_text\n");
                continue;
            }
        } else if(mode == INSERT_TEXT)
        {
            if(document->size == 0) {
                DoublyLinkedList *tmp = create_line(buff);
                push_back(document, tmp);
                free(tmp);
                line = 0;
                cursor = get_line(document, line)->size - 1;
            } else {
                insert_line(document, &line , &cursor, buff , inserted);
            }
            inserted++;
            //printf("[LOGGER] Insert line\n");
            continue;
        } else if(strncmp(buff,"s",1) == 0)
        {
            output_file = fopen("editor.out","wt");
            for(it = document->head ; it != NULL ; it=it->next)
            {
                for(it_2 = ((DoublyLinkedList*)it->val)->head ; it_2 != NULL ; it_2=it_2->next)
                    fprintf(output_file , "%c" , *(char*)it_2->val);
                fprintf(output_file,"\n");
            }
            while( history->size > 0 )
            {
                delete_document(((State*)history->top->val)->document);
                pop(history);
            }
            h_index = -1;
            //printf("[LOGGER] Saved document\n");
            fclose(output_file);
        }else if(strncmp(buff,"q",1) == 0)
        {
            delete_document(document);
            while( history->size > 0 )
            {
                delete_document(((State*)history->top->val)->document);
                pop(history);
            }
            freeStack(history);
            h_index = -1;
            //printf("[LOGGER] Quiting\n");
            fclose(input_file);
            return 0;
        }else if(strncmp(buff,"b",1) == 0)
        {
            //printf("[LOGGER] Backspace at index %d\n", cursor);
            if(cursor != -1) {
                erase(get_line(document, line), cursor);
                cursor--;
            }
        }else if( strncmp(buff,"gl",2) == 0)
        {
            sscanf(buff+3 , "%d" , &line);
            cursor = -1;
            line--;

        }else if( strncmp(buff,"gc",2) == 0)
        {
            if( sscanf(buff+3 , "%d %d" , &cursor , &line ) == 2)
                line--;
            cursor--;
        }
        else if( strncmp(buff,"dl",2) == 0 )
        {
            if( sscanf(buff+3 , "%d" , &line) == 1)
                 line--;
            DoublyLinkedList *curr_line = get_line(document, line);
            clearList(curr_line);
            erase(document,line);
        }else if( strncmp(buff , "dw" , 2) == 0)
        {
            sscanf( buff+3 , "%s" , old_w);
            int pos = find_word(document , line , cursor , old_w);
            if(pos != -1)
            {
                replace_word(document , line , pos , "" , strlen(old_w));
                //printf("[LOGGER] Word found at position %d %d\n", line , pos);
            }
            else{
                for(i = line+1 ; i<document->size ; i++)
                {
                    pos = find_word(document , i , 0 , old_w);
                    if(pos != -1)
                    {
                        replace_word(document , i , pos , "" , strlen(old_w));
                        //printf("[LOGGER] Word found at position %d %d\n", i , pos);
                        break;
                    }
                }
                if( pos == -1)
                {
                    for(i = 0 ; i<line ; i++)
                    {
                        pos = find_word(document , i , 0 , old_w);
                        if(pos != -1)
                        {
                            replace_word(document , i , pos , "" , strlen(old_w));
                            //printf("[LOGGER] Word found at position %d %d\n", i , pos);
                            break;
                        }
                    }
                }
            }
        }else if (strncmp(buff,"da",2) == 0)
        {
            sscanf( buff+3 , "%s" , old_w );
            int pos ;
            for(i = 0 ; i<document->size ; i++)
            {
                pos = find_word(document , i , 0 , old_w);
                while( pos != -1 )
                {
                    replace_word(document , i , pos , "" , strlen(old_w));
                    //printf("[LOGGER] Word found at position %d %d\n", i , pos);
                    pos = find_word(document , i , 0 , old_w);
                }
            }
        } else if( strncmp(buff,"d",1) == 0)
        {
            int cnt = 1;
            sscanf(buff+2 , "%d" , &cnt);
            for(i=1; i<=cnt ;i++)
            {
                 erase(get_line(document , line) , cursor + 1);
            }
        } else if (strncmp(buff, "u", 1) == 0) {
            h_index--;
            curr_state = *(State *)getStack(history, h_index)->val;
            delete_document(document);
            document = create_copy(curr_state.document);
            line = curr_state.line;
            cursor = curr_state.cursor;
            mode = curr_state.mode;
            continue;
        } else if (strncmp(buff,"re",2) == 0)
        {
            sscanf( buff+3 , "%s%s" , old_w , new_w);
            int pos = find_word(document , line , cursor , old_w);
            if( pos != -1)
            {
                replace_word(document , line , pos , new_w , strlen(old_w));
                //printf("[LOGGER] Word found at position %d %d\n", line , pos);
            }
            else{
                for(i = line+1 ; i<document->size ; i++)
                {
                    pos = find_word(document , i , 0 , old_w);
                    if( pos != -1)
                    {
                        replace_word(document , i , pos , new_w , strlen(old_w));
                        //printf("[LOGGER] Word found at position %d %d\n", i , pos);
                        break;
                    }
                }
            }
        }else if (strncmp(buff,"ra",2) == 0)
        {
            sscanf( buff+3 , "%s %s" , old_w , new_w);
            int pos;
            for(i = 0 ; i<document->size ; i++)
            {
                pos = find_word(document , i , 0 , old_w);
                while( pos != -1 )
                {
                    replace_word(document , i , pos , new_w , strlen(old_w));
                    //printf("[LOGGER] Word found at position %d %d\n", i , pos);
                    pos = find_word(document , i , 0 , old_w);
                }
            }
        }else if (strncmp(buff, "r", 1) == 0)
        {
            h_index++;
            curr_state = *(State *)getStack(history, h_index)->val;
            delete_document(document);
            document = create_copy(curr_state.document);
            line = curr_state.line;
            cursor = curr_state.cursor;
            mode = curr_state.mode;
            continue;
        }
        while( h_index != history->size-1 )
        {
            delete_document(((State*)history->top->val)->document );
            pop(history);
        }
        curr_state = create_state(document , line , cursor , mode);
        push(history , &curr_state);
        h_index++;
     }
}
