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

    }


}