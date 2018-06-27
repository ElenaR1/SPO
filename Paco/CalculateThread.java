package threads.calculateE;

import static threads.calculateE.CalculateE.calculateSumMember;
import static threads.calculateE.CalculateE.precision;

import java.io.PrintWriter;
import java.math.BigDecimal;

import javax.swing.JTextArea;

public class CalculateThread implements Runnable {

    private int start;
    private int step;
    private boolean quiet = false;
    private JTextArea area;
    private BigDecimal sum = BigDecimal.ZERO;
    private PrintWriter file;

    public CalculateThread(int start, int step, PrintWriter file) {
        this(start, step, file, null);
    }

    public CalculateThread(int start, int step, PrintWriter file, JTextArea area) {
        this.start = start;
        this.step = step;
        this.area = area;
        this.file = file;
        if (area == null) {
            quiet = true;
        }
    }

    @Override
    public void run() {
        long startTime = System.currentTimeMillis();
        if (area != null) {
            area.append("Thread " + (start + 1) + " started.\n");
        }
        if (!quiet) {
            file.println("Thread " + (start + 1) + " started.");
        }

        BigDecimal member;
        for (int k = this.start; k < precision; k += step) {
            member = calculateSumMember(k);
            sum = sum.add(member);
        }

        long executionTime = System.currentTimeMillis() - startTime;
        if (area != null) {
            area.append("Thread " + (start + 1) + " stopped.\n");
            area.append("Thread " + (start + 1) + " execution time was " + executionTime + " ms.\n");
        }
        if (!quiet) {
            file.println("Thread " + (start + 1) + " stopped.");
            file.println("Thread " + (start + 1) + " execution time was " + executionTime + " ms.");
        }
    }

    public BigDecimal getResult() {
        return sum;
    }
}
