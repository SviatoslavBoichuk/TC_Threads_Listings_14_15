#include <stdio.h>
#include <pthread.h>

int thread_flag;

pthread_cond_t thread_flag_cv;
pthread_mutex_t thread_flag_mutex;

void initialize_flag()
{
 /* Инициализация исключающего семафора и сигнальной
    переменной. */
 pthread_mutex_init(&thread_flag_mutex, NULL);
 pthread_cond_init(&thread_flag_cv, NULL);

 /* Инициализация флага. */
 thread_flag = 0;
}

void do_work()
{
    printf("hello\n");
}

/* Если флаг установлен, многократно вызывается функция
   do_work(). В противном случае поток блокируется. */
void* thread_function(void* thread_arg)
{
     /* Бесконечный цикл. */
     while (1)
     {
      /* Захватываем исключающий семафор, прежде чем обращаться
         к флагу. */
      pthread_mutex_lock(&thread_flag_mutex);

      while (!thread_flag)
      {
       /* Флаг сброшен. Ожидаем сигнала об изменении условной
          переменной, указывающего на то, что флаг установлен.
          При поступлении сигнала поток разблокируется и снова
          проверяет флаг. */
       pthread_cond_wait(&thread_flag_cv, &thread_flag_mutex);
      }
       /* При выходе из цикла освобождаем исключающий семафор. */
       pthread_mutex_unlock(&thread_flag_mutex);
       /* Выполняем требуемые действия. */
       do_work();
     }

     return NULL;
}

/* Задаем значение флага равным FLAG_VALUE. */
void set_thread_flag(int flag_value)
{
     /* Захватываем исключающий семафор, прежде чем изменять
        значение флага. */
     pthread_mutex_lock(&thread_flag_mutex);
     /* Устанавливаем флаг и посылаем сигнал функции
        thread_function(), заблокированной в ожидании флага.
        Правда, функция не сможет проверить флаг, пока
        исключающий семафор не будет освобожден. */
     thread_flag = flag_value;
     pthread_cond_signal(&thread_flag_cv);

     /* освобождаем исключающий семафор. */
     pthread_mutex_unlock(&thread_flag_mutex);
}

int main(int argc, char** argv)
{
    const int ciExitSuccess = 0;

    pthread_t thread;

    int inVal = 0;
    pthread_create(&thread, NULL, &thread_function, NULL);
    while (1)
    {
        printf("Enter any non zero number! ");
        scanf("%d", &inVal);
        set_thread_flag(inVal);
    }

    return ciExitSuccess;
}
