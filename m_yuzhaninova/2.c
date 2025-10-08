#include <stdio.h>
#include <time.h>


extern char *tzname[]; // Массив, который находится в time.h

int main()
{
    time_t now; // Переменная, которая содержит время в секундах и является целым числом
    time(&now); // Записываем время в секундах

    setenv("TZ", "America/Los_Angeles", 1); // Меняем часовой пояс 
    tzset(); // Перезагружаем часовой пояс

    struct tm *sp; // Структура, которая разбивает время на компоненты
    sp = localtime(&now); // Записываем в структу полученные данные

    printf("%s", asctime(sp)); // Выводим текущее время
    
    return 0;
}
