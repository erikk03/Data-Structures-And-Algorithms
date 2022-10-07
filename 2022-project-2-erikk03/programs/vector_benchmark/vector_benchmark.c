#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ADTVector.h"

int main(int argc, char *argv[]){
    
    // Βασικοι ελεγχοι και διαδικασιες
    if(argc == 1){
        printf("No aguments were found\n");
        exit(EXIT_FAILURE);
    }
    int size = strlen(argv[1]);
    char A[size];
    strcpy(A, argv[1]);
    FILE *vector_output = fopen("vector_output.csv", "w");
    if(vector_output == NULL){
        printf("The file.csv you are searching could not be found!\n");
        return 0;
    }
    
    // Αρχη της βασικης λειτουργιας
    Vector vec = vector_create(0, NULL);
    if(strcmp(A, "real") == 0){                             // Εαν το argument ειναι "real"        
        int r_steps;                                        // Βηματα που εκανε η λειτουργια vector_remove_last καθε συγκεκριμενη τιμη 
        for(int i=0; i<9000; i+=3){
            vector_insert_last(vec, &i);
            vector_insert_last(vec, &i);
            vector_remove_last(vec);
            r_steps = vector_steps(vec);                    // Κληση vector_steps, επιστρεφει τα βηματα της vector_remove_last
            fprintf(vector_output, "%d,%d\n", i+3, r_steps);   // Εκτυπωση αποτελεσματων στο αρχειο vector_output.csv
        }
    }
    else if(strcmp(A, "amortized") == 0){                   // Εαν το argument ειναι "amortized"
        int a_steps = vector_steps(vec);                    // Αρχικοποιηση βηματων
        for(int i=0; i<9000; i+=3){
            vector_insert_last(vec, &i);
            a_steps += vector_steps(vec);                   // Προσθηκη επιπλεον βηματων που εχουν πραγματοποιηθει σε καθε λειτουργια
            fprintf(vector_output, "%d,%d\n", i+1, a_steps/(i+1)); // Εκτυπωση καθε φορα (συνολικα βηματα μεχρι τωρα/συνολικες λειτουργιες μεχρι τωρα)
            vector_insert_last(vec, &i);
            a_steps += vector_steps(vec);
            fprintf(vector_output, "%d,%d\n", i+2, a_steps/(i+2));
            vector_remove_last(vec);
            a_steps += vector_steps(vec);
            fprintf(vector_output, "%d,%d\n", i+3, a_steps/(i+3));
        }
    }
    else{                                                   // Εαν το argument ειναι λανθασμενο
        printf("The argument given is not right!\n");
    }
    fclose(vector_output);
    exit(EXIT_SUCCESS);
}
