![run-tests](../../workflows/run-tests/badge.svg)

## 2022 Project 2

Εκφώνηση: https://k08.chatzi.org/projects/project2/


### Προσωπικά στοιχεία

__Όνομα__: ΕΡΙΚ ΚΑΓΙΑΤΣΚΑ

__Α.Μ.__: sdi2100043

### Ασκήσεις που παραδίδονται

- Ασκηση_1
- Ασκηση_2
- Ασκηση_3
    - Bonus_ασκ3
- Ασκηση_4
    - Bonus1_ασκ4

### Documentation
_________
_Ασκηση1_

Η λειτουργια queue_remove_front εχει πολυπλοκοτητα Ο(n).
Οι υπολοιπες λειτουργιες εχουν πολυπλοκοτητα O(1).
_________
_Ασκηση2_

Η λειτουργια queue_remove_front, οταν η 2η στοιβα ειναι κενη, εχει πολυπλοκοτητα Ο(n) επειδη γινεται αντιγραφη ολων των στοιχειων απο την 1η στη 2η στοιβα. Οταν υπαρχουν στοιχεια στη 2η στοιβα, αφαιρειται το στοιχειο στην κορυφη, οποτε εχει πολυπλοκοτητα Ο(1).
Επομενως, μπορουμε να πουμε οτι η υλοποιηση ADTQueue_alt ειναι πιο αποδοτικη οσων αφορα την λειτουργια queue_remove_front.
_________
_Ασκηση3_

GRAPHS:
QUEUE_BENCHMARK_USING_LIST:
1. Πολυπλοκοτητα Ο(1)
![queue_benchmark_using_list[real]](graphs/queue_benchmark_using_list[real].png?raw=true "queue_benchmark_using_list[real]")
2. Πολυπλοκοτητα Ο(1)
![queue_benchmark_using_list[amortized]](graphs/queue_benchmark_using_list[amortized].png?raw=true "queue_benchmark_using_list[amortized]")
QUEUE_BENCHMARK_USING_STACK:
1. Πολυπλοκοτητα Ο(n)
![queue_benchmark_using_stack[real]](graphs/queue_benchmark_using_stack[real].png?raw=true "queue_benchmark_using_stack[real]")
2. Πολυπλοκοτητα Ο(n)
![queue_benchmark_using_stack[amortized]](graphs/queue_benchmark_using_stack[amortized].png?raw=true "queue_benchmark_using_stack[amortized]")
QUEUE_BENCHMARK_USING_STACK_ALT:
1. Πολυπλοκοτητα Ο(n) καθως και Ο(1), αναλογα την περιπτωση
![queue_benchmark_using_stack_alt[real]](graphs/queue_benchmark_using_stack_alt[real].png?raw=true "queue_benchmark_using_stack_alt[real]")
2. Πολυπλοκοτητα Ο(1)
![queue_benchmark_using_stack_alt[amortized]](graphs/queue_benchmark_using_stack_alt[amortized].png?raw=true "queue_benchmark_using_stack_alt[amortized]")
VECTOR_BENCHMARK_USING_DYNAMIC_ARRAY:
1. Πολυπλοκοτητα Ο(1)
![vector_benchmark_using_dynamic_array[real]](graphs/vector_benchmark_using_dynamic_array[real].png?raw=true "vector_benchmark_using_dynamic_array[real]")
2. Πολυπλοκοτητα Ο(1)
![vector_benchmark_using_dynamic_array[amortized]](graphs/vector_benchmark_using_dynamic_array[amortized].png?raw=true "vector_benchmark_using_dynamic_array[amortized]")
_________
_Ασκηση4_

Η λειτουργια set_create_from_sorted_values εχει γραμμικη πολυπλοκοτητα, το δεντρο που παραγει ειναι balanced. Επισης, τα επιπλεον τεστ, που ελεγχουν τις υπολοιπες λειτουργιες, δεν εμφανιζουν leaks.

_Bonus1_
`Υπαρχει το προγραμμα /programs/set_benchmark/set_benchmark.c το οποιο κανει διαδoχικα inserts χρησιμοποιωντας την trivial υλοποιηση και την υλοποιηση που ζητουσε η ασκηση 4. Τα αποτελεσματα της καθε υλοποιησης εκτυπωνονται στα αντιστοιχα .csv αρχεια που υπαρχουν στο φακελο /programs/set_benchmark. To προγραμμα θα καλειται ως set_benchmark <type> οπου <type> = real | amortized`
SET_BENCHMARK_TRIVIAL:
1. Πολυπλοκοτητα Ο(1)
![set_benchmark_trivial[real]](graphs/set_benchmark_trivial[real].png?raw=true "set_benchmark_trivial[real]")
2. Πολυπλοκοτητα Ο(1)
![set_benchmark_trivial[amortized]](graphs/set_benchmark_trivial[amortized].png?raw=true "set_benchmark_trivial[amortized]")
SET_BENCHMARK_USING_VectorToBST:
1. Πολυπλοκοτητα Ο(n) οσο αφορα την προσθηκη στοιχειων απο το Vector, O(1) χρησιμοποιωντας την set_insert();
![set_benchmark_using_VectorToBST[real]](graphs/set_benchmark_using_VectorToBST[real].png?raw=true "set_benchmark_using_VectorToBST[real]")
2. Πολυπλοκοτητα Ο(n) οσο αφορα την προσθηκη στοιχειων απο το Vector, O(1) χρησιμοποιωντας την set_insert();
![set_benchmark_using_VectorToBST[amortized]](graphs/set_benchmark_using_VectorToBST[amortized].png?raw=true "set_benchmark_using_VectorToBST[amortized]")
_________ 
