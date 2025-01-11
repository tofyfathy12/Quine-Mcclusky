#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define alphabet "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

struct function
{
    int *minterms_arr;
    int size;
    int groups_num;
    char ***mcclusky_groups;
};

struct combination
{
    char **combgroup;
    int size;
};

typedef struct StringNode
{
    int length;
    char *str;
    struct StringNode *next;
} StringNode;

typedef struct IntArray
{
    int length;
    int *array;
} IntArray;

void get_int(char *str, int *n);
struct function get_function();
void delete_buffer();
void print_struct(struct function f);
char get_char(char *str, char *letter);
char *get_binary(int number);
char *get_expression(const char *binary);
int get_variables_num();
char *get_variable_name();
char **get_variables();
int ones_count(int number);
void get_mcclusky_groups(struct function *f);
int min(const int *arr, int size);
void print_arr(const int *arr);
int count(const int *arr, int size, int number);
void remove_dublicates(struct function *f);
void print_mcclusky_groups(struct function f);
int max(int arr[], int size);
char *get_full_bin(int var_num, int num);
struct combination combine0(const int *g1, const int *g2); // it doesn't matter anymore as I changed mcclusky groups to be strings (0s & 1s)
struct combination general_comb(struct combination g1, struct combination g2, StringNode *taken_node, StringNode *nottaken_node, int last_combination_of_column);
bool onedif(const char *bin1, const char *bin2);
int index_of_difference(const char *bin1, const char *bin2);
char digit_to_char(int num);
int get_length(const int group[]);
int binary_to_decimal(char *binary);
StringNode *CreateStringNode(char *str);
void InsertStringNode(char *str, StringNode *head);
void PrintStringNodes(StringNode *head, char seperator);
int Is_In(char *str, StringNode *head);
int RemoveStringNode(char *str, StringNode *head);
IntArray all_possible_minterms(char *str);
int **get_prime_implicants_chart(int *minterms, int num_of_minterms, char **terms, int num_of_terms);
IntArray get_petrick(int *minterms, int num_of_minterms, char **terms, int num_of_terms);
StringNode *get_final_functions(struct function f, StringNode *terms_head);
void FreeNodes(StringNode **head);
StringNode *get_solution();

int main()
{
    StringNode *solution = get_solution();
    printf("Possible Functions:\n");
    PrintStringNodes(solution, '\n');
    FreeNodes(&solution);
}

StringNode *CreateStringNode(char *str)
{
    StringNode *node = (StringNode *)malloc(sizeof(StringNode));
    node->str = strdup(str);
    node->length = 1;
    node->next = NULL;
    return node;
}

void InsertStringNode(char *str, StringNode *head)
{
    StringNode *node = head;
    if (strcmp(head->str, "") == 0)
    {
        free(head->str);
        head->str = strdup(str);
    }
    else
    {
        head->length += 1;
        while (node->next != NULL)
        {
            node = node->next;
        }
        node->next = CreateStringNode(str);
    }
}

void PrintStringNodes(StringNode *head, char seperator)
{
    StringNode *node = head;
    while (node != NULL)
    {
        if (node->next == NULL)
            printf("%s\n", node->str);
        else
            printf("%s%c", node->str, seperator);
        node = node->next;
    }
}

int Is_In(char *str, StringNode *head)
{
    StringNode *node = head;
    while (node != NULL)
    {
        if (strcmp(node->str, str) == 0)
            return 1;
        node = node->next;
    }
    return 0;
}

int RemoveStringNode(char *str, StringNode *head)
{
    head->length -= 1;
    StringNode *node = head;
    StringNode *before_node = node;
    node = node->next;
    if (strcmp(before_node->str, str) == 0)
    {
        *head = *node;
        free(before_node->str);
        before_node->next = NULL;
        free(before_node);
        before_node = NULL;
        return 1;
    }
    while (node != NULL)
    {
        if (strcmp(node->str, str) == 0)
        {
            before_node->next = node->next;
            free(node->str);
            node->next = NULL;
            free(node);
            node = NULL;
            return 1;
        }
        before_node = node;
        node = node->next;
    }
    return 0;
}

