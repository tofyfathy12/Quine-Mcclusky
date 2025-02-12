#include <gtk/gtk.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define alphabet "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define ALPHA_LENGTH 26
#define MAX_VARIABLES 64

FILE *logs;

struct function
{
    int *minterms_arr;
    int *dontcares;
    char ***mcclusky_groups;
    int size;
    int dontcares_size;
    int groups_num;
    int number_of_variables;
};

struct combination
{
    char **combgroup;
    int size;
};

typedef struct StringNode
{
    char *str;
    struct StringNode *next;
    int length;
} StringNode;

typedef struct IntArray
{
    int *array;
    int length;
} IntArray;

struct function get_function(char *minterms, GtkEntry *dontcares_entry, int vars_num, GtkTextView *textview);
void print_struct(struct function f);
char *get_binary(int number);
char *get_expression(const char *binary);
int ones_count(int number);
void get_mcclusky_groups(struct function *f);
int min(const int *arr, int size);
int count(const int *arr, int size, int number);
void remove_dublicates(struct function *f);
void print_mcclusky_groups(struct function f);
int max(int arr[], int size);
char *get_full_bin(int var_num, int num);
struct combination general_comb(struct combination g1, struct combination g2, StringNode *taken_node, StringNode *nottaken_node, int last_combination_of_column);
bool onedif(const char *bin1, const char *bin2);
int index_of_difference(const char *bin1, const char *bin2);
char digit_to_char(int num);
int binary_to_decimal(char *binary);
StringNode *CreateStringNode(char *str);
void InsertStringNode(char *str, StringNode *head);
void PrintStringNodes(StringNode *head, char seperator);
int Is_In(char *str, StringNode *head);
int RemoveStringNode(char *str, StringNode *head);
IntArray all_possible_minterms(char *str);
int **get_prime_implicants_chart(int *minterms, int num_of_minterms, char **terms, int num_of_terms);
IntArray get_petrick(int *minterms, int num_of_minterms, char **terms, int num_of_terms);
StringNode *get_final_functions(struct function *f, StringNode *terms_head);
void FreeNodes(StringNode **head);
StringNode *get_solution(char *minterms, GtkEntry *dontcares_entry, int vars_num, GtkTextView *textview);
void on_activate(GtkApplication *app, gpointer user_data);
void show_possible_functions(GtkButton *button, gpointer user_data);
void on_toggle(GtkWidget *tglbtn, gpointer user_data);

int main(int argc, char *argv[])
{   
    logs = fopen("logs.txt", "a+");
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.example.MyApp", G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    fclose(logs);

    return status;
}

void show_possible_functions(GtkButton *button, gpointer user_data)
{
    GtkBuilder *builder = GTK_BUILDER(user_data);

    GtkEntry *minterms_entry, *dontcares_entry;
    GtkSpinButton *vars_num_spinbutton;
    GtkTextView *possible_functions_textview;
    GtkTextBuffer *buffer;

    minterms_entry = GTK_ENTRY(gtk_builder_get_object(builder, "minterms_entry"));
    dontcares_entry = GTK_ENTRY(gtk_builder_get_object(builder, "dontcares_entry"));
    vars_num_spinbutton = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "vars_num_spinbutton"));
    possible_functions_textview = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "possible_functions"));
    buffer = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "textbuffer1"));

    char *minterms = (char *)g_strdup(gtk_entry_get_text(minterms_entry));
    int vars_num = (int)gtk_spin_button_get_value_as_int(vars_num_spinbutton);

    StringNode *solution = get_solution(minterms, dontcares_entry, vars_num, possible_functions_textview);
    g_free((gchar *)minterms);
    gchar *result = "";
    int i = 0;
    while (solution != NULL)
    {
        gchar *temp, F[10];
        g_snprintf(F, 10, "F%d = ", i);
        temp = solution->str;
        if (strcmp(temp, "ERROR") == 0)
        {
            result = g_strdup(temp);
            break;
        }
        result = g_strconcat(result, F, temp, "\n", NULL);
        solution = solution->next;
        i++;
    }
    gtk_text_buffer_set_text(buffer, "", 0);
    gtk_text_buffer_set_text(buffer, (const gchar *)result, (gint)strlen(result));
    g_free(result);
    FreeNodes(&solution);
}

