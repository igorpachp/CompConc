public class Main {
    static private final int numT1 = 4;
    static private final int numT2 = 4;
    static private final int numT3 = 2;

    public static void main(String[] args) {
        Monitor monitor = new Monitor();
        Integer number = 0;
        T1[] t1 = new T1[numT1];
        T2[] t2 = new T2[numT2];
        T3[] t3 = new T3[numT3];

        System.out.println("import verificaLE");
        System.out.println("le = verificaLE.LE()");

        for (int i = 0; i < numT1; i++) {
            t1[i] = new T1(i, number, monitor);
            t1[i].start();
        }

        for (int i = 0; i < numT2; i++) {
            t2[i] = new T2(i, number, monitor);
            t2[i].start();
        }

        for (int i = 0; i < numT3; i++) {
            t3[i] = new T3(i, number, monitor);
            t3[i].start();
        }
    }
}
