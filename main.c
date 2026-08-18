#include <stdio.h>

void decimal_to_binary(int *dec, int var, int *binary){
    int temp_dec = *dec;
    int temp_var = var;
    while (temp_var > 0){
        binary[temp_var - 1] = temp_dec % 2;
        temp_dec = temp_dec / 2;
        temp_var--;
    }
}
int count_ones(int *binary_minterm, int num_of_variables){
    int ones_count = 0;

    for (int i = 0; i < num_of_variables; i++) {
        if (binary_minterm[i] == 1) {
            ones_count++;
        }
    }

    return ones_count;
}

int main(int argc, char *argv[]) {

    int num_of_variables;
    int num_of_minterms;




    printf("Enter the number of Variable: \n");
    scanf("%d",&num_of_variables);

    printf("Enter numbers of minterms: \n");  
    scanf("%d",&num_of_minterms);

    printf("Enter the Minterms: ");
    int minterms[num_of_minterms];
    for (int i = 0; i < num_of_minterms; i++) {
        scanf("%d",&minterms[i]);
    }

    int binary[num_of_variables];
    int binary_minterms[num_of_minterms][num_of_variables];

    for (int i = 0; i < num_of_minterms; i++) {
        decimal_to_binary(&minterms[i], num_of_variables, binary);
        for (int k = 0; k < num_of_variables; k++) {
            binary_minterms[i][k] = binary[k];
        }
    }

    int ones_count[num_of_minterms];

    for (int i = 0; i < num_of_minterms; i++) {
        ones_count[i] = count_ones(binary_minterms[i], num_of_variables);
    }

    int groups[num_of_variables +1][num_of_minterms][num_of_variables];

    int grp_count[num_of_variables + 1];
    for (int i = 0; i <= num_of_variables; i++) {
        grp_count[i] = 0;
    }

    for (int i = 0; i < num_of_minterms; i++) {

    int group = ones_count[i];
    int position = grp_count[group];

    for (int k = 0; k < num_of_variables; k++) {
        groups[group][position][k] = binary_minterms[i][k];
    }

    grp_count[group]++;

    }

    int combined[num_of_minterms][num_of_variables];
    int combined_count = 0;

    for (int i = 0; i < num_of_variables; i++) {

        for (int j = 0; j < grp_count[i]; j++) {

            for (int k = 0; k < grp_count[i + 1]; k++) {

                int temp_binary[num_of_variables];
                int differences = 0;

                for (int l = 0; l < num_of_variables; l++) {

                    if (groups[i][j][l] == groups[i + 1][k][l]) {
                        temp_binary[l] = groups[i][j][l];
                    } else {
                        temp_binary[l] = -1;
                        differences++;
                    }
                }

                if (differences == 1) {
                    for (int l = 0; l < num_of_variables; l++) {
                        combined[combined_count][l] = temp_binary[l];
                    }

                    combined_count++;
                    }
                }
            }

        }
    
    int combined_ones_count[combined_count];

    for (int i = 0; i < combined_count; i++) {
        combined_ones_count[i] = 0;

        for (int j = 0; j < num_of_variables; j++) {
            if (combined[i][j] == 1) {
                combined_ones_count[i]++;
            }
        }
    }

    int combined_groups[num_of_variables + 1][combined_count][num_of_variables];

    int combined_grp_count[num_of_variables + 1];

    for (int i = 0; i <= num_of_variables; i++) {
    combined_grp_count[i] = 0;
    }


    for (int i = 0; i < combined_count; i++) {

        int group = combined_ones_count[i];
        int position = combined_grp_count[group];

        for (int j = 0; j < num_of_variables; j++) {
            combined_groups[group][position][j] = combined[i][j];
        }

        combined_grp_count[group]++;
    }

    int combined_iteration_2[combined_count][num_of_variables];
    int combined_iteration_2_count = 0;


    for (int i = 0; i < num_of_variables; i++) {

        for (int j = 0; j < combined_grp_count[i]; j++) {

            for (int k = 0; k < combined_grp_count[i + 1]; k++) {

                int temp_binary[num_of_variables];
                int difference = 0;
                int valid = 1;


                for (int l = 0; l < num_of_variables; l++) {

                    int bit1 = combined_groups[i][j][l];
                    int bit2 = combined_groups[i + 1][k][l];


                    /* Both are already don't-cares */

                    if (bit1 == -1 && bit2 == -1) {
                        temp_binary[l] = -1;
                    }


                    /* One is don't-care and the other isn't */

                    else if (bit1 == -1 || bit2 == -1) {
                        valid = 0;
                        break;
                    }


                    /* Both are normal bits */

                    else if (bit1 == bit2) {
                        temp_binary[l] = bit1;
                    }


                    /* 0 vs 1 */

                    else {
                        temp_binary[l] = -1;
                        difference++;
                    }
                }


                /* one new difference */

                if (valid && difference == 1) {

                    for (int l = 0; l < num_of_variables; l++) {
                        combined_iteration_2[combined_iteration_2_count][l]
                            = temp_binary[l];
                    }

                    combined_iteration_2_count++;
                }
            }
        }
    }
}