void on_activate(GtkApplication *app, gpointer user_data)
{
    GtkBuilder *builder;
    GError *Error = NULL;
    GtkWidget *window;
    GtkButton *calculation_button;
    GtkToggleButton *togglebutton;
    GtkEntry *dontcares_entry;

    builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, "main.glade", &Error))
    {
        fprintf(logs, "Error loading Glade file: %s\n", (char *)(Error->message));
        g_clear_error(&Error);
        g_application_quit(G_APPLICATION(app));
        g_object_unref(builder);
        return;
    }

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    gtk_application_add_window(app, GTK_WINDOW(window));

    calculation_button = GTK_BUTTON(gtk_builder_get_object(builder, "calculation_button"));
    g_signal_connect(calculation_button, "clicked", G_CALLBACK(show_possible_functions), g_object_ref(builder));

    togglebutton = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "togglebutton"));
    dontcares_entry = GTK_ENTRY(gtk_builder_get_object(builder, "dontcares_entry"));
    g_signal_connect(togglebutton, "toggled", G_CALLBACK(on_toggle), dontcares_entry);

    gtk_widget_show_all(window);
    gtk_widget_set_visible(GTK_WIDGET(dontcares_entry), FALSE);

    g_object_unref(builder);
}

void on_toggle(GtkWidget *tglbtn, gpointer user_data)
{
    GtkEntry *dontcares_entry = GTK_ENTRY(user_data);
    gboolean is_active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(tglbtn));
    gtk_widget_set_visible(GTK_WIDGET(dontcares_entry), is_active);
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
            fprintf(logs, "%s\n", node->str);
        else
            fprintf(logs, "%s%c", node->str, seperator);
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
    int max_var_num = f->number_of_variables;
    int ones_counts[f->size];
    for (int i = 0; i < f->size; i++)
        ones_counts[i] = ones_count(f->minterms_arr[i]);

    int min_ones_count = min(ones_counts, f->size);
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
                element_index++;
            }
        }
        *(*(f->mcclusky_groups + i) + element_index) = NULL; // last element to be null-terminator (\0)
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

