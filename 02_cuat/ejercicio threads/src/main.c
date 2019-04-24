enum E_RETURN_MAIN
{
    OK,
    NUM_ARGS,
    INVALID_ARGS
};


int main(int argc, char *argv[])
{
    int num_process, num_threads, life_process, life_threads;
    int i, j;

    if (argc != 5)
    {
        printf("Cantidad incorrecta de argumentos\n");
        printf ("USO: exe [cant_proc] [cant_hilos_x_proc] [vida_proc] [vida_hilo]\n");

        return NUM_ARGS;
    }

    num_process = atoi(argv[1]);
    num_threads = atoi(argv[2]);
    life_process = atoi(argv[3]);
    life_threads = atoi(argv[4]);

    if (num_process < 0 || num_threads < 0  || life_process < 0 || life_threads < 0)
    {
        printf("Argumentos invalidos. Deben ser positivos\n");
        printf ("USO: exe [cant_proc] [cant_hilos_x_proc] [vida_proc] [vida_hilo]\n");

        return INVALID_ARGS;
    }

    /*
    char flags_nuevo = SA_SIGINFO | SA_NOCLDSTOP | SA_RESTART;
    char flags_viejo = 0;

    sigaction ( &flags_nuevo, &flags_viejo)


    waitpid(-1, &status,    WNOHANG | WUNTRACED | WCONTINUED)
    */

    for (i = 0; i < num_process; i++)
    {

    }




    return OK;
}