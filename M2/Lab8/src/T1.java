public class T1 extends Thread {
    private final int tid;
    private final Monitor monitor;
    Integer number;

    public T1(int id, Integer n, Monitor m) {
        tid = id;
        number = n;
        monitor = m;
    }

    public void run() {
        try {
            while(true) {
                monitor.writerIn(tid);
                System.out.println(String.format("print('Thread T1(%d): incrementando valor atual %d em 1')", tid, number.intValue()));
                number++;
                System.out.println(String.format("print('Thread T1(%d): valor atual %d')", tid, number.intValue()));
                monitor.writerOut(tid);
                sleep(500);
            }
        } catch (InterruptedException e) { return; }
    }
}