void FreeNodes(StringNode **head)
{
    StringNode *current = *head;
    while (current != NULL)
    {
        StringNode *next = current->next;
        free(current->str);
        current->next = NULL;
        free(current);
        current = next;
    }
    *head = NULL;
}

int get_length(const int *group)
{
    int i = 0, size = 0;
    while (group[i] != -1)
    {
        size++;
        i++;
    }
    return size;
}

int ones_count(int number)
{
    char *binary = get_binary(number);
    if (binary == NULL)
        return 0;
    int count = 0;
    for (int i = 0; i < strlen(binary); i++)
    {
        if (binary[i] == '1')
            count++;
    }
    free(binary);
    binary = NULL;
    return count;
}

void get_mcclusky_groups(struct function *f)
{
    int maximum = max(f->minterms_arr, f->size);
    int max_var_num = 0;
    while ((1 << max_var_num) <= maximum)
        max_var_num++;
    // printf("Max Variables number = %d\n", max_var_num);
    int ones_counts[f->size];
    for (int i = 0; i < f->size; i++)
    {
        ones_counts[i] = ones_count(f->minterms_arr[i]);
        // printf("Number of ones in %d = %d\n", f->minterms_arr[i], ones_counts[i]);
    }

    int min_ones_count = min(ones_counts, f->size);
    // printf("min ones count = %d\n", min_ones_count);
    int temp = min_ones_count;
    int elements_taken = 0;
    f->groups_num = 0;
    int *groups_sizes = (int *)malloc((max_var_num + 1) * sizeof(int));
    while (elements_taken < f->size)
    {
        int ones_count = count(ones_counts, f->size, temp);
        elements_taken += ones_count;
        groups_sizes[f->groups_num] = ones_count;
        temp++;
        if (groups_sizes[f->groups_num] > 0)
            f->groups_num++;
    }
    f->mcclusky_groups = (char ***)malloc(f->groups_num * sizeof(char **));
    for (int i = 0; i < f->groups_num; i++)
        *(f->mcclusky_groups + i) = (char **)malloc((groups_sizes[i] + 1) * sizeof(char *));
    free(groups_sizes);
    groups_sizes = NULL;
    int i = 0;
    while (i < f->groups_num)
    {
        int element_index = 0;
        for (int j = 0; j < f->size; j++)
        {
            if (ones_counts[j] == min_ones_count)
            {
                *(*(f->mcclusky_groups + i) + element_index) = get_full_bin(max_var_num, f->minterms_arr[j]);
                // printf("mcclusky_groups[%d][%d] = %s\n", i, element_index, *(*(f->mcclusky_groups + i) + element_index));
                element_index++;
            }
        }
        *(*(f->mcclusky_groups + i) + element_index) = NULL; // last element to be null-terminator (\0)
        // printf("mcclusky_groups[%d][%d] = %s\n", i, element_index, *(*(f->mcclusky_groups + i) + element_index));
        min_ones_count++;
        if (element_index > 0)
            i++;
    }
}

int min(const int *arr, int size)
{
    int minimum = arr[0];
    for (int i = 1; i < size; i++)
        minimum = (minimum < arr[i]) ? minimum : arr[i];
    return minimum;
}

char **get_variables()
{
    int variables_num = get_variables_num();
    char **names = (char **)malloc(variables_num * sizeof(char *));
    for (int i = 0; i < variables_num; i++)
    {
        *(names + i) = get_variable_name();
    }
    return names;
}

char *get_variable_name()
{
    char *name = (char *)malloc(10 * sizeof(char));
    printf("Enter variable name: ");
    fgets(name, 10, stdin);
    name[strlen(name) - 1] = (name[strlen(name) - 1] == '\n') ? '\0' : '\0';
    return name;
}

int get_variables_num()
{
    int variables_num;
    get_int("Enter number of variables: ", &variables_num);
    return variables_num;
}

