public class T2 extends Thread {
    private final int tid;
    private final Monitor monitor;
    Integer number;

    public T2(int id, Integer n, Monitor m) {
        tid = id;
        number = n;
        monitor = m;
    }

    public void run() {
        try {
            while(true) {
                monitor.readerIn(tid);
                if (number.intValue() % 2 == 0) {
                    System.out.println(String.format("print('Thread T2(%d): valor atual %d eh par')", tid, number.intValue()));
                } else {
                    System.out.println(String.format("print('Thread T2(%d): valor atual %d eh impar')", tid, number.intValue()));
                }
                monitor.readerOut(tid);
                sleep(500);
            }
        } catch (InterruptedException e) { return; }
    }
}
