#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ADTSet.h"
#include "ADTVector.h"

// Δημιουργούμε μια ειδική compare συνάρτηση
int compare_ints(Pointer a, Pointer b) {
	return *(int*)a - *(int*)b;
}

//
//
int main(int argc, char *argv[]){
    
    // Βασικοι ελεγχοι και διαδικασιες
    if(argc == 1){
        printf("No aguments were found\n");
        exit(EXIT_FAILURE);
    }
    int size = strlen(argv[1]);
    char A[size];
    strcpy(A, argv[1]);
    FILE *set_output_trivial = fopen("set_output_trivial.csv", "w");
    FILE *set_output_VectorToBST = fopen("set_output_VectorToBST.csv", "w");
    if(set_output_trivial == NULL || set_output_VectorToBST == NULL){
        printf("The file.csv you are searching could not be found!\n");
        return 0;
    }
    
    // Αρχη της βασικης λειτουργιας
    Set set = set_create(compare_ints, NULL);
    if(strcmp(A, "real") == 0){                             // Εαν το argument ειναι "real"        
        int r_steps;                                        // Βηματα που εκανε η λειτουργια set_insert καθε συγκεκριμενη τιμη 
        for(int i=1; i<=9000; i++){
            set_insert(set, &i);
            r_steps = set_steps(set);                       // Κληση set_steps, επιστρεφει τα βηματα της set_insert
            fprintf(set_output_trivial, "%d,%d\n", i, r_steps);  // Εκτυπωση αποτελεσματων στο αρχειο vector_output.csv
        }
    }
    else if(strcmp(A, "amortized") == 0){                   // Εαν το argument ειναι "amortized"
        int a_steps;                                        // Αρχικοποιηση βηματων
        for(int i=1; i<=9000; i++){
            set_insert(set, &i);
            a_steps += set_steps(set);                       // Προσθηκη επιπλεον βηματων που εχουν πραγματοποιηθει σε καθε λειτουργια
            fprintf(set_output_trivial, "%d,%d\n", i, a_steps/i); // Εκτυπωση καθε φορα (συνολικα βηματα μεχρι τωρα/συνολικες λειτουργιες μεχρι τωρα)
        }
    }
    else{                                                   // Εαν το argument ειναι λανθασμενο
        printf("The argument given is not right!\n");
    }

    //
    //Λειτουργια με βαση την συναρτηση set_create_from_sorted_values
    Vector values = vector_create(0, NULL);
    Set set2 = set_create_from_sorted_values(compare_ints, NULL, values);
    if(strcmp(A, "real") == 0){                             // Εαν το argument ειναι "real"        
        int r_steps2;                                       // Βηματα που εκανε η λειτουργια set_insert καθε συγκεκριμενη τιμη 
        for(int i=1; i<=5000; i++){
            vector_insert_last(values, &i);
            r_steps2 = vector_size(values);
            fprintf(set_output_VectorToBST, "%d,%d\n", i, r_steps2);
        }                                                
        
        for(int i=5001; i<=9000; i++){
            set_insert(set2, &i);
            r_steps2 = set_steps(set2);                       // Κληση set_steps, επιστρεφει τα βηματα της set_insert
            fprintf(set_output_VectorToBST, "%d,%d\n", i, r_steps2);  // Εκτυπωση αποτελεσματων στο αρχειο vector_output.csv
        }
    }
    else if(strcmp(A, "amortized") == 0){                   // Εαν το argument ειναι "amortized"
        int a_steps2 = 0;                                  // Αρχικοποιηση βηματων
        for(int i=1; i<=5000; i++){
            vector_insert_last(values, &i);
            a_steps2 += vector_size(values);
            fprintf(set_output_VectorToBST, "%d,%d\n", i, a_steps2/i);
        }
        for(int i=5001; i<=9000; i++){
            set_insert(set2, &i);
            a_steps2 += set_steps(set2);                       // Προσθηκη επιπλεον βηματων που εχουν πραγματοποιηθει σε καθε λειτουργια
            fprintf(set_output_VectorToBST, "%d,%d\n", i, a_steps2/i); // Εκτυπωση καθε φορα (συνολικα βηματα μεχρι τωρα/συνολικες λειτουργιες μεχρι τωρα)
        }
    }
    else{                                                   // Εαν το argument ειναι λανθασμενο
        printf("The argument given is not right!\n");
    }
    fclose(set_output_VectorToBST);
    fclose(set_output_trivial);
    exit(EXIT_SUCCESS);
}