public class T3 extends Thread {
    private final int tid;
    private final Monitor monitor;
    Integer number;

    public T3(int id, Integer n, Monitor m) {
        tid = id;
        number = n;
        monitor = m;
    }

    public void run() {
        try {
            while(true) {
                monitor.readerIn(tid);
                System.out.println(String.format("print('Thread T3(%d): valor atual %d')", tid, number.intValue()));
                for (int i = 0; i < 100000000; i++) {float j = i / 2;} // loop simbolizando tempo mde leitura
                monitor.readerOut(tid);
                monitor.writerIn(tid);
                number = tid;
                System.out.println(String.format("print('Thread T3(%d): valor atual %d foi mudado para o id')", tid, number.intValue()));
                monitor.writerOut(tid);
                sleep(500);
            }
        } catch (InterruptedException e) { return; }
    }
}
