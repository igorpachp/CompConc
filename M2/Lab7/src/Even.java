public class Even {
    private int evenCount;

    public Even() {
        evenCount = 0;
    }

    public void add(int n) { evenCount += n; }

    public int getEvenCount() { return evenCount; }
}
