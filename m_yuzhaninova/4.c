#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Strings 
{
     
    char *data; // Строка
    
    struct Strings *next; // Указатель на следующий узел списка
}Str;

// Функция добавления нового узла в конец списка
void new_string(Str **head, const char *str) 
{
    Str *new = (Str *)malloc(sizeof(Str)); // Создаем новый узел

    int len = strlen(str);
    // Выделяем память в узле под строку + '\0'
    new->data = (char *)malloc(len + 1);  // Выделяем память подс строку и '\0'

    
    strcpy(new->data, str); // Копируем строку
    
    new->next = NULL; // Указатель на следующий узел делаем NULL

    // Если ни одного узла в списке не было
    if (*head == NULL) 
    {
        *head = new; // то новый узел это первый
    } 
    else 
    {
        // Доходим до последнего узла
        Str *p = *head;
        while (p->next != NULL)
        {
            p = p->next;
        }
        // Вставляем новый узел
        p->next = new;
    }
}

// Функция освобождения памяти списка
void free_list(Str *head) 
{
    // Пока узлы существуют
    while (head) 
    {
        Str *tmp = head; // Сохраняем текущий узел
        head = head->next; // Смещаемся на новый 

        // Удаляем сохраненный узел
        free(tmp->data);
        free(tmp);
    }
}

int main() 
{
    Str *head = NULL; // Начало списка 
    char str[1000]; // Новая строка

    printf("Введите строку\n");
    while (1) 
    {
        fgets(str, 1000, stdin); // Получаем строку

        // Удаляем символ новой строки
        int len = strlen(str);
        if (len > 0 && str[len - 1] == '\n')
        {
            str[len-1] = '\0';
        }

        // Конец цикла
        if (str[0] == '.') 
        {
            break; 
        }

        new_string(&head, str); // Добавляем строку в список
    }

    // Выводим строки из списка
    Str *p = head;
    while (p) 
    {
        printf("%s\n", p->data);
        p = p->next;
    }

    free_list(head); // Очистка памяти

    return 0;
}
