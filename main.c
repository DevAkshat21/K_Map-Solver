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

    for (int i = 0; i < num_of_variables; i++) {

        for (int j = 0; j < grp_count[i]; j++) {

            for (int k = 0; k < grp_count[i + 1]; k++) {

                int temp_binary[num_of_variables];
                int differences = 0;

                for (int l = 0; l < num_of_variables; l++) {

                    if (groups[i][j][l] == groups[i + 1][k][l]) {
                        temp_binary[l] = groups[i][j][l];
                    } else {
                        temp_binary[l] = -1;   // -1 represents '-'
                        differences++;
                    }
                }

                if (differences == 1) {

                    printf("Combined term: ");

                    for (int l = 0; l < num_of_variables; l++) {
                        if (temp_binary[l] == -1)
                            printf("-");
                        else
                            printf("%d", temp_binary[l]);
                    }

                    printf("\n");
                }
            }
        }
    }
}