char *get_expression(const char *binary)
{
    int size = (int)strlen(binary) * 2 + 1;
    char *expression = (char *)calloc(size, sizeof(char));
    int i = 0;
    int alpha_index = 0;
    int exp_index = 0;
    char temp;
    int underscores = 0;
    while (i < strlen(binary))
    {
        temp = alphabet[alpha_index % strlen(alphabet)];
        if (binary[i] == '0')
        {
            expression[exp_index] = temp;
            expression[exp_index + 1] = '\'';
            exp_index += 2;
        }
        else if (binary[i] == '1')
        {
            expression[exp_index] = temp;
            exp_index++;
        }
        else if (binary[i] == '_')
            underscores ++;
        alpha_index++;
        i++;
    }
    if (underscores == strlen(binary) && exp_index == 0)
    {
        expression[0] = '1';
        expression = (char *)realloc(expression, 2 * sizeof(char));
    }
    return expression;
}

char *get_binary(int num)
{
    char *result;
    if (num == 0)
    {
        result = (char *)calloc(2, sizeof(char));
        result[0] = '0';
        return result;
    }
    int size = 0;
    while ((1 << size) <= num)
        size++;
    result = (char *)calloc((size + 1), sizeof(char));
    int i = size - 1;
    while (num > 0)
    {
        result[i] = digit_to_char(num & 1);
        i--;
        num >>= 1;
    }
    return result;
}

char digit_to_char(int num)
{
    num += 48;
    char letter = (char)num;
    return letter;
}

struct function get_function()
{
    struct function f;
    start:
    f.size = 0;
    // f.minterms_arr = (int *)malloc(1 * sizeof(int));
    // int num;
    // get_int("Enter minterm (-1 to stop): ", &num);
    char minterms_input[1000];
    printf("Enter comma seperated minterms e.g.(0, 1, 2, ...): ");
    fgets(minterms_input, 1000, stdin);
    // if (minterms_input[strlen(minterms_input) - 1] == '\n') minterms_input[strlen(minterms_input) - 1] = '\0';
    for (int i = 0; i < strlen(minterms_input); i++)
        if (minterms_input[i] == ',') f.size++;
    
    f.size++;
    f.minterms_arr = (int *)malloc(f.size * sizeof(int));
    int i = 0;
    char *delim = ", \n";
    char *temp;
    temp = strtok(minterms_input, delim);
    while (temp != NULL && i < f.size)
    {
        f.minterms_arr[i] = atoi(temp);
        if (f.minterms_arr[i] < 0)
        {
            printf("ERROR: Negative Minterm\n");
            printf("Try Again\n");
            free(f.minterms_arr);
            f.minterms_arr = NULL;
            goto start;
        }
        temp = strtok(NULL, delim);
        i++;
    }
    if (temp != NULL)
    {
        printf("INPUT ERROR !!");
        printf("Try Again.\n");
        free(f.minterms_arr);
        f.minterms_arr = NULL;
        goto start;
    }
    // if (num == -1)
    //     f.minterms_arr = (int *)realloc(f.minterms_arr, f.size * sizeof(int));
    // while (num != -1)
    // {
    //     if (num < 0)
    //     {
    //         printf("Minterms can't be negative !!\n");
    //         get_int("Enter minterm (-1 to stop): ", &num);
    //         continue;
    //     }
    //     f.size++;
    //     f.minterms_arr = (int *)realloc(f.minterms_arr, f.size * sizeof(int));
    //     *(f.minterms_arr + i) = num;
    //     i++;
    //     get_int("Enter minterm (-1 to stop): ", &num);
    // }
    remove_dublicates(&f);
    get_mcclusky_groups(&f);
    return f;
}

char get_char(char *str, char *letter)
{
    while (1)
    {
        printf("%s", str);
        char newline;
        int checker = scanf("%c%c", letter, &newline);
        if (checker != 2 || newline != '\n')
        {
            delete_buffer();
            printf("THAT'S NOT VALID. TRY AGAIN.\n");
        }
        else
            break;
    }
    return *letter;
}

