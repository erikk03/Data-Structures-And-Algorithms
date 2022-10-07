#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ADTQueue.h"

int main(int argc, char *argv[]){
    
    // Βασικοι ελεγχοι και διαδικασιες
    if(argc == 1){
        printf("No aguments were found\n");
        exit(EXIT_FAILURE);
    }
    int size = strlen(argv[1]);
    char A[size];
    strcpy(A, argv[1]);
    FILE *output = fopen("output.csv", "w");
    if(output == NULL){
        printf("The file.csv you are searching could not be found!\n");
        return 0;
    }
    
    // Αρχη της βασικης λειτουργιας
    Queue queue = queue_create(NULL);
    if(strcmp(A, "real") == 0){                             // Εαν το argument ειναι "real"        
        int r_steps;                                        // Βηματα που εκανε η λειτουργια queue_remove_front καθε συγκεκριμενη τιμη 
        for(int i=0; i<9000; i+=3){
            queue_insert_back(queue, &i);
            queue_insert_back(queue, &i);
            queue_remove_front(queue);
            r_steps = queue_steps(queue);                   // Κληση queue_steps, επιστρεφει τα βηματα της queue_remove_front
            fprintf(output, "%d,%d\n", i+3, r_steps);       // Εκτυπωση αποτελεσματων στο αρχειο output.csv
        }
    }
    else if(strcmp(A, "amortized") == 0){                   // Εαν το argument ειναι "amortized"
        int a_steps = queue_steps(queue);                   // Αρχικοποιηση βηματων
        for(int i=0; i<9000; i+=3){
            queue_insert_back(queue, &i);
            a_steps += queue_steps(queue);                  // Προσθηκη επιπλεον βηματων που εχουν πραγματοποιηθει σε καθε λειτουργια
            fprintf(output, "%d,%d\n", i+1, a_steps/(i+1)); // Εκτυπωση καθε φορα (συνολικα βηματα μεχρι τωρα/συνολικες λειτουργιες μεχρι τωρα)
            queue_insert_back(queue, &i);
            a_steps += queue_steps(queue);
            fprintf(output, "%d,%d\n", i+2, a_steps/(i+2));
            queue_remove_front(queue);
            a_steps += queue_steps(queue);
            fprintf(output, "%d,%d\n", i+3, a_steps/(i+3));
        }
    }
    else{                                                   // Εαν το argument ειναι λανθασμενο
        printf("The argument given is not right!\n");
    }
    fclose(output);
    exit(EXIT_SUCCESS);
}
