public class FindEvenCount extends Thread {
    private final int[] arr;
    private final int tid;
    private final int NTHREADS;
    private final Even evenCounter;

    public FindEvenCount(int[] arr, int id, int nthreads, Even counter) {
        this.arr = arr;
        tid = id;
        NTHREADS = nthreads;
        this.evenCounter = counter;
    }

    public void run() {
        int count = 0;
        for (int i = tid; i < arr.length; i += NTHREADS) {
            if (arr[i] % 2 == 0) count++;
        }
        addCount(count);
    }

    public synchronized void addCount(int count) { evenCounter.add(count); }
}
