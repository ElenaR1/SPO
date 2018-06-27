package threads.calculateE;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintWriter;
import java.math.BigDecimal;

import javax.swing.JTextArea;

public class StartThreads {

    public static void start(int threadCount, JTextArea area, String output) {
        try {
            runThreads(threadCount, area, new PrintWriter(new FileOutputStream(output, false)));
        } catch (InterruptedException | FileNotFoundException e) {
            e.printStackTrace();
        }
    }

    private static void runThreads(int threadCount, JTextArea area, PrintWriter file)
            throws InterruptedException {
        CalculateThread[] calcThreads = new CalculateThread[threadCount];
        Thread[] threads = new Thread[threadCount];
        BigDecimal sum = BigDecimal.ZERO;
        int step = threadCount;

        long startTime = System.currentTimeMillis();
        for (int i = 0; i < threadCount; i++) {
            calcThreads[i] = new CalculateThread(i, step, file, area);
        }
        for (int i = 0; i < threadCount; i++) {
            threads[i] = new Thread(calcThreads[i]);
            threads[i].start();
        }
        for (Thread thread : threads) {
            thread.join();
        }
        for (CalculateThread calcThread : calcThreads) {
            sum = sum.add(calcThread.getResult());
        }

        long finishTime = System.currentTimeMillis() - startTime;
        if (area != null) {
            area.append("Total execution time was " + finishTime + "ms\n");
            area.append(sum.toString() + "\n");
        }
        file.println("Total execution time was " + finishTime + "ms");
        file.println(sum.toString());
        file.flush();
        file.close();
    }

}
