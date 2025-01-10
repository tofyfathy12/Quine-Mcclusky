#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define alphabet "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

typedef struct StringNode
{
    char *str;
    struct StringNode *next;
    int length;
} StringNode;

StringNode *CreateStringNode(char *str)
{
    StringNode *node = (StringNode *)malloc(sizeof(StringNode));
    node->str = strdup(str);
    node->next = NULL;
    node->length = 1;
    return node;
}

void InsertStringNode(char *str, StringNode *head)
{
    StringNode *node = head;
    if (strcmp(head->str, "") == 0)
        head->str = strdup(str);
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
        if (strcmp(node->str, str) == 0) return 1;
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
        free(before_node);
        return 1;
    }
    while (node != NULL)
    {
        if (strcmp(node->str, str) == 0)
        {
            before_node->next = node->next;
            free(node);
            return 1;
        }
        before_node = node;
        node = node->next;
    }
    return 0;
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

char digit_to_char(int num)
{
    num += 48;
    char letter = (char) num;
    return letter;
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

typedef struct IntArray
{
    int length;
    int *array;
} IntArray;

IntArray all_possible_minterms(char *str)
{
    char *temp = strdup(str);
    int count = 0;
    int ptr = 0;
    do
    {
        ptr = strcspn(temp, "_");
        if (temp[ptr] != '_') break;
        count ++;
        temp = strdup(temp + 1 + ptr);
    } while (ptr < strlen(temp));
    int *positions = (int *)malloc(count * sizeof(int));
    int index = 0;
    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == '_')
        {
            positions[index] = i;
            index ++;
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
    }
    IntArray arr;
    arr.array = possibilities;
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
        for (int pos = 0; pos < possibs.length; pos ++)
            // getting position of minterm and setting it to 1 in the PIs chart
            for (int j = 0; j < num_of_minterms; j++)
                if (minterms[j] == possibs.array[pos])
                {
                    prime_implicants_chart[i][j] = 1;
                    break;
                }
        free(possibs.array);
    }
    return prime_implicants_chart;
}

IntArray get_petrick(int *minterms, int num_of_minterms, char **terms, int num_of_terms)
{
    int **prime_implicants_chart = get_prime_implicants_chart(minterms, num_of_minterms, terms, num_of_terms);
    int *petrick0 = (int *)malloc(num_of_terms * sizeof(int));
    int p_index0 = 0;
    for (int row = 0; row < num_of_terms; row ++)
    {
        if (prime_implicants_chart[row][0] == 1)
        {
            petrick0[p_index0] = 1 << row;
            p_index0++;
        }
    }
    petrick0 = (int *)realloc(petrick0, p_index0*sizeof(int));
    int p_index;
    
    for (int column = 1; column < num_of_minterms; column++)
    {
        int *general_petrick = (int *)malloc(num_of_terms * sizeof(int));
        p_index = 0;
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
        
        for (int p0 = 0; p0 < p_index0; p0 ++)
        {
            for (int p = 0; p < p_index; p++)
            {
                p_result[p_result_index] = petrick0[p0] | general_petrick[p];
                p_result_index ++;
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
                else p_result_new_size ++;
            }
        }
        int *p_result2 = (int *)malloc(p_result_new_size * sizeof(int));
        int p_result2_index = 0;
        for (int i = 0; i < p_result_index; i++)
        {
            if (p_result[i] != 0)
            {
                p_result2[p_result2_index] = p_result[i];
                p_result2_index ++;
            }
        }
        petrick0 = (int *)realloc(p_result2, p_result2_index * sizeof(int));
        p_index0 = p_result2_index;
        free(general_petrick);
        free(p_result);
    }
    IntArray petrick;
    petrick.array = petrick0;
    petrick.length = p_index0;
    return petrick;
}

char *get_expression(const char *binary)
{
    int size = (int) strlen(binary) * 2 + 1;
    char *expression = (char *)calloc(size, sizeof(char));
    int i = 0;
    int alpha_index = 0;
    int exp_index = 0;
    char temp;
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
            exp_index ++;
        }
        alpha_index++;
        i++;
    }
    return expression;
}

StringNode *get_final_function(int *minterms, int num_of_minterms, char **terms, int num_of_terms)
{
    StringNode *final_functions = CreateStringNode("");
    IntArray petrick = get_petrick(minterms, num_of_minterms, terms, num_of_terms);
    int min_ones_num = 0;
    for (int i = 0; i < petrick.length; i++)
    {
        char *petrick_bin = get_full_bin(num_of_terms, petrick.array[i]);
        int ones_num = 0;
        for (int j = 0; j < strlen(petrick_bin); j++)
        {
            if (petrick_bin[j] == '1') ones_num ++;
        }
        if (i == 0) min_ones_num = ones_num;
        else if (ones_num < min_ones_num) min_ones_num = ones_num;
    }
    // printf("min_ones_num = %d\n", min_ones_num);
    for (int i = 0; i < petrick.length; i++)
    {
        char *petrick_bin = get_full_bin(num_of_terms, petrick.array[i]);
        int ones_num = 0;
        for (int j = 0; j < strlen(petrick_bin); j++)
        {
            if (petrick_bin[j] == '1') ones_num ++;
        }
        if (ones_num != min_ones_num) petrick.array[i] = 0;
    }
    int *minimized_petrick = (int *)malloc(petrick.length * sizeof(int));
    int minimized_index = 0;
    for (int i = 0; i < petrick.length; i++)
    {
        if (petrick.array[i] != 0)
        {
            minimized_petrick[minimized_index] = petrick.array[i];
            minimized_index ++;
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
        for (int j = 0; j < length_of_bin; j++)
        {
            if (petrick_bin[j] == '1')
            {
                char *expression = get_expression(terms[num_of_terms - j - 1]);
                if (f_index < 2)
                {
                    if (f_index == 0) str = strdup(expression);
                    else strcat(str, expression);
                    strcat(str, " + ");
                }
                else
                    strcat(str, expression);
                f_index ++;
            }
        }
        InsertStringNode(str, final_functions);
    }
    free(minimized_petrick);
    minimized_petrick = NULL;
    return final_functions;
}

void FreeNodes(StringNode **head)
{
    StringNode *current = *head;
    while (current != NULL)
    {
        StringNode *next = current->next;
        free(current);
        current = next;
    }
    *head = NULL;
    
}

int main()
{
    char *expression = "B\'";
    char *str;
    str = strdup(expression);
    printf("%s\n", str);
}