void delete_buffer()
{
    char garbage[1000];
    scanf("%[^\n]%*c", garbage);
}
void get_int(char *str, int *n)
{
    bool valid = false;
    char input[1000];
    while (!valid)
    {
        printf("%s", str);
        fgets(input, 1000, stdin);
        input[strlen(input) - 1] = '\0';
        if (strlen(input) == 0)
        {
            printf("!!! Invalid Input !!!\n");
            printf("    Try Again\n");
            continue;
        }
        bool notdouble = false;
        for (int i = 0; i < strlen(input); i++)
        {
            if (!(isdigit(input[i]) || (input[i] == '-' && i == 0)))
            {
                notdouble = true;
                break;
            }
        }
        if (notdouble)
        {
            printf("!!! Invalid Input !!!\n");
            printf("    Try Again\n");
        }
        else
            valid = true;
    }
    *n = atoi(input);
}

void print_struct(struct function f)
{
    for (int i = 0; i < f.size; i++)
        printf("%d ", f.minterms_arr[i]);
    printf("\n");
}

void print_arr(const int *arr)
{
    int i = 0;
    while (*(arr + i) != -1)
    {
        printf("%d ", *(arr + i));
        i++;
    }
    printf("\n");
}

int count(const int *arr, int size, int number)
{
    int result = 0;
    for (int i = 0; i < size; i++)
        if (arr[i] == number)
            result++;
    return result;
}

int max(int arr[], int size)
{
    int maximum = arr[0];
    for (int i = 1; i < size; i++)
        maximum = (arr[i] > maximum) ? arr[i] : maximum;
    return maximum;
}

void remove_dublicates(struct function *f) // This works during the get_function() function to remove the dublicated minterms
{
    int uniques_num = 0;
    int *temp_arr = (int *)calloc(f->size, sizeof(int));
    *(temp_arr + 0) = *(f->minterms_arr + 0);
    uniques_num++;
    for (int i = 1; i < f->size; i++)
        *(temp_arr + i) = 0;
    for (int i = 1; i < f->size; i++)
    {
        int counter = 0;
        for (int j = 1; j < f->size; j++)
        {
            if (*(f->minterms_arr + i) == *(temp_arr + j))
            {
                counter++;
                break;
            }
        }
        if (counter == 0)
        {
            *(temp_arr + uniques_num) = *(f->minterms_arr + i);
            uniques_num++;
        }
    }
    for (int i = 0; i < uniques_num; i++)
        *(f->minterms_arr + i) = *(temp_arr + i);
    free(temp_arr);
    temp_arr = NULL;
    f->minterms_arr = (int *)realloc(f->minterms_arr, uniques_num * sizeof(int));
    f->size = uniques_num;
}

void print_mcclusky_groups(struct function f) // This is for debugging
{
    char ***groups = f.mcclusky_groups;
    for (int i = 0; i < f.groups_num; i++)
    {
        int element_index = 0;
        char *numinbin = *(*(groups + i) + element_index);
        if (numinbin != NULL)
            printf("group%d:\n", i);
        while (numinbin != NULL)
        {
            printf("%d : %s\n", binary_to_decimal(numinbin), numinbin);
            element_index++;
            numinbin = *(*(groups + i) + element_index);
        }
    }
}

char *get_full_bin(int var_num, int num)
{
    int size = 0;
    while ((1 << size) <= num)
        size++;
    char *full_bin = (char *)calloc((var_num + 1), sizeof(char));
    int dif = var_num - size;
    for (int i = 0; i < dif; i++)
        full_bin[i] = '0';
    int i = var_num - 1;
    while (num > 0)
    {
        full_bin[i] = digit_to_char(num & 1);
        num >>= 1;
        i--;
    }
    return full_bin;
}

