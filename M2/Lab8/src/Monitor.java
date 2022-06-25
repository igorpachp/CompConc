public class Monitor {
    private int reading;
    private int writing;

    public Monitor(){
        reading = 0;
        writing = 0;
    }

    public synchronized void readerIn(int id) {
        try {
            while (writing > 0) {
                System.out.println(String.format("le.leitorBloqueado(%d)", id));
                wait();
            }
            reading++;
            System.out.println(String.format("le.leitorLendo(%d)", id));
        } catch (InterruptedException e) { return; }
    }

    public synchronized void readerOut(int id) {
        reading--;
        if (reading == 0) { notify(); }
        System.out.println(String.format("le.leitorSaindo(%d)", id));
    }

    public synchronized void writerIn(int id) {
        try {
            while (writing > 0 || reading > 0) {
                System.out.println(String.format("le.escritorBloqueado(%d)", id));
                wait();
            }
            writing++;
            System.out.println(String.format("le.escritorEscrevendo(%d)", id));
        } catch (InterruptedException e) { return; }
    }

    public synchronized void writerOut(int id) {
        writing--;
        notifyAll();
        System.out.println(String.format("le.escritorSaindo(%d)", id));
    }
}