char *get_expression(const char *binary)
{
    int binary_length = strlen(binary);
    int size = binary_length * 4 + 1;
    char *expression = (char *)calloc(size, sizeof(char));
    int i = 0;
    int alpha_index = 0;
    int exp_index = 0;
    char temp;
    int underscores = 0;
    int excess = 0;
    while (i < binary_length)
    {
        excess = alpha_index / ALPHA_LENGTH;
        temp = alphabet[alpha_index % ALPHA_LENGTH];
        if (binary[i] == '0')
        {
            expression[exp_index] = temp;
            if (excess > 0)
            {
                expression[exp_index + 1] = '\'';
                expression[exp_index + 2] = digit_to_char(excess);
                exp_index += 3;
            }
            else
            {
                expression[exp_index + 1] = '\'';
                exp_index += 2;
            }
        }
        else if (binary[i] == '1')
        {
            expression[exp_index] = temp;
            if (excess > 0)
            {
                expression[exp_index + 1] = digit_to_char(excess);
                exp_index += 2;
            }
            else
                exp_index++;
        }
        else if (binary[i] == '_')
            underscores ++;
        alpha_index++;
        i++;
    }
    if (underscores == binary_length && exp_index == 0)
    {
        expression[0] = '1';
        exp_index = 1;
    }
    expression = (char *)realloc(expression, (exp_index + 1) * sizeof(char));
    expression[exp_index] = '\0';
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

struct function get_function(char *minterms, GtkEntry *dontcares_entry, int vars_num, GtkTextView *textview)
{
    char *dontcares;
    if (gtk_widget_get_visible(GTK_WIDGET(dontcares_entry)))
    {
        dontcares = (char *)gtk_entry_get_text(dontcares_entry);
    }
    else
    {
        dontcares = (char *)calloc(1, sizeof(char));
    }
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(textview);
    gchar error_msg[100];
    struct function f = {NULL, NULL, NULL, 0, 0, 0, 0};
    int valid = 1;
    int ask_for_dontcares = 1;
    f.number_of_variables = vars_num;
    int freq_arr[1 << f.number_of_variables];
    memset(freq_arr, 0, (1 << f.number_of_variables) * sizeof(int));
    f.size = 0;
    if (strcmp(minterms, "") == 0)
    {
        f.minterms_arr = NULL;
        f.dontcares = NULL;
        f.mcclusky_groups = NULL;
        f.dontcares_size = 0;
        f.groups_num = 0;
        f.number_of_variables = 0;
        g_free((gchar *)dontcares);
        return f;
    }
    char *minterms_input = strdup(minterms);
    for (int i = 0; i < strlen(minterms_input); i++)
        if (minterms_input[i] == ',') f.size++;
    
    f.size++;
    f.minterms_arr = (int *)malloc(f.size * sizeof(int));
    int i = 0;
    char *delim = ", \t\n";
    char *temp;
    temp = strtok(minterms_input, delim);
    while (temp != NULL && i < f.size)
    {
        int num_of_digits = 0;
        for (int i = 0; i < strlen(temp); i++)
            if (isdigit(temp[i]))
                num_of_digits++;

        if (num_of_digits == 0)
        {
            temp = NULL;
            valid = 0;
            goto when_error;
        }
        f.minterms_arr[i] = atoi(temp);
        if (f.minterms_arr[i] < 0)
        {
            fprintf(logs, "ERROR: Negative Minterm !!\n");
            g_snprintf(error_msg, 100, "ERROR: Negative Minterm !!\nTry Again\n");
            gtk_text_buffer_set_text(buffer, error_msg, (gint)strlen(error_msg));
            valid = 0;
            goto when_error;
        }
        else if (f.minterms_arr[i] >= (1 << f.number_of_variables))
        {
            fprintf(logs, "ERROR: Maximum Minterm Exceeded (Maximum = %d) !!\n", (1 << f.number_of_variables) - 1);
            g_snprintf(error_msg, 100, "ERROR: Maximum Minterm Exceeded (Maximum = %d) !!\nTry Again\n", (1 << f.number_of_variables) - 1);
            gtk_text_buffer_set_text(buffer, error_msg, (gint)strlen(error_msg));
            valid = 0;
            goto when_error;
        }
        freq_arr[f.minterms_arr[i]]++;
        temp = strtok(NULL, delim);
        i++;
    }
    if (temp != NULL)
    {
        fprintf(logs, "INPUT ERROR !!\n");
        g_snprintf(error_msg, 100, "INPUT ERROR !!\nTry Again\n");
        gtk_text_buffer_set_text(buffer, error_msg, (gint)strlen(error_msg));
        valid = 0;
        goto when_error;
    }
    free(minterms_input);

    if (strcmp(dontcares, "") == 0 && f.minterms_arr != NULL)
    {
        f.dontcares = NULL;
        f.dontcares_size = 0;
    }
    else if (f.minterms_arr != NULL)
    {
        f.dontcares_size = 0;
        char *dontcares_input = strdup(dontcares);
        for (int i = 0; i < strlen(dontcares_input); i++)
            if (dontcares_input[i] == ',') f.dontcares_size++;
        
        f.dontcares_size++;
        f.dontcares = (int *)malloc(f.dontcares_size * sizeof(int));
        int i = 0;
        char *delim = ", \n";
        char *temp;
        temp = strtok(dontcares_input, delim);
        int num_of_digits = 0;
        while (temp != NULL && i < f.dontcares_size)
        {
            for (int i = 0; i < strlen(temp); i++)
                if (isdigit(temp[i]))
                    num_of_digits++;
            if (num_of_digits == 0)
            {
                temp = NULL;
                valid = 0;
                goto when_error;
            }
            f.dontcares[i] = atoi(temp);
            if (f.dontcares[i] < 0)
            {
                fprintf(logs, "ERROR: Negative Don't-Care !!\n");
                g_snprintf(error_msg, 100, "ERROR: Negative Don't-Care !!\nTry Again\n");
                gtk_text_buffer_set_text(buffer, error_msg, (gint)strlen(error_msg));
                valid = 0;
                goto when_error;
            }
            else if (f.dontcares[i] >= (1 << f.number_of_variables))
            {
                fprintf(logs, "ERROR: Maximum Don't-Care Exceeded (Maximum = %d) !!\n", (1 << f.number_of_variables) - 1);
                g_snprintf(error_msg, 100, "ERROR: Maximum Don't-Care Exceeded (Maximum = %d) !!\nTry Again\n", (1 << f.number_of_variables) - 1);
                gtk_text_buffer_set_text(buffer, error_msg, (gint)strlen(error_msg));
                valid = 0;
                goto when_error;
            }
            else if (freq_arr[f.dontcares[i]] > 0)
            {
                fprintf(logs, "ERROR: Minterm Found Among Don't-Cares !!\n");
                g_snprintf(error_msg, 100, "ERROR: Minterm Found Among Don't-Cares !!\nTry Again\n");
                gtk_text_buffer_set_text(buffer, error_msg, (gint)strlen(error_msg));
                valid = 0;
                goto when_error;
            }
            temp = strtok(NULL, delim);
            i++;
        }
        if (temp != NULL)
        {
            fprintf(logs, "INPUT ERROR !!\n");
            g_snprintf(error_msg, 100, "INPUT ERROR !!\nTry Again\n");
            gtk_text_buffer_set_text(buffer, error_msg, (gint)strlen(error_msg));
            valid = 0;
            goto when_error;
        }
        free(dontcares_input);
    }
    when_error:
    if (!valid)
    {
        if (f.minterms_arr)
        {
            free(f.minterms_arr);
            f.minterms_arr = NULL;
            f.size = 0;
        }
        if (f.dontcares)
        {
            free(f.dontcares);
            f.dontcares = NULL;
            f.dontcares_size = 0;
        }
        f.mcclusky_groups = NULL;
        f.groups_num = 0;
        f.number_of_variables = 0;
        g_free((gchar *)dontcares);
        return f;
    }
    remove_dublicates(&f);
    f.minterms_arr = (int *)realloc(f.minterms_arr, (f.size + f.dontcares_size) * sizeof(int));
    int realloc_line = __LINE__ - 1;
    if (f.minterms_arr == NULL)
    {
        fprintf(logs, "ERROR Reallocating Memory in Line %d\n", realloc_line);
        exit(EXIT_FAILURE);
    }
    memmove(f.minterms_arr + f.size, f.dontcares, f.dontcares_size * sizeof(int));
    f.size += f.dontcares_size;
    get_mcclusky_groups(&f);
    return f;
}

void print_struct(struct function f)
{
    for (int i = 0; i < f.size; i++)
        printf("%d ", f.minterms_arr[i]);
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
    int freq_arr_size = max(f->minterms_arr, f->size) + 1;
    int freq_arr[freq_arr_size];
    memset(freq_arr, 0, freq_arr_size * sizeof(int));
    int new_size = 0;
    for (int i = 0; i < f->size; i++)
    {
        freq_arr[f->minterms_arr[i]]++;
        if (freq_arr[f->minterms_arr[i]] == 1) new_size++;
    }
    int *new_arr = (int *)malloc(new_size * sizeof(int));
    int new_arr_index = 0;
    for (int i = 0; i < freq_arr_size; i++)
    {
        if (freq_arr[i] > 0)
        {
            new_arr[new_arr_index] = i;
            new_arr_index++;
        }
    }
    free(f->minterms_arr);
    f->minterms_arr = (int *)realloc(new_arr, new_arr_index * sizeof(int));
    f->size = new_arr_index;
    if (f->dontcares_size > 0)
    {
        int freq_arr_size = max(f->dontcares, f->dontcares_size) + 1;
        int freq_arr[freq_arr_size];
        memset(freq_arr, 0, freq_arr_size * sizeof(int));
        int new_size = 0;
        for (int i = 0; i < f->dontcares_size; i++)
        {
            freq_arr[f->dontcares[i]]++;
            if (freq_arr[f->dontcares[i]] == 1) new_size++;
        }
        int *new_arr = (int *)malloc(new_size * sizeof(int));
        int new_arr_index = 0;
        for (int i = 0; i < freq_arr_size; i++)
        {
            if (freq_arr[i] > 0)
            {
                new_arr[new_arr_index] = i;
                new_arr_index++;
            }
        }
        free(f->dontcares);
        f->dontcares = (int *)realloc(new_arr, new_arr_index * sizeof(int));
        f->dontcares_size = new_arr_index;
    }
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
            return index;
        }
    }
    return -1;
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
    int str_length = strlen(str);
    char temp[str_length + 1];
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
    for (int i = 0; i < str_length; i++)
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
        int *p_result2 = (int *)malloc(p_result_index * sizeof(int));
        int p_result2_index = 0;
        for (int i = 0; i < p_result_index; i++)
        {
            if (p_result[i] != 0)
            {
                int num = p_result[i];
                p_result2[p_result2_index] = num;
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

StringNode *get_final_functions(struct function *f, StringNode *terms_head)
{
    f->minterms_arr = (int *)realloc(f->minterms_arr, (f->size - f->dontcares_size) * sizeof(int));
    f->size -= f->dontcares_size;

    int *minterms = f->minterms_arr;
    int num_of_minterms = f->size;
    int num_of_terms = terms_head->length;
    char **terms = (char **)malloc(num_of_terms * sizeof(char *));
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

StringNode *get_solution(char *minterms, GtkEntry *dontcares_entry, int vars_num, GtkTextView *textview)
{
    struct function boolean_function = get_function(minterms, dontcares_entry, vars_num, textview);
    // print_struct(boolean_function);
    // printf("Mcclusky Groups:\n");
    // print_mcclusky_groups(boolean_function);
    // printf("Groups Num = %d\n", boolean_function.groups_num);
    struct combination **combinations = NULL;
    if (boolean_function.groups_num > 0)
        combinations = (struct combination **)malloc(boolean_function.groups_num * sizeof(struct combination *));

    for (int i = 0; i < boolean_function.groups_num; i++)
        combinations[i] = (struct combination *)malloc((boolean_function.groups_num - i) * sizeof(struct combination));

    for (int j = 0; j < boolean_function.groups_num; j++)
    {
        int size = 0;
        while (boolean_function.mcclusky_groups[j][size] != NULL)
            size++;
        combinations[0][j].size = size;
        combinations[0][j].combgroup = (char **)realloc(boolean_function.mcclusky_groups[j], size * sizeof(char *));
    }
    StringNode *not_taken = CreateStringNode("");
    StringNode *taken = CreateStringNode("");
    if (boolean_function.groups_num == 1)
    {
        for (int i = 0; i < combinations[0][0].size; i++)
            InsertStringNode(combinations[0][0].combgroup[i], not_taken);
    }
    for (int i = 1; i < boolean_function.groups_num; i++)
    {
        int last_check = 0;
        for (int j = 0; j < boolean_function.groups_num - i; j++)
        {
            if (j == boolean_function.groups_num - i - 1)
                last_check = 1;
            combinations[i][j] = general_comb(combinations[i - 1][j], combinations[i - 1][j + 1], taken, not_taken, last_check);
            // printf("comb%d_%d:\n", j, j + 1);
            for (int k = 0; k < combinations[i][j].size; k++)
            {
                if (i == boolean_function.groups_num - 1 && j == boolean_function.groups_num - i - 1)
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
    // printf("not taken: ");
    // PrintStringNodes(not_taken, ' ');
    // printf("not taken length = %d\n", not_taken->length);
    // printf("taken: ");
    // PrintStringNodes(taken, ' ');
    for (int i = 0; i < boolean_function.groups_num; i++)
    {
        for (int j = 0; j < boolean_function.groups_num - i; j++)
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
    if (combinations != NULL)
    {
        free(combinations);
        combinations = NULL;
    }
    StringNode *possible_functions;
    if (boolean_function.minterms_arr != NULL)
        possible_functions = get_final_functions(&boolean_function, not_taken);
    else
    {
        GtkTextIter start, end;
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(textview);
        gtk_text_buffer_get_bounds(buffer, &start, &end);
        gchar *str = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
        if (g_strcmp0(str, "") == 0)
            possible_functions = CreateStringNode("ERROR");
        else
            possible_functions = CreateStringNode(str);
        g_free(str);
    }
    FreeNodes(&taken);
    FreeNodes(&not_taken);
    if (boolean_function.minterms_arr != NULL)
    {
        free(boolean_function.minterms_arr);
        free(boolean_function.dontcares);
        free(boolean_function.mcclusky_groups);
    }
    return possible_functions;
}