struct combination combine0(const int *g1, const int *g2) // This function serves nothing to the program
{
    int size1 = get_length(g1);
    int size2 = get_length(g2);
    struct combination comb1;
    char **combined = (char **)malloc(size1 * size2 * sizeof(char *));
    for (int i = 0; i < size1 * size2; i++)
        *(combined + i) = (char *)malloc(4 * sizeof(char)); // num of variables should be fixes to be variable
    int combined_index = 0;
    for (int i = 0; i < size1; i++)
    {
        char *bin1 = get_full_bin(4, *(g1 + i));
        for (int j = 0; j < size2; j++)
        {
            char *bin2 = get_full_bin(4, *(g2 + j));
            if (onedif(bin1, bin2))
            {
                int index = index_of_difference(bin1, bin2);
                char *new_comb = strdup(bin1);
                new_comb[index] = '_';
                *(combined + combined_index) = new_comb;
                combined_index++;
                free(new_comb);
                new_comb = NULL;
            }
            free(bin2);
            bin2 = NULL;
        }
        free(bin1);
        bin1 = NULL;
    }
    combined = (char **)realloc(combined, combined_index * sizeof(char *));
    comb1.combgroup = combined;
    // int num_of_groups = combined_index;
    comb1.size = combined_index;
    return comb1;
}

struct combination general_comb(struct combination g1, struct combination g2, StringNode *taken_node, StringNode *nottaken_node, int last_combination_of_column)
{
    struct combination combination;
    int term_length = (g1.size > 0) ? strlen(g1.combgroup[0]) : 0;
    char **combined = NULL;
    if ((g2.size * g2.size) > 0)
        combined = (char **)malloc((g1.size * g2.size) * sizeof(char *));
    int combined_index = 0;
    for (int i = 0; i < g1.size; i++)
    {
        char *bin1 = strdup(*(g1.combgroup + i));
        int ones_difs = 0;
        for (int j = 0; j < g2.size; j++)
        {
            char *bin2 = strdup(*(g2.combgroup + j));
            if (onedif(bin1, bin2))
            {
                ones_difs++;
                InsertStringNode(bin1, taken_node);
                InsertStringNode(bin2, taken_node);
                if (Is_In(bin1, nottaken_node))
                {
                    RemoveStringNode(bin1, nottaken_node);
                }
                int index = index_of_difference(bin1, bin2);
                combined[combined_index] = strdup(bin1);
                combined[combined_index][index] = '_';
                combined_index++;
            }
            free(bin2);
            bin2 = NULL;
        }
        if (ones_difs == 0)
        {
            if (!Is_In(bin1, nottaken_node) && !Is_In(bin1, taken_node))
                InsertStringNode(bin1, nottaken_node);
        }
        free(bin1);
        bin1 = NULL;
    }
    if (last_combination_of_column)
    {
        for (int i = 0; i < g2.size; i++)
        {
            char *bin1 = strdup(*(g2.combgroup + i));
            int ones_difs = 0;
            for (int j = 0; j < g1.size; j++)
            {
                char *bin2 = strdup(*(g1.combgroup + j));
                if (onedif(bin1, bin2))
                {
                    ones_difs++;
                    if (Is_In(bin1, nottaken_node))
                    {
                        RemoveStringNode(bin1, nottaken_node);
                    }
                }
                free(bin2);
                bin2 = NULL;
            }
            if (ones_difs == 0)
            {
                if (!Is_In(bin1, nottaken_node) && !Is_In(bin1, taken_node))
                {
                    InsertStringNode(bin1, nottaken_node);
                }
            }
            free(bin1);
            bin1 = NULL;
        }
    }
    if (combined_index > 0)
        combined = (char **)realloc(combined, combined_index * sizeof(char *));
    else
    {
        if (combined != NULL)
        {
            free(combined);
            combined = NULL;
        }
    }
    // printf("Result: ");
    // for (int i = 0; i < combined_index; i++)
    // {
    //     printf("%s ", combined[i]);
    // }
    // printf("\n");
    combination.combgroup = combined;
    combination.size = combined_index;
    return combination;
}

bool onedif(const char *bin1, const char *bin2)
{
    bool valid = true;
    int differences = 0;
    int length = strlen(bin1); // or strlen(bin2);
    for (int i = 0; i < length; i++)
    {
        if (*(bin1 + i) != *(bin2 + i))
            differences++;
        if (differences > 1)
        {
            valid = false;
            break;
        }
    }
    return valid;
}

int index_of_difference(const char *bin1, const char *bin2)
{
    int length = strlen(bin1);
    int index = 0;
    for (int i = 0; i < length; i++)
    {
        if (*(bin1 + i) != *(bin2 + i))
        {
            index = i;
            break;
        }
    }
    return index;
}

