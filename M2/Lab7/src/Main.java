public class Main {
    static final int NTHREADS = 4;
    static final int size = 100000;

    public static void main(String[] args) {
        Thread[] threads = new Thread[NTHREADS];
        int[] arr = new int[size];
        Even evencount = new Even();

        // inicializandoo vetor de inteiros
        for (int i = 0; i < size; i++) {
            arr[i] = i;
        }

        // criando as threads
        for (int i = 0; i < NTHREADS; i++) {
            threads[i] = new FindEvenCount(arr, i, NTHREADS, evencount);
        }

        // iniciando as threads
        for (int i = 0; i < NTHREADS; i++) {
            threads[i].start();;
        }

        // esperando termino
        for (int i = 0; i < NTHREADS; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                return;
            }
        }

        System.out.println(evencount.getEvenCount());
    }
}
