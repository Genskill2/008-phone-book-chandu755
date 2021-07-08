#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const char *DB="directory.db";
struct entry0 {
  char name[20];
  char phone[20];
  struct entry0 *next;
};
typedef struct entry0 entry;
/* Command handlers */
void add(char *, char *);
int search(FILE *,char *);
void list(FILE *);
int delete(FILE *, char *);
/* Utility functions  */
FILE * open_db_file(); /* Opens the database file. Prints error and
                          quits if it's not available */
void print_usage(char *, char *);  /* Prints usage */
entry *load_entries(FILE *);         /* Load all entries from the
                                      database file. Returns pointer
                                      to first entry */
entry *create_entry_node(char *, char *);  /* Create a new entry
                                              node. Has to be freed by
                                              user. */
void free_entries(entry *); /* TBD Given the first node of a linked list
                               of entries, will free all the nodes */ 
void write_all_entries(entry *); /* Given the first node of a linked
                                    list of entries, will delete the
                                    database file on the disk and save
                                    the given entries into the file */
int main(int argc, char *argv[]) {
  if (argc == 1) {
    print_usage("Insufficient arguments", argv[0]);
    exit(1);
  } 
  if (strcmp(argv[1], "add") == 0) {   /* Handle add */
    if (argc != 4) {
      print_usage("Improper arguments for add", argv[0]);
      exit(1);
    }
    char *name = argv[2];
    char *phone = argv[3];
    add(name, phone);
    exit(0);
  } else if (strcmp(argv[1], "list") == 0) {  /* Handle list */
    if (argc != 2) {
      print_usage("Improper arguments for list", argv[0]);
      exit(1);
    }
    FILE *fp = open_db_file();
    list(fp);
    fclose(fp);
    exit(0);
  } else if (strcmp(argv[1], "search") == 0) {  /* Handle search */
    printf("NOT IMPLEMENTED!\n"); /* TBD  */
    if(argc != 3){
       print_usage("Improper arguments for search", argv[0]);
      exit(1);
    }/* TBD  */
    FILE *fp = open_db_file();
    char *name = argv[2];
    if (!search(fp, name)) {
      printf("no match\n");
      fclose(fp);
      exit(1);
    }
    fclose(fp);
    exit(0); 
  } else if (strcmp(argv[1], "delete") == 0) {  /* Handle delete */
    if (argc != 3) {
      print_usage("Improper arguments for delete", argv[0]);
@@ -94,7 +106,12 @@ FILE *open_db_file() {

void free_entries(entry *p) {
  /* TBD */
  printf("Memory is not being freed. This needs to be fixed!\n");  
 while(p!=NULL){
  free(p);
  p =p->next;
 }

  //printf("Memory is not being freed. This needs to be fixed!\n");  
}

void print_usage(char *message, char *progname) {
@@ -131,20 +148,17 @@ entry *load_entries(FILE *fp) {
  entry *tmp = NULL;
  /* Description of %20[^,\n]
     % is the start of the specifier (like %s, %i etc.)
     20 is the maximum number of characters that this will take. We
        know that names and phone numbers will be 20 bytes maximum so
        we limit it to that. %20s will read in 20 character strings
        (including the , to separate the name and phone number. That's
        why we use
    [^,\n] Square brackets are used to indicate a set of allowed
           characters [abc] means only a, b, or c. With the ^, it's
           used to specify a set of disallowed characters. So [^abc]
           means any character *except* a, b, or c. [^,] means any
           character except a , [^,\n] means any character except a
           comma(,) or a newline(\n).
    %20[^,\n] will match a string of characters with a maximum length
     of 20 characters that doesn't have a comma(,) or a newline(\n).
  */        
@@ -174,16 +188,37 @@ void add(char *name, char *phone) {
  fprintf(fp, "%s,%s\n", name, phone);
  fclose(fp);
}
int search(FILE *db_file,char *name){
  entry *p = load_entries(db_file);
  entry *base = p;
  int found = 0;
  while(p!=NULL){
  if(strcmp(p->name,name) == 0){
  printf("%s\n",p->phone);

  found = 1;
  }
   p=p->next;
  }
  free_entries(base);
  return found;
}

int size = 0;
void list(FILE *db_file) {
  entry *p = load_entries(db_file);
  entry *base = p;
  int num = 0;
  while (p!=NULL) {
    printf("%-20s : %10s\n", p->name, p->phone);
    num++;
    p=p->next;
  }
  /* TBD print total count */
  size =num;
  printf("Total entries :  %d\n",num);
  free_entries(base);

}


@@ -193,6 +228,7 @@ int delete(FILE *db_file, char *name) {
  entry *prev = NULL;
  entry *del = NULL ; /* Node to be deleted */
  int deleted = 0;

  while (p!=NULL) {
    if (strcmp(p->name, name) == 0) {
      /* Matching node found. Delete it from the linked list.
@@ -207,7 +243,23 @@ int delete(FILE *db_file, char *name) {
      */

      /* TBD */

       if(strcmp(base->name, name) == 0){
         base = p->next;
       }
      else{ p = p->next;

       prev->next = p;
          }

       deleted = 1;
       break;
    }
    else{
      prev = p;
      p = p->next;
    }

  }
  write_all_entries(base);
  free_entries(base);