int binary_to_decimal(char *binary)
{
    int length = strlen(binary);
    int result = 0;
    for (int i = 0; i < length; i++)
    {
        if (binary[i] == '1')
            result += (1 << (length - 1 - i));
    }
    return result;
}

IntArray all_possible_minterms(char *str)
{
    char temp[strlen(str) + 1];
    strcpy(temp, str);
    int count = 0;
    int index_of_char = 0;
    do
    {
        index_of_char = strcspn(temp, "_");
        if (temp[index_of_char] != '_')
            break;
        count++;
        memmove(temp, temp + index_of_char + 1, strlen(temp + index_of_char + 1) + 1);
        index_of_char = 0;
    }
    while (index_of_char < strlen(temp));
    int *positions = (int *)malloc(count * sizeof(int));
    int index = 0;
    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == '_')
        {
            positions[index] = i;
            index++;
        }
    }
    int num_of_possibs = 1 << count;
    int *possibilities = (int *)malloc(num_of_possibs * sizeof(int));
    for (int i = 0; i < num_of_possibs; i++)
    {
        char *possib_bin = get_full_bin(count, i);
        char *temp2 = strdup(str);
        for (int j = 0; j < count; j++)
        {
            temp2[positions[j]] = possib_bin[j];
        }
        possibilities[i] = binary_to_decimal(temp2);
        free(possib_bin);
        possib_bin = NULL;
        free(temp2);
        temp2 = NULL;
    }
    free(positions);
    IntArray arr;
    arr.array = (int *)realloc(possibilities, num_of_possibs * sizeof(int));
    arr.length = num_of_possibs;
    return arr;
}

int **get_prime_implicants_chart(int *minterms, int num_of_minterms, char **terms, int num_of_terms)
{
    int **prime_implicants_chart = (int **)malloc((num_of_terms) * sizeof(int *));
    for (int i = 0; i < num_of_terms; i++)
    {
        prime_implicants_chart[i] = (int *)calloc(num_of_minterms, sizeof(int));
        char *str = strdup(terms[i]);
        IntArray possibs = all_possible_minterms(str);
        for (int pos = 0; pos < possibs.length; pos++)
            // getting position of minterm and setting it to 1 in the PIs chart
            for (int j = 0; j < num_of_minterms; j++)
                if (minterms[j] == possibs.array[pos])
                {
                    prime_implicants_chart[i][j] = 1;
                    break;
                }
        free(possibs.array);
        possibs.array = NULL;
        free(str);
    }
    return prime_implicants_chart;
}

IntArray get_petrick(int *minterms, int num_of_minterms, char **terms, int num_of_terms)
{
    int **prime_implicants_chart = get_prime_implicants_chart(minterms, num_of_minterms, terms, num_of_terms);
    int *petrick0 = (int *)malloc(num_of_terms * sizeof(int));
    int p_index0 = 0;
    for (int row = 0; row < num_of_terms; row++)
    {
        if (prime_implicants_chart[row][0] == 1)
        {
            petrick0[p_index0] = 1 << row;
            p_index0++;
        }
    }
    petrick0 = (int *)realloc(petrick0, p_index0 * sizeof(int));

    for (int column = 1; column < num_of_minterms; column++)
    {
        int *general_petrick = (int *)malloc(num_of_terms * sizeof(int));
        int p_index = 0;
        for (int row = 0; row < num_of_terms; row++)
        {
            if (prime_implicants_chart[row][column] == 1)
            {
                general_petrick[p_index] = 1 << row;
                p_index++;
            }
        }
        general_petrick = (int *)realloc(general_petrick, p_index * sizeof(int));
        int *p_result = (int *)malloc((p_index0 * p_index) * sizeof(int));
        int p_result_index = 0;

        for (int p0 = 0; p0 < p_index0; p0++)
        {
            for (int p = 0; p < p_index; p++)
            {
                p_result[p_result_index] = petrick0[p0] | general_petrick[p];
                p_result_index++;
            }
        }
        int p_result_new_size = 0;
        for (int i = 0; i < p_result_index; i++)
        {
            for (int j = i + 1; j < p_result_index; j++)
            {
                if ((p_result[i] & p_result[j]) == p_result[i] && (p_result[i] != 0))
                {
                    p_result[j] = 0;
                }
                else if ((p_result[i] & p_result[j]) == p_result[j] && (p_result[j] != 0))
                {
                    p_result[i] = 0;
                    break;
                }
                else
                    p_result_new_size++;
            }
        }
        int *p_result2 = (int *)malloc(p_result_new_size * sizeof(int));
        int p_result2_index = 0;
        for (int i = 0; i < p_result_index; i++)
        {
            if (p_result[i] != 0)
            {
                p_result2[p_result2_index] = p_result[i];
                p_result2_index++;
            }
        }
        free(petrick0);
        petrick0 = (int *)malloc(p_result2_index * sizeof(int));
        for (int i = 0; i < p_result2_index; i++)
            petrick0[i] = p_result2[i];
        free(p_result2);
        p_index0 = p_result2_index;
        free(general_petrick);
        free(p_result);
    }
    for (int i = 0; i < num_of_terms; i++)
        free(prime_implicants_chart[i]);
    free(prime_implicants_chart);
    IntArray petrick;
    petrick.array = petrick0;
    petrick.length = p_index0;
    return petrick;
}

