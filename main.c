#include <stdio.h>

#define MAX_BITS 10
#define MAX_MINTERMS 1024


struct Term {
    int binary[MAX_BITS];
    int minterms[MAX_MINTERMS];

    int minterm_count;
    int combined;
};


/* Convert decimal minterm to binary */
void decimal_to_binary(int dec, int var, int *binary)
{
    int temp_dec = dec;

    for (int i = var - 1; i >= 0; i--) {
        binary[i] = temp_dec % 2;
        temp_dec /= 2;
    }
}


/* Count the number of 1s in a term */
int count_ones(struct Term *term, int num_of_variables)
{
    int ones_count = 0;

    for (int i = 0; i < num_of_variables; i++) {
        if (term->binary[i] == 1) {
            ones_count++;
        }
    }

    return ones_count;
}


/* Print a term */
void print_term(struct Term *term, int num_of_variables)
{
    for (int i = 0; i < num_of_variables; i++) {

        if (term->binary[i] == -1)
            printf("-");
        else
            printf("%d", term->binary[i]);
    }
}


/*
    Try to combine two terms.

    Returns 1 if they can be combined.
    Returns 0 otherwise.

    The resulting term is stored in result.
*/
int combine_terms(
    struct Term *term1,
    struct Term *term2,
    struct Term *result,
    int num_of_variables
)
{
    int differences = 0;

    result->minterm_count = 0;
    result->combined = 0;

    for (int i = 0; i < num_of_variables; i++) {

        int bit1 = term1->binary[i];
        int bit2 = term2->binary[i];

        /* Both are already don't-cares */
        if (bit1 == -1 && bit2 == -1) {
            result->binary[i] = -1;
        }

        /* One is don't-care and the other isn't */
        else if (bit1 == -1 || bit2 == -1) {
            return 0;
        }

        /* Same bit */
        else if (bit1 == bit2) {
            result->binary[i] = bit1;
        }

        /* Different bit */
        else {
            result->binary[i] = -1;
            differences++;
        }

        /* More than one difference means no combination */
        if (differences > 1) {
            return 0;
        }
    }

    if (differences != 1)
        return 0;


    /*
        This combination was successful.
        Mark both original terms as combined.
    */
    term1->combined = 1;
    term2->combined = 1;


    /*
        Store all original minterms represented
        by the new term.
    */
    for (int i = 0; i < term1->minterm_count; i++) {
        result->minterms[result->minterm_count]
            = term1->minterms[i];

        result->minterm_count++;
    }

    for (int i = 0; i < term2->minterm_count; i++) {
        result->minterms[result->minterm_count]
            = term2->minterms[i];

        result->minterm_count++;
    }

    return 1;
}


int main(void)
{
    int num_of_variables;
    int num_of_minterms;


    /* ---------------- INPUT ---------------- */

    printf("Enter the number of variables: ");
    scanf("%d", &num_of_variables);

    printf("Enter number of minterms: ");
    scanf("%d", &num_of_minterms);


    int minterms[num_of_minterms];

    printf("Enter the minterms: ");

    for (int i = 0; i < num_of_minterms; i++) {
        scanf("%d", &minterms[i]);
    }


    /* ---------------- CREATE TERMS ---------------- */

    struct Term terms[num_of_minterms];


    for (int i = 0; i < num_of_minterms; i++) {

        decimal_to_binary(
            minterms[i],
            num_of_variables,
            terms[i].binary
        );

        terms[i].minterms[0] = minterms[i];
        terms[i].minterm_count = 1;
        terms[i].combined = 0;
    }


    /* ---------------- PRINT INITIAL TERMS ---------------- */

    printf("\nInitial terms:\n");

    for (int i = 0; i < num_of_minterms; i++) {

        printf("%d -> ", minterms[i]);

        print_term(
            &terms[i],
            num_of_variables
        );

        printf("\n");
    }


    /* ---------------- FIRST GROUPING ---------------- */

    struct Term groups[num_of_variables + 1][num_of_minterms];

    int group_count[num_of_variables + 1] = {0};


    for (int i = 0; i < num_of_minterms; i++) {

        int group =
            count_ones(&terms[i], num_of_variables);

        int position = group_count[group];

        groups[group][position] = terms[i];

        group_count[group]++;
    }


    printf("\nGroups:\n");

    for (int i = 0; i <= num_of_variables; i++) {

        printf("Group %d:\n", i);

        for (int j = 0; j < group_count[i]; j++) {

            print_term(
                &groups[i][j],
                num_of_variables
            );

            printf("\n");
        }
    }


    /* ---------------- FIRST COMBINATION ---------------- */

    struct Term combined[num_of_minterms * num_of_minterms];

    int combined_count = 0;


    for (int i = 0; i < num_of_variables; i++) {

        for (int j = 0; j < group_count[i]; j++) {

            for (int k = 0; k < group_count[i + 1]; k++) {

                struct Term result;


                if (combine_terms(
                        &groups[i][j],
                        &groups[i + 1][k],
                        &result,
                        num_of_variables)) {

                    combined[combined_count] = result;

                    combined_count++;
                }
            }
        }
    }


    /* ---------------- PRINT FIRST COMBINATIONS ---------------- */

    printf("\nFirst combinations:\n");

    for (int i = 0; i < combined_count; i++) {

        print_term(
            &combined[i],
            num_of_variables
        );

        printf(" -> ");

        for (int j = 0;
             j < combined[i].minterm_count;
             j++) {

            printf("%d ",
                combined[i].minterms[j]);
        }

        printf("\n");
    }


    /* ---------------- SECOND GROUPING ---------------- */

    struct Term combined_groups[num_of_variables + 1]
                               [combined_count];

    int combined_group_count[num_of_variables + 1] = {0};


    for (int i = 0; i < combined_count; i++) {

        int group =
            count_ones(
                &combined[i],
                num_of_variables
            );

        int position =
            combined_group_count[group];

        combined_groups[group][position]
            = combined[i];

        combined_group_count[group]++;
    }


    /* ---------------- SECOND COMBINATION ---------------- */

    struct Term combined_iteration_2[combined_count];

    int combined_iteration_2_count = 0;


    for (int i = 0; i < num_of_variables; i++) {

        for (int j = 0;
             j < combined_group_count[i];
             j++) {

            for (int k = 0;
                 k < combined_group_count[i + 1];
                 k++) {

                struct Term result;


                if (combine_terms(
                        &combined_groups[i][j],
                        &combined_groups[i + 1][k],
                        &result,
                        num_of_variables)) {

                    combined_iteration_2[
                        combined_iteration_2_count
                    ] = result;

                    combined_iteration_2_count++;
                }
            }
        }
    }


    /* ---------------- PRINT SECOND COMBINATIONS ---------------- */

    printf("\nSecond combinations:\n");

    for (int i = 0;
         i < combined_iteration_2_count;
         i++) {

        print_term(
            &combined_iteration_2[i],
            num_of_variables
        );

        printf(" -> ");

        for (int j = 0;
             j < combined_iteration_2[i].minterm_count;
             j++) {

            printf("%d ",
                combined_iteration_2[i].minterms[j]);
        }

        printf("\n");
    }


    return 0;
}