StringNode *get_final_functions(struct function f, StringNode *terms_head)
{
    int *minterms = f.minterms_arr;
    int num_of_minterms = f.size;
    int num_of_terms = terms_head->length;
    char **terms = (char **)malloc(num_of_minterms * sizeof(char *));
    int terms_index = 0;
    while (terms_head != NULL)
    {
        terms[terms_index] = terms_head->str;
        terms_head = terms_head->next;
        terms_index++;
    }
    StringNode *final_functions = CreateStringNode("");
    IntArray petrick = get_petrick(minterms, num_of_minterms, terms, num_of_terms);
    int min_ones_num = 0;
    for (int i = 0; i < petrick.length; i++)
    {
        char *petrick_bin = get_full_bin(num_of_terms, petrick.array[i]);
        int ones_num = 0;
        for (int j = 0; j < strlen(petrick_bin); j++)
        {
            if (petrick_bin[j] == '1')
                ones_num++;
        }
        if (i == 0)
            min_ones_num = ones_num;
        else if (ones_num < min_ones_num)
            min_ones_num = ones_num;
        free(petrick_bin);
        petrick_bin = NULL;
    }
    for (int i = 0; i < petrick.length; i++)
    {
        char *petrick_bin = get_full_bin(num_of_terms, petrick.array[i]);
        int ones_num = 0;
        for (int j = 0; j < strlen(petrick_bin); j++)
        {
            if (petrick_bin[j] == '1')
                ones_num++;
        }
        if (ones_num != min_ones_num)
            petrick.array[i] = 0;
        free(petrick_bin);
        petrick_bin = NULL;
    }
    int *minimized_petrick = (int *)malloc(petrick.length * sizeof(int));
    int minimized_index = 0;
    for (int i = 0; i < petrick.length; i++)
    {
        if (petrick.array[i] != 0)
        {
            minimized_petrick[minimized_index] = petrick.array[i];
            minimized_index++;
        }
    }
    free(petrick.array);
    petrick.array = NULL;
    minimized_petrick = (int *)realloc(minimized_petrick, minimized_index * sizeof(int));
    int num_of_functions = minimized_index;
    for (int i = 0; i < num_of_functions; i++)
    {
        char *petrick_bin = get_full_bin(num_of_terms, minimized_petrick[i]);
        int length_of_bin = strlen(petrick_bin);
        int f_index = 0;
        char *str;
        int loop_tracker = 0;
        for (int j = 0; j < length_of_bin; j++)
        {
            if (petrick_bin[j] == '1')
            {
                char *expression = get_expression(terms[num_of_terms - j - 1]);
                if (f_index < min_ones_num - 1)
                {
                    if (f_index == 0)
                        str = strdup(expression);
                    else
                    {
                        size_t len = strlen(expression) + strlen(str) + 1;
                        str = (char *)realloc(str, len * sizeof(char));
                        strcat(str, expression);
                    }
                    loop_tracker ++;
                    size_t len = strlen(str) + 4;
                    str = (char *)realloc(str, len * sizeof(char));
                    strcat(str, " + ");
                }
                else
                {
                    if (loop_tracker == 0)
                        str = strdup(expression);
                    else
                    {
                        size_t len = strlen(expression) + strlen(str) + 1;
                        str = (char *)realloc(str, len * sizeof(char));
                        strcat(str, expression);
                    }
                    loop_tracker ++;
                }
                f_index++;
                free(expression);
                expression = NULL;
            }
        }
        InsertStringNode(str, final_functions);
        free(str);
        str = NULL;
        free(petrick_bin);
        petrick_bin = NULL;
    }
    free(minimized_petrick);
    minimized_petrick = NULL;
    free(terms);
    terms = NULL;
    return final_functions;
}

StringNode *get_solution()
{
    struct function f1 = get_function();
    print_struct(f1);
    printf("Mcclusky Groups:\n");
    print_mcclusky_groups(f1);
    printf("Groups Num = %d\n", f1.groups_num);
    struct combination **combinations = (struct combination **)malloc(f1.groups_num * sizeof(struct combination *));
    for (int i = 0; i < f1.groups_num; i++)
        combinations[i] = (struct combination *)malloc((f1.groups_num - i) * sizeof(struct combination));

    for (int j = 0; j < f1.groups_num; j++)
    {
        int size = 0;
        while (f1.mcclusky_groups[j][size] != NULL)
            size++;
        combinations[0][j].size = size;
        combinations[0][j].combgroup = (char **)realloc(f1.mcclusky_groups[j], size * sizeof(char *));
    }
    StringNode *not_taken = CreateStringNode("");
    StringNode *taken = CreateStringNode("");
    if (f1.groups_num == 1)
    {
        int j = 0;
        while (f1.mcclusky_groups[0][j] != NULL)
        {
            InsertStringNode(f1.mcclusky_groups[0][j], not_taken);
            j ++;
        }
    }
    for (int i = 1; i < f1.groups_num; i++)
    {
        int last_check = 0;
        for (int j = 0; j < f1.groups_num - i; j++)
        {
            if (j == f1.groups_num - i - 1)
                last_check = 1;
            combinations[i][j] = general_comb(combinations[i - 1][j], combinations[i - 1][j + 1], taken, not_taken, last_check);
            // printf("comb%d_%d:\n", j, j + 1);
            // printf("size = %d\n", combinations[i][j].size);
            for (int k = 0; k < combinations[i][j].size; k++)
            {
                if (i == f1.groups_num - 1 && j == f1.groups_num - i - 1)
                {
                    if (!Is_In(combinations[i][j].combgroup[k], not_taken))
                    {
                        InsertStringNode(combinations[i][j].combgroup[k], not_taken);
                    }
                }
                // printf("%s ", combinations[i][j].combgroup[k]);
                // if (k == combinations[i][j].size - 1) printf("\n");
            }
        }
    }
    printf("not taken: ");
    PrintStringNodes(not_taken, ' ');
    printf("not taken length = %d\n", not_taken->length);
    printf("taken: ");
    PrintStringNodes(taken, ' ');
    for (int i = 0; i < f1.groups_num; i++)
    {
        for (int j = 0; j < f1.groups_num - i; j++)
        {
            if (combinations[i][j].combgroup != NULL)
            {
                for (int k = 0; k < combinations[i][j].size; k++)
                    free(combinations[i][j].combgroup[k]);
                free(combinations[i][j].combgroup);
                combinations[i][j].combgroup = NULL;
            }
        }
        free(combinations[i]);
        combinations[i] = NULL;
    }
    free(combinations);
    combinations = NULL;
    free(f1.mcclusky_groups);
    StringNode *possible_functions = get_final_functions(f1, not_taken);
    FreeNodes(&taken);
    FreeNodes(&not_taken);
    free(f1.minterms_arr);
    return possible_functions;
}
